#include "Shell.h"

#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMInvokable.h"
#include "Universe.h"
#include <sstream>

// maximal length of an input line from the shell
#define INPUT_MAX_SIZE BUFSIZ

// some constants for assembling Smalltalk code
#define SHELL_PREFIX "Shell_Class_"
#define SHELL_PART_1 " = (run: it = ( | tmp | tmp := ("
#define SHELL_PART_2 "). 'it = ' print. ^tmp println) )"

Shell::Shell()
{
    bootstrap_method = NULL;
}

Shell::Shell(VMMethod* bsm)
{
    this->bootstrap_method = bsm;
}

Shell::~Shell()
{
    // TODO
}

void Shell::Start()
{
#define QUIT_CMD "system exit"
#define QUIT_CMD_L 11 + 1

    if (bootstrap_method == NULL) {
        _UNIVERSE->ErrorExit("Shell needs bootstrap method!");
    }
    // the statement to evaluate
    char      inbuf[INPUT_MAX_SIZE];
    int       bytecode_index, counter = 0;
    VMFrame*  current_frame;
    VMClass*  runClass;
    VMObject* it = nil_object; // last evaluation result.

    cout << "SOM Shell. Type \"" << QUIT_CMD << "\" to exit.\n";

    // Create a fake bootstrap frame
    current_frame = _UNIVERSE->GetInterpreter()->PushNewFrame(this->GetBootstrapMethod());
    // Remember the first bytecode index, e.g. index of the halt instruction
    bytecode_index = current_frame->GetBytecodeIndex();// SEND(current_frame, get_bytecode_index);

    /**
     * Main Shell Loop
     */
    while(true) {
        // initialize empty strings
        pString   statement = pString("");
        pString   inp = pString("");
        
        //printf("---> ");
        cout << "---> ";
        // Read a statement from the keyboard
        // and avoid buffer overflow.

        cin.getline(inbuf, INPUT_MAX_SIZE);

        inp = pString(inbuf);

        if (inp.length() == 0) continue;
        //if (inp == pString(QUIT_CMD)) return;
        //do {
        //    if(!feof(stdin))
        //        SEND(inp, concatChar, fgetc(stdin));
        //    else {
        //        if(SEND(inp, length) <= 1) 
        //        { SEND(inp, free); inp=String_new(QUIT_CMD); in_len=QUIT_CMD_L+1;}
        //        break;
        //    }
        //    in_len = SEND(inp, length);
        //} while((in_len < INPUT_MAX_SIZE) && 
        //        (SEND(inp, charAt, (in_len++ - 1)) != '\n'));
        //// undo last increment
        //in_len--;
        //
        //if(!(in_len-1)) // line empty
        //    continue;
        //
        //// strip newline.
        //inp = SEND(inp, substring, 0, in_len - 2); 
        
        // Generate a temporary class with a run method
        stringstream ss;
        ss << SHELL_PREFIX << counter << SHELL_PART_1 << inp << SHELL_PART_2;
        statement = ss.str();
        /*statement += SHELL_PREFIX;
        statement += counter;
        statement += SHELL_PART_1;
        statement += inp;
        statement += SHELL_PART_2;*/
        //sprintf(stmt, "%s%d%s", SHELL_PREFIX, counter, SHELL_PART_1);
        ++counter;
        runClass = _UNIVERSE->LoadShellClass(statement);
        // Compile and load the newly generated class
        if(runClass == NULL) {
            cout << "can't compile statement.";
            continue;            
        }
            
        current_frame = _UNIVERSE->GetInterpreter()->GetFrame();

        // Go back, so we will evaluate the bootstrap frames halt
        // instruction again
        current_frame->SetBytecodeIndex(bytecode_index);
        
        // Create and push a new instance of our class on the stack
        current_frame->Push(_UNIVERSE->NewInstance(runClass));
        
        // Push the old value of "it" on the stack
        current_frame->Push(it);
        
        // Lookup the run: method
        VMInvokable* initialize = 
            (VMInvokable*)runClass->LookupInvokable(_UNIVERSE->SymbolFor("run:"));
        
        // Invoke the run method
        initialize->Invoke(current_frame);
        
        // Start the Interpreter

        _UNIVERSE->GetInterpreter()->Start();
        
        // Save the result of the run method
        it = current_frame->Pop();
    }
}


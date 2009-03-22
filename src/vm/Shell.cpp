#include <sstream>

#include "Universe.h"
#include "Shell.h"

#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMInvokable.h"

// maximal length of an input line from the shell
#define INPUT_MAX_SIZE BUFSIZ

// some constants for assembling Smalltalk code
#define SHELL_PREFIX "Shell_Class_"
#define SHELL_PART_1 " = (run: it = ( | tmp | tmp := ("
#define SHELL_PART_2 "). 'it = ' print. ^tmp println) )"


Shell::Shell() {
    bootstrapMethod = NULL;
}


Shell::Shell(pVMMethod bsm) {
    this->bootstrapMethod = bsm;
}


Shell::~Shell() {
    // TODO
}


void Shell::Start() {
#define QUIT_CMD "system exit"
#define QUIT_CMD_L 11 + 1

    if (bootstrapMethod == NULL) {
        _UNIVERSE->ErrorExit("Shell needs bootstrap method!");
    }
    // the statement to evaluate
    char      inbuf[INPUT_MAX_SIZE];
    int       bytecodeIndex, counter = 0;
    pVMFrame  currentFrame;
    pVMClass  runClass;
    pVMObject it = nilObject; // last evaluation result.

    cout << "SOM Shell. Type \"" << QUIT_CMD << "\" to exit.\n";

    // Create a fake bootstrap frame
    currentFrame = _UNIVERSE->GetInterpreter()->PushNewFrame(this->GetBootstrapMethod());
    // Remember the first bytecode index, e.g. index of the halt instruction
    bytecodeIndex = currentFrame->GetBytecodeIndex();// SEND(current_frame, get_bytecode_index);

    /**
     * Main Shell Loop
     */
    while(true) {
        // initialize empty strings
        StdString   statement;
        StdString   inp;
        
        //printf("---> ");
        cout << "---> ";
        // Read a statement from the keyboard
        // and avoid buffer overflow.

        cin.getline(inbuf, INPUT_MAX_SIZE);

        inp = StdString(inbuf);

        if (inp.length() == 0) continue;
        
        // Generate a temporary class with a run method
        stringstream ss;
        ss << SHELL_PREFIX << counter << SHELL_PART_1 << inp << SHELL_PART_2;
        statement = ss.str();
        
        ++counter;
        runClass = _UNIVERSE->LoadShellClass(statement);
        // Compile and load the newly generated class
        if(runClass == NULL) {
            cout << "can't compile statement.";
            continue;            
        }
            
        currentFrame = _UNIVERSE->GetInterpreter()->GetFrame();

        // Go back, so we will evaluate the bootstrap frames halt
        // instruction again
        currentFrame->SetBytecodeIndex(bytecodeIndex);
        
        // Create and push a new instance of our class on the stack
        currentFrame->Push(_UNIVERSE->NewInstance(runClass));
        
        // Push the old value of "it" on the stack
        currentFrame->Push(it);
        
        // Lookup the run: method
        pVMInvokable initialize = 
            dynamic_cast<pVMInvokable>(runClass->LookupInvokable(
                                    _UNIVERSE->SymbolFor("run:")));
        
        // Invoke the run method
        (*initialize)(currentFrame);
        
        // Start the Interpreter

        _UNIVERSE->GetInterpreter()->Start();
        
        // Save the result of the run method
        it = currentFrame->Pop();
    }
}


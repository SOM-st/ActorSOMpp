#pragma once

#ifndef ROUTINE_H_
#define ROUTINE_H_

#include "VMObject.h"
#include "VMFrame.h"

// abstract base class
   class TFunctor
   {
   public:

      // two possible functions to call member function. virtual cause derived
      // classes will use a pointer to an object and a pointer to a member function
      // to make the function call
      virtual void operator()(VMObject*, VMFrame*)=0;  // call using operator
      virtual void Call(VMObject*, VMFrame*)=0;        // call using function
   };


   // derived template class
   template <class TClass> class Routine : public TFunctor
   {
   private:
      void (TClass::*fpt)(VMObject*, VMFrame*);   // pointer to member function
      TClass* pt2Object;                  // pointer to object

   public:

      // constructor - takes pointer to an object and pointer to a member and stores
      // them in two private variables
      Routine(TClass* _pt2Object, void(TClass::*_fpt)(VMObject*, VMFrame*))
         { pt2Object = _pt2Object;  fpt=_fpt; };

      // override operator "()"
      virtual void operator()(VMObject* obj, VMFrame* frm)
       { (*pt2Object.*fpt)(obj, frm);};              // execute member function

      // override function "Call"
      virtual void Call(VMObject* obj, VMFrame* frm)
        { (*pt2Object.*fpt)(obj, frm);};             // execute member function
   };


#endif

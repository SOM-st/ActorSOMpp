#pragma once

#ifndef ROUTINE_H_
#define ROUTINE_H_
   // derived template class
   template <class TClass> class Routine : public PrimitiveRoutine
   {
   private:
      void (TClass::*fpt)(VMObject*, VMFrame*);   // pointer to member function
      TClass* pt2Object;                  // pointer to object

   public:

      // constructor - takes pointer to an object and pointer to a member and stores
      // them in two private variables
       Routine(TClass* _pt2Object, void(TClass::*_fpt)(VMObject*, VMFrame*)) : PrimitiveRoutine(2)
         { pt2Object = _pt2Object;  fpt=_fpt; };

      // override operator "()"
      virtual void operator()(VMObject* obj, VMFrame* frm)
       { (*pt2Object.*fpt)(obj, frm);};              // execute member function

      // override function "Call"
      virtual void Call(VMObject* obj, VMFrame* frm)
        { (*pt2Object.*fpt)(obj, frm);};             // execute member function
   };
#endif;
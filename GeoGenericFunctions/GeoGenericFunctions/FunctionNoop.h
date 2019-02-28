//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionNoop------------------------------------//
//                                                                          //
// Class FunctionNoop, output of the unary + operator:                      //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionNoop_h
#define FunctionNoop_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

 class FunctionNoop : public AbsFunction {
  
    FUNCTION_OBJECT_DEF(FunctionNoop)

      public:

    // Constructor
    FunctionNoop(const AbsFunction *arg1);
  
    // Copy constructor.
    FunctionNoop(const FunctionNoop &right);
  
    // Destructor
    virtual ~FunctionNoop();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const;
  
    // Dimensionality 
    virtual unsigned int dimensionality() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

 private:
  
    // It is illegal to assign a FunctionNoop
    const FunctionNoop & operator=(const FunctionNoop &right);

    // The function we're negating.  
    const AbsFunction *_arg1;

 };
} // namespace GeoGenfun
#endif

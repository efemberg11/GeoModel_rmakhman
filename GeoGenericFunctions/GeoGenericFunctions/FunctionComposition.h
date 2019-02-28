//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionComposition-----------------------------//
//                                                                          //
// FunctionComposition, result of composition of two functions.             //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionComposition_h
#define FunctionComposition_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FunctionComposition : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionComposition)

      public:

    // Constructor
    FunctionComposition(const AbsFunction *arg1, const AbsFunction *arg2);

    // Copy constructor
    FunctionComposition(const FunctionComposition &right);
  
    // Destructor
    virtual ~FunctionComposition();

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

    // It is illegal to assign a FunctionComposition
    const FunctionComposition & operator=(const FunctionComposition &right);

    const AbsFunction *_arg1;
    const AbsFunction *_arg2;  
  };
} // namespace GeoGenfun
#endif

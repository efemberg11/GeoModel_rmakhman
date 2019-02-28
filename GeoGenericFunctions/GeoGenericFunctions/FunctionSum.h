//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionSum-------------------------------------//
//                                                                          //
// FunctionSum, result of addition of two functions.                        //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionSum_h
#define FunctionSum_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FunctionSum : public AbsFunction  {

    FUNCTION_OBJECT_DEF(FunctionSum)
  
      public:
  
    // Constructor
    FunctionSum(const AbsFunction *arg1, const AbsFunction *arg2);
  
    // Copy constructor
    FunctionSum(const FunctionSum &right);

    // Destructor
    virtual ~FunctionSum();
  
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

    // It is illegal to assign a FunctionSum
    const FunctionSum & operator=(const FunctionSum &right);

    const AbsFunction *_arg1;
    const AbsFunction *_arg2;
  };
} // namespace GeoGenfun
#endif

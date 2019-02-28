//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionTimesParameter--------------------------//
//                                                                          //
// Function times parameter, result of multiplying a function by a          //
// parameter. 
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionTimesParameter_h
#define FunctionTimesParameter_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FunctionTimesParameter : public AbsFunction  {

    FUNCTION_OBJECT_DEF(FunctionTimesParameter)
  
      public:
  
    // Constructor
    FunctionTimesParameter(const AbsParameter *parameter, const AbsFunction *function);
  
    // Copy constructor
    FunctionTimesParameter(const FunctionTimesParameter &right);

    // Destructor
    virtual ~FunctionTimesParameter();
  
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

    // It is illegal to assign a FunctionTimesParameter
    const FunctionTimesParameter & operator=(const FunctionTimesParameter &right);

    const AbsFunction  *_function;
    AbsParameter       *_parameter;

  };
} // namespace GeoGenfun
#endif

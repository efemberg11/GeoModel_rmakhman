//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstOverFunction-------------------------------//
//                                                                          //
// ConstOverFunction, result of multiplying a constant and a function       //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstTimesFunction_hh
#define ConstTimesFunction_hh
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class ConstTimesFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstTimesFunction)
  
      public:
  
    // Constructor
    ConstTimesFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstTimesFunction(const ConstTimesFunction &right);

    // Destructor
    virtual ~ConstTimesFunction();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const;

    // Dimensionality 
    virtual unsigned int dimensionality() const;

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function.hhave an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a ConstTimesFunction
    const ConstTimesFunction & operator=(const ConstTimesFunction &right);

    double             _constant;
    const AbsFunction *_arg;
  };
} // namespace GeoGenfun
#endif

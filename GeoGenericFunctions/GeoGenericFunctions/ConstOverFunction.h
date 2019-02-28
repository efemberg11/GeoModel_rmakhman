//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstOverFunction-------------------------------//
//                                                                          //
// ConstOverFunction, result of dividing a constant by a function           //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstOverFunction_hh
#define ConstOverFunction_hh
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class ConstOverFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstOverFunction)
  
      public:
  
    // Constructor
    ConstOverFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstOverFunction(const ConstOverFunction &right);

    // Destructor
    virtual ~ConstOverFunction();
  
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

    // It is illegal to assign a ConstOverFunction
    const ConstOverFunction & operator=(const ConstOverFunction &right);

    double             _constant;
    const AbsFunction *_arg;
  };
} // namespace GeoGenfun

#endif



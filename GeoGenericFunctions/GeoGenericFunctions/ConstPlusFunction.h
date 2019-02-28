//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstPlusFunction-------------------------------//
//                                                                          //
// ConstPlusFunction, result of adding a function and a constant            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstPlusFunction_hh 
#define ConstPlusFunction_hh 
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class ConstPlusFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstPlusFunction)
  
      public:
  
    // Constructor
    ConstPlusFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstPlusFunction(const ConstPlusFunction &right);

    // Destructor
    virtual ~ConstPlusFunction();
  
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

    // It is illegal to assign a ConstPlusFunction
    const ConstPlusFunction & operator=(const ConstPlusFunction &right);

    double             _constant;
    const AbsFunction *_arg;
  };
} // namespace GeoGenfun
#endif

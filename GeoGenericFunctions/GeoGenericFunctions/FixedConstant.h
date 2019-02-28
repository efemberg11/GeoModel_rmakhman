//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//----------------------FixedConstant --------------------------------------//
//                                                                          //
//  Class FixedConstant                                                     //
//                                                                          //
//  FixedConstant allows use to treat "promote" fixed contants to constant  //
//  functions so that they automatically will inherit all the algebraic     //
//  operations we have so painstakingly defined for functions.              //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FixedConstant_h
#define FixedConstant_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FixedConstant : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(FixedConstant)

      public:

    // Constructor
    FixedConstant(double value);
  
    // Copy constructor
    FixedConstant(const FixedConstant &right);
  
    // Destructor
    virtual ~FixedConstant();
  
    // Retrieve function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  private:

    // It is illegal to assign a fixed constant
    const FixedConstant & operator=(const FixedConstant &right);

    // The value of the constant:
    double _value;
  };
} // namespace GeoGenfun
#endif

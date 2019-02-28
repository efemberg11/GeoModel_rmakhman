//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------Sqrt--------- --------------------------------------//
//                                                                          //
//  Class Sqrt                                                              //
//                                                                          //
//  Sqrt is a function that returns the square root of the argument         //
//--------------------------------------------------------------------------//
#ifndef Sqrt_h
#define Sqrt_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Sqrt : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sqrt)
  
      public:

    // Constructor
    Sqrt();
  
    // Copy constructor
    Sqrt(const Sqrt &right);
  
    // Destructor
    virtual ~Sqrt();
  
    // Retrieve function value
    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Sqrt & operator=(const Sqrt &right);

  };
} // namespace GeoGenfun
#endif



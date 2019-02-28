//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Theta------------------------------------------------//
//                                                                          //
// Class Theta, also known as the heavyside step function.  The derivative  //
// is zero.  Everywhere, except at the origin.  However QatGenericFuctions  //
// cannot handle the delta-function, so in this implementation the          //
// derivative is zero, everywhere.                                          //
//--------------------------------------------------------------------------//
#ifndef Theta_h
#define Theta_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Theta : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Theta)

      public:

    // Constructor
    Theta();

    // Destructor
    virtual ~Theta();
  
    // Copy constructor
    Theta(const Theta &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  
  private:

    // It is illegal to assign a Theta
    const Theta & operator=(const Theta &right);

  };
} // namespace GeoGenfun

#endif

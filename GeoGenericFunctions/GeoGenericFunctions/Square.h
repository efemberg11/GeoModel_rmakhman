//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//--------------------------------------------------------------------------//
//  Class Square                                                            //
//  Square(x) returns x*x.                                                  //
//--------------------------------------------------------------------------//
#ifndef Square_h
#define Square_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Square : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Square)
  
      public:

    // Constructor
    Square();
  
    // Copy constructor
    Square(const Square &right);
  
    // Destructor
    virtual ~Square();
  
    // Retrieve function value
    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Square & operator=(const Square &right);

  };
} // namespace GeoGenfun
#endif

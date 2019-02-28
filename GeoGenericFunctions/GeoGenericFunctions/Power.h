//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------Power-----------------------------------------------//
//                                                                          //
//  Class Power                                                             //
//                                                                          //
//  Power(x) returns x^n                                                    //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Power_h
#define Power_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Power : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Power)
  
      public:

    // Constructor
    Power(double n);
    Power(int n);
    Power(unsigned int n);
  
    // Copy constructor
    Power(const Power &right);
  
    // Destructor
    virtual ~Power();
  
    // Retrieve function value

    virtual double operator ()(double argument) const; 
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
  
    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Power & operator=(const Power &right);

    double _doublePower; // power (as a double)
    int    _intPower;    // power (as an integer)
    bool   _asInteger;   // flag:  object constructed with integer argument

  };
} // namespace GeoGenfun
#endif

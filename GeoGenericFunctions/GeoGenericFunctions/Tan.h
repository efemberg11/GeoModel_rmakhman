//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Tan--------------------------------------------------//
//                                                                          //
// Class Tan                                                                //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Tan_h
#define Tan_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Tan : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Tan)

      public:

    // Constructor
    Tan();

    // Destructor
    virtual ~Tan();
  
    // Copy constructor
    Tan(const Tan &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  
  private:

    // It is illegal to assign a Tan
    const Tan & operator=(const Tan &right);

  };
} // namespace GeoGenfun

#endif

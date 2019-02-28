//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Cos--------------------------------------------------//
//                                                                          //
// Class Cos                                                                //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Cos_h
#define Cos_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Cos : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Cos)

      public:

    // Constructor
    Cos();

    // Destructor
    virtual ~Cos();
  
    // Copy constructor
    Cos(const Cos &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  private:

    // It is illegal to assign a Cosine
    const Cos & operator=(const Cos &right);

  };
} // namespace GeoGenfun
#endif

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Sin--------------------------------------------------//
//                                                                          //
// Class Sin                                                                //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef Sin_h
#define Sin_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Sin : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sin)

      public:

    // Constructor
    Sin();

    // Destructor
    virtual ~Sin();
  
    // Copy constructor
    Sin(const Sin &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  
  private:

    // It is illegal to assign a Sin
    const Sin & operator=(const Sin &right);

  };
} // namespace GeoGenfun

#endif

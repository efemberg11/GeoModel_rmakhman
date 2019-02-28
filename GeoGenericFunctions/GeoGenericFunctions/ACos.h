//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------ACos-------------------------------------------------//
//                                                                          //
// Class ACos (arc cos)                                                     //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef ACos_h
#define ACos_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class ACos : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ACos)

      public:

    // Constructor
    ACos();

    // Destructor
    virtual ~ACos();
  
    // Copy constructor
    ACos(const ACos &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return true;}
  
  private:

    // It is illegal to assign a ACosine
    const ACos & operator=(const ACos &right);

  };

} // end namespace GeoGenfun

#endif

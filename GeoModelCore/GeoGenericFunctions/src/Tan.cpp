//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Tan.h"
#include "GeoGenericFunctions/Cos.h"
#include <stdexcept>
#include <cmath>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Tan)

Tan::Tan()
{}

Tan::~Tan() {
}

Tan::Tan(const Tan & right) : AbsFunction(right)
{  }


double Tan::operator() (double x) const {
  return tan(x);
}



Derivative Tan::partial(unsigned int index) const {

  if (index!=0) throw std::range_error("Tan: partial derivative index out of range");

  const AbsFunction & fPrime = 1.0/Cos()/Cos();
  return Derivative(& fPrime);
}

} // namespace GeoGenfun

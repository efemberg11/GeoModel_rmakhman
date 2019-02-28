//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Sin.h"
#include "GeoGenericFunctions/Cos.h"
#include <stdexcept>
#include <cmath>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Sin)

Sin::Sin()
{}

Sin::~Sin() {
}

Sin::Sin(const Sin & right) : AbsFunction(right)
{  }


double Sin::operator() (double x) const {
  return sin(x);
}



Derivative Sin::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Sin: partial derivative index out of range");
  const AbsFunction & fPrime = Cos();
  return Derivative(& fPrime);
}

} // namespace GeoGenfun

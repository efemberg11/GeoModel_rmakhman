//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Theta.h"
#include "GeoGenericFunctions/FixedConstant.h"
#include <stdexcept>
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Theta)

Theta::Theta()
{}

Theta::~Theta() {
}

Theta::Theta(const Theta & right) : AbsFunction(right)
{  }


double Theta::operator() (double x) const {
  return (x>=0) ? 1.0:0.0;
}



Derivative Theta::partial(unsigned int index) const {
  if (index!=0) throw std::runtime_error("Theta::Partial: index out of range");
  const AbsFunction & fPrime = FixedConstant(0.0);
  return Derivative(& fPrime);
}

} // namespace GeoGenfun

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Cos.h"
#include "GeoGenericFunctions/Sin.h"
#include <cmath>
#include <stdexcept>
namespace GeoGenfun {

FUNCTION_OBJECT_IMP(Cos)

Cos::Cos()
{}

Cos::~Cos() {
}

Cos::Cos(const Cos & right) : AbsFunction(right)
{  }


double Cos::operator() (double x) const {
  return cos(x);
}


Derivative Cos::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Cos:  partial derivative index ot of range");
  const AbsFunction & fPrime = -Sin();
  return Derivative(& fPrime);
}


} // namespace GeoGenfun

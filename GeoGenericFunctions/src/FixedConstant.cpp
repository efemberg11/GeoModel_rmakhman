//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FixedConstant.h"
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FixedConstant)

FixedConstant::FixedConstant(double value):
_value(value)
{
}

FixedConstant::FixedConstant(const FixedConstant & right):
AbsFunction(right), _value(right._value) {
}

FixedConstant::~FixedConstant()
{
}

double FixedConstant::operator ()(double) const
{
  return _value;
}

Derivative FixedConstant::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("FixedConstant: partial derivative index out of range");
  FixedConstant fPrime(0.0);
  return Derivative(&fPrime);
}

} // namespace GeoGenfun

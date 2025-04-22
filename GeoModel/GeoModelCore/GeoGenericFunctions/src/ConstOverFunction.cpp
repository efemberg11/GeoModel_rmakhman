//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ConstOverFunction.h"

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ConstOverFunction)

ConstOverFunction::ConstOverFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstOverFunction::ConstOverFunction(const ConstOverFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstOverFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstOverFunction::~ConstOverFunction()
{
  delete _arg;
}



double ConstOverFunction::operator ()(double x) const
{
  return _constant / (*_arg)(x);
}


double ConstOverFunction::operator ()(const Argument & x) const
{
  return _constant / (*_arg)(x);
}


Derivative ConstOverFunction::partial(unsigned int index) const {
  // d/dx (k/f) = -(k/f^2)(df/dx)
  Derivative d=_arg->partial(index);
  const AbsFunction & fPrime = -_constant/(*_arg)/(*_arg)*(d);
  return Derivative(& fPrime);
}


} // namespace GeoGenfun

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ConstTimesFunction.h"
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ConstTimesFunction)

ConstTimesFunction::ConstTimesFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstTimesFunction::ConstTimesFunction(const ConstTimesFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstTimesFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstTimesFunction::~ConstTimesFunction()
{
  delete _arg;
}



double ConstTimesFunction::operator ()(double x) const
{
  return _constant * (*_arg)(x);
}


double ConstTimesFunction::operator ()(const Argument & x) const
{
  return _constant * (*_arg)(x);
}

  Derivative ConstTimesFunction::partial(unsigned int index) const {
    // d/dx (k*f) = k*(df/dx)
    const Derivative & d=_arg->partial(index);
    const AbsFunction & fPrime = _constant*d;
    return Derivative(& fPrime);
  }
  
} // namespace GeoGenfun

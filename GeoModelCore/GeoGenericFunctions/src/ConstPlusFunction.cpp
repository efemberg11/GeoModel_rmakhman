//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ConstPlusFunction.h"
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ConstPlusFunction)

ConstPlusFunction::ConstPlusFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstPlusFunction::ConstPlusFunction(const ConstPlusFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstPlusFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstPlusFunction::~ConstPlusFunction()
{
  delete _arg;
}



double ConstPlusFunction::operator ()(double x) const
{
  return _constant + (*_arg)(x);
}


double ConstPlusFunction::operator ()(const Argument & x) const
{
  return _constant + (*_arg)(x);
}


Derivative ConstPlusFunction::partial(unsigned int index) const {
  const Derivative & d=_arg->partial(index);
  return d;
}


} // namespace GeoGenfun

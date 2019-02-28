//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ConstMinusFunction.h"

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ConstMinusFunction)

ConstMinusFunction::ConstMinusFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstMinusFunction::ConstMinusFunction(const ConstMinusFunction & right) :
AbsFunction(right),
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstMinusFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstMinusFunction::~ConstMinusFunction()
{
  delete _arg;
}



double ConstMinusFunction::operator ()(double x) const
{
  return _constant - (*_arg)(x);
}


double ConstMinusFunction::operator ()(const Argument & x) const
{
  return _constant - (*_arg)(x);
}


Derivative ConstMinusFunction::partial(unsigned int index) const {
  const Derivative & d=_arg->partial(index);
  const AbsFunction & fPrime = -d;
  return Derivative(& fPrime);
}


} // namespace GeoGenfun

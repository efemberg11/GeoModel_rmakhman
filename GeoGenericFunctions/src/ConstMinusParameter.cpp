//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ConstMinusParameter.h"
#include "GeoGenericFunctions/Parameter.h"

namespace GeoGenfun {
PARAMETER_OBJECT_IMP(ConstMinusParameter)


ConstMinusParameter::ConstMinusParameter(double xconstant, const AbsParameter *aparm):
  _constant(xconstant),
  _parameter(aparm->clone())
{
  if (aparm->parameter() && _parameter->parameter()) _parameter->parameter()->connectFrom(aparm->parameter());
}

ConstMinusParameter::ConstMinusParameter(const ConstMinusParameter & right) :
AbsParameter(right),
_constant(right._constant),
_parameter(right._parameter->clone())
{}


ConstMinusParameter::~ConstMinusParameter()
{
  delete _parameter;
}


double ConstMinusParameter::getValue() const {
  return _constant - _parameter->getValue();
}



} // namespace GeoGenfun

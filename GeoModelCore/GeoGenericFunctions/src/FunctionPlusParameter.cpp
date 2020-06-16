//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionPlusParameter.h"
#include "GeoGenericFunctions/Parameter.h"
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionPlusParameter)

FunctionPlusParameter::FunctionPlusParameter(const AbsParameter *parameter, const AbsFunction *function):
  _function(function->clone()),
  _parameter(parameter->clone())
{
  if (parameter->parameter() && _parameter->parameter()) {
    _parameter->parameter()->connectFrom(parameter->parameter());
  }
}

FunctionPlusParameter::FunctionPlusParameter(const FunctionPlusParameter & right) :
  AbsFunction(right),
  _function(right._function->clone()),
  _parameter(right._parameter->clone())
{}

unsigned int FunctionPlusParameter::dimensionality() const {
  return _function->dimensionality();
}

FunctionPlusParameter::~FunctionPlusParameter()
{
  delete _function;
  delete _parameter;
}


double FunctionPlusParameter::operator ()(double x) const
{
  return _parameter->getValue() + (*_function)(x);
}


double FunctionPlusParameter::operator ()(const Argument & x) const
{
  return _parameter->getValue() + (*_function)(x);
}

Derivative FunctionPlusParameter::partial(unsigned int index) const {
  const Derivative & d=_function->partial(index);
  return d;
}

} // namespace GeoGenfun

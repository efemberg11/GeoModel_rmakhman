//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionTimesParameter.h"
#include "GeoGenericFunctions/Parameter.h"
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionTimesParameter)

FunctionTimesParameter::FunctionTimesParameter(const AbsParameter *parameter, const AbsFunction *function):
  _function(function->clone()),
  _parameter(parameter->clone())
{
  if (parameter->parameter() && _parameter->parameter()) {
    _parameter->parameter()->connectFrom(parameter->parameter());
  }
}

FunctionTimesParameter::FunctionTimesParameter(const FunctionTimesParameter & right) :
  AbsFunction(right),
  _function(right._function->clone()),
  _parameter(right._parameter->clone())
{}

unsigned int FunctionTimesParameter::dimensionality() const {
  return _function->dimensionality();
}

FunctionTimesParameter::~FunctionTimesParameter()
{
  delete _function;
  delete _parameter;
}


double FunctionTimesParameter::operator ()(double x) const
{
  return _parameter->getValue() * (*_function)(x);
}


double FunctionTimesParameter::operator ()(const Argument & x) const
{
  return _parameter->getValue() * (*_function)(x);
}




Derivative FunctionTimesParameter::partial(unsigned int index) const {
  const Derivative & d=_function->partial(index);
  const AbsFunction & fPrime  = (*_parameter)*d;
  return Derivative(&fPrime);
}

} // namespace GeoGenfun

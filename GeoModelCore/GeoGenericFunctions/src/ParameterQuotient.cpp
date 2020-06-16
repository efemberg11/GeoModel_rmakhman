//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ParameterQuotient.h"
#include "GeoGenericFunctions/Parameter.h"

namespace GeoGenfun {
PARAMETER_OBJECT_IMP(ParameterQuotient)

ParameterQuotient::ParameterQuotient(const AbsParameter *arg1, const AbsParameter *arg2):
  _arg1(arg1->clone()),
  _arg2(arg2->clone())
{
  if (arg1->parameter() && _arg1->parameter()) _arg1->parameter()->connectFrom(arg1->parameter());
  if (arg2->parameter() && _arg2->parameter()) _arg2->parameter()->connectFrom(arg2->parameter());
}

ParameterQuotient::ParameterQuotient(const ParameterQuotient & right) :
AbsParameter(),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone())
{}


ParameterQuotient::~ParameterQuotient()
{
  delete _arg1;
  delete _arg2;
}


double ParameterQuotient::getValue() const {
  return _arg1->getValue()/_arg2->getValue();
}

} // namespace GeoGenfun

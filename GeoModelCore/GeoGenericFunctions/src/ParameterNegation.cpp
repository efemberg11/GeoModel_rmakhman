//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ParameterNegation.h"
#include "GeoGenericFunctions/Parameter.h"

namespace GeoGenfun {
PARAMETER_OBJECT_IMP(ParameterNegation)

ParameterNegation::ParameterNegation(const AbsParameter *arg1):
  _arg1(arg1->clone())
{
  if (arg1->parameter() && _arg1->parameter()) _arg1->parameter()->connectFrom(arg1->parameter());
}

ParameterNegation::ParameterNegation(const ParameterNegation & right) :
AbsParameter(),
_arg1(right._arg1->clone())
{}


ParameterNegation::~ParameterNegation()
{
  delete _arg1;
}


double ParameterNegation::getValue() const {
  return - _arg1->getValue();
}

} // namespace GeoGenfun

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/ParameterComposition.h"
#include "GeoGenericFunctions/Parameter.h"
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {
PARAMETER_OBJECT_IMP(ParameterComposition)

ParameterComposition::ParameterComposition(const AbsFunction *arg1, const AbsParameter *arg2):
  AbsParameter(),
  _arg1(arg1->clone()),
  _arg2(arg2->clone())
{
  if (arg2->parameter() && _arg2->parameter()) _arg2->parameter()->connectFrom(arg2->parameter());
}

ParameterComposition::ParameterComposition(const ParameterComposition & right) :
AbsParameter(),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone())
{}


ParameterComposition::~ParameterComposition()
{
  delete _arg1;
  delete _arg2;
}


double ParameterComposition::getValue() const {
  return _arg1->operator() (_arg2->getValue());
}

} // namespace GeoGenfun

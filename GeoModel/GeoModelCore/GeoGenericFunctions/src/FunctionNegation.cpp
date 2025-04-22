//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionNegation.h"

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionNegation)

FunctionNegation::FunctionNegation(const AbsFunction *arg1):
  _arg1(arg1->clone())
{
}

FunctionNegation::FunctionNegation(const FunctionNegation & right):
  AbsFunction(right),
  _arg1(right._arg1->clone())
{
}


FunctionNegation::~FunctionNegation()
{
  delete _arg1;
}

unsigned int FunctionNegation::dimensionality() const {
  return _arg1->dimensionality();
}


double FunctionNegation::operator ()(double x) const
{
  return -((*_arg1)(x));
}

double FunctionNegation::operator ()(const Argument & x) const
{
  return -((*_arg1)(x));
}


Derivative FunctionNegation::partial(unsigned int index) const {
  const Derivative & d = _arg1->partial(index);
  const AbsFunction & fPrime  = -d;
  return Derivative(&fPrime);
}

  
} // namespace GeoGenfun

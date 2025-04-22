//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionNoop.h"

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionNoop)

FunctionNoop::FunctionNoop(const AbsFunction *arg1):
_arg1(arg1->clone())
{
}

FunctionNoop::FunctionNoop(const FunctionNoop & right):
  AbsFunction(right), _arg1(right._arg1->clone())
{
}


FunctionNoop::~FunctionNoop()
{
  delete _arg1;
}

unsigned int FunctionNoop::dimensionality() const {
  return _arg1->dimensionality();
}


double FunctionNoop::operator ()(double x) const
{
  return +((*_arg1)(x));
}

double FunctionNoop::operator ()(const Argument & x) const
{
  return +((*_arg1)(x));
}


Derivative FunctionNoop::partial(unsigned int index) const {
  return _arg1->partial(index);
}

} // namespace GeoGenfun

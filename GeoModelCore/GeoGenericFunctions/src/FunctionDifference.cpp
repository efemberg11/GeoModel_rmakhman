//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionDifference.h"
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionDifference)

FunctionDifference::FunctionDifference(const AbsFunction *arg1, const AbsFunction *arg2):
  _arg1(arg1->clone()),
  _arg2(arg2->clone())
{
  if (arg1->dimensionality()!=arg2->dimensionality()) {
    throw std::runtime_error("FunctionDifference: dimension mismatch");
  }
}


FunctionDifference::FunctionDifference(const FunctionDifference & right):
  AbsFunction(right),
  _arg1(right._arg1->clone()),
  _arg2(right._arg2->clone())
{
}


unsigned int FunctionDifference::dimensionality() const {
  return _arg1->dimensionality();
}

FunctionDifference::~FunctionDifference()
{
  delete _arg1;
  delete _arg2;
}



double FunctionDifference::operator ()(double x) const
{
  return (*_arg1)(x)-(*_arg2)(x);
}

double FunctionDifference::operator ()(const Argument & x) const
{
  return (*_arg1)(x)-(*_arg2)(x);
}


Derivative FunctionDifference::partial(unsigned int index) const {
  const Derivative & d1=_arg1->partial(index);
  const Derivative & d2=_arg2->partial(index);
  const AbsFunction & fPrime = d1-d2;
  return Derivative(&fPrime);
}



} // namespace GeoGenfun

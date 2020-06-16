//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionSum.h"
#include <stdexcept>
    

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionSum)

FunctionSum::FunctionSum(const AbsFunction *arg1, const AbsFunction *arg2):
  _arg1(arg1->clone()),
  _arg2(arg2->clone())
{
  if (arg1->dimensionality()!=arg2->dimensionality()) {
    throw std::runtime_error ("FunctionSum:  dimension mismatch");
  }
}

FunctionSum::FunctionSum(const FunctionSum & right) :
AbsFunction(right),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone())
{}

unsigned int FunctionSum::dimensionality() const {
  return _arg1->dimensionality();
}

FunctionSum::~FunctionSum()
{
  delete _arg1;
  delete _arg2;
}



double FunctionSum::operator ()(double x) const
{
  return (*_arg1)(x)+(*_arg2)(x);
}


double FunctionSum::operator ()(const Argument & x) const
{
  return (*_arg1)(x)+(*_arg2)(x);
}



Derivative FunctionSum::partial(unsigned int index) const {
  const Derivative & d1=_arg1->partial(index);
  const Derivative & d2=_arg2->partial(index);
  const AbsFunction & fPrime  = d1+d2;
  return Derivative(&fPrime);
}



} // namespace GeoGenfun

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/FunctionDirectProduct.h"
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(FunctionDirectProduct)

FunctionDirectProduct::FunctionDirectProduct(const AbsFunction *arg1, const AbsFunction *arg2):
_arg1(arg1->clone()),
_arg2(arg2->clone()),
_m(arg1->dimensionality()),
_n(arg2->dimensionality())
{
}

FunctionDirectProduct::FunctionDirectProduct(const FunctionDirectProduct & right):
AbsFunction(right),
_arg1(right._arg1->clone()),
_arg2(right._arg2->clone()),
_m(right._m),
_n(right._n)
{
}


FunctionDirectProduct::~FunctionDirectProduct()
{
  delete _arg1;
  delete _arg2;
}


double FunctionDirectProduct::operator() (const Argument & a) const {
  unsigned int P = a.dimension();
  Argument x1(_m);
  Argument x2(_n);
  if (_m+_n != P) {
    throw std::runtime_error("FunctionDirectProduct:  dimension mismatch");
    return 0;
  }
  for (unsigned int i = 0; i<_m;i++) {
    x1[i]=a[i];
  }
  for (unsigned int j = 0;j<_n;j++) {
    x2[j]=a[j+_m];
  }
  return (*_arg1)(x1) * (*_arg2)(x2);
}

unsigned int FunctionDirectProduct::dimensionality() const {
  return _m+_n;
}

double FunctionDirectProduct::operator ()(double) const
{
  throw std::runtime_error("FunctionDirectProduct:  evaluation with scalar argument");
  return 0;
}



Derivative FunctionDirectProduct::partial(unsigned int index) const {
  if (index>=(_m+_n)) throw std::range_error ("FunctionDirectProduct:  partial derivative index out of range");
  if (index<_m) {
    const Derivative & d1=_arg1->partial(index);
    const AbsFunction & fPrime = d1%(*_arg2);
    return Derivative(&fPrime);
  }
  else {
    const Derivative &d2=_arg2->partial(index-_m);
    const AbsFunction & fPrime = (*_arg1)%d2;
    return Derivative(&fPrime);
  }
}


} // namespace GeoGenfun

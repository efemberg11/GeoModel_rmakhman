//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Pi.h"
#include "GeoGenericFunctions/Sigma.h"
#include "GeoGenericFunctions/FixedConstant.h"
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Pi)

void Pi::accumulate( const AbsFunction & fcn) {
  _fcn.push_back(fcn.clone());
}

Pi::Pi()
{
}

Pi::Pi(const Pi & right) : AbsFunction(right)
{
  for (size_t i=0; i<right._fcn.size();i++) {
    _fcn.push_back(right._fcn[i]->clone());
  }
}

unsigned int Pi::dimensionality() const {
  if (_fcn.size()==0) throw std::runtime_error("Cannot determine dimensionality from empty product");
  return _fcn[0]->dimensionality();
}

Pi::~Pi()
{
  for (size_t i=0; i<_fcn.size();i++) {
    delete _fcn[i];
  }
}

double Pi::operator ()(double x) const
{
  double retVal=1.0;
  for (size_t i=0;i<_fcn.size();i++) retVal *= (*_fcn[i])(x);
  return retVal;
}


double Pi::operator ()(const Argument & x) const
{
  double retVal=1.0;
  for (size_t i=0;i<_fcn.size();i++) retVal *= (*_fcn[i])(x);
  return retVal;
}



Derivative Pi::partial(unsigned int index) const {

  Sigma fPrime;
  fPrime.accumulate(FixedConstant(0.0));
  for (size_t i=0;i<_fcn.size();i++) {
    Pi subproductI;
    subproductI.accumulate(FixedConstant(1.0));
    for (size_t j=0;j<_fcn.size();j++) {
      if (j!=i) subproductI.accumulate(*_fcn[i]);
    }
    fPrime.accumulate(_fcn[i]->partial(index)*subproductI);
  }
  return Derivative(&fPrime);
}



} // namespace GeoGenfun

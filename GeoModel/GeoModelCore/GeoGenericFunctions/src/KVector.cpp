//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/KVector.h"
#include "GeoGenericFunctions/FixedConstant.h"
#include <stdexcept>
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(KVector)

KVector::KVector(unsigned int dmsnlty, double val):
  _dimensionality(dmsnlty),
  _value(val)
{}

KVector::KVector(const KVector & right):
  AbsFunction(right),
  _dimensionality(right._dimensionality),
  _value(right._value)
{
}

KVector::~KVector() {
}

double KVector::operator() (double ) const {
  if (_dimensionality!=1) throw std::runtime_error("GeoGenfun::KVector: dimensionality !=1") ;
  return _value;
}

double KVector::operator () (const Argument & a) const {
 
  if  (_dimensionality!=a.dimension()) {
    throw std::runtime_error("GeoGenfun::KVector dimensionality mismatch");
  }
  return _value;
}



Derivative KVector::partial(unsigned int ) const {

  KVector vec(_dimensionality,0.0);
  return Derivative(&vec);
}

unsigned int KVector::dimensionality() const {
  return _dimensionality;
} 

} // namespace GeoGenfun

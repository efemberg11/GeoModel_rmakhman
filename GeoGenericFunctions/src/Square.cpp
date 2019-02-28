//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Square.h"
#include "GeoGenericFunctions/Variable.h"
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Square)

Square::Square()
{}

Square::Square(const Square & right) : AbsFunction(right)
{
}

Square::~Square() {
}

double Square::operator() (double x) const {
  return x*x;
}



Derivative Square::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Square: partial derivative index out of range"); 
  Variable x;
  const AbsFunction & fPrime = 2*x;
  return Derivative(&fPrime);
}


} // namespace GeoGenfun

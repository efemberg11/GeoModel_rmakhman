//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/ACos.h"
#include "GeoGenericFunctions/Sqrt.h"
#include "GeoGenericFunctions/Square.h"
#include <cmath>      
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ACos)

ACos::ACos()
{}

ACos::~ACos() {
}

ACos::ACos(const ACos & right) : AbsFunction(right)
{  }


double ACos::operator() (double x) const {
  return acos(x);
}


Derivative ACos::partial(unsigned int index) const {
 if (index!=0) throw std::range_error("ACos: partial derivative index out of range");

  Square square;
  Sqrt   root;

  const AbsFunction & fPrime = - 1.0/root(1.0-square) ;
  return Derivative(& fPrime);
}

} // end namespace GeoGenfun



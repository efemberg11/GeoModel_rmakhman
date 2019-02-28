//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/ASin.h"
#include "GeoGenericFunctions/Sqrt.h"
#include "GeoGenericFunctions/Square.h"
#include <cmath>     
#include <stdexcept>

namespace GeoGenfun {
FUNCTION_OBJECT_IMP(ASin)

ASin::ASin()
{}

ASin::~ASin() {
}

ASin::ASin(const ASin & right) : AbsFunction(right)
{  }


double ASin::operator() (double x) const {
  return asin(x);
}



Derivative ASin::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("ASin: partial derivative index out of range");

  Square square;
  Sqrt   root;

  const AbsFunction & fPrime =  1.0/root(1.0-square) ;
  return Derivative(& fPrime);
}

} // end namespace GeoGenFun

//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Mod.h"
#include <cmath>
#include <climits>
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Mod)

Mod::Mod(double y):
AbsFunction(),
  _y(y)
{}

Mod::Mod(const Mod & right)
  : AbsFunction(right), _y(right._y)
{}

Mod::~Mod() {
}


double Mod::operator() (double x) const {
  return (x - _y*floor(x/_y));
}

} // namespace GeoGenfun

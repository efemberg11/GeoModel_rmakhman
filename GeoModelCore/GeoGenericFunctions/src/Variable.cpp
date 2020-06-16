//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/KVector.h"
#include <stdexcept>
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Variable)

Variable::Variable(unsigned int selectionIndex, unsigned int dmsnlty):
  _selectionIndex(selectionIndex),
  _dimensionality(dmsnlty)
{}

Variable::Variable(const Variable & right):
  AbsFunction(right),
  _selectionIndex(right._selectionIndex),
  _dimensionality(right._dimensionality)
{
}

Variable::~Variable() {
}

double Variable::operator() (double x) const {
  if (_selectionIndex!=0) throw std::runtime_error("GeoGenfun::Variable: selection index !=0") ;
  return x;
}

double Variable::operator () (const Argument & a) const {
  if  (!(_selectionIndex<a.dimension())) throw std::runtime_error("GeoGenfun::Varaible selection index out of bounds");
  return a[_selectionIndex];
}

unsigned int Variable::index() const {
  return _selectionIndex;
}


Derivative Variable::partial(unsigned int mindex) const {
  double kroneckerDelta = mindex==_selectionIndex ? 1.0 : 0.0;
  KVector vec(_dimensionality,kroneckerDelta);
  return Derivative(&vec);
}

unsigned int Variable::dimensionality() const {
  return _dimensionality;
} 

} // namespace GeoGenfun

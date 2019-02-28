//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/ArrayFunction.h"
#include "GeoGenericFunctions/FixedConstant.h"

namespace GeoGenfun {

FUNCTION_OBJECT_IMP(ArrayFunction)
  
// Constructor
ArrayFunction::ArrayFunction(const double *begin, const double *end) :
_values(begin,end) {}
  
// Initializer list constructor:
  ArrayFunction::ArrayFunction(std::initializer_list<double> values):
    _values(values)
  {
  }

// Destructor
ArrayFunction::~ArrayFunction(){}
  
// Copy constructor
ArrayFunction::ArrayFunction(const ArrayFunction &right):
  AbsFunction(right), _values(right._values) {}
  
  // Retreive function value
double ArrayFunction::operator ()(double argument) const {
  int i =  int (argument+0.5);
  if (i<0 || i>=int(_values.size())) return 0;
  else return _values[i];
}
Derivative ArrayFunction::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("ArrayFunction: partial derivative index out of range");
  FixedConstant fPrime(0.0);
  return Derivative(&fPrime);
}

}

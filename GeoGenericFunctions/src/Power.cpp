//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#include "GeoGenericFunctions/Power.h"
#include <cmath>      // for pow()
#include <stdexcept>
namespace GeoGenfun {
FUNCTION_OBJECT_IMP(Power)

Power::Power(int n):
    _intPower(n),
    _asInteger(true)
{}

Power::Power(unsigned int n):
    _intPower(n),
    _asInteger(true)
{}

Power::Power(double n):
    _doublePower(n),
    _asInteger(false)
{}

Power::Power(const Power & right)
    : AbsFunction(right),
      _doublePower(right._doublePower),
      _intPower(right._intPower),
      _asInteger(right._asInteger)
{}

Power::~Power() {
}

double Power::operator() (double x) const {
    if (_asInteger) {
	if (_intPower==0) {
	    return 1;
	}
	else if (_intPower>0) {
	    double f = 1;
	    for (int i=0;i<_intPower;i++) {
		f *=x;
	    }
	    return f;
	}
	else {
	    double f = 1;
	    for (int i=0;i<-_intPower;i++) {
		f /=x;
	    }
	    return f;
	}	    
    }
    else {
	return std::pow(x,_doublePower);
    }

}



Derivative Power::partial(unsigned int index) const {
  if (index!=0) throw std::range_error("Power:  partial derivative index out of range");
  if (_asInteger) {
    const AbsFunction & fPrime = _intPower*Power(_intPower-1);
    return Derivative(&fPrime);
  }
  else {
    const AbsFunction & fPrime = _doublePower*Power(_doublePower-1);
    return Derivative(&fPrime);
  }

}


} // namespace GeoGenfun

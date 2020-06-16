//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/ConstTimesFunction.h"
#include "GeoGenericFunctions/ConstOverFunction.h"
#include "GeoGenericFunctions/ConstPlusFunction.h"
#include "GeoGenericFunctions/ConstMinusFunction.h"
#include "GeoGenericFunctions/FunctionSum.h"
#include "GeoGenericFunctions/FunctionDifference.h"
#include "GeoGenericFunctions/FunctionProduct.h"
#include "GeoGenericFunctions/FunctionQuotient.h"
#include "GeoGenericFunctions/FunctionNegation.h"
#include "GeoGenericFunctions/FunctionDirectProduct.h"
#include "GeoGenericFunctions/FixedConstant.h"
#include "GeoGenericFunctions/FunctionNumDeriv.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/FunctionNoop.h"
#include "GeoGenericFunctions/ParameterComposition.h"
#include <stdexcept>
namespace GeoGenfun {
AbsFunction::AbsFunction() {
}

AbsFunction::~AbsFunction() {
}

AbsFunction::AbsFunction(const AbsFunction &) {
}

FunctionComposition AbsFunction::operator () (const AbsFunction &function) const {
  return FunctionComposition(this, &function);
}

ParameterComposition AbsFunction::operator() (const AbsParameter &p) const {
  return ParameterComposition(this, &p);
}

Derivative AbsFunction::partial(unsigned int index) const {
  FunctionNumDeriv fPrime(this,index);
  return Derivative(&fPrime);
}


Derivative AbsFunction::partial(const Variable &v) const {
  return partial(v.index());
}

Derivative AbsFunction::prime() const {
  if (dimensionality()!=1) throw std::runtime_error("AbsFunction::prime() called for a function of more than one variable");
  return partial(0);
}

FunctionSum operator + (const AbsFunction & a, const AbsFunction & b) {
  return FunctionSum(&a,&b);
}

FunctionDifference operator - (const AbsFunction & a, const AbsFunction & b) {
  return FunctionDifference(&a,&b);
}

FunctionProduct operator * (const AbsFunction & a, const AbsFunction & b) {
  return FunctionProduct(&a,&b);
}

FunctionQuotient operator / (const AbsFunction & a, const AbsFunction & b) {
  return FunctionQuotient(&a,&b);
}


FunctionNegation operator - (const AbsFunction & a) {
  return FunctionNegation(&a);
}

unsigned int AbsFunction::dimensionality() const {
  return 1;
}

FunctionDirectProduct operator % (const AbsFunction & a, const AbsFunction & b) {
  return FunctionDirectProduct(&a,&b);
}

ConstTimesFunction operator * (const AbsFunction &op2, double c) {
  return ConstTimesFunction(c, &op2);
}

ConstPlusFunction  operator + (const AbsFunction &op2, double c) {
  return ConstPlusFunction(c,&op2);
}

ConstPlusFunction  operator - (const AbsFunction &op2, double c) {
  return ConstPlusFunction(-c, &op2);
}

ConstTimesFunction operator / (const AbsFunction &op2, double c) {
  return ConstTimesFunction(1/c,&op2);
}


ConstTimesFunction           operator * (double c, const AbsFunction &op2) {
  return ConstTimesFunction(c,&op2);
}

ConstPlusFunction               operator + (double c, const AbsFunction &op2) {
  return ConstPlusFunction(c,&op2);
}

ConstMinusFunction        operator - (double c, const AbsFunction &op2) {
  return ConstMinusFunction(c,&op2);
}

ConstOverFunction          operator / (double c, const AbsFunction &op2) {
  return ConstOverFunction(c,&op2);
}


FunctionTimesParameter operator * (const AbsFunction &f, const AbsParameter & p) {
  return FunctionTimesParameter(&p, &f);
}

FunctionPlusParameter operator + (const AbsFunction &f, const AbsParameter & p) {
  return FunctionPlusParameter(&p, &f);
}

FunctionPlusParameter operator - (const AbsFunction &f, const AbsParameter & p) {
  GENPARAMETER MinusP = -p;
  return FunctionPlusParameter(&MinusP, &f);
  
}

FunctionTimesParameter operator / (const AbsFunction &f, const AbsParameter & p) {
  GENPARAMETER oneOverP = 1.0/p;
  return FunctionTimesParameter(&oneOverP, &f);
}

FunctionTimesParameter operator * (const AbsParameter & p, const AbsFunction &f) {
  return FunctionTimesParameter(&p, &f);
}

FunctionPlusParameter operator + (const AbsParameter & p, const AbsFunction &f) {
  return FunctionPlusParameter(&p, &f);
}

FunctionPlusParameter operator - (const AbsParameter & p, const AbsFunction &f) {
  GENFUNCTION MinusF = -f;
  return FunctionPlusParameter(&p, &MinusF);
}

FunctionTimesParameter operator / (const AbsParameter & p, const AbsFunction &f) {
  GENFUNCTION oneOverF = 1.0/f;
  return FunctionTimesParameter(&p, &oneOverF);
}
} // namespace GeoGenfun

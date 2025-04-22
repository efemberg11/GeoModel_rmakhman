//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

#include "GeoGenericFunctions/AbsParameter.h"
#include "GeoGenericFunctions/Parameter.h"
namespace GeoGenfun {

// Two do-nothing functions:
AbsParameter::AbsParameter() {
}
  
AbsParameter::~AbsParameter(){
}

AbsParameter::AbsParameter(const AbsParameter &)
{}

AbsParameter *AbsParameter::clone() const {
  return _clone();
}

  
ParameterSum operator + (const AbsParameter & a, const AbsParameter & b) {
  return ParameterSum(&a,&b);
}

ParameterDifference operator - (const AbsParameter & a, const AbsParameter & b) {
  return ParameterDifference(&a,&b);
}

ParameterProduct operator * (const AbsParameter & a, const AbsParameter & b) {
  return ParameterProduct(&a,&b);
}

ParameterQuotient operator / (const AbsParameter & a, const AbsParameter & b) {
  return ParameterQuotient(&a,&b);
}


ParameterNegation operator - (const AbsParameter & a) {
  return ParameterNegation(&a);
}


ConstTimesParameter           operator * (double c, const AbsParameter &op2){
  return ConstTimesParameter (c, &op2);
}

ConstPlusParameter               operator + (double c, const AbsParameter &op2){
  return ConstPlusParameter (c, &op2);
}

ConstMinusParameter        operator - (double c, const AbsParameter &op2){
  return ConstMinusParameter(c, &op2);
}

ConstOverParameter          operator / (double c, const AbsParameter &op2){
  return ConstOverParameter(c, &op2);
}

ConstTimesParameter           operator * (const AbsParameter &op2, double c){
  return ConstTimesParameter (c, &op2);
}

ConstPlusParameter               operator + (const AbsParameter &op2, double c){
  return ConstPlusParameter (c, &op2);
}

ConstPlusParameter        operator - (const AbsParameter &op2, double c){
  return ConstPlusParameter(-c, &op2);
}

ConstTimesParameter          operator / (const AbsParameter &op2, double c){
  return ConstTimesParameter(1/c, &op2);
}


} // namespace GeoGenfun

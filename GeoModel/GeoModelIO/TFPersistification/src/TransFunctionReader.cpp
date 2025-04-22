#include "TFPersistification/TransFunctionInterpreter.h"
#include "TFPersistification/GenFunctionInterpreter.h"
#include "TFPersistification/TransFunctionReader.h"

#include "GeoModelKernel/GeoXF.h"

#include "GeoGenericFunctions/FixedConstant.h"
#include "GeoGenericFunctions/FunctionProduct.h"
#include "GeoGenericFunctions/FunctionComposition.h"

#include <stdexcept>
#include <sstream>

TransFunctionReader::TransFunctionReader(const std::string & name, TransFunctionInterpreter * interpreter):
  interpreter(interpreter) {
  interpreter->add(name, this);
}

const TransFunctionInterpreter *TransFunctionReader::getInterpreter() const {
  return interpreter;
}

std::pair<std::string, std::string> TransFunctionReader::split(const std::string & arg) const {
  size_t comma=0;
  auto cSearch=arg.begin();
  while (1) {
    auto cComma=std::find(cSearch,arg.end(),'|');
    int cOpen=std::count(arg.begin(),cComma,'{');
    int cClose=std::count(arg.begin(),cComma,'}');
    comma=cComma-arg.begin();
    if (cOpen==cClose) break;
    cSearch = ++cComma;
  }
  std::string op1=arg.substr(0,comma);
  std::string op2=arg.substr(comma+1);
  return std::make_pair(op1,op2);
}
TransFunctionReader::~TransFunctionReader () {}

#include "GeoModelKernel/GeoDefinitions.h"

#include <iostream>
#include <typeinfo>


//===========Now come implementations of specific types of transfunction:
ProductReader::ProductReader(TransFunctionInterpreter *interpreter):TransFunctionReader("GeoXF::Product", interpreter) {}
PreMultReader::PreMultReader(TransFunctionInterpreter *interpreter):TransFunctionReader("GeoXF::PreMult", interpreter) {}
PostMultReader::PostMultReader(TransFunctionInterpreter *interpreter):TransFunctionReader("GeoXF::PostMult", interpreter) {}
PowReader::PowReader(TransFunctionInterpreter *interpreter):TransFunctionReader("GeoXF::Pow", interpreter) {}


TFPTR  ProductReader::execute(const std::string & arg,  std::deque<double> *fpData) const {
  auto pair=split(arg);
  TFPTR t1=getInterpreter()->interpret(pair.first,fpData);
  TFPTR t2= getInterpreter()->interpret(pair.second,fpData);
    
  return TFPTR(GeoXF::Product(t1.get(), t2.get()).clone());
}


TFPTR  PreMultReader::execute(const std::string & arg,  std::deque<double> *fpData) const {
  auto pair=split(arg);
  GeoTrf::Transform3D t1 = scanT(pair.first,fpData);
  TFPTR p2=getInterpreter()->interpret(pair.second,fpData);
  return TFPTR(GeoXF::PreMult(t1, p2.get()).clone());
}


TFPTR  PostMultReader::execute(const std::string & arg,  std::deque<double> *fpData) const {
  auto pair=split(arg);
  TFPTR p1=getInterpreter()->interpret(pair.first,fpData);
  GeoTrf::Transform3D t2=scanT(pair.second,fpData);
  return TFPTR(GeoXF::PostMult(p1.get(), t2).clone());
}

TFPTR  PowReader::execute(const std::string & arg, std::deque<double> *fpData) const {
  auto pair=split(arg);
  GeoTrf::Transform3D t1=scanT(pair.first,fpData);
  GFPTR f1 = getInterpreter()->getGenFunctionInterpreter()->interpret(pair.second.begin(), pair.second.end(),fpData);
  
  return TFPTR(GeoXF::Pow(t1,*f1).clone());
}

GeoTrf::Transform3D
TransFunctionReader::scanT(const std::string & exprString, std::deque<double> *fpData) const {

  if (exprString!="Transform[XF]") {
    throw std::runtime_error("Error in restoration of a TransFunction");
  }
  
  GeoTrf::Transform3D t;
  for (int i=0;i<3;i++) {
    for (int j=0;j<4;j++) {
      t(i,j) = (*fpData).back();
      fpData->pop_back();
    }
  }
  return t;
}

#include "GeoGenericFunctions/ArrayFunction.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/GenFunctionInterpreter.h"
#include "TFPersistification/ArrayFunctionIO.h"
#include <algorithm> 
ArrayFunctionReader::ArrayFunctionReader(GenFunctionInterpreter *interpreter):GenFunctionReader("ArrayFunction", interpreter) {}


GFPTR  ArrayFunctionReader::execute(std::string::const_iterator cStart, std::string::const_iterator cEnd, std::deque<double> *fpData) const {
  std::string aNumberStr(cStart,cEnd);
  std::istringstream stream(aNumberStr);
  std::string real;
  unsigned int len;
  stream >> real >> len;
  if (real != "REAL" || !stream) {
    throw std::runtime_error ("Parse error in ArrayFunctionReader");
  }
  std::vector<double> elements;
  for (int i=0;i<len;i++) {
    elements.push_back(fpData->back()); fpData->pop_back();
  }
  
  return GFPTR(new GeoGenfun::ArrayFunction(&elements.front(), &elements.back() + 1));
}


ArrayFunctionRecorder::ArrayFunctionRecorder(GenFunctionPersistifier *persistifier):
  GenFunctionRecorder(typeid(GeoGenfun::ArrayFunction),persistifier) {}

void ArrayFunctionRecorder::execute(const GeoGenfun::AbsFunction & F) const {
  const GeoGenfun::ArrayFunction * ptr = dynamic_cast<const GeoGenfun::ArrayFunction *> (&F);
  if (!ptr) throw std::runtime_error("Error in ArrayFunctionRecorder:: wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "ArrayFunction";
  stream << "(REAL ";
  stream << ptr->values().size();
  stream << ")";
  std::copy(ptr->values().begin(), ptr->values().end(), std::front_inserter(getPersistifier()->getFloatingPointData()));
}

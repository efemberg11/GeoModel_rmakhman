#include "GeoGenericFunctions/FixedConstant.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/GenFunctionInterpreter.h"
#include "TFPersistification/FixedConstantIO.h"
FixedConstantReader::FixedConstantReader(GenFunctionInterpreter *interpreter):GenFunctionReader("FixedConstant", interpreter) {}


GFPTR  FixedConstantReader::execute(std::string::const_iterator begin, std::string::const_iterator end, std::deque<double> *fpData) const {
  std::string aNumberStr(begin,end);
  if (aNumberStr!="REAL") {
    throw std::runtime_error ("Parse error in FixedConstantReader");
  }
  double v=fpData->back();fpData->pop_back();
  return GFPTR(new GeoGenfun::FixedConstant(v));
}


FixedConstantRecorder::FixedConstantRecorder(GenFunctionPersistifier *persistifier):
  GenFunctionRecorder(typeid(GeoGenfun::FixedConstant),persistifier) {}

void FixedConstantRecorder::execute(const GeoGenfun::AbsFunction & F) const {
  const GeoGenfun::FixedConstant * ptr = dynamic_cast<const GeoGenfun::FixedConstant *> (&F);
  if (!ptr) throw std::runtime_error("Error in FixedConstantRecorder:: wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  getPersistifier()->getFloatingPointData().push_front((*ptr)(0));
  stream << "FixedConstant";
  stream << "(REAL)";
}

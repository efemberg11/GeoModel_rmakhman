#include "GeoGenericFunctions/Mod.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/GenFunctionInterpreter.h"
#include "TFPersistification/ModIO.h"

ModReader::ModReader(GenFunctionInterpreter *interpreter):GenFunctionReader("Mod", interpreter) {}


GFPTR  ModReader::execute(std::string::const_iterator begin, std::string::const_iterator end, std::deque<double> *fpData) const {

  std::string aNumberStr(begin,end);
  if (aNumberStr!="REAL") {
    throw std::runtime_error ("Parse error in ModReader");
  }
  double y=fpData->back();fpData->pop_back();
  return GFPTR(new GeoGenfun::Mod(y));
}


ModRecorder::ModRecorder(GenFunctionPersistifier *persistifier):
  GenFunctionRecorder(typeid(GeoGenfun::Mod),persistifier) {}

void ModRecorder::execute(const GeoGenfun::AbsFunction & F) const {
  const GeoGenfun::Mod * ptr = dynamic_cast<const GeoGenfun::Mod *> (&F);
  if (!ptr) throw std::runtime_error("Error in ModRecorder:: wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  getPersistifier()->getFloatingPointData().push_front(ptr->modulus());
  stream << "Mod";
  stream << "(";
  stream<<"REAL";
  stream << ")";
}

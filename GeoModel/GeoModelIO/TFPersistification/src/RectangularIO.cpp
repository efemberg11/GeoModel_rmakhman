#include "GeoGenericFunctions/Rectangular.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/GenFunctionInterpreter.h"
#include "TFPersistification/RectangularIO.h"

RectangularReader::RectangularReader(GenFunctionInterpreter *interpreter):GenFunctionReader("Rectangular", interpreter) {}


GFPTR  RectangularReader::execute(std::string::const_iterator begin, std::string::const_iterator end, std::deque<double> *fpData) const {
  std::string aNumberStr(begin,end);
  if (aNumberStr!=("REAL,REAL,REAL,REAL")) {
    throw std::runtime_error("Parse error in RectangularReader");
  }
  GeoGenfun::Rectangular *ptr=new GeoGenfun::Rectangular();
  ptr->x0().setValue(fpData->back());       fpData->pop_back();
  ptr->x1().setValue(fpData->back());       fpData->pop_back();
  ptr->baseline().setValue(fpData->back()); fpData->pop_back();
  ptr->height().setValue(fpData->back());   fpData->pop_back();
  return GFPTR(ptr);
}


RectangularRecorder::RectangularRecorder(GenFunctionPersistifier *persistifier):
  GenFunctionRecorder(typeid(GeoGenfun::Rectangular),persistifier) {}

void RectangularRecorder::execute(const GeoGenfun::AbsFunction & F) const {
  const GeoGenfun::Rectangular * ptr = dynamic_cast<const GeoGenfun::Rectangular *> (&F);
  if (!ptr) throw std::runtime_error("Error in RectangularRecorder:: wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  getPersistifier()->getFloatingPointData().push_front(ptr->x0().getValue());
  getPersistifier()->getFloatingPointData().push_front(ptr->x1().getValue());
  getPersistifier()->getFloatingPointData().push_front(ptr->baseline().getValue());
  getPersistifier()->getFloatingPointData().push_front(ptr->height().getValue());
  stream << "Rectangular";
  stream << "(REAL,REAL,REAL,REAL)";
  
}

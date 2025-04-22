#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/GenFunctionRecorder.h"
#include "TFPersistification/SinIO.h"
#include "TFPersistification/TanIO.h"
#include "TFPersistification/CosIO.h"
#include "TFPersistification/ASinIO.h"
#include "TFPersistification/ATanIO.h"
#include "TFPersistification/ACosIO.h"
#include "TFPersistification/SqrtIO.h"
#include "TFPersistification/SquareIO.h"
#include "TFPersistification/AbsIO.h"
#include "TFPersistification/ArrayFunctionIO.h"
#include "TFPersistification/ModIO.h"
#include "TFPersistification/FixedConstantIO.h"
#include "TFPersistification/RectangularIO.h"
#include "TFPersistification/GenfunIO.h"

#include "GeoGenericFunctions/AbsFunction.h"


#include <sstream>
#include <iomanip>

const std::string & GenFunctionPersistifier::getCodedString() const {
  codedString=getStream().str();
  delete stream;
  stream=NULL;
  return codedString;
}

std::deque<double> & GenFunctionPersistifier::getFloatingPointData() const {
  return floatingPointData;
}


GenFunctionPersistifier::~GenFunctionPersistifier() {
  delete stream;
  for (auto i=recorderMap.begin();i!=recorderMap.end();i++) {
    delete (*i).second;
  }
}
GenFunctionPersistifier::GenFunctionPersistifier():stream(NULL) {
  new ConstPlusFunctionRecorder(this);
  new ConstTimesFunctionRecorder(this);
  new ConstMinusFunctionRecorder(this);
  new ConstOverFunctionRecorder(this);
  new FunctionNegationRecorder(this);
  new FunctionNoopRecorder(this);
  new FunctionProductRecorder(this);
  new FunctionQuotientRecorder(this);
  new FunctionCompositionRecorder(this);
  new FunctionSumRecorder(this);
  new FunctionDifferenceRecorder(this);
  new SinRecorder(this);
  new CosRecorder(this);
  new TanRecorder(this);
  new ASinRecorder(this);
  new ACosRecorder(this);
  new ATanRecorder(this);
  new AbsRecorder(this);
  new SqrtRecorder(this);
  new SquareRecorder(this);
  new ArrayFunctionRecorder(this);
  new ModRecorder(this);
  new FixedConstantRecorder(this);
  new RectangularRecorder(this);
  new VariableRecorder(this);
}

void GenFunctionPersistifier::add(const std::type_info & tInfo, const GenFunctionRecorder * recorder) {
  recorderMap[tInfo.name()]=recorder;
}

std::ostringstream & GenFunctionPersistifier::getStream() const {
  if (!stream) {
    stream = new std::ostringstream();
    (*stream) << std::setprecision(16);
  }
  return *stream;
}

void GenFunctionPersistifier::persistify(const GeoGenfun::AbsFunction & f) const {

  auto rIter = recorderMap.find(typeid(f).name());
  if (rIter!=recorderMap.end()) {
    const GenFunctionRecorder *recorder = (*rIter).second;
    recorder->execute(f);
  }
  else {
    std::ostringstream message;
    message << "Ominous warning in GenFunctionPersistifier: cannot persistify function with type id: " << typeid(f).name();
    throw std::runtime_error (message.str());
  }
}

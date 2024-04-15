#include "TFPersistification/TransFunctionPersistifier.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/TransFunctionRecorder.h"

// This is a data recorder. It breaks encapsulation. Normal. ==========//
#include "GeoModelKernel/GeoXF.h"
//=====================================================================//


#include <stdexcept>
TransFunctionRecorder::TransFunctionRecorder(const std::type_info & info, TransFunctionPersistifier * persistifier):
  persistifier(persistifier) {
  persistifier->add(info, this);
}

const TransFunctionPersistifier *TransFunctionRecorder::getPersistifier() const {
  return persistifier;
}

TransFunctionRecorder::~TransFunctionRecorder () {}

#include "GeoModelKernel/GeoXF.h"                          //

#include "GeoModelKernel/GeoDefinitions.h"

#include <iostream>
#include <typeinfo>


//===========Now come implementations of specific types of transfunction:
ProductRecorder::ProductRecorder(TransFunctionPersistifier *persistifier):TransFunctionRecorder(typeid(GeoXF::Product),persistifier) {}
PreMultRecorder::PreMultRecorder(TransFunctionPersistifier *persistifier):TransFunctionRecorder(typeid(GeoXF::PreMult),persistifier) {}
PostMultRecorder::PostMultRecorder(TransFunctionPersistifier *persistifier):TransFunctionRecorder(typeid(GeoXF::PostMult),persistifier) {}
PowRecorder::PowRecorder(TransFunctionPersistifier *persistifier):TransFunctionRecorder(typeid(GeoXF::Pow),persistifier) {}

void ProductRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::Product * ptr= dynamic_cast<const GeoXF::Product *> (&function);
  if (!ptr) throw std::runtime_error("Error in ProductRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();

  stream << "GeoXF::Product" << "{";
  getPersistifier()->persistify(*ptr->arg1());
  stream << "|";
  getPersistifier()->persistify(*ptr->arg2());
  stream << "}";
  return;
}


void PreMultRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::PreMult * ptr= dynamic_cast<const GeoXF::PreMult *> (&function);
  if (!ptr) throw std::runtime_error("Error in PreMultRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::PreMult" << "{";
  stream << "Transform[XF]";
  for (int i=0;i<3;i++) {
    for (int j=0;j<4;j++) {
      getPersistifier()->getFloatingPointData().push_front(ptr->arg1()(i,j));
    }
  }
  stream << "|";
  getPersistifier()->persistify(*ptr->arg2());
  stream << "}";
  return;
}


void PostMultRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::PostMult * ptr= dynamic_cast<const GeoXF::PostMult *> (&function);
  if (!ptr) throw std::runtime_error("Error in PostMultRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::PostMult" << "{";
  getPersistifier()->persistify(*ptr->arg1());
  stream << "|";
  stream << "Transform[XF]";
  for (int i=0;i<3;i++) {
    for (int j=0;j<4;j++) {
      getPersistifier()->getFloatingPointData().push_front(ptr->arg2()(i,j));
    }
  }
  stream << "}";
  return;
}



void PowRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::Pow * ptr= dynamic_cast<const GeoXF::Pow *> (&function);
  if (!ptr) throw std::runtime_error("Error in PowRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::Pow" << "{";
  stream << "Transform[XF]";
  for (int i=0;i<3;i++) {
    for (int j=0;j<4;j++) {
      getPersistifier()->getFloatingPointData().push_front(ptr->transform()(i,j));
    }
  }
  stream << "|";
  getPersistifier()->getGenFunctionPersistifier()->persistify(*ptr->function());
  stream << getPersistifier()->getGenFunctionPersistifier()->getCodedString();
  stream << "}";

  std::deque<double> & floatingPointData=getPersistifier()->getGenFunctionPersistifier()->getFloatingPointData();
  while (!floatingPointData.empty()) {
    getPersistifier()->getFloatingPointData().push_front(floatingPointData.back());
    floatingPointData.pop_back();
  }


  return;
}

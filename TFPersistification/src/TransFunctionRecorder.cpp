#include "TFPersistification/TransFunctionPersistifier.h"
#include "TFPersistification/GenFunctionPersistifier.h"
#include "TFPersistification/TransFunctionRecorder.h"

// This is a data recorder. It breaks encapsulation. Normal. ==========//
#define private public
#include "GeoModelKernel/GeoXF.h"
#undef private
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

#define private public                                     //
#include "GeoModelKernel/GeoXF.h"                          //
#undef private                                             //

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
  getPersistifier()->persistify(*ptr->m_arg1);
  stream << "|";
  getPersistifier()->persistify(*ptr->m_arg2);
  stream << "}";
  return;
}


void PreMultRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::PreMult * ptr= dynamic_cast<const GeoXF::PreMult *> (&function);
  if (!ptr) throw std::runtime_error("Error in PreMultRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::PreMult" << "{";
  stream << "Transform["
	 << ptr->m_arg1(0,0) << ";" << ptr->m_arg1(0,1) << ";" << ptr->m_arg1(0,2) << ";"
	 << ptr->m_arg1(1,0) << ";" << ptr->m_arg1(1,1) << ";" << ptr->m_arg1(1,2) << ";"
	 << ptr->m_arg1(2,0) << ";" << ptr->m_arg1(2,1) << ";" << ptr->m_arg1(2,2) << ";"
	 << ptr->m_arg1(0,3) << ";" << ptr->m_arg1(1,3) << ";" << ptr->m_arg1(2,3) << "]";
  stream << "|";
  getPersistifier()->persistify(*ptr->m_arg2);
  stream << "}";
  return;
}


void PostMultRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::PostMult * ptr= dynamic_cast<const GeoXF::PostMult *> (&function);
  if (!ptr) throw std::runtime_error("Error in PostMultRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::PostMult" << "{";
  getPersistifier()->persistify(*ptr->m_arg1);
  stream << "|";
  stream << "Transform["
	 << ptr->m_arg2(0,0) << ";" << ptr->m_arg2(0,1) << ";" << ptr->m_arg2(0,2) << ";"
	 << ptr->m_arg2(1,0) << ";" << ptr->m_arg2(1,1) << ";" << ptr->m_arg2(1,2) << ";"
	 << ptr->m_arg2(2,0) << ";" << ptr->m_arg2(2,1) << ";" << ptr->m_arg2(2,2) << ";"
	 << ptr->m_arg2(0,3) << ";" << ptr->m_arg2(1,3) << ";" << ptr->m_arg2(2,3) << "]";
  stream << "}";
  return;
}



void PowRecorder::execute( const GeoXF::Function & function) const {
  const GeoXF::Pow * ptr= dynamic_cast<const GeoXF::Pow *> (&function);
  if (!ptr) throw std::runtime_error("Error in PowRecorder:  wrong function type");
  std::ostringstream & stream = getPersistifier()->getStream();
  stream << "GeoXF::Pow" << "{";
  stream << "Transform["
	 << ptr->m_xf(0,0) << ";" << ptr->m_xf(0,1) << ";" << ptr->m_xf(0,2) << ";"
	 << ptr->m_xf(1,0) << ";" << ptr->m_xf(1,1) << ";" << ptr->m_xf(1,2) << ";"
	 << ptr->m_xf(2,0) << ";" << ptr->m_xf(2,1) << ";" << ptr->m_xf(2,2) << ";"
	 << ptr->m_xf(0,3) << ";" << ptr->m_xf(1,3) << ";" << ptr->m_xf(2,3) << "]";
  stream << "|";
  getPersistifier()->getGenFunctionPersistifier()->persistify(*ptr->m_function);
  stream << getPersistifier()->getGenFunctionPersistifier()->getCodedString();
  stream << "}";
  return;
}

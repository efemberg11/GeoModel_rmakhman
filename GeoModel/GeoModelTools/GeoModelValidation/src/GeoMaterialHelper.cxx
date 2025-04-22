/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

///////////////////////////////////////////////////////////////////
// GeoMaterialHelper.cxx, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////

#include "GeoModelValidation/GeoMaterialHelper.h"

// GeoModelKernel
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoPhysVol.h"

#include <string>

double GeoModelTools::GeoMaterialHelper::evaluateMass( const PVConstLink gv, bool inclusive) const {

  const GeoLogVol* lv = gv->getLogVol();
  double weight{0.}, motherVolume{0.};

  
  // skip volume calculation for dummy material configuration  
  if  ( !dummy_material(gv)) motherVolume = lv->getShape()->volume();

  // subtract children volumes
  //GeoVolumeVec_t vols = geoGetVolumes(gv);
  //for (const GeoVolumeVec_t::value_type& p : vols) {
  unsigned int nc = gv->getNChildVols();
  for (unsigned int ic = 0; ic < nc; ic++) {
     const PVConstLink cv = gv->getChildVol(ic);
     double childVolume = cv->getLogVol()->getShape()->volume() ;
     // skip volume calculation for dummy material configuration  
     if  (!dummy_material(gv) ) motherVolume -= childVolume;        // child volumes fully confined by construction    
     if (inclusive) weight += evaluateMass(cv, inclusive);
  }

  // skip mass calculation for dummy material configuration
  if (!dummy_material(gv))  weight += motherVolume * lv->getMaterial()->getDensity(); 

  return weight;

}

GeoModelTools::MaterialComponent GeoModelTools::GeoMaterialHelper::collectMaterial(PVConstLink gv) const {

  //  iterative loop over substructure
  std::vector< GeoModelTools::MaterialComponent > materialContent ;
  collectMaterialContent( gv, materialContent );

  // material blending  
  double volume{0.}, mass{0.};
  for ( auto matInput : materialContent ) {
    volume += matInput.second;
    mass += matInput.first->getDensity() * matInput.second; 
  }

  auto gmMatTmp= make_intrusive<GeoMaterial>("MaterialBlend", mass / volume);
  for ( auto matInput : materialContent ) {
    gmMatTmp->add( matInput.first, matInput.first->getDensity() * matInput.second / mass ); 
  }

  gmMatTmp->lock();  
  return std::make_pair( gmMatTmp, volume );

}						   

void GeoModelTools::GeoMaterialHelper::collectMaterialContent(const PVConstLink gv, 
                                                              std::vector< GeoModelTools::MaterialComponent >& materialContent ) const {

  const GeoLogVol* lv = gv->getLogVol();

  double motherVolume{0.};

  // skip volume calculation for dummy material configuration  
  if  (!dummy_material(gv) ) motherVolume = lv->getShape()->volume();

  // subtract children volumes from mother volume & collect their material content
  //GeoVolumeVec_t vols = geoGetVolumes(gv);
  //for (const GeoVolumeVec_t::value_type& p : vols) {
  unsigned int nc = gv->getNChildVols();
  for (unsigned int ic = 0; ic < nc; ic++) {
     const GeoVPhysVol* cv = &(*(gv->getChildVol(ic)));
     double childVolume = cv->getLogVol()->getShape()->volume() ;
     // skip for dummy material configuration 
     if  (!dummy_material(gv) )  motherVolume -= childVolume;      // child volumes fully confined by construction    
     collectMaterialContent( cv, materialContent );
  }

  // skip for dummy material configuration 
  if  (!dummy_material(gv) ) materialContent.emplace_back(lv->getMaterial(), motherVolume); 

  return;

}  

bool GeoModelTools::GeoMaterialHelper::dummy_material(const PVConstLink vol) const {
 static const std::array<std::string, 4> dummyMatNames{"special::Ether","WorldLog:Air", "std::Air" , "Air"};
  const std::string& matName = vol->getLogVol()->getMaterial()->getName();
  return std::find(dummyMatNames.begin(), dummyMatNames.end(),matName ) != dummyMatNames.end(); 
}       


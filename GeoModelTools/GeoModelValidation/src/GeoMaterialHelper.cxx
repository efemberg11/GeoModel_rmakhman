/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
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

float GeoModelTools::GeoMaterialHelper::evaluateMass( const GeoVPhysVol* gv, bool inclusive) const {

  const GeoLogVol* lv = gv->getLogVol();

  float weight = 0.;

  double motherVolume = 0.;

  // skip volume calculation for dummy material configuration  
  if  ( !this->dummy_material(gv) )  motherVolume = lv->getShape()->volume();

  // subtract children volumes
  //GeoVolumeVec_t vols = geoGetVolumes(gv);
  //for (const GeoVolumeVec_t::value_type& p : vols) {
  unsigned int nc = gv->getNChildVols();
  for (unsigned int ic = 0; ic < nc; ic++) {
     const GeoVPhysVol* cv = &(*(gv->getChildVol(ic)));
     double childVolume = cv->getLogVol()->getShape()->volume() ;
     // skip volume calculation for dummy material configuration  
     if  (!this->dummy_material(gv) ) motherVolume -= childVolume;        // child volumes fully confined by construction    
     if (inclusive) weight += evaluateMass(cv, inclusive);
  }

  // skip mass calculation for dummy material configuration
  if (!this->dummy_material(gv))  weight += motherVolume * lv->getMaterial()->getDensity(); 

  return weight;

}

GeoModelTools::MaterialComponent GeoModelTools::GeoMaterialHelper::collectMaterial( const GeoVPhysVol* gv) const {

  //  iterative loop over substructure
  std::vector< GeoModelTools::MaterialComponent > materialContent ;
  collectMaterialContent( gv, materialContent );

  // material blending  
  double volume = 0.;
  double mass = 0.;
  for ( auto matInput : materialContent ) {
    volume += matInput.second;
    mass += matInput.first->getDensity() * matInput.second; 
  }

  GeoMaterial* gmMatTmp=new GeoMaterial("MaterialBlend", mass / volume);
  for ( auto matInput : materialContent ) {
    gmMatTmp->add( matInput.first, matInput.first->getDensity() * matInput.second / mass ); 
  }

  gmMatTmp->lock();
  
  return std::pair< const GeoMaterial*, double > ( gmMatTmp, volume );

}						   

void GeoModelTools::GeoMaterialHelper::collectMaterialContent( const GeoVPhysVol* gv, std::vector< GeoModelTools::MaterialComponent >& materialContent ) const {

  const GeoLogVol* lv = gv->getLogVol();

  double motherVolume = 0.;

  // skip volume calculation for dummy material configuration  
  if  (!this->dummy_material(gv) ) motherVolume = lv->getShape()->volume();

  // subtract children volumes from mother volume & collect their material content
  //GeoVolumeVec_t vols = geoGetVolumes(gv);
  //for (const GeoVolumeVec_t::value_type& p : vols) {
  unsigned int nc = gv->getNChildVols();
  for (unsigned int ic = 0; ic < nc; ic++) {
     const GeoVPhysVol* cv = &(*(gv->getChildVol(ic)));
     double childVolume = cv->getLogVol()->getShape()->volume() ;
     // skip for dummy material configuration 
     if  (!this->dummy_material(gv) )  motherVolume -= childVolume;      // child volumes fully confined by construction    
     collectMaterialContent( cv, materialContent );
  }

  // skip for dummy material configuration 
  if  (!this->dummy_material(gv) ) materialContent.push_back( std::pair<const GeoMaterial*, double> ( lv->getMaterial(), motherVolume ) ); 

  return;

}  

bool GeoModelTools::GeoMaterialHelper::dummy_material(const GeoVPhysVol* vol) const {
 
  bool dummyMat = false;
  
  std::string mat = vol->getLogVol()->getMaterial()->getName();
 
  if ( mat=="special::Ether" ) dummyMat = true;    
  if ( mat=="WorldLog:Air" ) dummyMat = true;    
  if ( mat=="std::Air" ) dummyMat = true;    
  if ( mat=="Air" ) dummyMat = true;    

  return dummyMat; 
}       


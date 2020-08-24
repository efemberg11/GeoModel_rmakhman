/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "LArGeoBarrel/BarrelCryostatConstruction.h"
//#include "LArGeoTBBarrel/TBBarrelCryostatConstruction.h"
#include "LArGeoEndcap/EndcapCryostatConstruction.h"
#include "LArGeoUtils/LArGeoMaterialManager.h"

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"
#include "LArGeoUtils/LArGeoInpUtils.h"

#include <iostream>
#include <chrono>

#define SYSTEM_OF_UNITS GeoModelKernelUnits


class LArGeoPlugin : public GeoVGeometryPlugin
{
public:
  
  LArGeoPlugin() {};
  ~LArGeoPlugin();

  virtual void create(GeoPhysVol* world, bool publish) override;

private:
  
  const LArGeoPlugin & operator=(const LArGeoPlugin &right)=delete;
  LArGeoPlugin(const LArGeoPlugin &right) = delete;
  
  LArGeoMaterialManager* m_matman{new LArGeoMaterialManager()};
  
  // Configuration parameters
  bool         m_barrelSagging{false};
  int          m_barrelVisLimit{-1};
  int          m_fcalVisLimit{-1};
  bool         m_buildBarrel{true};
  bool         m_buildEndcap{true};
  int          m_testbeam{0};
  bool         m_fullGeo{true};
  std::string  m_EMECVariantInner{"Wheel"};
  std::string  m_EMECVariantOuter{"Wheel"};
  bool         m_activateFT{false};
};


LArGeoPlugin::~LArGeoPlugin()
{
  delete m_matman;
}

void LArGeoPlugin::create(GeoPhysVol* world, bool /*not used here*/)
{
  // ________ Time measure ____________
  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::milliseconds ms;
  typedef std::chrono::duration<float> fsec;
  auto t0 = Time::now();
  // ________ Time measure ____________
  
  /*
  // Detector Managers
  EMBDetectorManager*  embDetectorManager{nullptr};
  EMECDetectorManager* emecDetectorManager{nullptr};
  HECDetectorManager*  hecDetectorManager{nullptr};
  FCALDetectorManager* fcalDetectorManager{nullptr};
  */
  
  // Tree Tops:
  GeoFullPhysVol *barrelEnvelope{nullptr};
  GeoFullPhysVol *endcapEnvelopePos{nullptr};
  GeoFullPhysVol *endcapEnvelopeNeg{nullptr};

//  double projectivityDisplacement{0.};
  
//  if(m_testbeam==0 || m_testbeam==1) {

  // Get XML Material Manager
  const GeoXmlMatManager* materialManager = GeoXmlMatManager::getManager();
  
  // Get XML Input Manager
  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();
  inpman->parse("LAr-Defs.xml");
  inpman->parse("LAr-noelec.xml");
  
  GeoInpRecordset_ptr larPosition = inpman->getRecordsetPtr("LArPosition");
  if (larPosition->size()==0) throw std::runtime_error("Error, no lar position table in database!");

//  GeoInpRecordset_ptr emecGeometry = inpman->getRecordsetPtr("EmecGeometry");
//  projectivityDisplacement = (*emecGeometry)[0].getDouble("ZSHIFT");

    //
    // Patch the materials list by adding special materials.  These
    // cannot be added for the moment to the standard list because they
    // are blended materials and to do the blending requires access to
    // LAr Geometry.
    //
  m_matman->buildMaterials();

//    if (m_testbeam==0) {

  LArGeo::BarrelCryostatConstruction barrelCryostatConstruction(m_fullGeo, m_matman, m_activateFT);
  barrelCryostatConstruction.setBarrelSagging(m_barrelSagging);
  barrelCryostatConstruction.setBarrelCellVisLimit(m_barrelVisLimit);
      
  LArGeo::EndcapCryostatConstruction endcapCryostatConstruction(m_matman,m_fullGeo, m_EMECVariantInner, m_EMECVariantOuter, m_activateFT);
  endcapCryostatConstruction.setFCALVisLimit(m_fcalVisLimit);
  
  if(m_buildBarrel) {
    barrelEnvelope = barrelCryostatConstruction.GetEnvelope();
  }
      
  if(m_buildEndcap) {
    endcapEnvelopePos = endcapCryostatConstruction.createEnvelope(true);
    endcapEnvelopeNeg = endcapCryostatConstruction.createEnvelope(false);
  }
      
  world->add(new GeoNameTag("LAr"));

  if(m_buildBarrel && m_buildEndcap) {
    // Typical scenario for each of the Tree Tops:
    // 1. Construct Alignable Transform from LArPositions
    // 2. Store FullPV in SG
    // 3. Store Alignable XF in SG

    // --- Barrel
    const GeoInpRecord *barrelRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_B");
    if(!barrelRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfBarrel = LArGeoInpUtils::getTransform(barrelRec);
	
    GeoAlignableTransform* barrelAlXf = new GeoAlignableTransform(xfBarrel);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(barrelEnvelope);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_B");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_B PhysVol");
	  
	  StoredAlignX *sAlignX = new StoredAlignX(barrelAlXf);
	  status=detStore->record(sAlignX,"LARCRYO_B");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_B Alignable XF");
	}
	*/
    world->add(new GeoNameTag("LArBarrel"));
    world->add(barrelAlXf);
    world->add(barrelEnvelope);

    // --- Endcap Pos
    const GeoInpRecord *posRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_EC_POS");
    if(!posRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfPos = LArGeoInpUtils::getTransform(posRec);

    GeoAlignableTransform *xfEndcapPos = new GeoAlignableTransform(xfPos);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(endcapEnvelopePos);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_EC_POS");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_EC_POS PhysVol");
	  
	  StoredAlignX *sAlignX = new StoredAlignX(xfEndcapPos);
	  status=detStore->record(sAlignX,"LARCRYO_EC_POS");
	  if(!status.isSuccess()) throw std::runtime_error ("Cannot store LARCRYO_EC_POS");
	}
	*/
    // --- Endcap Neg
    const GeoInpRecord *negRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_EC_NEG");
    if(!negRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfNeg = LArGeoInpUtils::getTransform(negRec);
    
    GeoAlignableTransform *xfEndcapNeg = new GeoAlignableTransform(xfNeg);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(endcapEnvelopeNeg);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_EC_NEG");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_EC_NEG PhysVol");

	  StoredAlignX *sAlignX = new StoredAlignX(xfEndcapNeg);
	  status=detStore->record(sAlignX,"LARCRYO_EC_NEG");
	  if(!status.isSuccess()) throw std::runtime_error ("Cannot store LARCRYO_EC_NEG");
	}
	*/
    world->add( new GeoNameTag("LArEndcapPos"));
    world->add(xfEndcapPos);
    world->add(endcapEnvelopePos);
    world->add( new GeoNameTag("LArEndcapNeg"));
    world->add(xfEndcapNeg);
    world->add( new GeoTransform(GeoTrf::RotateY3D(180.0*SYSTEM_OF_UNITS::deg)));
    world->add(endcapEnvelopeNeg);
  }
  else if(!m_buildEndcap) {
    // -- Build the Barrel only
    const GeoInpRecord *barrelRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_B");
    if(!barrelRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfBarrel = LArGeoInpUtils::getTransform(barrelRec);
    
    GeoAlignableTransform* barrelAlXf = new GeoAlignableTransform(xfBarrel);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(barrelEnvelope);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_B");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_B PhysVol");

	  StoredAlignX *sAlignX = new StoredAlignX(barrelAlXf);
	  status=detStore->record(sAlignX,"LARCRYO_B");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_B Alignable XF");
	}
	*/
    world->add(new GeoNameTag("LArBarrel"));
    world->add(barrelAlXf);
    world->add(barrelEnvelope);
	
  }
  else if(!m_buildBarrel) {
    // --- Endcap Pos
    const GeoInpRecord *posRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_EC_POS");
    if(!posRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfPos = LArGeoInpUtils::getTransform(posRec);
	
    GeoAlignableTransform *xfEndcapPos = new GeoAlignableTransform(xfPos);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(endcapEnvelopePos);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_EC_POS");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_EC_POS PhysVol");

	  StoredAlignX *sAlignX = new StoredAlignX(xfEndcapPos);
	  status=detStore->record(sAlignX,"LARCRYO_EC_POS");
	  if(!status.isSuccess()) throw std::runtime_error ("Cannot store LARCRYO_EC_POS");
	}
	*/
    // --- Endcap Neg
    const GeoInpRecord *negRec = LArGeoInpUtils::getTransformRecord(larPosition,"LARCRYO_EC_NEG");
    if(!negRec) throw std::runtime_error("Error, no lar position record in the XML database");
    GeoTrf::Transform3D xfNeg = LArGeoInpUtils::getTransform(negRec);
    
    GeoAlignableTransform *xfEndcapNeg = new GeoAlignableTransform(xfNeg);
	/*
	{
	  StoredPhysVol *sPhysVol = new StoredPhysVol(endcapEnvelopeNeg);
	  StatusCode status=detStore->record(sPhysVol,"LARCRYO_EC_NEG");
	  if(!status.isSuccess()) throw std::runtime_error("Cannot store LARCRYO_EC_NEG PhysVol");

	  StoredAlignX *sAlignX = new StoredAlignX(xfEndcapNeg);
	  status=detStore->record(sAlignX,"LARCRYO_EC_NEG");
	  if(!status.isSuccess()) throw std::runtime_error ("Cannot store LARCRYO_EC_NEG");
	}
	*/
    world->add( new GeoNameTag("LArEndcapPos"));
    world->add(xfEndcapPos);
    world->add(endcapEnvelopePos);
    world->add( new GeoNameTag("LArEndcapNeg"));
    world->add(xfEndcapNeg);
    world->add( new GeoTransform(GeoTrf::RotateY3D(180.0*SYSTEM_OF_UNITS::deg)));
    world->add(endcapEnvelopeNeg);
    
  }
  /*      
    } // if(m_testbeam==0)
    else {
      TBBarrelCryostatConstruction tbbarrelCryostatConstruction;
      tbbarrelCryostatConstruction.setBarrelSagging(m_barrelSagging);
      tbbarrelCryostatConstruction.setBarrelCellVisLimit(m_barrelVisLimit);
      
      barrelEnvelope = tbbarrelCryostatConstruction.GetEnvelope();

      world->add(new GeoNameTag("LAr"));
      world->add(barrelEnvelope); 
    }
  }
  */
  /*
  try {
    fcalDetectorManager = new FCALDetectorManager(&(m_hvManager->getFCALHVManager()));
    std::string fcalModName[2][3]={{"FCAL1_NEG","FCAL2_NEG","FCAL3_NEG"},
				   {"FCAL1_POS","FCAL2_POS","FCAL3_POS"}};
    for (int e=0;e<2;e++) {
      for (int m=0;m<3;m++) {
	StoredPhysVol *vol;
	if(detStore->contains<StoredPhysVol>(fcalModName[e][m])){
	  if (detStore->retrieve(vol,fcalModName[e][m])==StatusCode::SUCCESS) {
	    FCALModule *module=new FCALModule(vol->getPhysVol(),FCALModule::Module(m+1),FCALModule::Endcap(e),projectivityDisplacement);
	    fcalDetectorManager->addModule(module);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << fcalModName[e][m] << " in Detector Store" << endmsg;
      }
    }
  }
  catch (std::exception & ex) {
    log << MSG::WARNING << "Unable to build FCAL detector manager. " << ex.what() << endmsg;
  }

  try {
    hecDetectorManager = new  HECDetectorManager(&(m_hvManager->getHECHVManager()));
    for (unsigned int s=0;s<4;s++) {
      for (unsigned int r=0;r<2;r++) {
	unsigned int nPhi    = r==0? 2:1;
	for (unsigned int endcap=0;endcap<2;endcap++) {
	  StoredPhysVol *vol;
	  std::string nameTag= endcap==0 ? (s<2 ? "HEC1_NEG":"HEC2_NEG") : (s<2 ? "HEC1_POS":"HEC2_POS");
	  if(detStore->contains<StoredPhysVol>(nameTag)){
	    if (StatusCode::SUCCESS==detStore->retrieve(vol, nameTag)) {
	      
	      unsigned int width = 32;
	      double startPhi =  endcap==0?  M_PI : 0;
	      double endPhi   =  endcap==0? -M_PI : 2*M_PI ;
	      
	      CellBinning  phiBinning(startPhi, endPhi, width*nPhi);
	      HECDetDescr *d = new HECDetDescr(hecDetectorManager,s,r,phiBinning);
	      
	      HECDetectorRegion::DetectorSide side = (HECDetectorRegion::DetectorSide) endcap;
	      HECDetectorRegion *region = new HECDetectorRegion(vol->getPhysVol(),d,side,projectivityDisplacement);
	      hecDetectorManager->addDetectorRegion(region);
	    }
	  }
	  else
	    log << MSG::DEBUG << " No Stored PV for " << nameTag
		<< " in Detector Store" << endmsg;
	}
      }
    }
  }
  catch (std::exception & ex) {
    log << MSG::WARNING << "Unable to build HEC detector manager. " << ex.what() << endmsg;
  }

  try {
    emecDetectorManager  = new EMECDetectorManager(&(m_hvManager->getEMECHVManager(EMECHVModule::INNER))
						   ,&(m_hvManager->getEMECHVManager(EMECHVModule::OUTER))
						   ,&(m_hvManager->getEMECPresamplerHVManager()));
    
    // Here is a table of min and max eta for different sampling layers, radial part (i/o) and region.
    for (int e=0;e<2;e++) {
      double startPhi =  e==0?  M_PI-2*M_PI/768/2 : -2*M_PI/768/2;
      double endPhi   =  e==0? -M_PI-2*M_PI/768/2 : 2*M_PI-2*M_PI/768/2 ;

      StoredPhysVol *sPhys;

      if(detStore->contains<StoredPhysVol>(e==0 ? "EMEC_OUTER_WHEEL_NEG" : "EMEC_OUTER_WHEEL_POS")){
	if (detStore->retrieve(sPhys,e==0 ? "EMEC_OUTER_WHEEL_NEG" : "EMEC_OUTER_WHEEL_POS")==StatusCode::SUCCESS) {
	  GeoFullPhysVol *emecEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();

	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,0,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  
	  // Outer Wheel Sampling 1 Region 1:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,1,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 1 Region 2:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,2,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 1 Region 3:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,3,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 1 Region 4:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,4,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 1 Region 5:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,5,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 2 Region 0:
	  {
	    CellBinning phiBinning(startPhi,endPhi,256);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,2,0,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 2 Region 1:
	  {
	    CellBinning phiBinning(startPhi,endPhi,256);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,2,1,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  // Outer Wheel Sampling 3 Region 0:
	  {
	    CellBinning phiBinning(startPhi,endPhi,256);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,3,0,0,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	}
      }
      else
	log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMEC_OUTER_WHEEL_NEG" : "EMEC_OUTER_WHEEL_POS")
	    << " in Detector Store" << endmsg;
      
      if(detStore->contains<StoredPhysVol>(e==0 ? "EMEC_INNER_WHEEL_NEG" : "EMEC_INNER_WHEEL_POS")){
	if (detStore->retrieve(sPhys,e==0 ? "EMEC_INNER_WHEEL_NEG" : "EMEC_INNER_WHEEL_POS")==StatusCode::SUCCESS) {
	  GeoFullPhysVol *emecEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();

	  // Inner Wheel Sampling 1 Region 0:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,1,0,1,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }

	  // Inner Wheel Sampling 2 Region 0:
	  {
	    CellBinning phiBinning(startPhi,endPhi,64);
	    EMECDetDescr *detDescr = new EMECDetDescr(emecDetectorManager,2,0,1,phiBinning);
	    EMECDetectorRegion *region = new EMECDetectorRegion(emecEnvelope,detDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	    emecDetectorManager->addDetectorRegion(region);
	  }
	  
	}
      }
      else
	log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMEC_INNER_WHEEL_NEG" : "EMEC_INNER_WHEEL_POS")
	    << " in Detector Store" << endmsg;
      
      StoredPhysVol *sPresamplerEnvelope;
      
      if(detStore->contains<StoredPhysVol>(e==0 ? "PRESAMPLER_EC_NEG":"PRESAMPLER_EC_POS" )){
	if (StatusCode::SUCCESS==detStore->retrieve(sPresamplerEnvelope, e==0 ? "PRESAMPLER_EC_NEG":"PRESAMPLER_EC_POS" )) {
	  GeoFullPhysVol * PresamplerEnvelope=sPresamplerEnvelope->getPhysVol();
	  CellBinning presamplerPhiBinning(startPhi,endPhi,64);
	  EMECDetDescr *presamplerDetDescr = new EMECDetDescr(emecDetectorManager,0,0,0,presamplerPhiBinning);
	  EMECDetectorRegion *presamplerRegion = new EMECDetectorRegion(PresamplerEnvelope,presamplerDetDescr,EMECDetectorRegion::DetectorSide(e),projectivityDisplacement);
	  emecDetectorManager->addDetectorRegion(presamplerRegion);
	}
      }
      else
	log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "PRESAMPLER_EC_NEG":"PRESAMPLER_EC_POS")
	    << " in Detector Store" << endmsg;
    }
    
  }
  catch (std::exception & ex) {
    log << MSG::WARNING << "Unable to build EMEC detector manager. " << ex.what() << endmsg;
  }

  try {
    embDetectorManager  = new EMBDetectorManager(m_hvManager->getEMBHVManager(),m_hvManager->getEMBPresamplerHVManager());
    int firstEndcap=m_testbeam==0 ? 0:1, endEndcap=2;
    for (int e= firstEndcap ;e<endEndcap;e++) {
      
      double startPhi =  e==0?  M_PI-2*M_PI/1024/2 : -2*M_PI/1024/2;
      double endPhi   =  e==0? -M_PI-2*M_PI/1024/2 : 2*M_PI-2*M_PI/1024/2 ;
      int    NDIV     = 1;
      if (m_testbeam==1) {
	startPhi=0*M_PI/180;
	endPhi  =22.5*M_PI/180;
	NDIV=16;
      }
      
      // Sampling layer 3 region 0:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,256/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,3,0,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS" )<< " in Detector Store" << endmsg;
      }
      // Sampling layer 2 region 0:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,256/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,2,0,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS" )<< " in Detector Store" << endmsg;
      }
      // Sampling layer 2 region 1:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,256/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,2,1,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS") << " in Detector Store" << endmsg;
      }
      // Sampling layer 1 region 0:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,64/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,1,0,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS") << " in Detector Store" << endmsg;
      }
      // Sampling layer 1 region 0:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,256/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,1,1,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS") << " in Detector Store" << endmsg;
      }
      // Sampling layer 0 region 0:
      if (1) {
	StoredPhysVol *sPhys;
	if(detStore->contains<StoredPhysVol>(e==0 ? "EMB_NEG" : "EMB_POS")) {
	  if (detStore->retrieve(sPhys,e==0 ? "EMB_NEG" : "EMB_POS")==StatusCode::SUCCESS) {
	    CellBinning phiBinning(startPhi,endPhi,64/NDIV);
	    EMBDetDescr *detDescr = new EMBDetDescr(embDetectorManager,0,0,phiBinning);
	    GeoFullPhysVol *embEnvelope=(GeoFullPhysVol *) sPhys->getPhysVol();
	    EMBDetectorRegion *region = new EMBDetectorRegion(embEnvelope,detDescr,EMBDetectorRegion::DetectorSide(e));
	    embDetectorManager->addDetectorRegion(region);
	  }
	}
	else
	  log << MSG::DEBUG << " No Stored PV for " << (e==0 ? "EMB_NEG" : "EMB_POS") << " in Detector Store" << endmsg;
      }
    }
  }
  catch (std::exception & ex) {
    log << MSG::WARNING << "Unable to build EMB detector manager. " << ex.what() << endmsg;
  }

  m_detectorManager = new LArDetectorManager(embDetectorManager,emecDetectorManager,hecDetectorManager,fcalDetectorManager);
  m_detectorManager->isTestBeam(false);
  if (barrelEnvelope)    m_detectorManager->addTreeTop(barrelEnvelope);
  if (endcapEnvelopePos) m_detectorManager->addTreeTop(endcapEnvelopePos);
  if (endcapEnvelopeNeg) m_detectorManager->addTreeTop(endcapEnvelopeNeg);

  StatusCode sc;
  if (embDetectorManager) {
    sc = detStore->record(embDetectorManager,  embDetectorManager->getName());
    if(sc.isFailure())
      log << MSG::ERROR << "Unable to record embDetectorManager" << endmsg;
  }

  if (emecDetectorManager) {
    sc = detStore->record(emecDetectorManager, emecDetectorManager->getName());
    if(sc.isFailure())
      log << MSG::ERROR << "Unable to record emecDetectorManager" << endmsg;
  }

  if (hecDetectorManager) {
    sc = detStore->record(hecDetectorManager,  hecDetectorManager->getName());
    if(sc.isFailure())
      log << MSG::ERROR << "Unable to record hecDetectorManager" << endmsg;
  }

  if (fcalDetectorManager) {
    sc = detStore->record(fcalDetectorManager, fcalDetectorManager->getName());
    if(sc.isFailure())
      log << MSG::ERROR << "Unable to record fcalDetectorManager" << endmsg;
  }
  */
  // ________ Time measure ____________
  auto t1 = Time::now();
  fsec fs = t1 - t0;
  ms d = std::chrono::duration_cast<ms>(fs);
  std::cout << "LArGeoPlugin elapsed time " << d.count() << "ms" << std::endl;
  // ________ Time measure ____________
}

extern "C" LArGeoPlugin *createLArGeoPlugin() {
    return new LArGeoPlugin();
}

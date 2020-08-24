/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#include "LArGeoFcal/FCALConstruction.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoSerialIdentifier.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoXF.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

// volumes used:  Pcon, Tubs, Cons, Box, Trap 
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"
#include "LArGeoUtils/LArGeoInpUtils.h"

// For functions:
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/ArrayFunction.h"

#include <string>
#include <cmath>
#include <cfloat>
#include <sstream>
#include <fstream>
#include <stdexcept>

LArGeo::FCALConstruction::FCALConstruction()
  : m_fcalPhysical(0)
  , m_absPhysical1(0)
  , m_absPhysical2(0)
  , m_absPhysical3(0)
  , m_VisLimit(0)
  , m_fullGeo(true)
{
}  

LArGeo::FCALConstruction::~FCALConstruction()
{
}

GeoVFullPhysVol* LArGeo::FCALConstruction::GetEnvelope(bool bPos)
{
  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();
  const GeoXmlMatManager* materialManager = GeoXmlMatManager::getManager();
  
  if(m_absPhysical1==0) {
    inpman->parse("LAr-fcalelec1.xml");
    inpman->parse("LAr-fcalelec2.xml");
    inpman->parse("LAr-fcalelec3.xml");

    m_fcalMod = inpman->getRecordsetPtr("FCalMod");        
    m_LArPosition = inpman->getRecordsetPtr("LArPosition");
  }
  
  const GeoMaterial *Copper  = materialManager->getMaterial("std::Copper");
  if (!Copper) throw std::runtime_error("Error in FCALConstruction, std::Copper is not found.");
  
  const GeoMaterial *Iron  = materialManager->getMaterial("std::Iron");
  if (!Iron) throw std::runtime_error("Error in FCALConstruction, std::Iron is not found.");
  
  const GeoMaterial *Lead  = materialManager->getMaterial("std::Lead");
  if (!Lead) throw std::runtime_error("Error in FCALConstruction, std::Lead is not found.");
  
  const GeoMaterial *LAr  = materialManager->getMaterial("std::LiquidArgon");
  if (!LAr) throw std::runtime_error("Error in FCALConstruction, std::LiquidArgon is not found.");
  
  const GeoMaterial *Air  = materialManager->getMaterial("std::Air");
  if (!Air) throw std::runtime_error("Error in FCALConstruction, std::Air is not found.");
  
  const GeoMaterial *Kapton  = materialManager->getMaterial("std::Kapton");
  if (!Kapton) throw std::runtime_error("Error in FCALConstruction, std::Kapton is not found.");
  
  const GeoMaterial *Glue  = materialManager->getMaterial("LAr::Glue");
  if (!Glue) throw std::runtime_error("Error in FCALConstruction, LAr::Glue is not found.");
  
  const GeoMaterial *G10  = materialManager->getMaterial("LAr::G10");
  if (!G10) throw std::runtime_error("Error in FCALConstruction, LAr::G10 is not found.");
  
  
  const GeoMaterial *FCal1Absorber = materialManager->getMaterial("LAr::FCal1Absorber");
  if (!FCal1Absorber) throw std::runtime_error("Error in FCALConstruction, LAr::FCal1Absorber is not found.");

  const GeoMaterial *FCal23Absorber = materialManager->getMaterial("LAr::FCal23Absorber");
  if (!FCal23Absorber) throw std::runtime_error("Error in FCALConstruction, LAr::FCal23Absorber is not found.");

  const GeoMaterial *FCalCableHarness = materialManager->getMaterial("LAr::FCalCableHarness");
  if (!FCalCableHarness) throw std::runtime_error("Error in FCALConstruction, LAr::FCalCableHarness is not found.");

  const GeoMaterial *FCal23Slugs = materialManager->getMaterial("LAr::FCal23Slugs");
  if (!FCal23Slugs) throw std::runtime_error("Error in FCALConstruction, LAr::FCal23Slugs is not found.");


  GeoFullPhysVol *fcalPhysical{nullptr};

  std::string baseName = "LAr::FCAL::";

  double startZFCal1 = (*m_fcalMod)[0].getDouble("STARTPOSITION"); //466.85 * cm;
  //double startZFCal2 = (*m_fcalMod)[1]->getDouble("STARTPOSITION"); //512.83 * cm;
  double startZFCal3 = (*m_fcalMod)[2].getDouble("STARTPOSITION"); //560.28 * cm;

  double outerModuleRadius1=(*m_fcalMod)[0].getDouble("OUTERMODULERADIUS");
  double outerModuleRadius2=(*m_fcalMod)[1].getDouble("OUTERMODULERADIUS");
  double outerModuleRadius3=(*m_fcalMod)[2].getDouble("OUTERMODULERADIUS");
  double innerModuleRadius1=(*m_fcalMod)[0].getDouble("INNERMODULERADIUS");
  double innerModuleRadius2=(*m_fcalMod)[1].getDouble("INNERMODULERADIUS");
  double innerModuleRadius3=(*m_fcalMod)[2].getDouble("INNERMODULERADIUS");
  double fullModuleDepth1=(*m_fcalMod)[0].getDouble("FULLMODULEDEPTH");
  double fullModuleDepth2=(*m_fcalMod)[1].getDouble("FULLMODULEDEPTH");
  double fullModuleDepth3=(*m_fcalMod)[2].getDouble("FULLMODULEDEPTH");
  double fullGapDepth1=(*m_fcalMod)[0].getDouble("FULLGAPDEPTH");
  double fullGapDepth2=(*m_fcalMod)[1].getDouble("FULLGAPDEPTH");
  double fullGapDepth3=(*m_fcalMod)[2].getDouble("FULLGAPDEPTH");
  double outerGapRadius1=(*m_fcalMod)[0].getDouble("OUTERGAPRADIUS");
  double outerGapRadius2=(*m_fcalMod)[1].getDouble("OUTERGAPRADIUS");
  double outerGapRadius3=(*m_fcalMod)[2].getDouble("OUTERGAPRADIUS");
  double innerGapRadius1=(*m_fcalMod)[0].getDouble("INNERGAPRADIUS");
  double innerGapRadius2=(*m_fcalMod)[1].getDouble("INNERGAPRADIUS");
  double innerGapRadius3=(*m_fcalMod)[2].getDouble("INNERGAPRADIUS");


  // FCAL VOLUME.  IT DOES NOT INCLUDE THE COPPER PLUG, ONLY THE LAR AND MODS 1-3
  {

    double outerRadius = std::max(outerModuleRadius1,std::max(outerModuleRadius2,outerModuleRadius3));
    double innerRadius = std::min(innerModuleRadius1,std::min(innerModuleRadius2,innerModuleRadius3));
    double depthZFCal3 = fullModuleDepth3;
    double stopZFCal3  = startZFCal3 + depthZFCal3;
    
    double totalDepth  = stopZFCal3 - startZFCal1;
    double halfDepth   = totalDepth/2.;

    std::string name = baseName + "LiquidArgonC";
    GeoTubs *tubs = new GeoTubs(innerRadius,outerRadius,halfDepth,0,360*SYSTEM_OF_UNITS::deg);
    GeoLogVol *logVol= new GeoLogVol(name, tubs, LAr);
    fcalPhysical = new GeoFullPhysVol(logVol);
  }

  GeoFullPhysVol *modPhysical{nullptr};
  // Module 1
  {
    double halfDepth       = fullModuleDepth1/2;
    double innerRadius     = innerModuleRadius1;
    double outerRadius     = outerModuleRadius1;
    GeoFullPhysVol *physVol;
    
    if(m_absPhysical1) {
      physVol = m_absPhysical1->clone();
    }
    else {
      GeoTubs *tubs          = new GeoTubs( innerRadius, outerRadius, halfDepth, 0, 2*M_PI);
      GeoLogVol  *logVol     = new GeoLogVol(baseName + "Module1::Absorber", tubs, FCal1Absorber);
      physVol     = new GeoFullPhysVol(logVol);
    }

    // Alignable transform
	
    const GeoInpRecord *posRec = LArGeoInpUtils::getTransformRecord(m_LArPosition, bPos ? "FCAL1_POS":"FCAL1_NEG");
    if (!posRec) throw std::runtime_error("Error, no lar position record in the database") ;
    GeoTrf::Transform3D xfPos = LArGeoInpUtils::getTransform(posRec);
    GeoAlignableTransform *xfAbs1 = new GeoAlignableTransform(xfPos);
	
    fcalPhysical->add(xfAbs1);
    if (!bPos)  fcalPhysical->add(new GeoTransform(GeoTrf::RotateY3D(180*SYSTEM_OF_UNITS::deg)));
    fcalPhysical->add(physVol);
    modPhysical = physVol;
	
    std::string tag = bPos? std::string("FCAL1_POS") : std::string("FCAL1_NEG");
/*
    StatusCode status;
	
    StoredPhysVol *sPhysVol = new StoredPhysVol(physVol);
    status=detStore->record(sPhysVol,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
    
    StoredAlignX *sAlignX = new StoredAlignX(xfAbs1);
    status=detStore->record(sAlignX,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
*/    
  }   
  // 16 Troughs representing  Cable Harnesses:
  if(m_fullGeo) {
    if(m_absPhysical1==0) {
      double troughDepth       = 1.0 * SYSTEM_OF_UNITS::cm;
      double outerRadius       = outerModuleRadius1;
      double innerRadius       = outerRadius - troughDepth;
      double halfLength        = fullModuleDepth1/ 2.0;
      double deltaPhi          = 5.625 * SYSTEM_OF_UNITS::deg;
      double startPhi          = 11.25 * SYSTEM_OF_UNITS::deg - deltaPhi/2.0;
      GeoTubs * tubs = new GeoTubs(innerRadius,outerRadius,halfLength,startPhi,deltaPhi );
      GeoLogVol *logVol = new GeoLogVol(baseName+"Module1::CableTrough",tubs,FCalCableHarness);
      GeoPhysVol *physVol = new GeoPhysVol(logVol);
      GeoGenfun::Variable i;
      GeoGenfun::GENFUNCTION rotationAngle = 22.5*SYSTEM_OF_UNITS::deg*i;
      GeoXF::TRANSFUNCTION xf = GeoXF::Pow(GeoTrf::RotateZ3D(1.0),rotationAngle);
      GeoSerialTransformer *st = new GeoSerialTransformer(physVol,&xf,16);
      modPhysical->add(st);
    }
  }

  if(m_absPhysical1==0) {
    double halfDepth    = fullGapDepth1/2.0;
    double innerRadius  = innerGapRadius1;
    double outerRadius  = outerGapRadius1;
    GeoPhysVol *physVol = 0;
    if(m_fullGeo) {
      GeoTubs *tubs       = new GeoTubs(innerRadius,outerRadius,halfDepth,0.0, 2.0*M_PI);
      GeoLogVol *logVol   = new GeoLogVol(baseName + "Module1::Gap",tubs, LAr);
      physVol = new GeoPhysVol(logVol);
      modPhysical->add(new GeoSerialIdentifier(0));
    }
    
    int counter=0;	  
    // Electrodes:
    GeoInpRecordset_ptr mod1Rec = inpman->getRecordsetPtr("LArFCalElectrodesMod1");
    for(const GeoInpRecord& record : *(mod1Rec.get())) {
      double thisTubeX= record.getDouble("X");
      double thisTubeY= record.getDouble("Y");

      if (m_VisLimit != -1 && (counter++ > m_VisLimit)) continue;
      if(m_fullGeo) {
        GeoTransform *xf = new GeoTransform(GeoTrf::Translate3D(thisTubeX*SYSTEM_OF_UNITS::cm, thisTubeY*SYSTEM_OF_UNITS::cm,0));
        modPhysical->add(xf);
        modPhysical->add(physVol);
      }
    }
    m_absPhysical1 = modPhysical;
  }

  // Module 2
  {
    double halfDepth       = fullModuleDepth2/2;
    double innerRadius     = innerModuleRadius2;
    double outerRadius     = outerModuleRadius2;
    GeoFullPhysVol *physVol;
	
    if(m_absPhysical2) {
      physVol = m_absPhysical2->clone();
    }
    else {
      GeoTubs *tubs          = new GeoTubs( innerRadius, outerRadius, halfDepth, 0, 2*M_PI);
      GeoLogVol  *logVol     = new GeoLogVol(baseName + "Module2::Absorber", tubs, FCal23Absorber);
      physVol     = new GeoFullPhysVol(logVol);
    }
    
    // Alignable transform

    const GeoInpRecord *posRec = LArGeoInpUtils::getTransformRecord(m_LArPosition, bPos ? "FCAL2_POS":"FCAL2_NEG");
    if (!posRec) throw std::runtime_error("Error, no lar position record in the database") ;
    GeoTrf::Transform3D xfPos = LArGeoInpUtils::getTransform(posRec);
    GeoAlignableTransform *xfAbs2 = new GeoAlignableTransform(xfPos);
    
    fcalPhysical->add(xfAbs2);
    if (!bPos)  fcalPhysical->add(new GeoTransform(GeoTrf::RotateY3D(180*SYSTEM_OF_UNITS::deg)));
    fcalPhysical->add(physVol);
    modPhysical = physVol;
    
    std::string tag = bPos? std::string("FCAL2_POS") : std::string("FCAL2_NEG");
/*
    StatusCode status;
	
    StoredPhysVol *sPhysVol = new StoredPhysVol(physVol);
    status=detStore->record(sPhysVol,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
    
    StoredAlignX *sAlignX = new StoredAlignX(xfAbs2);
    status=detStore->record(sAlignX,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
*/
    
  }   
  // 16 Troughs representing  Cable Harnesses:
  if(m_fullGeo) {
    if(m_absPhysical2==0) {
      double troughDepth       = 1.0 * SYSTEM_OF_UNITS::cm;
      double outerRadius       = outerModuleRadius2;
      double innerRadius       = outerRadius - troughDepth;
      double halfLength        = fullModuleDepth2/ 2.0;
      double deltaPhi          = 5.625 * SYSTEM_OF_UNITS::deg;
      double startPhi          = 11.25 * SYSTEM_OF_UNITS::deg - deltaPhi/2.0;
      GeoTubs * tubs = new GeoTubs(innerRadius,outerRadius,halfLength,startPhi,deltaPhi );
      GeoLogVol *logVol = new GeoLogVol(baseName+"Module2::CableTrough",tubs,FCalCableHarness);
      GeoPhysVol *physVol = new GeoPhysVol(logVol);
      GeoGenfun::Variable i;
      GeoGenfun::GENFUNCTION rotationAngle = 22.5*SYSTEM_OF_UNITS::deg*i;
      GeoXF::TRANSFUNCTION xf = GeoXF::Pow(GeoTrf::RotateZ3D(1.0),rotationAngle);
      GeoSerialTransformer *st = new GeoSerialTransformer(physVol,&xf,16);
      modPhysical->add(st);
    }
  }
  // Electrodes:
  if(m_absPhysical2==0) {
    double halfDepth    = fullGapDepth2/2.0;
    double innerRadius  = innerGapRadius2;
    double outerRadius  = outerGapRadius2;
    
    GeoPhysVol *gapPhys    = 0;
    GeoPhysVol *rodPhys    = 0;
    if(m_fullGeo) {
      GeoTubs *gapTubs       = new GeoTubs(0,outerRadius,halfDepth,0.0, 2.0*M_PI);
      GeoLogVol *gapLog      = new GeoLogVol(baseName + "Module2::Gap",gapTubs, LAr);
      gapPhys    = new GeoPhysVol(gapLog);
      
      GeoTubs *rodTubs       = new GeoTubs(0,innerRadius,halfDepth,0.0, 2.0*M_PI);
      GeoLogVol *rodLog      = new GeoLogVol(baseName + "Module2::Rod",rodTubs, FCal23Slugs);
      rodPhys    = new GeoPhysVol(rodLog);
      gapPhys->add(rodPhys);
      modPhysical->add(new GeoSerialIdentifier(0));
    }
    
    int counter=0;
    GeoInpRecordset_ptr mod2Rec = inpman->getRecordsetPtr("LArFCalElectrodesMod2");
    for(const GeoInpRecord& record : *(mod2Rec.get())) {
      double thisTubeX= record.getDouble("X");
      double thisTubeY= record.getDouble("Y");
    	    
      if (m_VisLimit != -1 && (counter++ > m_VisLimit)) continue;
      if(m_fullGeo) {	      
	GeoTransform *xf = new GeoTransform(GeoTrf::Translate3D(thisTubeX*SYSTEM_OF_UNITS::cm, thisTubeY*SYSTEM_OF_UNITS::cm,0));
	modPhysical->add(xf);
	modPhysical->add(gapPhys);
      }
    }

    m_absPhysical2 = modPhysical;
  }

  // Module 3
  {
    double halfDepth       = fullModuleDepth3/2;
    double innerRadius     = innerModuleRadius3;
    double outerRadius     = outerModuleRadius3;
    GeoFullPhysVol *physVol;
    
    if(m_absPhysical3) {
      physVol = m_absPhysical3->clone();
    }
    else {
      GeoTubs *tubs          = new GeoTubs( innerRadius, outerRadius, halfDepth, 0, 2*M_PI);
      GeoLogVol  *logVol     = new GeoLogVol(baseName + "Module3::Absorber", tubs, FCal23Absorber);
      physVol     = new GeoFullPhysVol(logVol);
    }
    
    // Alignable transform
    const GeoInpRecord *posRec = LArGeoInpUtils::getTransformRecord(m_LArPosition, bPos ? "FCAL3_POS":"FCAL3_NEG");
    if (!posRec) throw std::runtime_error("Error, no lar position record in the database") ;
    GeoTrf::Transform3D xfPos = LArGeoInpUtils::getTransform(posRec);
    GeoAlignableTransform *xfAbs3 = new GeoAlignableTransform(xfPos);
    
    fcalPhysical->add(xfAbs3);
    if (!bPos)  fcalPhysical->add(new GeoTransform(GeoTrf::RotateY3D(180*SYSTEM_OF_UNITS::deg)));
    fcalPhysical->add(physVol);
    modPhysical = physVol;
    
    std::string tag = bPos? std::string("FCAL3_POS") : std::string("FCAL3_NEG");
/*
    StatusCode status;
    
    StoredPhysVol *sPhysVol = new StoredPhysVol(physVol);
    status=detStore->record(sPhysVol,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
    
    StoredAlignX *sAlignX = new StoredAlignX(xfAbs3);
    status=detStore->record(sAlignX,tag);
    if(!status.isSuccess()) throw std::runtime_error ((std::string("Cannot store")+tag).c_str());
*/   
  }   
  // 16 Troughs representing  Cable Harnesses:
  if(m_fullGeo) {
    if(m_absPhysical3==0) {
      static double rotAngles[] =
	{ 11.25 * SYSTEM_OF_UNITS::deg,
	  22.50 * SYSTEM_OF_UNITS::deg,
	  45.00 * SYSTEM_OF_UNITS::deg,
	  56.25 * SYSTEM_OF_UNITS::deg,
	  67.50 * SYSTEM_OF_UNITS::deg,
	  90.00 * SYSTEM_OF_UNITS::deg,  // first quarter
	  101.25 * SYSTEM_OF_UNITS::deg,
	  112.50 * SYSTEM_OF_UNITS::deg,
	  135.00 * SYSTEM_OF_UNITS::deg,
	  146.25 * SYSTEM_OF_UNITS::deg,
	  157.50 * SYSTEM_OF_UNITS::deg,
	  180.00 * SYSTEM_OF_UNITS::deg,  // second quarter
	  191.25 * SYSTEM_OF_UNITS::deg,
	  202.50 * SYSTEM_OF_UNITS::deg,
	  225.00 * SYSTEM_OF_UNITS::deg,
	  236.25 * SYSTEM_OF_UNITS::deg,
	  247.50 * SYSTEM_OF_UNITS::deg,
	  270.00 * SYSTEM_OF_UNITS::deg,  // third quarter
	  281.25 * SYSTEM_OF_UNITS::deg,
	  292.50 * SYSTEM_OF_UNITS::deg,
	  315.00 * SYSTEM_OF_UNITS::deg,
	  326.25 * SYSTEM_OF_UNITS::deg,
	  337.50 * SYSTEM_OF_UNITS::deg,
	  360.00 * SYSTEM_OF_UNITS::deg };
      
      GeoGenfun::ArrayFunction rotationAngle(rotAngles,rotAngles+24);
      double troughDepth       = 1.0 * SYSTEM_OF_UNITS::cm;
      double outerRadius       = outerModuleRadius3;
      double innerRadius       = outerRadius - troughDepth;
      double halfLength        = fullModuleDepth3/ 2.0;
      double deltaPhi          = 5.625 * SYSTEM_OF_UNITS::deg;
      double startPhi          = 11.25 * SYSTEM_OF_UNITS::deg - deltaPhi/2.0;
      GeoTubs * tubs = new GeoTubs(innerRadius,outerRadius,halfLength,startPhi,deltaPhi );
      GeoLogVol *logVol = new GeoLogVol(baseName+"Module3::CableTrough",tubs,FCalCableHarness);
      GeoPhysVol *physVol = new GeoPhysVol(logVol);
      GeoXF::TRANSFUNCTION xf = GeoXF::Pow(GeoTrf::RotateZ3D(1.0),rotationAngle);
      GeoSerialTransformer *st = new GeoSerialTransformer(physVol,&xf,24);
      modPhysical->add(st);
    }
  }  
  // Electrodes:
  if(m_absPhysical3==0) {
    double halfDepth    = fullGapDepth3/2.0;
    double innerRadius  = innerGapRadius3;
    double outerRadius  = outerGapRadius3;
    
    GeoPhysVol *gapPhys    = 0;
    GeoPhysVol *rodPhys    = 0;
    if(m_fullGeo) {
      GeoTubs *gapTubs       = new GeoTubs(0,outerRadius,halfDepth,0.0, 2.0*M_PI);
      GeoLogVol *gapLog      = new GeoLogVol(baseName + "Module3::Gap",gapTubs, LAr);
      gapPhys    = new GeoPhysVol(gapLog);
      
      GeoTubs *rodTubs       = new GeoTubs(0,innerRadius,halfDepth,0.0, 2.0*M_PI);
      GeoLogVol *rodLog      = new GeoLogVol(baseName + "Module3::Rod",rodTubs, FCal23Slugs);
      rodPhys    = new GeoPhysVol(rodLog);
      gapPhys->add(rodPhys);
      modPhysical->add(new GeoSerialIdentifier(0));
    }
    
    int counter=0;    
    GeoInpRecordset_ptr mod3Rec = inpman->getRecordsetPtr("LArFCalElectrodesMod3");
    for(const GeoInpRecord& record : *(mod3Rec.get())) {
      double thisTubeX= record.getDouble("X");
      double thisTubeY= record.getDouble("Y");
	    
      if (m_VisLimit != -1 && (counter++ > m_VisLimit)) continue;
      if(m_fullGeo) {	      
	GeoTransform *xf = new GeoTransform(GeoTrf::Translate3D(thisTubeX*SYSTEM_OF_UNITS::cm, thisTubeY*SYSTEM_OF_UNITS::cm,0));
	modPhysical->add(xf);
	modPhysical->add(gapPhys);
      }
    }
    
    m_absPhysical3 = modPhysical;
  }
  
  return fcalPhysical;
}

void LArGeo::FCALConstruction::setFullGeo(bool flag)
{
  m_fullGeo = flag;
}

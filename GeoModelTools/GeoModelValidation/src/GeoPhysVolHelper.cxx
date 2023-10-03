/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

///////////////////////////////////////////////////////////////////
// GeoPhysVolHelper.cxx, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////

#include "GeoModelValidation/GeoPhysVolHelper.h"

// GeoModelKernel
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoPhysVol.h"
//#include "GeoModelUtilities/GeoVisitVolumes.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelWrite/WriteGeoModel.h"
#include "GeoModelRead/ReadGeoModel.h" 
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoPara.h"

#include <string>

int GeoModelTools::GeoPhysVolHelper::compareGeoVolumes( const GeoVPhysVol* gv1, const GeoVPhysVol* gv2, float tolerance, bool dumpInfo, int level , int returnCode ) const  {

  m_diff = returnCode;

  // CASE 1: naming difference 
  if (gv1->getLogVol()->getName() != gv2->getLogVol()->getName())  {
    if (dumpInfo) {
      std::cout <<"CASE 1: names differ at level:"<<level<<":"<<gv1->getLogVol()->getName() <<":"<<gv2->getLogVol()->getName() << std::endl; 
      m_diff = 1000*level+1;
      //printInfo(gv1);
      //printInfo(gv2);
    }
    else return 1000*level+1;
  }  
  // CASE 2: material type difference
  if (gv1->getLogVol()->getMaterial()->getName() != gv2->getLogVol()->getMaterial()->getName())  {
    if (dumpInfo) {
      std::cout <<"CASE 2: material types differ at level:"<<level<<":"<<gv1->getLogVol()->getMaterial()->getName() <<":"<<gv2->getLogVol()->getMaterial()->getName() << std::endl; 
      m_diff = 1000*level+2;
      //printInfo(gv1);
      //printInfo(gv2);
    }
    else return 1000*level+2;
  }
  //  CASE 3: shape type difference
  if (gv1->getLogVol()->getShape()->type() != gv2->getLogVol()->getShape()->type() ) {
    if (dumpInfo) {
      std::cout <<"CASE 3: shape types differ at level:"<<level<<":"<<gv1->getLogVol()->getShape()->type() <<":"<<gv2->getLogVol()->getShape()->type() << std::endl; 
      m_diff = 1000*level+3;
    } 
    else  return 1000*level+3;
  }
  //  CASE 4: difference in shape definition   
  if ( !compareShapes( gv1->getLogVol()->getShape(), gv2->getLogVol()->getShape(), tolerance ) ) {
    if (dumpInfo) {
      std::cout <<"CASE 4: shape definition differ at level:"<<level<<", decoding:"<< std::endl; 
      std::cout <<"(a) decoding shape definition for child volume:"<<gv1->getLogVol()->getName()<< std::endl;
      decodeShape(gv1->getLogVol()->getShape());
      std::cout <<"(b) decoding shape definition for child volume:"<<gv2->getLogVol()->getName()<< std::endl;
      decodeShape(gv2->getLogVol()->getShape());
      m_diff = 1000*level+4;
    }
    else return 1000*level+4;
  }
  // CASE 5: difference in the number of child volumes
  if (gv1->getNChildVols() != gv2->getNChildVols())  {
    if (dumpInfo) { 
      std::cout <<"CASE 5: number of child vols differ at level:"<<level<< ":" <<gv1->getNChildVols()<<":"<<gv2->getNChildVols()<< std::endl;
      //printInfo(gv1);
      //printInfo(gv2);
      m_diff = 1000*level+5;
    } 
    else return 1000*level+5;  
  }

  // CASE 6 & 7: transform to child difference 
  for (unsigned int ic = 0; ic < gv1->getNChildVols() ; ic++) {
    GeoTrf::Transform3D transf1 = gv1->getXToChildVol(ic);
    GeoTrf::Transform3D transf2 = gv2->getXToChildVol(ic);

    const GeoVPhysVol* cv1 = &(*(gv1->getChildVol(ic)));
    const GeoVPhysVol* cv2 = &(*(gv2->getChildVol(ic)));

    if ((transf1.translation()-transf2.translation()).norm()>tolerance) {
      if (dumpInfo) {
	std::cout <<"CASE 6: translation differs between mother and child:"<<gv1->getLogVol()->getName()<<":"<< cv1->getLogVol()->getName()<<":"<<transf1.translation()<<":"<<transf2.translation()<< ":"<<
	(transf1.translation()-transf2.translation()).norm()<<": using tolerance limit:"<<tolerance<< std::endl;
        m_diff = 1000*level + 10*ic + 6;
      }
      else return 1000*level+10*ic+6; 
    }
    GeoTrf::RotationMatrix3D rot = transf1.rotation()*transf2.rotation().inverse();
    if ( fabs(rot(0,1))>1.e-6 || fabs(rot(0,2))>tolerance || fabs(rot(1,2))>tolerance)  {
      if (dumpInfo) {
	std::cout <<"CASE 7: rotation differs between mother and child:"<<gv1->getLogVol()->getName()<<":"<< cv1->getLogVol()->getName()<<":"<<transf1.rotation()<<":"<<transf2.rotation()<< ":"<< fabs(rot(0,1))<<":"<<fabs(rot(0,2))<<": using tolerance limit:"<<tolerance<< std::endl;
        m_diff = 1000*level + 10*ic + 7;
      }
     else return 1000*level+ic*10+7; 
    }
 
    int child_comp = compareGeoVolumes(cv1,cv2,tolerance,dumpInfo,level+1, m_diff); 
    if (child_comp != m_diff) std::cout <<"WARNING: faulty logic in return code" << std::endl;
  } 

  return m_diff;
}


void GeoModelTools::GeoPhysVolHelper::saveToDb(const GeoVPhysVol* pv, std::string filename)  {

     // open the DB connection
     GMDBManager db(filename);

     // check the DB connection
    if (db.checkIsDBOpen()) {
      std::cout << "OK! Database is open!" << std::endl;
    } else {
      std::cout << "Database ERROR!! Exiting..."<< filename << std::endl;
     return;
    }
 
    // Dump the tree volumes to a local file
    std::cout << "Dumping the GeoModel geometry to the DB file..." << std::endl;
    GeoModelIO::WriteGeoModel dumpGeoModelGraph3(db); // init the GeoModel node action
    pv->exec(&dumpGeoModelGraph3); // visit all GeoModel nodes
    dumpGeoModelGraph3.saveToDB(); // save to the SQlite DB file
    std::cout << "DONE. Geometry saved into "<<filename <<std::endl;

    return;
}

GeoPhysVol* GeoModelTools::GeoPhysVolHelper::retrieveFromDb(std::string filename)  {

  // open the DB
  GMDBManager* db = new GMDBManager(filename);

  if (!db->checkIsDBOpen()) { 
    std::cout << "error in database readout, no GeoVolume retrieved"<< std::endl;
    return nullptr;
  }
 
   /* setup the GeoModel reader */
   GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);

   /* build the GeoModel geometry */
   GeoPhysVol* dbPhys = dynamic_cast<GeoPhysVol*>(readInGeo.buildGeoModel()); // builds the GeoModel tree in memory

   return dbPhys;
}

void GeoModelTools::GeoPhysVolHelper::printInfo(const GeoVPhysVol* pv) const {

  const GeoLogVol* lv = pv->getLogVol();
  std::cout <<"printInfo : new Object:" << lv->getName() << ", made of " << lv->getMaterial()->getName() << " x0 "
	    << lv->getMaterial()->getRadLength() << "," << lv->getShape()->type()<<std::endl;
  //shapeCnv.decodeShape(lv->getShape());
  int level = 0;
  printChildren(pv,level);
}

void GeoModelTools::GeoPhysVolHelper::printChildren(const GeoVPhysVol* pv, int level) const {

  // subcomponents
  unsigned int nc = pv->getNChildVols();
  level++;  std::string cname;
  for (unsigned int ic = 0; ic < nc; ic++) {
    GeoTrf::Transform3D transf = pv->getXToChildVol(ic);

    const GeoVPhysVol* cv = &(*(pv->getChildVol(ic)));
    const GeoFullPhysVol* cfv = dynamic_cast<const GeoFullPhysVol*> (cv);
    const GeoLogVol* clv = cv->getLogVol();
    if (ic==0) cname = clv->getName();
    if (cfv) {
      //const std::string&  fname = new GeoFullPhysVol(*cfv)->getAbsoluteName();
      std::cout <<"GeoFullPhysVol:"<<clv->getName()<< std::endl;
    }
    std::cout <<"subcomponent:" << level<<":"<<ic << ":" << clv->getName() << ", made of " << clv->getMaterial()->getName()
                                    << " x0 " << clv->getMaterial()->getRadLength() << " , "
                                    << clv->getShape()->type() << "," << transf.translation().x() << " "
	      << transf.translation().y() << " " << transf.translation().z()<<": nChild:"<< cv->getNChildVols() << std::endl;;

    // shapeCnv.decodeShape(clv->getShape());

    if (ic==0 || clv->getName()!=cname) printChildren(cv,level);
 
    cname = clv->getName();
  }
}

bool GeoModelTools::GeoPhysVolHelper::compareShapes( const GeoShape* sh1, const GeoShape* sh2, float tol ) const {

  if (sh1->type() != sh2->type()) return false;

  if ( sh1->type()=="Pgon") {
    const GeoPgon* pgon1 = dynamic_cast<const GeoPgon*>(sh1);
    const GeoPgon* pgon2 = dynamic_cast<const GeoPgon*>(sh2);
    if (!pgon1 || !pgon2)  return false;

    if (pgon1->getNPlanes() != pgon2->getNPlanes())  return false;
    if (pgon1->getNSides() != pgon2->getNSides())  return false;
    if (fabs(pgon1->getSPhi() - pgon2->getSPhi()) > tol) return false; 
    if (fabs(pgon1->getDPhi() - pgon2->getDPhi()) > tol) return false; 

    return true;

  } else if (sh1->type()=="Trd") {
    const GeoTrd* trd1 = dynamic_cast<const GeoTrd*> (sh1);
    const GeoTrd* trd2 = dynamic_cast<const GeoTrd*> (sh2);
    
    if ((trd1->getXHalfLength1() - trd2->getXHalfLength1())>tol) return false;
    if ((trd1->getXHalfLength2() - trd2->getXHalfLength2())>tol) return false;
    if ((trd1->getYHalfLength1() - trd2->getYHalfLength1())>tol) return false;
    if ((trd1->getYHalfLength2() - trd2->getYHalfLength2())>tol) return false;
    if ((trd1->getZHalfLength() - trd2->getZHalfLength())>tol) return false;

    return true;

  } else if ( sh1->type()=="Box") {
    const GeoBox* box1 = dynamic_cast<const GeoBox*> (sh1);
    const GeoBox* box2 = dynamic_cast<const GeoBox*> (sh2);

    if (fabs(box1->getXHalfLength() - box2->getXHalfLength()) > tol) return false;
    if (fabs(box1->getYHalfLength() - box2->getYHalfLength()) > tol) return false;
    if (fabs(box1->getZHalfLength() - box2->getZHalfLength()) > tol) return false;

   return true;  

  } else if ( sh1->type() == "Tube" ) {
    const GeoTube* tube1=dynamic_cast<const GeoTube*> (sh1);
    const GeoTube* tube2=dynamic_cast<const GeoTube*> (sh2);
    
    if ( fabs(tube1->getRMin() - tube2->getRMin()) > tol) return false;
    if ( fabs(tube1->getRMax() - tube2->getRMax()) > tol) return false;
    if ( fabs(tube1->getZHalfLength()  - tube2->getZHalfLength()) > tol) return false;
 
    return true;

  } else   if ( sh1->type() == "Tubs" ) {
    const GeoTubs* tubs1=dynamic_cast<const GeoTubs*> (sh1);
    const GeoTubs* tubs2=dynamic_cast<const GeoTubs*> (sh2);

    if ( fabs(tubs1->getRMin()  - tubs2->getRMin()) >tol) return false;
    if ( fabs(tubs1->getRMax()  - tubs2->getRMax()) > tol) return false;
    if ( fabs(tubs1->getZHalfLength()  - tubs2->getZHalfLength()) > tol) return false;
    if ( fabs(tubs1->getSPhi()  - tubs2->getSPhi()) > tol) return false;
    if ( fabs(tubs1->getDPhi() - tubs2->getDPhi()) > tol) return false;
 
    return true;

  }  else if  ( sh1->type() == "Cons" ) {
    const GeoCons* cons1=dynamic_cast<const GeoCons*> (sh1);
    const GeoCons* cons2=dynamic_cast<const GeoCons*> (sh2);

    if ( fabs(cons1->getRMin1()  - cons2->getRMin1())  > tol) return false;
    if ( fabs(cons1->getRMin2()  - cons2->getRMin2()) >tol) return false;
    if ( fabs(cons1->getRMax1()  - cons2->getRMax1()) > tol)  return false;
    if ( fabs(cons1->getRMax2()  - cons2->getRMax2()) > tol) return false;
    if ( fabs(cons1->getDZ()  - cons2->getDZ()) > tol) return false;
    if ( fabs(cons1->getSPhi()  - cons2->getSPhi()) > tol) return false;
    if ( fabs(cons1->getDPhi()  - cons2->getDPhi()) > tol) return false;

    return true;

  } else  if ( sh1->type()=="SimplePolygonBrep") {
    const GeoSimplePolygonBrep* spb1 = dynamic_cast<const GeoSimplePolygonBrep*> (sh1);
    const GeoSimplePolygonBrep* spb2 = dynamic_cast<const GeoSimplePolygonBrep*> (sh2);
    if (!spb1 || !spb2) return false;

    unsigned int nv1 = spb1->getNVertices();
    unsigned int nv2 = spb2->getNVertices();
    if (nv1 != nv2)  return false;
    if (fabs(spb1->getDZ() - spb2->getDZ()) > tol)  return false;
   
    for (unsigned int iv = 0; iv < nv1; iv++) {

      if (fabs(spb1->getXVertex(iv) - spb2->getXVertex(iv)) > tol) return false; 
      if (fabs(spb1->getYVertex(iv) - spb2->getYVertex(iv)) > tol) return false; 
    }

    return true;

  } else  if ( sh1->type()=="Pcon") {
    const GeoPcon* pc1 = dynamic_cast<const GeoPcon*> (sh1);
    const GeoPcon* pc2 = dynamic_cast<const GeoPcon*> (sh2);
    if (!pc1 || !pc2) return false;

    if ( fabs(pc1->getSPhi()  - pc2->getSPhi()) > tol) return false;
    if ( fabs(pc1->getDPhi()  - pc2->getDPhi()) > tol) return false;

    unsigned int nv1 = pc1->getNPlanes();
    unsigned int nv2 = pc2->getNPlanes();
    if (nv1 != nv2)  return false;
  
    for (unsigned int iv = 0; iv < nv1; iv++) {

      if (fabs(pc1->getZPlane(iv) - pc2->getZPlane(iv)) > tol) return false; 
      if (fabs(pc1->getRMinPlane(iv) - pc2->getRMinPlane(iv)) > tol) return false; 
      if (fabs(pc1->getRMaxPlane(iv) - pc2->getRMaxPlane(iv)) > tol) return false; 
    }

    return true;
 
  } else   if ( sh1->type()=="Subtraction") {
    const GeoShapeSubtraction* sub1 = dynamic_cast<const GeoShapeSubtraction*> (sh1);
    const GeoShapeSubtraction* sub2 = dynamic_cast<const GeoShapeSubtraction*> (sh2);

    if (!sub1 || !sub2) return false;
  
    if (!compareShapes(sub1->getOpA(),sub2->getOpA(),tol)) return false;
    if (!compareShapes(sub1->getOpB(),sub2->getOpB(),tol)) return false;

    return true;

  } else  if ( sh1->type()=="Union") {
    const GeoShapeUnion* sub1 = dynamic_cast<const GeoShapeUnion*> (sh1);
    const GeoShapeUnion* sub2 = dynamic_cast<const GeoShapeUnion*> (sh2);

    if (!sub1 || !sub2) return false;

    if (!compareShapes(sub1->getOpA(),sub2->getOpA(),tol)) return false;
    if (!compareShapes(sub1->getOpB(),sub2->getOpB(),tol)) return false;

    return true;

  } else  if ( sh1->type()=="Shift") {
    const GeoShapeShift* shift1 = dynamic_cast<const GeoShapeShift*> (sh1);
    const GeoShapeShift* shift2 = dynamic_cast<const GeoShapeShift*> (sh2);

    if (!shift1 || !shift2) return false;

    if (!compareShapes(shift1->getOp(),shift2->getOp(),tol)) return false;
    
    const GeoTrf::Transform3D& transf1 = shift1->getX();
    const GeoTrf::Transform3D& transf2 = shift2->getX();
    
    if ((transf1.translation()-transf2.translation()).norm()>tol)    return false;
  
    if (!identity_check(transf1.rotation()*transf2.rotation().inverse(), tol))   return false;
    
    return true;
  }

  std::cout <<"unknown shape to compare:"<<sh1->type()<< std::endl;
  
  return false;

}

bool GeoModelTools::GeoPhysVolHelper::identity_check(GeoTrf::RotationMatrix3D rotation, float tol) const {

  if (fabs(rotation(0,1))>tol) return false;  
  if (fabs(rotation(0,2))>tol) return false;  
  if (fabs(rotation(1,2))>tol) return false;  

  return true;
} 

void GeoModelTools::GeoPhysVolHelper::decodeShape(const GeoShape* sh) const {

  std::cout << "decoding shape:"<< sh->type() << std::endl;

  if ( sh->type()=="Trd") {
    const GeoTrd* trd = dynamic_cast<const GeoTrd*> (sh);
    std::cout << "dimensions:" << trd->getXHalfLength1() << ","
	      << trd->getXHalfLength2() << ","
	      << trd->getYHalfLength1() << ","
	      << trd->getYHalfLength2() << ","
	      << trd->getZHalfLength() << std::endl;
  }

  if ( sh->type()=="SimplePolygonBrep") {
    const GeoSimplePolygonBrep* spb = dynamic_cast<const GeoSimplePolygonBrep*> (sh);
 
    int nv = spb->getNVertices();
    std::cout <<"number of planes: dZ:"<<nv<<":"<<spb->getDZ()<< std::endl;
    
    for (unsigned int iv = 0; iv < nv; iv++) {

      std::cout <<"x,y:"<<spb->getXVertex(iv)<<","<<spb->getYVertex(iv)<< std::endl;
    }
  }


  if ( sh->type()=="Subtraction") {
    const GeoShapeSubtraction* sub = dynamic_cast<const GeoShapeSubtraction*> (sh);
    const GeoShape* sha = sub->getOpA();
    const GeoShape* shs = sub->getOpB();
    std::cout << "decoding subtracted shape:" << std::endl;
    decodeShape(sha);
    decodeShape(shs);
  }

  if ( sh->type()=="Union") {
    const GeoShapeUnion* sub = dynamic_cast<const GeoShapeUnion*> (sh);
    const GeoShape* shA = sub->getOpA();
    const GeoShape* shB = sub->getOpB();
    std::cout << "decoding shape A:" << std::endl;
    decodeShape(shA);
    std::cout << "decoding shape B:" << std::endl;
    decodeShape(shB);
  }

  if ( sh->type()=="Shift") {
    const GeoShapeShift* shift = dynamic_cast<const GeoShapeShift*> (sh);
    const GeoShape* shA = shift->getOp();
    const GeoTrf::Transform3D& transf = shift->getX();
    std::cout << "shifted by:transl:" <<transf.translation() <<", rot:"
              << transf(0,0)<<"," << transf(1,1) <<"," << transf(2,2) << std::endl;
    decodeShape(shA);
  }

}
						   

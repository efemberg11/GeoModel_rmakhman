/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "VP1GeometrySystems/DumpShape.h"

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTwistedTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoVSurfaceShape.h"
#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoAnnulusSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"

#include "GeoModelHelpers/throwExcept.h"
#include "Inventor/SbMatrix.h"
#include <cassert>



//TODO: Add support for the SimpleBREP GeoShape.
//TODO: Add GeoTorus

QStringList DumpShape::shapeToStringList(const GeoVSurfaceShape* surf_shape)
{
  QStringList out;
  static const double mm=1.0;
  if(surf_shape->type() == "RectangleSurface"){
    const GeoRectSurface* rectsurf = dynamic_cast<const GeoRectSurface*>(surf_shape);
    if(rectsurf){
      out << " =========> Rectangle Surface:";
      out << "  X = "+QString::number(rectsurf->getXHalfLength()/mm)+" mm";
      out << "  Y = "+QString::number(rectsurf->getYHalfLength()/mm)+" mm";
    }
  }
  else if(surf_shape->type() == "TrapezoidSurface"){
    const GeoTrapezoidSurface* trapezoid = dynamic_cast<const GeoTrapezoidSurface*>(surf_shape);
    if(trapezoid){
      out << " =========> Trapezoid Surface:";
      out << "  Short Half Base = "+QString::number(trapezoid->getXHalfLengthMin()/mm)+" mm";
      out << "  Long Half Base = "+QString::number(trapezoid->getXHalfLengthMax()/mm)+" mm";
      out << "  Height = "+QString::number(trapezoid->getYHalfLength()/mm)+" mm";
    }
  }
  else if(surf_shape->type() == "AnnulusSurface"){
    const GeoAnnulusSurface* annulus = dynamic_cast<const GeoAnnulusSurface*>(surf_shape);
    if(annulus){
      out << " =========> Annulus Surface:";
      out << "  Ox (Deviation in X-direction) = "+QString::number(annulus->getOx()/mm)+" mm";
      out << "  Oy (Deviation in Y-direction) = "+QString::number(annulus->getOy()/mm)+" mm";
      out << "  Inner Radius = "+QString::number(annulus->getRadiusIn()/mm)+" mm";
      out << "  Outer Radius = "+QString::number(annulus->getRadiusOut()/mm)+" mm";
      out << "  Phi (Span Angle) = "+QString::number(annulus->getPhi())+" rad"; 
    }
  }
  else if(surf_shape->type() == "DiamondSurface"){
    const GeoDiamondSurface* diamond = dynamic_cast<const GeoDiamondSurface*>(surf_shape);
    if(diamond){
      out << " =========> Diamond Surface:";
      out << "  X1 = "+QString::number(diamond->getXbottomHalf()/mm)+" mm";
      out << "  X2 = "+QString::number(diamond->getXmidHalf()/mm)+" mm";
      out << "  X3 = "+QString::number(diamond->getXtopHalf()/mm)+" mm";
      out << "  Y1 = "+QString::number(diamond->getYbottomHalf()/mm)+" mm";
      out << "  Y2 = "+QString::number(diamond->getYtopHalf()/mm)+" mm";
    }
  }
  else{
    //out << " =========> Unknown surface...";
    THROW_EXCEPTION("ERROR!!! VSurfaceShape is Unknown, cannot print out shape information!");
  }
  
  return out; 
}

//____________________________________________________________________
QStringList DumpShape::shapeToStringList(const GeoShape* shape)
{
  QStringList out;
  static const double mm=1.0;
  if(shape->typeID() == GeoBox::getClassTypeID() ) {
    const GeoBox* theBox = dynamic_cast<const GeoBox*> (shape);
    assert(theBox);
    if (theBox){
      out << " =========> Box:";
      out << "  X = "+QString::number(theBox->getXHalfLength()/mm)+" mm";
      out << "  Y = "+QString::number(theBox->getYHalfLength()/mm)+" mm";
      out << "  Z = "+QString::number(theBox->getZHalfLength()/mm)+" mm";
    }
  } else if(shape->typeID() == GeoTube::getClassTypeID() ) {
    const GeoTube* theTube = dynamic_cast<const GeoTube*> (shape);
    assert(theTube);
    if (theTube){
      out << " =========> Tube:";
      out << "  RMin = "+QString::number(theTube->getRMin()/mm)+" mm";
      out << "  RMax = "+QString::number(theTube->getRMax()/mm)+" mm";
      out << "  DZ = "+QString::number(theTube->getZHalfLength()/mm)+" mm";
    }
  } else if(shape->typeID() == GeoTubs::getClassTypeID() ) {
    const GeoTubs* theTubs = dynamic_cast<const GeoTubs*> (shape);
    assert(theTubs);
    if (theTubs){
      out << " =========> Tubs:";
      out << "  RMin = "+QString::number(theTubs->getRMin()/mm)+" mm";
      out << "  RMax = "+QString::number(theTubs->getRMax()/mm)+" mm";
      out << "  DZ = "+QString::number(theTubs->getZHalfLength()/mm)+" mm";
      out << "  SPhi = "+QString::number(theTubs->getSPhi());
      out << "  DPhi = "+QString::number(theTubs->getDPhi());
    }
  } else if(shape->typeID() == GeoTrd::getClassTypeID() ) {
    const GeoTrd* theTrd = dynamic_cast<const GeoTrd*> (shape);
    assert(theTrd);
    if (theTrd){
      out << " =========> Trd:";
      out << "  X1 = "+QString::number(theTrd->getXHalfLength1()/mm)+" mm";
      out << "  X2 = "+QString::number(theTrd->getXHalfLength2()/mm)+" mm";
      out << "  Y1 = "+QString::number(theTrd->getYHalfLength1()/mm)+" mm";
      out << "  Y2 = "+QString::number(theTrd->getYHalfLength2()/mm)+" mm";
      out << "  Z  = "+QString::number(theTrd->getZHalfLength()/mm)+" mm";
    }
  } else if(shape->typeID() == GeoPcon::getClassTypeID()) {
    const GeoPcon* thePcon = dynamic_cast<const GeoPcon*>(shape);
    assert(thePcon);//only in dbg builds
    if (thePcon){
      out << " =========> Pcon:";
      out<<"  Start phi = "+QString::number(thePcon->getSPhi())+", Delta phi = "+QString::number(thePcon->getDPhi());
      for (unsigned index=0; index < thePcon->getNPlanes(); ++index) {
        out << "  Plane "+QString::number(index);
        out << "    -> RMin = "+QString::number(thePcon->getRMinPlane(index)/mm)+" mm";
        out << "    -> RMax = "+QString::number(thePcon->getRMaxPlane(index)/mm)+" mm";
        out << "    -> Z    = "+QString::number(thePcon->getZPlane(index)/mm)+" mm";
      }
    }
  } else if(shape->typeID() == GeoCons::getClassTypeID()) {
    const GeoCons* theCons = dynamic_cast<const GeoCons*>(shape);
    assert(theCons);
    if (theCons){
      out << " =========> Cons:";
      out << "  RMin1 = "<<QString::number(theCons->getRMin1()/mm)+" mm";
      out << "  RMin2 = "<<QString::number(theCons->getRMin2()/mm)+" mm";
      out << "  RMax1 = "<<QString::number(theCons->getRMax1()/mm)+" mm";
      out << "  RMax2 = "<<QString::number(theCons->getRMax2()/mm)+" mm";
      out << "  DZ = "<<QString::number(theCons->getDZ()/mm)+" mm";
      out << "  SPhi = "+QString::number(theCons->getSPhi());
      out << "  DPhi = "+QString::number(theCons->getDPhi());
    }
  } else if(shape->typeID() == GeoPara::getClassTypeID()) {
    const GeoPara* thePara = dynamic_cast<const GeoPara*>(shape);
    assert(thePara);
    if (thePara){
      out << " =========> Para:";
      out << "  X = "+QString::number(thePara->getXHalfLength()/mm)+" mm";
      out << "  Y = "+QString::number(thePara->getYHalfLength()/mm)+" mm";
      out << "  Z = "+QString::number(thePara->getZHalfLength()/mm)+" mm";
      out << "  Alpha = "+QString::number(thePara->getAlpha());
      out << "  Theta = "+QString::number(thePara->getTheta());
      out << "  Phi = "+QString::number(thePara->getPhi());
    }
  } else if(shape->typeID() == GeoPgon::getClassTypeID()) {
    const GeoPgon* thePgon = dynamic_cast<const GeoPgon*>(shape);
    assert(thePgon);
    if (thePgon){
      out << " =========> Pgon:";
      for (unsigned int index=0; index<thePgon->getNPlanes(); index++)
      {
        out << "  Plane "+QString::number(index);
        out << ":   -> RMin = "+QString::number(thePgon->getRMinPlane(index)/mm)+" mm";
        out << ":   -> RMax = "+QString::number(thePgon->getRMaxPlane(index)/mm)+" mm";
        out << "    ->    Z = "+QString::number(thePgon->getZPlane(index)/mm)+" mm";
      }
    }
  } else if(shape->typeID() == GeoTrap::getClassTypeID()) {
    const GeoTrap* theTrap = dynamic_cast<const GeoTrap*>(shape);
    assert(theTrap);
    if (theTrap){
      out << " =========> Trap:";
      out << "  DZ = "+QString::number(theTrap->getZHalfLength()/mm)+" mm";
      out << "  Theta = "+QString::number(theTrap->getTheta());
      out << "  Phi = "+QString::number(theTrap->getPhi());
      out << "  Dydzn = "+QString::number(theTrap->getDydzn()/mm)+" mm";
      out << "  Dxdyndzn = "+QString::number(theTrap->getDxdyndzn()/mm)+" mm";
      out << "  Dxdypdzn = "+QString::number(theTrap->getDxdypdzn()/mm)+" mm";
      out << "  Angleydzn = "+QString::number(theTrap->getAngleydzn());
      out << "  Dydzp = "+QString::number(theTrap->getDydzp()/mm)+" mm";
      out << "  Dxdyndzp = "+QString::number(theTrap->getDxdyndzp()/mm)+" mm";
      out << "  Dxdypdzp = "+QString::number(theTrap->getDxdypdzp()/mm)+" mm";
      out << "  Angleydzp = "+QString::number(theTrap->getAngleydzp());
    }
  }
  else if(shape->typeID() == GeoTwistedTrap::getClassTypeID()) {
    const GeoTwistedTrap* theTwistedTrap = dynamic_cast<const GeoTwistedTrap*>(shape);
    assert(theTwistedTrap);
    if (theTwistedTrap){
      out << " =========> TwistedTrap:";
      out << "  PhiTwist = "+QString::number(theTwistedTrap->getPhiTwist());
      out << "  ZHalfLength = "+QString::number(theTwistedTrap->getZHalfLength()/mm)+" mm";
      out << "  Theta = "+QString::number(theTwistedTrap->getTheta());
      out << "  Phi = "+QString::number(theTwistedTrap->getPhi());
      out << "  DY1HalfLength = "+QString::number(theTwistedTrap->getY1HalfLength()/mm)+" mm";
      out << "  DX1HalfLength = "+QString::number(theTwistedTrap->getX1HalfLength()/mm)+" mm";
      out << "  DX2HalfLength = "+QString::number(theTwistedTrap->getX2HalfLength()/mm)+" mm";
      out << "  DY2HalfLength = "+QString::number(theTwistedTrap->getY2HalfLength());
      out << "  DX3HalfLength = "+QString::number(theTwistedTrap->getX3HalfLength()/mm)+" mm";
      out << "  DX4HalfLength = "+QString::number(theTwistedTrap->getX4HalfLength()/mm)+" mm";
      out << "  DTiltAngleAlpha = "+QString::number(theTwistedTrap->getTiltAngleAlpha());
    }
  }
  // Boolean volumes:
  else if (shape->typeID() == GeoShapeShift::getClassTypeID() ) {
    const GeoShapeShift* theShift = dynamic_cast<const GeoShapeShift*>(shape);
    assert(theShift);
    if (theShift){
      out << " =========> GeoShapeShift:";
      const GeoTrf::Transform3D &X=theShift->getX();
      GeoTrf::AngleAxis3D       AA;
      AA.fromRotationMatrix(X.rotation());
      out << "   Information about contained transformation:";
      out << "===&gt; Translation:";
      out << QString::number(X.translation()(0)*mm)+" mm";
      out << QString::number(X.translation()(1)*mm)+" mm";
      out << QString::number(X.translation()(2)*mm)+" mm";
      out << "===&gt; Rotation:";
      out << "        axis x = "+QString::number(AA.axis().x());
      out << "        axis y = "+QString::number(AA.axis().y());
      out << "        axis z = "+QString::number(AA.axis().z());
      out << "        angle = "+QString::number(AA.angle()*180.0/M_PI)+" deg";

      out << "   Information about contained GeoShape:";
      foreach(QString str,shapeToStringList(theShift->getOp())){
        out <<"  ==>  "+str;
      }
    }
  } else if (shape->typeID() == GeoShapeUnion::getClassTypeID() ) {
    const GeoShapeUnion* theUnion = dynamic_cast<const GeoShapeUnion*>(shape);
    assert(theUnion);
    if (theUnion){
      out << " =========> GeoShapeUnion:";
      out << "   First operand:";
      foreach (QString s,shapeToStringList(theUnion->getOpA()))
        out << "  ==> "+s;
      out << "   Second operand:";
      foreach (QString s,shapeToStringList(theUnion->getOpB()))
        out << "  ==> "+s;
    }
  } else if (shape->typeID() == GeoShapeIntersection::getClassTypeID() ) {
    const GeoShapeIntersection* theIntersection = dynamic_cast<const GeoShapeIntersection*>(shape);
    assert(theIntersection);
    if (theIntersection){
      out << " =========> GeoShapeIntersection:";
      out << "   First operand:";
      foreach (QString s,shapeToStringList(theIntersection->getOpA()))
        out << "  ==> "+s;
      out << "   Second operand:";
      foreach (QString s,shapeToStringList(theIntersection->getOpB()))
        out << "  ==> "+s;
    }
  } else if (shape->typeID() == GeoShapeSubtraction::getClassTypeID() ) {
    const GeoShapeSubtraction* theSubtraction = dynamic_cast<const GeoShapeSubtraction*>(shape);
    assert(theSubtraction);
    if (theSubtraction){
      out << " =========> GeoShapeSubtraction:";
      out << "   First operand:";
      foreach (QString s,shapeToStringList(theSubtraction->getOpA()))
        out << "  ==> "+s;
      out << "   Second operand:";
      foreach (QString s,shapeToStringList(theSubtraction->getOpB()))
        out << "  ==> "+s;
    }
  } else if (shape->typeID() == GeoGenericTrap::getClassTypeID() ) {
    const GeoGenericTrap* theGenericTrap = dynamic_cast<const GeoGenericTrap*>(shape);
    assert(theGenericTrap);
    if (theGenericTrap){
      out << " =========> GenericTrap:";
      out << " halfZlength = "+QString::number(theGenericTrap->getZHalfLength()/mm)+" mm";
      std::vector<Eigen::Vector2d> vertices;
      vertices.reserve(theGenericTrap->getVertices().size());
      for(const auto& geoVertex : theGenericTrap->getVertices()) {
	vertices.push_back(Eigen::Vector2d(geoVertex.x(),geoVertex.y()));
      }
      //in total 8 vertices by definition!
      out << "==> First 4 vertices at - hz/2";
      out << "(Ax,Ay) = ( "+QString::number(vertices[0].x()/mm)+" , "+QString::number(vertices[0].y()/mm)+" ) mm";
      out << "(Bx,By) = ( "+QString::number(vertices[1].x()/mm)+" , "+QString::number(vertices[1].y()/mm)+" ) mm";
      out << "(Cx,Cy) = ( "+QString::number(vertices[2].x()/mm)+" , "+QString::number(vertices[2].y()/mm)+" ) mm";
      out << "(Dx,Dy) = ( "+QString::number(vertices[3].x()/mm)+" , "+QString::number(vertices[3].y()/mm)+" ) mm";
      out <<"==> Second 4 vertices at +hz/2";
      out << "(Ax,Ay) = ( "+QString::number(vertices[4].x()/mm)+" , "+QString::number(vertices[4].y()/mm)+" ) mm";
      out << "(Bx,By) = ( "+QString::number(vertices[5].x()/mm)+" , "+QString::number(vertices[5].y()/mm)+" ) mm";
      out << "(Cx,Cy) = ( "+QString::number(vertices[6].x()/mm)+" , "+QString::number(vertices[6].y()/mm)+" ) mm";
      out << "(Dx,Dy) = ( "+QString::number(vertices[7].x()/mm)+" , "+QString::number(vertices[7].y()/mm)+" ) mm";
    }
  } else {
    out << " =========> Unknown shape...";
  }

  return out;

}

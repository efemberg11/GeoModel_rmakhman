/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/GeoShapeSorter.h"
#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelHelpers/TransformToStringConverter.h"
#include "GeoModelHelpers/GeoShapeUtils.h"
#include "GeoModelKernel/throwExcept.h"

#include "GeoModelKernel/Units.h"

#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"


#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoEllipticalTube.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTwistedTrap.h"

#include <exception>
#include <sstream>
#include <iostream>

namespace {
   constexpr double tolerance = 1.* GeoModelKernelUnits::micrometer;
   static const GeoTrf::TransformSorter transCmp{};
}

#define CHECK_PROPERTY(shapeA, shapeB, PROP_NAME)         \
   {                                                      \
      const double diffValue = 1.*shapeA->PROP_NAME() -   \
                               1.*shapeB->PROP_NAME();    \
      if (std::abs(diffValue) > tolerance) {              \
        return diffValue < 0 ? - 1 : 1;                   \
      }                                                   \
   }                                                      \

#define CHECK_VEC_PROPERTY(shapeA, shapeB, PROP_NAME, nChecks)  \
    {                                                           \
      for(unsigned int n = 0 ; n < nChecks; ++n) {              \
        const double diffValue = 1.*shapeA->PROP_NAME(n) -      \
                                 1.*shapeB->PROP_NAME(n);       \
                                                                \
        if (std::abs(diffValue) > tolerance) {                  \
          return diffValue < 0 ? - 1 : 1;                       \
        }                                                       \
      }                                                         \
    }

#define CALL_SORTER(shapeA, shapeB)             \
    {                                           \
      const int cmpAB{compare(shapeA, shapeB)}; \
      if (cmpAB) return cmpAB;                  \
    }

bool GeoShapeSorter::operator()(const GeoShape* objA, const GeoShape* objB) const {  
  if (!objA || !objB) {
     THROW_EXCEPTION("Nullptr given to the comparator");
  }
  return compare(objA, objB) < 0;
}

int GeoShapeSorter::compare(const GeoShape* objA, const GeoShape* objB) const {
  CHECK_PROPERTY(objA, objB, typeID);
  /// Check the defining parameters of each shape one by one
  const int typeID = objA->typeID(); 
  if (typeID == GeoShapeUnion::getClassTypeID() ||
      typeID == GeoShapeIntersection::getClassTypeID() ||
      typeID == GeoShapeSubtraction::getClassTypeID()) {
    
    std::pair<const GeoShape*, const GeoShape*> shapeOpsA{getOps(objA)};
    std::pair<const GeoShape*, const GeoShape*> shapeOpsB{getOps(objB)};
    
    CALL_SORTER(shapeOpsA.first, shapeOpsB.first);
    CALL_SORTER(shapeOpsA.second, shapeOpsB.second);
  } 
  ///
  else if (typeID == GeoShapeShift::getClassTypeID()) {
    const GeoShapeShift* shapeA = dynamic_cast<const GeoShapeShift*>(objA);
    const GeoShapeShift* shapeB = dynamic_cast<const GeoShapeShift*>(objB);
    const int  xCmp = transCmp.compare(shapeA->getX(), shapeB->getX());
    if (xCmp) {
        return xCmp;
    }
    CALL_SORTER(shapeA->getOp(), shapeB->getOp());
  } 
  else if (typeID == GeoBox::getClassTypeID()) {
    const GeoBox* boxA = dynamic_cast<const GeoBox*>(objA);
    const GeoBox* boxB = dynamic_cast<const GeoBox*>(objB);
    CHECK_PROPERTY(boxA, boxB, getXHalfLength);
    CHECK_PROPERTY(boxA, boxB, getYHalfLength);
    CHECK_PROPERTY(boxA, boxB, getZHalfLength);
  } else if (typeID == GeoTrd::getClassTypeID()) {
    const GeoTrd* trdA = dynamic_cast<const GeoTrd*>(objA);
    const GeoTrd* trdB = dynamic_cast<const GeoTrd*>(objB);
    CHECK_PROPERTY(trdA, trdB, getXHalfLength1);
    CHECK_PROPERTY(trdA, trdB, getXHalfLength2);
    CHECK_PROPERTY(trdA, trdB, getYHalfLength1);
    CHECK_PROPERTY(trdA, trdB, getYHalfLength2);
    CHECK_PROPERTY(trdA, trdB, getZHalfLength);
  } else if (typeID == GeoTube::getClassTypeID()) {
    const GeoTube* tubeA = dynamic_cast<const GeoTube*>(objA);
    const GeoTube* tubeB = dynamic_cast<const GeoTube*>(objB);
    CHECK_PROPERTY(tubeA, tubeB, getRMin);
    CHECK_PROPERTY(tubeA, tubeB, getRMax);
    CHECK_PROPERTY(tubeA, tubeB, getZHalfLength);
  } else if (typeID == GeoTubs::getClassTypeID()) {
    const GeoTubs* tubeA = dynamic_cast<const GeoTubs*>(objA);
    const GeoTubs* tubeB = dynamic_cast<const GeoTubs*>(objB);
    CHECK_PROPERTY(tubeA, tubeB, getRMin);
    CHECK_PROPERTY(tubeA, tubeB, getRMax);
    CHECK_PROPERTY(tubeA, tubeB, getZHalfLength);
    CHECK_PROPERTY(tubeA, tubeB, getSPhi);
    CHECK_PROPERTY(tubeA, tubeB, getDPhi);
  } else if (typeID == GeoCons::getClassTypeID()) {
    const GeoCons* consA = dynamic_cast<const GeoCons*>(objA);
    const GeoCons* consB = dynamic_cast<const GeoCons*>(objB);
    CHECK_PROPERTY(consA, consB, getRMin1);
    CHECK_PROPERTY(consA, consB, getRMin2);
    CHECK_PROPERTY(consA, consB, getRMin1);
    CHECK_PROPERTY(consA, consB, getRMax1);
    CHECK_PROPERTY(consA, consB, getRMax2);
    CHECK_PROPERTY(consA, consB, getDZ);
    CHECK_PROPERTY(consA, consB, getSPhi);
    CHECK_PROPERTY(consA, consB, getDPhi);
  } else if (typeID == GeoEllipticalTube::getClassTypeID()) {
    const GeoEllipticalTube* tubeA = dynamic_cast<const GeoEllipticalTube*>(objA);
    const GeoEllipticalTube* tubeB = dynamic_cast<const GeoEllipticalTube*>(objB);
    CHECK_PROPERTY(tubeA, tubeB, getXHalfLength);
    CHECK_PROPERTY(tubeA, tubeB, getYHalfLength);
    CHECK_PROPERTY(tubeA, tubeB, getZHalfLength);
   } else if (typeID == GeoPara::getClassTypeID()) {
     const GeoPara* paraA = dynamic_cast<const GeoPara*>(objA);
     const GeoPara* paraB = dynamic_cast<const GeoPara*>(objB);
     CHECK_PROPERTY(paraA, paraB, getXHalfLength);
     CHECK_PROPERTY(paraA, paraB, getYHalfLength);
     CHECK_PROPERTY(paraA, paraB, getZHalfLength);   
     CHECK_PROPERTY(paraA, paraB, getTheta);
     CHECK_PROPERTY(paraA, paraB, getAlpha);
     CHECK_PROPERTY(paraA, paraB, getPhi); 
   } else if (typeID == GeoGenericTrap::getClassTypeID()) {
      const GeoGenericTrap* trapA = dynamic_cast<const GeoGenericTrap*>(objA);
      const GeoGenericTrap* trapB = dynamic_cast<const GeoGenericTrap*>(objB);
      CHECK_PROPERTY(trapA, trapB, getZHalfLength);
      CHECK_PROPERTY(trapA, trapB, getVertices().size);
      for (size_t v = 0; v < trapA->getVertices().size(); ++v) {
          const int compare = transCmp.compare(trapA->getVertices()[v], trapB->getVertices()[v]);
          if (compare) return compare;
      }
   } else if (typeID == GeoPcon::getClassTypeID()) {
     const GeoPcon* pconA = dynamic_cast<const GeoPcon*>(objA);
     const GeoPcon* pconB = dynamic_cast<const GeoPcon*>(objB);
     CHECK_PROPERTY(pconA, pconB, getNPlanes);
     CHECK_PROPERTY(pconA, pconB, getSPhi);
     CHECK_PROPERTY(pconA, pconB, getDPhi);
     CHECK_VEC_PROPERTY(pconA, pconB, getZPlane, pconA->getNPlanes());
     CHECK_VEC_PROPERTY(pconA, pconB, getRMinPlane, pconA->getNPlanes());
     CHECK_VEC_PROPERTY(pconA, pconB, getRMaxPlane, pconA->getNPlanes());
   } else if (typeID == GeoPgon::getClassTypeID()) {
     const GeoPgon* pgonA = dynamic_cast<const GeoPgon*>(objA);
     const GeoPgon* pgonB = dynamic_cast<const GeoPgon*>(objB);
     CHECK_PROPERTY(pgonA, pgonB, getNPlanes);
     CHECK_PROPERTY(pgonA, pgonB, getNSides);
     CHECK_PROPERTY(pgonA, pgonB, getSPhi);
     CHECK_PROPERTY(pgonA, pgonB, getDPhi);
     CHECK_VEC_PROPERTY(pgonA, pgonB, getZPlane, pgonA->getNPlanes());
     CHECK_VEC_PROPERTY(pgonA, pgonB, getRMinPlane, pgonA->getNPlanes());
     CHECK_VEC_PROPERTY(pgonA, pgonB, getRMaxPlane, pgonA->getNPlanes());
   } else if (typeID == GeoSimplePolygonBrep::getClassTypeID()) {
      const GeoSimplePolygonBrep* brepA = dynamic_cast<const GeoSimplePolygonBrep*>(objA);
      const GeoSimplePolygonBrep* brepB = dynamic_cast<const GeoSimplePolygonBrep*>(objB);
      CHECK_PROPERTY(brepA, brepB, getNVertices);
      CHECK_PROPERTY(brepA, brepB, getDZ);
      CHECK_VEC_PROPERTY(brepA, brepB, getXVertex, brepA->getNVertices());
      CHECK_VEC_PROPERTY(brepA, brepB, getYVertex, brepA->getNVertices());
   } else if (typeID == GeoTorus::getClassTypeID()) {
      const GeoTorus* torA = dynamic_cast<const GeoTorus*>(objA);
      const GeoTorus* torB = dynamic_cast<const GeoTorus*>(objB);
      CHECK_PROPERTY(torA, torB, getRMin);
      CHECK_PROPERTY(torA, torB, getRMax);
      CHECK_PROPERTY(torA, torB, getRTor);
      CHECK_PROPERTY(torA, torB, getSPhi);
      CHECK_PROPERTY(torA, torB, getDPhi);
   } else if (typeID == GeoTrap::getClassTypeID()) {
      const GeoTrap* trapA = dynamic_cast<const GeoTrap*>(objA);
      const GeoTrap* trapB = dynamic_cast<const GeoTrap*>(objB);
      CHECK_PROPERTY(trapA, trapB, getZHalfLength);
      CHECK_PROPERTY(trapA, trapB, getTheta);
      CHECK_PROPERTY(trapA, trapB, getPhi);
      CHECK_PROPERTY(trapA, trapB, getDydzn);
      CHECK_PROPERTY(trapA, trapB, getDxdyndzn);      
      CHECK_PROPERTY(trapA, trapB, getDxdypdzn);
      CHECK_PROPERTY(trapA, trapB, getAngleydzn);
      CHECK_PROPERTY(trapA, trapB, getDydzp);
      CHECK_PROPERTY(trapA, trapB, getDxdyndzp);
      CHECK_PROPERTY(trapA, trapB, getDxdypdzp);
      CHECK_PROPERTY(trapA, trapB, getAngleydzp);
   } else if (typeID == GeoTwistedTrap::getClassTypeID()) {
      const GeoTwistedTrap* trapA = dynamic_cast<const GeoTwistedTrap*>(objA);
      const GeoTwistedTrap* trapB = dynamic_cast<const GeoTwistedTrap*>(objB);
      CHECK_PROPERTY(trapA, trapB, getY1HalfLength);
      CHECK_PROPERTY(trapA, trapB, getX1HalfLength);
      CHECK_PROPERTY(trapA, trapB, getX2HalfLength);
      CHECK_PROPERTY(trapA, trapB, getY2HalfLength);
      CHECK_PROPERTY(trapA, trapB, getX3HalfLength);
      CHECK_PROPERTY(trapA, trapB, getX4HalfLength);
      CHECK_PROPERTY(trapA, trapB, getZHalfLength);
      CHECK_PROPERTY(trapA, trapB, getPhiTwist);
      CHECK_PROPERTY(trapA, trapB, getTheta);
      CHECK_PROPERTY(trapA, trapB, getPhi);
      CHECK_PROPERTY(trapA, trapB, getTiltAngleAlpha);
   } else {
    THROW_EXCEPTION("The shape "<<objA->type()<<" has not yet been implemented into the sorter");
  }
  return 0;
}

#undef CHECK_PROPERTY
#undef CHECK_VEC_PROPERTY
#undef COMPARE_GEOVEC
#undef CALL_SORTER
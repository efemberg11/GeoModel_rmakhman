/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelFuncSnippets/GeoShapeUtils.h"
#include "GeoModelFuncSnippets/TransformToStringConverter.h"

/// Boolean volume shapes
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShape.h"
/// Ordinary shapes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoEllipticalTube.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoTorus.h"

#include "GeoModelKernel/Units.h"

#include <vector>

std::pair<const GeoShape* , const GeoShape*> getOps(const GeoShape* composed) {
    if (composed->typeID() == GeoShapeUnion::getClassTypeID()) {
        const GeoShapeUnion* unionShape = dynamic_cast<const GeoShapeUnion*>(composed);
        return std::make_pair(unionShape->getOpA(), unionShape->getOpB());
    } else if (composed->typeID() == GeoShapeSubtraction::getClassTypeID()) {
        const GeoShapeSubtraction* shapeSubtract = dynamic_cast<const GeoShapeSubtraction*>(composed);
        return std::make_pair(shapeSubtract->getOpA(), shapeSubtract->getOpB());
    } else if (composed->typeID() == GeoShapeUnion::getClassTypeID()) {
        const GeoShapeIntersection* shapeIntersect = dynamic_cast<const GeoShapeIntersection*>(composed);
        return std::make_pair(shapeIntersect->getOpA(), shapeIntersect->getOpB());
    } else if (composed->typeID() == GeoShapeShift::getClassTypeID()) {
        const GeoShapeShift* shapeShift = dynamic_cast<const GeoShapeShift*>(composed);
        return std::make_pair(shapeShift->getOp(), nullptr);
    }
    return std::make_pair(nullptr, nullptr);
}

unsigned int countComposedShapes(const GeoShape* shape) {
    std::pair<const GeoShape*, const GeoShape*> ops = getOps(shape);
    return 1 + (ops.first ? countComposedShapes(ops.first) : 0) +
                (ops.second ? countComposedShapes(ops.second) : 0);
}

GeoIntrusivePtr<const GeoShape> compressShift(const GeoShape* shift) {    
    if (shift->typeID() != GeoShapeShift::getClassTypeID()) return GeoIntrusivePtr<const GeoShape>{shift};
    const GeoShapeShift* shapeShift = dynamic_cast<const GeoShapeShift*>(shift);
    if (shapeShift->getOp()->typeID() != GeoShapeShift::getClassTypeID()) return GeoIntrusivePtr<const GeoShape>{shift};
    GeoIntrusivePtr<const GeoShape> subShape{compressShift(shapeShift->getOp())};
    const GeoShapeShift* subShift = dynamic_cast<const GeoShapeShift*>(subShape.get());
    return GeoIntrusivePtr<const GeoShape>{new GeoShapeShift(subShift->getOp(), subShift->getX() * shapeShift->getX())};
}
std::vector<const GeoShape*> getBooleanComponents(const GeoShape* booleanShape) {
     std::pair<const GeoShape*, const GeoShape*> operands = getOps(booleanShape);
     if (!operands.first || !operands.second){
        return {compressShift(booleanShape)};
     }
     std::vector<const GeoShape*> components{};
     if (booleanShape->typeID() != GeoShapeSubtraction::getClassTypeID()) {
        components = getBooleanComponents(operands.first);
        std::vector<const GeoShape*> secCmp = getBooleanComponents(operands.second);
        components.insert(components.end(), std::make_move_iterator(secCmp.begin()), 
                                            std::make_move_iterator(secCmp.end()));
     } else {
        if (operands.first->typeID() == GeoShapeSubtraction::getClassTypeID()) {
            components = getBooleanComponents(operands.first);
        }
        components.push_back(compressShift(operands.second));             
     }
     return components;
}



std::string printGeoShape(const GeoShape* shape) {
    std::stringstream ostr{};
    constexpr double toDeg{1./GeoModelKernelUnits::deg};
    ostr<<shape->type()<<" ("<<shape<<") ";
    const int typeID = shape->typeID();
    if (typeID == GeoShapeUnion::getClassTypeID()) {
        const GeoShapeUnion* shapeUnion = dynamic_cast<const GeoShapeUnion*>(shape);
        ostr<<"of {"<<printGeoShape(shapeUnion->getOpA())<<"} & {"<<printGeoShape(shapeUnion->getOpB())<<"}";        
    } else if (typeID == GeoShapeSubtraction::getClassTypeID()) {
        const GeoShapeSubtraction* subtractShape = dynamic_cast<const GeoShapeSubtraction*>(shape);
        ostr<<"of {"<<printGeoShape(subtractShape->getOpB())<<"} from {"<<printGeoShape(subtractShape->getOpA())<<"}";
    } else if (typeID == GeoShapeIntersection::getClassTypeID()) {
        const GeoShapeIntersection* intersectShape = dynamic_cast<const GeoShapeIntersection*>(shape);
        ostr<<"between {"<<printGeoShape(intersectShape->getOpA())<<"} & {"<<printGeoShape(intersectShape->getOpB())<<"}";        
    } else if (typeID == GeoShapeShift::getClassTypeID()) {
        const GeoShapeShift* shiftShape = dynamic_cast<const GeoShapeShift*>(shape);
        ostr<<"of "<<printGeoShape(shiftShape->getOp())<<" by "<<GeoTrf::toString(shiftShape->getX()); 
    }
    /// Elementary shape types
    else if (typeID == GeoBox::getClassTypeID()) {
        const GeoBox* boxShape = dynamic_cast<const GeoBox*>(shape);
        ostr<<" halfX="<<boxShape->getXHalfLength()<<", halfY="<<boxShape->getYHalfLength()<<", halfZ="<<boxShape->getZHalfLength();
    } else if (typeID == GeoTrd::getClassTypeID()) {
        const GeoTrd* trd = dynamic_cast<const GeoTrd*>(shape);
        ostr<<"halfX="<<trd->getXHalfLength1()<<"/"<<trd->getXHalfLength2()<<", ";
        ostr<<"halfY="<<trd->getYHalfLength1()<<"/"<<trd->getYHalfLength2()<<", ";
        ostr<<"halfZ="<<trd->getZHalfLength();
   } else if (typeID == GeoTube::getClassTypeID()) {
        const GeoTube* tube = dynamic_cast<const GeoTube*>(shape);
        ostr<<"rMin="<<tube->getRMin()<<", ";
        ostr<<"rMax="<<tube->getRMax()<<", ";
        ostr<<"halfZLength="<<tube->getZHalfLength();
    } else if (typeID == GeoTubs::getClassTypeID()) {
        const GeoTubs* tube = dynamic_cast<const GeoTubs*>(shape);
        ostr<<"rMin="<<tube->getRMin()<<", ";
        ostr<<"rMax="<<tube->getRMax()<<", ";
        ostr<<"start phi="<<tube->getSPhi()*toDeg<<", ";
        ostr<<"dPhi="<<tube->getDPhi()*toDeg<<", ";
        ostr<<"halfZ="<<tube->getZHalfLength();
    } else if (typeID == GeoCons::getClassTypeID()) {
        const GeoCons* cons = dynamic_cast<const GeoCons*>(shape);
        ostr<<"r1="<<cons->getRMin1()<<"/"<<cons->getRMax1()<<", ";
        ostr<<"r2="<<cons->getRMin2()<<"/"<<cons->getRMax2()<<", ";
        ostr<<"dZ="<<cons->getDZ()<<", ";
        ostr<<"start phi="<<cons->getSPhi()*toDeg<<", ";
        ostr<<"dPhi="<<cons->getDPhi()*toDeg;
   } else if (typeID == GeoEllipticalTube::getClassTypeID()) {
        const GeoEllipticalTube* tube = dynamic_cast<const GeoEllipticalTube*>(shape);
        ostr<<"halfX="<<tube->getXHalfLength()<<", halfY="<<tube->getYHalfLength()<<", halfZ="<<tube->getZHalfLength();
   } else if (typeID == GeoPara::getClassTypeID()) {
        const GeoPara* para = dynamic_cast<const GeoPara*>(shape);
        ostr<<"halfX="<<para->getXHalfLength()<<", halfY="<<para->getYHalfLength()<<", halfZ="<<para->getZHalfLength();
        ostr<<"theta="<<para->getTheta()*toDeg<<", alpha="<<para->getAlpha()*toDeg<<", phi="<<para->getPhi()*toDeg; 
   } else if (typeID == GeoTorus::getClassTypeID()) {
      const GeoTorus* torus = dynamic_cast<const GeoTorus*>(shape);
      ostr<<"r="<<torus->getRMin()<<"/"<<torus->getRMax()<<", ";
      ostr<<"Torus R="<<torus->getRTor()<<", ";
      ostr<<"sPhi="<<torus->getSPhi()*toDeg<<", ";
      ostr<<"dPhi="<<torus->getDPhi()*toDeg;
   }
    return ostr.str();
}

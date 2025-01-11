/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/GeoShapeUtils.h"
#include "GeoModelHelpers/TransformToStringConverter.h"
#include "GeoModelKernel/throwExcept.h"

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
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoGenericTrap.h"

#include "GeoModelKernel/Units.h"
#include "GeoModelHelpers/TransformSorter.h"

#include <vector>

namespace{
    constexpr unsigned tubeApproxEdges = 8;
}

std::pair<const GeoShape* , const GeoShape*> getOps(const GeoShape* composed) {
    if (composed->typeID() == GeoShapeUnion::getClassTypeID()) {
        const GeoShapeUnion* unionShape = dynamic_cast<const GeoShapeUnion*>(composed);
        return std::make_pair(unionShape->getOpA(), unionShape->getOpB());
    } else if (composed->typeID() == GeoShapeSubtraction::getClassTypeID()) {
        const GeoShapeSubtraction* shapeSubtract = dynamic_cast<const GeoShapeSubtraction*>(composed);
        return std::make_pair(shapeSubtract->getOpA(), shapeSubtract->getOpB());
    } else if (composed->typeID() == GeoShapeIntersection::getClassTypeID()) {
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
    GeoIntrusivePtr<const GeoShape> retPtr{shift};
    if (shift->typeID() != GeoShapeShift::getClassTypeID()) {
        return retPtr;
    }
    
    GeoIntrusivePtr<const GeoShapeShift> shapeShift = dynamic_pointer_cast<const GeoShapeShift>(retPtr);
    /// If the shape shift is an Identity no need to go go further.
    if (!GeoTrf::TransformSorter{}.compare(shapeShift->getX(), GeoTrf::Transform3D::Identity())) {
        retPtr = shapeShift->getOp();
    }
    if (shapeShift->getOp()->typeID() != GeoShapeShift::getClassTypeID()) {
        return retPtr;
    }
    GeoIntrusivePtr<const GeoShape> subShape{compressShift(shapeShift->getOp())};
    
    GeoIntrusivePtr<const GeoShapeShift> subShift = dynamic_pointer_cast<const GeoShapeShift>(subShape);
    /// Check that the GeoShape shift is actually neccesary. If not bail out
    GeoTrf::Transform3D shiftTrf{subShift->getX() * shapeShift->getX()};
    if (!GeoTrf::TransformSorter{}.compare(shiftTrf, GeoTrf::Transform3D::Identity())) {
        return GeoIntrusivePtr<const GeoShape>{subShift->getOp()};
    }
    return make_intrusive<GeoShapeShift>(subShift->getOp(), std::move(shiftTrf));
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
    ostr<<shape->type()<<" ("<<shape<<"/"<<shape->refCount()<<") ";
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
   } else if (typeID == GeoGenericTrap::getClassTypeID()) {
        const GeoGenericTrap* trap = dynamic_cast<const GeoGenericTrap*>(shape);
        ostr<<"half Z: "<<trap->getZHalfLength()<<" n Vertices: "<<trap->getVertices().size()<<std::endl;
        for ( const GeoTrf::Vector2D& vec : trap->getVertices()) {
            ostr<<"  **** "<<GeoTrf::toString(vec)<<std::endl;
        }
   }
    return ostr.str();
}

std::vector<GeoTrf::Vector3D> getPolyShapeEdges(const GeoShape* shape,
                                                const GeoTrf::Transform3D& refTrf) {

    if (!shape) {
        THROW_EXCEPTION("Nullptr was given ");
    }
    constexpr double boundary = 0.;
    std::vector<GeoTrf::Vector3D> edgePoints{};
    std::pair<const GeoShape*, const GeoShape*> ops = getOps(shape);
    if (shape->typeID() == GeoShapeUnion::getClassTypeID()){
        edgePoints = getPolyShapeEdges(ops.first, refTrf);
        std::vector<GeoTrf::Vector3D> edgePoints2{getPolyShapeEdges(ops.second, refTrf)};
        edgePoints.insert(edgePoints.end(),
                          std::make_move_iterator(edgePoints2.begin()),
                          std::make_move_iterator(edgePoints2.end()));
    } else if (shape->typeID() == GeoShapeSubtraction::getClassTypeID()) {
        return getPolyShapeEdges(ops.first, refTrf);
    } else if (shape->typeID() == GeoBox::getClassTypeID()) {
        edgePoints.reserve(6);
        const GeoBox* box = static_cast<const GeoBox*>(shape);
        for (double sX :{-1., 1.}) {
            for (double sY :{-1., 1.}) {
                for (double sZ: {-1., 1.}) {
                    edgePoints.emplace_back(refTrf * GeoTrf::Vector3D{sX* (box->getXHalfLength() - boundary),
                                                                      sY* (box->getYHalfLength() - boundary),
                                                                      sZ* (box->getZHalfLength() - boundary)});
                }
            }
        }
    } else if (shape->typeID() == GeoShapeShift::getClassTypeID()) {
        GeoIntrusivePtr<const GeoShape> shift = compressShift(shape);
        const GeoShapeShift* shiftPtr = static_cast<const GeoShapeShift*>(shift.get());
        std::vector<GeoTrf::Vector3D> shiftedEdges = getPolyShapeEdges(ops.first, shiftPtr->getX());
        std::transform(shiftedEdges.begin(), shiftedEdges.end(), std::back_inserter(edgePoints),
                    [&refTrf](const GeoTrf::Vector3D& shift){
                            return refTrf * shift;
                    });
    } else if (shape->typeID() == GeoTrd::getClassTypeID()) {
        const GeoTrd* trd = static_cast<const GeoTrd*>(shape);
        edgePoints.reserve(6);
        for (double sZ : {-1., 1.}){
            double dX = (sZ < 0 ? trd->getXHalfLength1() : trd->getXHalfLength2()) - boundary;
            double dY = (sZ < 0 ? trd->getYHalfLength1() : trd->getYHalfLength2()) - boundary;
            for (double sX: {-1., 1.}) {
                for (double sY: {-1., 1.}) {
                    edgePoints.emplace_back(refTrf * GeoTrf::Vector3D{sX* dX, sY* dY,
                                                                      sZ* (trd->getZHalfLength()- boundary)});

                }
            }
            
        }
    } else if (shape->typeID() == GeoSimplePolygonBrep::getClassTypeID()) {
        const GeoSimplePolygonBrep* brep = static_cast<const GeoSimplePolygonBrep*>(shape);
        edgePoints.reserve(2* brep->getNVertices());
        for (double sZ: {-1., 1.}) {
            for (unsigned int vtx = 0 ; vtx < brep->getNVertices(); ++vtx){
                edgePoints.emplace_back(refTrf * GeoTrf::Vector3D{brep->getXVertex(vtx),
                                                                  brep->getYVertex(vtx),
                                                                  sZ * brep->getDZ()});
            
            }
        }
    } else if (shape->typeID() == GeoTube::getClassTypeID()) {
        constexpr double stepLength = 2.*M_PI / tubeApproxEdges;
        const GeoTube* tube = static_cast<const GeoTube*>(shape);
        for (const double z :{-1., 1.}){
            for (unsigned int e = 0; e < tubeApproxEdges; ++e) {
                edgePoints.emplace_back(tube->getRMax()*std::cos(stepLength*e),
                                        tube->getRMax()*std::sin(stepLength*e),
                                        z*tube->getZHalfLength());
            }
        }
    } else if (shape->typeID() == GeoTubs::getClassTypeID()) {
        const GeoTubs* tubs = static_cast<const GeoTubs*>(shape);
        const double stepSize = tubs->getDPhi() / tubeApproxEdges;
        for (const double z : {-1., 1.}) {
            for (unsigned e = 0; e <= tubeApproxEdges; ++e) {
                edgePoints.emplace_back(tubs->getRMax()*std::cos(tubs->getSPhi() + stepSize*e), 
                                        tubs->getRMax()*std::sin(tubs->getSPhi() + stepSize*e), 
                                        z*tubs->getZHalfLength());
            }
        }
    } else if (shape->typeID() == GeoEllipticalTube::getClassTypeID()) {
        const GeoEllipticalTube* tube = static_cast<const GeoEllipticalTube*>(shape);
        constexpr double stepSize = 2.*M_PI / tubeApproxEdges;
        for (const double z : {-1.,1.}) {
            for (unsigned e = 0; e<tubeApproxEdges; ++e){
                edgePoints.emplace_back(tube->getXHalfLength()*std::cos(stepSize*e),
                                        tube->getYHalfLength()*std::sin(stepSize*e),
                                        z* tube->getZHalfLength());
            }
        }
    } else if (shape->typeID() == GeoTorus::getClassTypeID()) {
        constexpr double stepSize = 2.*M_PI / tubeApproxEdges;
        const GeoTorus* torus = static_cast<const GeoTorus*>(shape);
        for (unsigned int donut =0; donut < tubeApproxEdges; ++donut) {
            for (unsigned int slice =0; slice < tubeApproxEdges; ++slice) {
                edgePoints.emplace_back((torus->getRTor() + torus->getRMax()* std::cos(slice *stepSize))*std::cos(donut*stepSize),
                                        (torus->getRTor() + torus->getRMax()* std::cos(slice *stepSize))*std::sin(donut*stepSize),
                                         torus->getRMax()* std::sin(slice*stepSize));
            }
        }
    } else {
        THROW_EXCEPTION("The shape "<<shape->type()<<" is not supported. Please add it to the list");
    }
    return edgePoints;
}

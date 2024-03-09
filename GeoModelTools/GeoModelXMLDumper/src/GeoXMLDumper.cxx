/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelXMLDumper/GeoXMLDumper.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoVolumeCursor.h"

#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"


#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoEllipticalTube.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"

#include "GeoModelHelpers/GeoShapeUtils.h"
#include "GeoModelHelpers/getChildNodesWithTrf.h"
#include "GeoModelHelpers/throwExcept.h"
#include "GeoModelHelpers/TransformToStringConverter.h"
#include "GeoModelHelpers/StringUtils.h"
#include "GeoModelHelpers/FileUtils.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <fstream>
#include <string_view>
#include <functional>


using namespace GeoStrUtils;
using namespace GeoFileUtils;
namespace {
    constexpr unsigned int stdIndent = 4;
    constexpr double transTolerance = 0.1 * GeoModelKernelUnits::micrometer;
    constexpr double toDeg = 1./ GeoModelKernelUnits::deg;

    std::string shapeRefTag(const std::string& shapeName, unsigned int nWhite) {
        std::stringstream sstr{};
        sstr<<whiteSpaces(nWhite)<<"<shaperef ref=\""<<shapeName<<"\" />";
        return sstr.str();
    }

    std::string fileNameBody(const std::string_view path) {
        const unsigned slashPos = path.rfind("/") + 1;
        return std::string{path.substr(slashPos, path.rfind(".") - slashPos)};
    }
    static const GeoTrf::TransformSorter transformSorter{};
}


GeoXMLDumper::GeoXMLDumper(const std::string& outDir,
                           const std::string& systemName,
                           const std::string& xmlLayoutFile):
    m_outDir{outDir},
    m_systemName{systemName},
    m_xmlLayoutFile{xmlLayoutFile} {
        if (m_outDir.size() && m_outDir[m_outDir.size() -1] != '/') {
            m_outDir+="/";
        }
}

void GeoXMLDumper::countVolume(PVConstLink volume){
    ++(m_physVolCounts[volume]);
    GeoVolumeCursor cursor{volume};
    while (!cursor.atEnd()) {
        countVolume(cursor.getVolume());
        cursor.next();
    }
}

bool GeoXMLDumper::publishWorld(PVConstLink world) {
    if (m_outDir.empty()) {
        std::cerr<<"Please define an output directory "<<std::endl;
        return false;
    }
    /// Count how many distinct volumes are in the world and also at how many nodes each volume appears
    std::cout<<"Traverse the GeoModel tree to count the different physical volumes "<<std::endl;
    countVolume(world);
    {
        unsigned int counts{0};
        for (const auto& [vol, c] : m_physVolCounts) counts+=c;
        std::cout<<"Found in total "<<m_physVolCounts.size()<<" different volumes used in "<<counts<<" nodes."<<std::endl;        
    }
    /// If the world only has one volume get to its child
    while (world->getNChildVols() == 1)  {
        world = world->getChildVol(0);
    }

    GeoVolumeCursor cursor{world};

    topVolWithPathList publishedTops{};
    while (!cursor.atEnd()) {
        PVConstLink worldNode{cursor.getVolume()};
        const std::string out_xmlDir = m_outDir + "volumes/";
        if (!mkdir(out_xmlDir)) {
            std::cerr<<"Failed to create directory "<<out_xmlDir<<std::endl;
            return false;
        }
        const std::string out_xmlFile = out_xmlDir + 
                                        replaceExpInString(cursor.getName()," ", "_") + ".xml";

        std::ofstream outFile{out_xmlFile};
        if (!outFile.good()) {
            std::cerr<<"Failed to open "<<out_xmlFile<<std::endl;
            return false;
        }
        const std::string pubVolume{publishPhysVol(outFile, cursor.getVolume(),  cursor.getName())};
        publishedTops.emplace_back(std::make_pair(out_xmlFile, pubVolume));
        cursor.next();
    }

    const std::string out_xmlFile = m_outDir+ m_systemName +".xml";

    std::ofstream outFile{out_xmlFile};
    if (!outFile.good()) {
        std::cerr<<"Failed to open "<<out_xmlFile<<std::endl;
        return false;
    } 
    dumpGeoModelHeader(outFile, publishedTops);
    return true;
}
std::string GeoXMLDumper::uniqueNameTag(std::string nameTag,
                                        const std::string_view idToken) {
    nameTag = replaceExpInString(nameTag, " ", "_");
    unsigned int& nameTagCounts{m_nameTagCounts[nameTag]};
    if (nameTagCounts> 0) {
        nameTag += std::string{idToken}+"No"+std::to_string(nameTagCounts);
    }
    ++nameTagCounts;
    return nameTag;
}
std::string GeoXMLDumper::publishPhysVol(std::ostream& outStream,
                                         PVConstLink physVol,
                                         const std::string& volName,
                                         unsigned int indentation) {
    
    physVolMap::const_iterator itr = m_physVolNameTags.find(physVol);
    if (itr != m_physVolNameTags.end()) return itr->second;
    
    std::string nameTag{volName};

    std::stringstream shapeStream{}, childStream{};
 
    /// Retrieve the children together with their transformations
    std::vector<GeoChildNodeWithTrf> children = getChildrenWithRef(physVol, true);
    for (const GeoChildNodeWithTrf& child : children) {
        /// Dump the transformation
        
        unsigned int childIndent = indentation + stdIndent;

        const bool dumpTrf{child.isAlignable || transformSorter.compare(GeoTrf::Transform3D::Identity(), child.transform)};

        if (dumpTrf) {
            childStream<<whiteSpaces(indentation + stdIndent)
                       <<"<transform name=\""<<uniqueNameTag(child.nodeName, "VolTrf")<<"\" ";
            if (child.isAlignable)childStream<<"alignable=\"true\" ";
            childStream<<">"<<std::endl;
            
            childIndent += stdIndent;
            dumpTransform(childStream, child.transform, child.nodeName, childIndent);           
        }
        if (child.nCopies > 1) {
            childStream<<whiteSpaces(childIndent)<<"<assembly name=\""
                       <<uniqueNameTag(child.nodeName, "Assembly")<<"\">"<<std::endl;
            childIndent +=stdIndent;
            childStream<<whiteSpaces(childIndent)<<"<multicopy n=\""<<child.nCopies<<"\" "
                       <<"name=\""<<uniqueNameTag(child.nodeName, "MultiCopy")<<"\">"<<std::endl;
            childIndent+=stdIndent;
            dumpTransform(childStream, child.inductionRule, child.nodeName, childIndent);
        }
        childStream<<whiteSpaces(childIndent)<<"<logvolref ref=\""<<publishPhysVol(outStream, child.volume, child.nodeName)<<"\" />"<<std::endl;
        if (child.nCopies > 1) {
            childIndent-=stdIndent;
            childStream<<whiteSpaces(childIndent)<<"</multicopy>"<<std::endl;
            childIndent-=stdIndent;
            childStream<<whiteSpaces(childIndent)<<"</assembly>"<<std::endl;
        }
        if (dumpTrf) {
            childStream<<whiteSpaces(childIndent - stdIndent)<<"</transform>"<<std::endl;
        }
        childStream<<std::endl;        
    }
    
    const std::string shapeName = publishShape(shapeStream, physVol->getLogVol()->getShape(), nameTag);
    const std::string pubShapes{shapeStream.str()};
    if (pubShapes.size()) {
        outStream<<"<shapes>"<<std::endl;
        outStream<<pubShapes;
        outStream<<"</shapes>"<<std::endl;
    }
    nameTag = uniqueNameTag(nameTag, "logVol");

    outStream<<whiteSpaces(indentation)<<"<logvol name=\""<<nameTag<<"\" ";
    const std::string matName = replaceExpInString(physVol->getLogVol()->getMaterial()->getName(), "::", "__dColon__");

    outStream<<"shape=\""<<shapeName<<"\" material=\""<<matName<<"\" ";
    if (children.empty()) {
        outStream<<"/>"<<std::endl;
    } else {
        outStream<<">"<<std::endl;
        outStream<<childStream.str();    
        outStream<<whiteSpaces(indentation)<<"</logvol>"<<std::endl;
    }
    outStream<<std::endl<<std::endl;
 
    auto insert_itr = m_physVolNameTags.insert(std::make_pair(physVol, nameTag));
    if (!insert_itr.second) {
        THROW_EXCEPTION("Failed to cache physical volume "<<physVol.get()<<" with shape "<<printGeoShape(physVol->getLogVol()->getShape())
                       <<" as "<<nameTag<<std::endl<<"The volume "<<insert_itr.first->first.get()<<" with shape "
                       <<printGeoShape(insert_itr.first->first->getLogVol()->getShape())<<" as "<<insert_itr.first->second
                       <<" is blocking the way");
    }

    return insert_itr.first->second;
}

std::string GeoXMLDumper::publishShape(std::ostream& shapeStream,
                                       GeoIntrusivePtr<const GeoShape> shape,
                                       const std::string& shapeName) {
    
    /// The Volume has been already published before refer to it
    shapeMap::const_iterator itr = m_shapeNameTags.find(shape);
    if (itr != m_shapeNameTags.end()) return itr->second;
    
    
    std::pair<const GeoShape*, const GeoShape*> operands = getOps(shape);
    /// The shape is a boolean volume (GeoShapeUnion, Subtraction, Intersection)
    if (operands.first && operands.second) {
        publishShape(shapeStream, operands.first, shapeName);
        publishShape(shapeStream, operands.second, shapeName);
    } 
    /// The shape is a  GeoShapeShift 
    else if (operands.first) {
        /// In fact there might be several Shape shifts in a row. Collapse them into a single shift
        /// and publish the underlying shape
        operands = getOps(compressShift(shape));
        publishShape(shapeStream, operands.first, shapeName);
    }
    
    /// Create a unique name tag
    const std::string nameTag{uniqueNameTag(shapeName.size() ? shapeName : shape->type(), "Shape")};
    /// Register the shape
    const auto insert_itr = m_shapeNameTags.insert(std::make_pair(shape, nameTag));
    if (!insert_itr.second) {
        THROW_EXCEPTION("The insertion of shape "<<printGeoShape(shape)<<" as "<<nameTag<<" failed."<<std::endl<<
                        "The shape "<<printGeoShape(insert_itr.first->first)<<" as "<<insert_itr.first->second
                        << " has been added before. ");
    }

    auto dumpParameter = [&shapeStream](const std::string_view parName, 
                                        const double value,
                                        bool isAngle = false) {
        
        shapeStream<<parName<<"=\"";
        if (isAngle) {
            shapeStream<<value*toDeg<<"*deg\" ";
        } else {
            shapeStream<<value<<"\" ";
        }
    };

    constexpr unsigned int indentation = stdIndent;
    const unsigned int shapeID = shape->typeID();    
    shapeStream<<whiteSpaces(indentation);
    if (shapeID == GeoShapeUnion::getClassTypeID() ||
        shapeID == GeoShapeSubtraction::getClassTypeID() ||
        shapeID == GeoShapeIntersection::getClassTypeID()) {       
        
        const std::string xmlField = shapeID == GeoShapeUnion::getClassTypeID() ? "union" :
                                     shapeID == GeoShapeSubtraction::getClassTypeID()? "subtraction" : "intersection";
        shapeStream<<"<"<<xmlField<<" name=\""<<nameTag<<"\" >"<<std::endl;
        shapeStream<<shapeRefTag(publishShape(shapeStream, operands.first, shapeName), indentation + stdIndent)<<std::endl;
        if (operands.second->typeID() == GeoShapeShift::getClassTypeID()) {
            GeoIntrusivePtr<const GeoShape> collapsedShift = compressShift(operands.second);
            const GeoShapeShift* shift = dynamic_cast<const GeoShapeShift*>(collapsedShift.get());
            dumpTransform(shapeStream, shift->getX(), shapeName, indentation + stdIndent);
            shapeStream<<shapeRefTag(publishShape(shapeStream, shift->getOp(), shapeName), indentation + stdIndent)<<std::endl;
        } else{
            shapeStream<<shapeRefTag(publishShape(shapeStream, operands.second, shapeName), indentation + stdIndent)<<std::endl;
        }
        shapeStream<<whiteSpaces(indentation)<<"</"<<xmlField<<">"<<std::endl;
    } else if (shapeID == GeoShapeShift::getClassTypeID()) {
        shapeStream<<"<shapeshift name=\""<<nameTag<<"\" >"<<std::endl;
        GeoIntrusivePtr<const GeoShape> collapsedShift = compressShift(shape);
        const GeoShapeShift* shift = dynamic_cast<const GeoShapeShift*>(collapsedShift.get());
        dumpTransform(shapeStream, shift->getX(), shapeName, indentation + stdIndent);        
        shapeStream<<shapeRefTag(publishShape(shapeStream, operands.first, shapeName), 
                                              indentation + stdIndent)<<std::endl;
        shapeStream<<whiteSpaces(indentation)<<"</shapeshift>"<<std::endl;

    } else if (shapeID == GeoBox::getClassTypeID()) {
        shapeStream<<"<box name=\""<<nameTag<<"\" ";
        const GeoBox* box = dynamic_cast<const GeoBox*>(shape.get());
        dumpParameter("xhalflength", box->getXHalfLength());
        dumpParameter("yhalflength", box->getYHalfLength());
        dumpParameter("zhalflength", box->getZHalfLength());
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoCons::getClassTypeID()) {
        const GeoCons* cons = static_cast<const GeoCons*>(shape.get());
        shapeStream<<"<cons name=\""<<nameTag<<"\" ";
        dumpParameter("rmin1", cons->getRMin1());
        dumpParameter("rmin2", cons->getRMin2());
        dumpParameter("rmax1", cons->getRMax1());
        dumpParameter("rmax2", cons->getRMax2());
        dumpParameter("dz", cons->getDZ());
        dumpParameter("sphi", cons->getSPhi(), true);
        dumpParameter("dphi", cons->getDPhi(), true);
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoEllipticalTube::getClassTypeID()) {
        const GeoEllipticalTube* tube = static_cast<const GeoEllipticalTube*>(shape.get());
        shapeStream<<"<ellipticaltube name=\""<<nameTag<<"\" ";
        dumpParameter("xhalflength", tube->getXHalfLength());
        dumpParameter("yhalflength", tube->getYHalfLength());
        dumpParameter("zhalflength", tube->getZHalfLength());
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoGenericTrap::getClassTypeID()) {
        const GeoGenericTrap* trap = static_cast<const GeoGenericTrap*>(shape.get());
        shapeStream<<"<generictrap name=\""<<nameTag<<"\" ";
        dumpParameter("zhalflength", trap->getZHalfLength());
        for (unsigned int v = 0; v < std::min(8lu, trap->getVertices().size()); ++v){
            shapeStream<<"x"<<v<<"=\""<<trap->getVertices()[v].x()<<"\" ";
            shapeStream<<"y"<<v<<"=\""<<trap->getVertices()[v].y()<<"\" ";
        } 
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoPara::getClassTypeID()) {
        const GeoPara* para = dynamic_cast<const GeoPara*>(shape.get());
        shapeStream<<"<para name=\""<<nameTag<<"\" ";
        dumpParameter("xhalflength", para->getXHalfLength());
        dumpParameter("yhalflength", para->getYHalfLength());
        dumpParameter("zhalflength", para->getZHalfLength());
        dumpParameter("alpha", para->getAlpha(), true);
        dumpParameter("theta", para->getTheta(), true);
        dumpParameter("phi", para->getPhi(), true);
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoPcon::getClassTypeID()) {
        const GeoPcon* pcon = dynamic_cast<const GeoPcon*>(shape.get());
        shapeStream<<"<pcon name=\""<<nameTag<<"\" ";
        dumpParameter("sphi", pcon->getSPhi(), true);
        dumpParameter("dphi", pcon->getSPhi(), true);
        shapeStream<<">"<<std::endl;
        for (unsigned int plane = 0 ; plane < pcon->getNPlanes(); ++plane) {
            shapeStream<<whiteSpaces(indentation + stdIndent);
            shapeStream<<"<addplane ";
            dumpParameter("zplane", pcon->getZPlane(plane));
            dumpParameter("rminplane", pcon->getRMinPlane(plane));            
            dumpParameter("rmaxplane", pcon->getRMaxPlane(plane));
            shapeStream<<"/>"<<std::endl;
        }        
        shapeStream<<whiteSpaces(indentation)<<"</pcon>"<<std::endl;
    } else if (shapeID == GeoPgon::getClassTypeID()) {
        const GeoPgon* pgon = dynamic_cast<const GeoPgon*>(shape.get());
        shapeStream<<"<pgon name=\""<<nameTag<<"\" ";
        dumpParameter("sphi", pgon->getSPhi(), true);
        dumpParameter("dphi", pgon->getSPhi(), true);
        dumpParameter("nsides", pgon->getNSides());
        for (unsigned int plane = 0 ; plane < pgon->getNPlanes(); ++plane) {
            shapeStream<<whiteSpaces(indentation + stdIndent);
            shapeStream<<"<plane ";
            dumpParameter("zplane", pgon->getZPlane(plane));
            dumpParameter("rminplane", pgon->getRMinPlane(plane));            
            dumpParameter("rmaxplane", pgon->getRMaxPlane(plane));
            shapeStream<<"/>"<<std::endl;
        }        
        shapeStream<<whiteSpaces(indentation)<<"</pgon>"<<std::endl;
    } else if (shapeID == GeoTorus::getClassTypeID()) {
        shapeStream<<"<torus name=\""<<nameTag<<"\" ";
        const GeoTorus* torus{dynamic_cast<const GeoTorus*>(shape.get())};
        dumpParameter("rmin", torus->getRMin());
        dumpParameter("rmax", torus->getRMax());
        dumpParameter("rtor", torus->getRTor());
        dumpParameter("sphi", torus->getSPhi(), true);
        dumpParameter("dphi", torus->getDPhi(), true);
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoTrap::getClassTypeID()) {
        shapeStream<<"<trap name=\""<<nameTag<<"\" ";
        const GeoTrap* trap{dynamic_cast<const GeoTrap*>(shape.get())};
        dumpParameter("zhalflength", trap->getZHalfLength());
        dumpParameter("theta", trap->getTheta(), true);
        dumpParameter("phi", trap->getPhi(), true);
        dumpParameter("dydzn", trap->getDydzn());
        dumpParameter("dxdyndzn", trap->getDxdyndzn());
        dumpParameter("dxdypdzn", trap->getDxdypdzn());
        dumpParameter("angleydzn", trap->getAngleydzn(), true);
        dumpParameter("dydzp", trap->getDydzn());
        dumpParameter("dxdyndzp", trap->getDxdyndzp());
        dumpParameter("dxdypdzp", trap->getDxdypdzp());
        dumpParameter("angleydzp", trap->getAngleydzp());
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoTrd::getClassTypeID()) {
        shapeStream<<"<trd name=\""<<nameTag<<"\" ";
        const GeoTrd* trd = dynamic_cast<const GeoTrd*>(shape.get());
        dumpParameter("xhalflength1", trd->getXHalfLength1());
        dumpParameter("xhalflength2", trd->getXHalfLength2());
        dumpParameter("yhalflength1", trd->getYHalfLength1());
        dumpParameter("yhalflength2", trd->getYHalfLength2());
        dumpParameter("zhalflength", trd->getZHalfLength());
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoTube::getClassTypeID()) {
        const GeoTube* tube = dynamic_cast<const GeoTube*>(shape.get());
        shapeStream<<"<tube name=\""<<nameTag<<"\" ";
        dumpParameter("rmin", tube->getRMin());
        dumpParameter("rmax", tube->getRMax());
        dumpParameter("zhalflength", tube->getZHalfLength());
        shapeStream<<"/>"<<std::endl;
    } else if (shapeID == GeoTubs::getClassTypeID()) {
        const GeoTubs* tube = dynamic_cast<const GeoTubs*>(shape.get());
        shapeStream<<"<tubs name=\""<<nameTag<<"\" ";
        dumpParameter("rmin", tube->getRMin());
        dumpParameter("rmax", tube->getRMax());
        dumpParameter("zhalflength", tube->getZHalfLength());
        dumpParameter("sphi", tube->getSPhi(), true);
        dumpParameter("dphi", tube->getDPhi(), true);
        shapeStream<<"/>"<<std::endl;
    } else if(shapeID == GeoSimplePolygonBrep::getClassTypeID()) {
        const GeoSimplePolygonBrep* brep = dynamic_cast<const GeoSimplePolygonBrep*>(shape.get());
        shapeStream<<"<simplepolygonbrep name=\""<<nameTag<<"\" ";
        dumpParameter("zhalflength" , brep->getDZ());
        const unsigned int N = brep->getNVertices();
        shapeStream<<"xpoints=\""<<chainUp<double>(N, [brep](unsigned int v){return brep->getXVertex(v); }, ";")<<"\" ";
        shapeStream<<"ypoints=\""<<chainUp<double>(N, [brep](unsigned int v){return brep->getYVertex(v); }, ";")<<"\" ";        
        shapeStream<<"/>"<<std::endl;
    } else {
        THROW_EXCEPTION("The shape "<<printGeoShape(shape)<<"is not implemented.");
    }
    return nameTag;
}

void GeoXMLDumper::dumpTransform(std::ostream& ostr,
                                 const GeoTrf::Transform3D& trans,
                                 const std::string& name,
                                 unsigned int indentation) {    


    transformMap::const_iterator itr = m_transformNameTags.find(trans);
    if (!name.empty() && itr != m_transformNameTags.end()) {
        ostr<<whiteSpaces(indentation)<<"<transformationref  ref=\""<<itr->second<<"\" />"<<std::endl;
        return;
    }


    std::stringstream transstr{};
    transstr<< std::setiosflags(std::ios::fixed) << std::setprecision(3);
    std::string nameTag{replaceExpInString(name," ","_")};
    if (nameTag.size()) {
        nameTag = uniqueNameTag(nameTag, "Trf");
        auto insert_itr = m_transformNameTags.insert(std::make_pair(trans, nameTag));
        if(!insert_itr.second) {
            THROW_EXCEPTION("Failed to insert transformation "<<GeoTrf::toString(trans));
        }
    }
    transstr<<whiteSpaces(indentation)<<"<transformation ";
    if (nameTag.size()) transstr<<"name=\""<<nameTag<<"\" ";   
    transstr<<">"<<std::endl;
   
    if (!transformSorter.compare(trans, GeoTrf::Transform3D::Identity())) {
        transstr<<whiteSpaces(indentation + stdIndent)
                <<"<translation x=\"0\" y=\"0\" z=\"0\" />"<<std::endl;
    } else {
        const GeoTrf::CoordEulerAngles angles = GeoTrf::getCoordRotationAngles(trans);
        const GeoTrf::Vector3D translation = trans.translation();
        /// Dump the translation
        if (translation.dot(translation) > transTolerance) {
            transstr<<whiteSpaces(indentation +stdIndent)<<"<translation ";
            if (std::abs(translation.x()) > transTolerance) {
                transstr<<"x=\""<<translation.x()<<"\" ";
            }
            if (std::abs(translation.y()) > transTolerance) {
                transstr<<"y=\""<<translation.y()<<"\" ";
            }
            if (std::abs(translation.z()) > transTolerance) {
                transstr<<"z=\""<<translation.z()<<"\" ";
            }
            transstr<<"/>"<<std::endl;
        }
        transstr<<std::setiosflags(std::ios::fixed) << std::setprecision(2);


        /// Dump the rotations
        if (angles.alpha) {
            transstr<<whiteSpaces(indentation+stdIndent)<<"<rotation xcos=\"1\" angle=\""
                    <<angles.alpha*toDeg<<"*deg\" />"<<std::endl;
        }
        if (angles.beta) {
            transstr<<whiteSpaces(indentation+stdIndent)
                        <<"<rotation ycos=\"1\" angle=\""<<angles.beta*toDeg<<"*deg\" />"<<std::endl;
        }
        if (angles.gamma) {
            transstr<<whiteSpaces(indentation+stdIndent)
                    <<"<rotation zcos=\"1\" angle=\""<<angles.gamma*toDeg<<"*deg\" />"<<std::endl;
        }
    }
    transstr<<whiteSpaces(indentation)<<"</transformation>"<<std::endl;
    ostr<<transstr.str();
}
void GeoXMLDumper::dumpGeoModelHeader(std::ostream& fileStream,
                                      const topVolWithPathList& pubVolsWithFiles) const {

    fileStream<<"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"<<std::endl;
    fileStream<<"<!DOCTYPE geomodel SYSTEM \""<<m_xmlLayoutFile<<"\" [ "<<std::endl;
    
    std::vector<std::string> entities{};
    for (const auto& [fileName, pubVolName] : pubVolsWithFiles) {
        entities.emplace_back(fileNameBody(fileName));
        fileStream<<whiteSpaces(stdIndent)<<"<!ENTITY "<<entities.back();
        fileStream<<" SYSTEM \""<<fileName<<"\" >"<<std::endl;
    }

    fileStream<<"]>"<<std::endl<<std::endl<<std::endl;

    fileStream<<"<geomodel name=\""<<m_systemName<<"\" version=\"1.0\" >"<<std::endl;
    fileStream<<whiteSpaces(stdIndent)<<"<defines>"<<std::endl;
    fileStream<<whiteSpaces(2*stdIndent)<<"<var name =\"deg\" value=\""<<GeoModelKernelUnits::deg<<"\" />"<<std::endl;
    fileStream<<whiteSpaces(stdIndent)<<"</defines>"<<std::endl<<std::endl<<std::endl;

    for (const std::string& entity : entities) {
        fileStream<<whiteSpaces(stdIndent)<<"&"<<entity<<";"<<std::endl;
    }
    fileStream<<std::endl<<std::endl;

    fileStream<<whiteSpaces(stdIndent)<<"<addbranch>"<<std::endl;
    for (const auto& [fileName, pubVolName] : pubVolsWithFiles) {
        fileStream<<whiteSpaces(2*stdIndent)<<"<logvolref ref=\""<<pubVolName<<"\" />"<<std::endl;
    }
    fileStream<<whiteSpaces(stdIndent)<<"</addbranch>"<<std::endl;
    fileStream<<"</geomodel>"<<std::endl;
}
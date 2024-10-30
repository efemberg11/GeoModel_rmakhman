/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELXMLDUMPER_GEOXMLDUMPER_H
#define GEOMODELXMLDUMPER_GEOXMLDUMPER_H

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GeoModelKernel/GeoIntrusivePtr.h"
#include "GeoModelHelpers/GeoShapeSorter.h"
#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/TransformSorter.h"


#include <string>
#include <map>

/** @brief: The GeoXmlDumper class traverses the GeoModel tree and translates its content 
 *          into plain GeoModelXML
 * 
*/

class GeoXMLDumper {
public:
    GeoXMLDumper(const std::string& outDir,
                 const std::string& systemName,
                 const std::string& xmlLayoutFile);

    bool publishWorld(PVConstLink world);




private:
     std::string m_outDir{};
     std::string m_systemName{};
     std::string m_xmlLayoutFile{};

     void countVolume(PVConstLink volume);

private:
     /// @brief 
     /// @param outStream 
     /// @param physVol 
     /// @param volName 
     /// @param indentation 
     /// @return 
     std::string publishPhysVol(std::ostream& outStream,
                                PVConstLink physVol,
                                const std::string& volName,
                                unsigned int indentation = 0);

     std::string publishShape(std::ostream& shapeStream,
                              GeoIntrusivePtr<const GeoShape> shape,
                              const std::string& shapeName);

    /// @brief  Creates the actual header file of the GeoModel description
    /// @param ostr 
    using topVolWithPathList = std::vector<std::pair<std::string, std::string>>;
    void dumpGeoModelHeader(std::ostream& ostr,
                            const topVolWithPathList& pubVolsWithFiles) const;
    
    void writeMaterials(std::ostream& ostr,
                        unsigned int indentation = 0) const;


    void dumpTransform(std::ostream& ostr,
                       const GeoTrf::Transform3D& trans,
                       const std::string& refName,
                       unsigned int indentation = 0);

    std::string uniqueNameTag(std::string nameTag,
                              const std::string_view idToken);

    using shapeMap = std::map<GeoIntrusivePtr<const GeoShape>, std::string, GeoShapeSorter>;
    using physVolMap = std::map<PVConstLink, std::string, GeoPhysVolSorter>;
    using transformMap = std::map<GeoTrf::Transform3D, std::string, GeoTrf::TransformSorter>;


    using physVolCounter = std::map<PVConstLink, unsigned int, GeoPhysVolSorter>;    
    using nameTagCounter = std::map<std::string, unsigned int>;

    physVolMap m_physVolNameTags{};
    shapeMap m_shapeNameTags{};
    transformMap m_transformNameTags{};

    nameTagCounter m_nameTagCounts{};
    physVolCounter m_physVolCounts{};
    
};

#endif
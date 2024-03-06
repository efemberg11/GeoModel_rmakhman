/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMDOELFUNCSNPPETS_GETCHILDNODESWITHTF_H
#define GEOMDOELFUNCSNPPETS_GETCHILDNODESWITHTF_H

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"


class GeoVolumeCursor;

struct GeoChildNodeWithTrf {
        
        /// @brief  Transformation to the child node
        GeoTrf::Transform3D transform{GeoTrf::Transform3D::Identity()};
        /// @brief In cases of multiple copies. What's the transformation 
        ///        to go from n --> n+1
        GeoTrf::Transform3D inductionRule{GeoTrf::Transform3D::Identity()};
        /// @brief Physical volume pointer to the child
        PVConstLink volume{};
        /// @brief Name of the physical volume empty if cursor finds ANON
        std::string nodeName{};
        /// @brief How many times is the volume placed in the child tree
        unsigned int nCopies{1};
        /// @brief tag whether the transformation is alignable
        bool isAlignable{false};
        /// @brief tag whether the physical volume is a full physVol
        bool isSensitive{false};

        GeoChildNodeWithTrf() = default;
        GeoChildNodeWithTrf(GeoVolumeCursor& curs);
};

std::vector <GeoChildNodeWithTrf> getChildrenWithRef (PVConstLink physVol,
                                                      bool summarizeEqualVol = true);

/**
 *  Returns whether a volume has fullPhysical volume nodes in 
 *  it's children and grand children tree
*/
bool hasFullPhysVolInTree(PVConstLink physVol);



#endif
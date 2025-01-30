/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMDOELFUNCSNPPETS_GETCHILDNODESWITHTF_H
#define GEOMDOELFUNCSNPPETS_GETCHILDNODESWITHTF_H

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include <optional>

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
        /// @brief Identifier of the volume. If there's any and no copies are active
        std::optional<int> volumeId{std::nullopt};
        GeoChildNodeWithTrf() = default;
        GeoChildNodeWithTrf(GeoVolumeCursor& curs);      
};

/*** @brief Returns all direct children of a volume with their transform. Equicalent volumes can be summarized.
 *          In this case, the inductionRule transform to move from one copy to the next, is non-trivial
*/
std::vector <GeoChildNodeWithTrf> getChildrenWithRef (PVConstLink physVol,
                                                      bool summarizeEqualVol = true);
/**** @brief Traverses through the GeoVPhysVol tree and returns all sub volumes of this tree.*/
std::vector<GeoChildNodeWithTrf> getAllSubVolumes(PVConstLink physVol);
/*** @brief Traverses thorugh the GeoVPhysVOl tree and returns all subvolumes satisfying an external 
 *          selection.
 */
std::vector<GeoChildNodeWithTrf> getAllSubVolumes(PVConstLink physVol,
                                                  const std::function<bool(const GeoChildNodeWithTrf&)>& selector,
                                                  bool summarizeEqualVol = false);

/**
 *  Returns whether a volume has fullPhysical volume nodes in 
 *  it's children and grand children tree
*/
bool hasFullPhysVolInTree(const PVConstLink& physVol);



#endif
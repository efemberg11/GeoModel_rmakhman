/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/getChildNodesWithTrf.h"

#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoFullPhysVol.h"

namespace {
    constexpr std::string_view dummyNodeName{"ANON"};
}


 GeoChildNodeWithTrf::GeoChildNodeWithTrf(GeoVolumeCursor& curs):
    transform{curs.getTransform()}, 
    volume{curs.getVolume()},
    nodeName{curs.getName()},
    isAlignable{curs.hasAlignableTransform()},
    isSensitive{typeid(*volume) == typeid(GeoFullPhysVol)} {
    //// Do not specify a node name if it's a dummy one
    if (nodeName == dummyNodeName) {
        nodeName = volume->getLogVol()->getName();

    }
}

std::vector <GeoChildNodeWithTrf> getChildrenWithRef(PVConstLink physVol,
                                                     bool summarizeEqualVol) {
    std::vector<GeoChildNodeWithTrf> children{};
 
    static const GeoPhysVolSorter physVolSort{};
    static const GeoTrf::TransformSorter transSort{};

    
    GeoVolumeCursor cursor{physVol};
    GeoTrf::Transform3D lastNodeTrf{GeoTrf::Transform3D::Identity()};

    while (!cursor.atEnd()) {
        if (children.empty() || !summarizeEqualVol) {
            children.emplace_back(cursor);
            cursor.next();
            continue;
         }
         GeoChildNodeWithTrf& prevChild{children.back()};
         GeoChildNodeWithTrf currentChild{cursor};
         if (prevChild.isAlignable != currentChild.isAlignable ||
             prevChild.isSensitive != currentChild.isSensitive ||
             physVolSort.compare(prevChild.volume, currentChild.volume)) {
            children.emplace_back(std::move(currentChild));
         } else if (prevChild.nCopies == 1) {
            ++prevChild.nCopies;
            prevChild.inductionRule = prevChild.transform.inverse() * 
                                      currentChild.transform;            
         } else if (!transSort.compare(prevChild.inductionRule, 
                                      lastNodeTrf.inverse() * currentChild.transform)) {
            ++prevChild.nCopies;
        } else {
            children.emplace_back(std::move(currentChild));
        }
        lastNodeTrf = cursor.getTransform();   
        cursor.next();
    }
    return children;
}

std::vector<GeoChildNodeWithTrf> getAllSubVolumes(PVConstLink physVol) {
    std::vector<GeoChildNodeWithTrf> children{getChildrenWithRef(physVol, false)}, subVolumes{};
    subVolumes.reserve(children.size());
    for (const GeoChildNodeWithTrf& child : children) {
        subVolumes.push_back(child);
        std::vector<GeoChildNodeWithTrf> grandChildren = getAllSubVolumes(child.volume);
        subVolumes.reserve(grandChildren.size() + subVolumes.size());
        std::transform(std::make_move_iterator(grandChildren.begin()),
                       std::make_move_iterator(grandChildren.end()),
                       std::back_inserter(subVolumes), [&child](GeoChildNodeWithTrf&& grandChild){
                            grandChild.transform = child.transform * grandChild.transform;
                            return grandChild;
                       });
    }
    return subVolumes;
}

bool hasFullPhysVolInTree(PVConstLink physVol) {
    if (typeid(*physVol) == typeid(GeoFullPhysVol) ||
        typeid(*physVol) == typeid(GeoVFullPhysVol)){
        return true;
    }
    for (unsigned int ch = 0; ch < physVol->getNChildVols(); ++ch) {
        if (hasFullPhysVolInTree(physVol->getChildVol(ch))) return true;
    }

    return false;
}
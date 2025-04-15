/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/
#include <utility>

#include "GeoModelHelpers/getChildNodesWithTrf.h"

#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoFullPhysVol.h"

namespace {
    constexpr std::string_view dummyNodeName{"ANON"};
    ///typeis function uses simple arguments to typeid (not an expression) to avoid warnings
    template<class A, class B>
    bool
    typeis(B && b){
      return typeid(A) == typeid(b);
    }
}


 GeoChildNodeWithTrf::GeoChildNodeWithTrf(GeoVolumeCursor& curs):
    transform{curs.getTransform()}, 
    volume{curs.getVolume()},
    nodeName{curs.getName()},
    isAlignable{curs.hasAlignableTransform()},
    isSensitive{typeis<GeoFullPhysVol>(*volume)},
    volumeId{static_cast<const std::optional<int>&>(curs.getId())} {
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

    
    GeoVolumeCursor cursor{std::move(physVol)};
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
            prevChild.volumeId = std::nullopt;
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
std::vector<GeoChildNodeWithTrf> getAllSubVolumes(PVConstLink physVol,
                                                  const std::function<bool(const GeoChildNodeWithTrf&)>& selector,
                                                  bool summarizeEqualVol) {
    std::vector<GeoChildNodeWithTrf> children{getChildrenWithRef(std::move(physVol), summarizeEqualVol)}, subVolumes{};
    subVolumes.reserve(children.size());
    for (const GeoChildNodeWithTrf& child : children) {
        std::vector<GeoChildNodeWithTrf> grandChildren = getAllSubVolumes(child.volume, selector, summarizeEqualVol);
        if (selector(child)) subVolumes.push_back(child);
  
        subVolumes.reserve(grandChildren.size() + subVolumes.capacity());
        std::transform(std::make_move_iterator(grandChildren.begin()),
                       std::make_move_iterator(grandChildren.end()),
                       std::back_inserter(subVolumes), [&child](GeoChildNodeWithTrf&& grandChild){
                            grandChild.transform = child.transform * grandChild.transform;
                            return grandChild;
                       });
    }
    return subVolumes;
 }


std::vector<GeoChildNodeWithTrf> getAllSubVolumes(PVConstLink physVol) {
    return getAllSubVolumes(std::move(physVol), 
                            [](const GeoChildNodeWithTrf& child){
                                        return true;}, false);
}

bool hasFullPhysVolInTree(const PVConstLink& physVol) {
    if (typeis<GeoFullPhysVol>(*physVol) ||
        typeis<GeoVFullPhysVol>(*physVol)){
        return true;
    }
    for (unsigned int ch = 0; ch < physVol->getNChildVols(); ++ch) {
        if (hasFullPhysVolInTree(physVol->getChildVol(ch))) return true;
    }

    return false;
}
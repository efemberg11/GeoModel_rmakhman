// Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration


#include <GeoModelKernel/GeoFullPhysVol.h>
#include <GeoModelKernel/GeoTransform.h>
#include <GeoModelKernel/GeoPhysVol.h>
#include <GeoModelKernel/GeoFullPhysVol.h>
#include <GeoModelKernel/GeoBox.h>
#include <GeoModelKernel/GeoIdentifierTag.h>

#include <GeoModelHelpers/defineWorld.h>
#include <GeoModelHelpers/TransformSorter.h>
#include <GeoModelHelpers/TransformToStringConverter.h>
#include <GeoModelHelpers/getChildNodesWithTrf.h>
#include <iostream>


int main(int argc, char *argv[]){
    GeoIntrusivePtr<GeoMaterial> material = make_intrusive<GeoMaterial>("Snow", 1.45);
    GeoIntrusivePtr<GeoBox> uniCave = make_intrusive<GeoBox>(100., 100., 100.);
    /// Create the logical volume forklift which should increase the counters by one unit
    GeoIntrusivePtr<GeoLogVol> logVol = make_intrusive<GeoLogVol>("Forklift", uniCave, material);

    PVLink world = createGeoWorld();
    PVConstLink cWorld{world};


    GeoIntrusivePtr<GeoTransform> transNode0 = make_intrusive<GeoTransform>(GeoTrf::get3DRotMatX(0.3 * M_PI) * GeoTrf::TranslateX3D(50.));   
    GeoIntrusivePtr<GeoTransform> transNode1 = make_intrusive<GeoTransform>(GeoTrf::get3DRotMatZ(0.3 * M_PI) * GeoTrf::TranslateY3D(50.));
    
    PVLink parent{world};
    GeoTrf::Transform3D testTrf{GeoTrf::Transform3D::Identity()};
    
    const GeoTrf::TransformSorter trfSorter{};
    constexpr unsigned int treeDepth = 10;
    for (unsigned int k = 0 ; k< treeDepth; ++k) {
        GeoIntrusivePtr<GeoTransform> transNode { k %2 ? transNode0 : transNode1};
        parent->add(make_intrusive<GeoIdentifierTag>(k));
        parent->add(transNode);        
        testTrf = testTrf * transNode->getTransform();

        if (k % 2) {
            GeoIntrusivePtr<GeoFullPhysVol> physVol{make_intrusive<GeoFullPhysVol>(logVol)};
            parent->add(physVol);
            if (!parent->getChildVol(0)) {
                std::cerr<<__FILE__<<":"<<__LINE__<<" Failed to access the 0-th child volume "<<physVol.get()<<std::endl;
                return EXIT_FAILURE;
            }
            parent = physVol;
            if (trfSorter.compare(testTrf, physVol->getAbsoluteTransform())){
                std::cerr<<__FILE__<<":"<<__LINE__<<" Transform application failed "<<GeoTrf::toString(testTrf)
                         <<" vs. "<<GeoTrf::toString(physVol->getAbsoluteTransform())<<std::endl;
                return EXIT_FAILURE;
            }
        } else {
            PVLink physVol{make_intrusive<GeoPhysVol>(logVol)};
            parent->add(physVol);
            /// Ensure that the 0-th volume can be accessed
            if (!parent->getChildVol(0)) {
                std::cerr<<__FILE__<<":"<<__LINE__<<" Failed to access the 0-th child volume "<<physVol.get()<<std::endl;
                return EXIT_FAILURE;
            }
            parent = physVol;
        }
    }
    
    {
        /// Test shared mechanism
        PVLink sharedVol{make_intrusive<GeoPhysVol>(logVol)};
        if (sharedVol->isShared()) {
            std::cerr<<__FILE__<<":"<<__LINE__<<" Volume is constructed as a shared volume "<<std::endl;
            return EXIT_FAILURE;
        }
        world->add(sharedVol);
        if (sharedVol->isShared() || sharedVol->getParent() != cWorld) {
            std::cerr<<__FILE__<<":"<<__LINE__<<" Volume is only added to the world once. Should not be shared yet "<<std::endl;
            return EXIT_FAILURE;
        }
        world->add(sharedVol);
        if (!sharedVol->isShared() || sharedVol->getParent()) {
            std::cerr<<__FILE__<<":"<<__LINE__<<" The volume should be from now on treated as shared "<<std::endl;
            return EXIT_FAILURE;
        }
    }

    /// Find the GeoIdentifier tag
    unsigned int currentK = treeDepth -1;
    PVConstLink cParent{parent->getParent()};
    parent.reset();
    while (cParent) {        
        Query<int> query = cParent->getIdOfChildVol(0);
        if (!query.isValid()) {
            std::cerr<<__FILE__<<":"<<__LINE__<<" Failed to obtain a valid child volume ID. Expected "<<currentK
                     <<" "<<typeid(*cParent->getChildVol(0)).name()<<std::endl;
            return EXIT_FAILURE;
        }
        unsigned int parentID = query;
        if (parentID != currentK) {
            std::cerr<<__FILE__<<":"<<__LINE__<<" Expected "<<currentK<<" but got "<<parentID;
            return EXIT_FAILURE;
        }
        cParent = cParent->getParent();
        --currentK;
    }
    /// Test tree traversing
    std::vector<GeoChildNodeWithTrf> geoTree = getAllSubVolumes(world);
    for (const GeoChildNodeWithTrf& childNode : geoTree) {
        /// Volume is shared
        if (childNode.volume->isShared()) {
             continue;
        }
        /// The volume is a full physical volume
        if (childNode.isSensitive) {
            GeoIntrusivePtr<const GeoVFullPhysVol> fullVol = dynamic_pointer_cast<const GeoVFullPhysVol>(childNode.volume);
            if (trfSorter.compare(fullVol->getAbsoluteTransform(), childNode.transform)) {
                std::cerr<<__FILE__<<":"<<__LINE__<<" Transform application failed "<<GeoTrf::toString(childNode.transform)
                         <<" vs. "<<GeoTrf::toString(fullVol->getAbsoluteTransform())<<std::endl;
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

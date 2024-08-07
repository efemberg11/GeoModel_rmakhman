/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/TransformToStringConverter.h"
#include "GeoModelHelpers/defineWorld.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/Units.h"
#include <cstdlib>
#include <iostream>
#include <set>


int main() {
    std::set<PVLink, GeoPhysVolSorter> physVolSet{};

    GeoIntrusivePtr<GeoPhysVol> world{createGeoWorld()};

    const GeoMaterial* air = world->getLogVol()->getMaterial();
    GeoIntrusivePtr<GeoBox> externalBox{new GeoBox(500.,500., 500.)};
    GeoIntrusivePtr<GeoBox> internalBox{new GeoBox(100.,100., 100.)};
    auto  makeBox = [&](bool bigOne) {
        return PVLink(new GeoPhysVol(new GeoLogVol("TestVolume", bigOne ? externalBox : internalBox, air)));
    };

    auto makeFullBox = [&](bool bigOne) {
        return PVLink(new GeoFullPhysVol(new GeoLogVol("TestFullPhysVol", bigOne ? externalBox : internalBox, air)));
    };
    /// 
    PVLink extVolume = makeBox(true);
    extVolume->add(makeBox(false));

    if (!physVolSet.insert(extVolume).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Cannot insert the box into the empty set "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!physVolSet.insert(makeBox(true)).second){
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Cannot inssert the empty big box "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!physVolSet.insert(makeBox(false)).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Cannot inssert the empty small box "<<std::endl;
        return EXIT_FAILURE;
    }

    /// Test whether a box with a subbox cannot be added
    extVolume = makeBox(true);
    extVolume->add(makeBox(false));
    if (physVolSet.insert(extVolume).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" PhysVols can be shared. "<<std::endl;
        return EXIT_FAILURE;
    }
    extVolume = makeBox(true);
    extVolume->add(new GeoTransform(GeoTrf::TranslateX3D(50.)));
    extVolume->add(makeBox(false));
    if (!physVolSet.insert(extVolume).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" A box with a displaced box inside is not the same as box ception "<<std::endl;
        return EXIT_FAILURE;
    }
    /// Test the full physical volumes

    if (!physVolSet.insert(makeFullBox(true)).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Full physical volume should be always added. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!physVolSet.insert(makeFullBox(true)).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Full physical volume should be always added. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!physVolSet.insert(makeFullBox(false)).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Full physical volume should be always added. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!physVolSet.insert(makeFullBox(false)).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" Full physical volume should be always added. "<<std::endl;
        return EXIT_FAILURE;
    }

    extVolume = makeBox(true);
    extVolume->add(makeFullBox(false));    
    if (!physVolSet.insert(extVolume).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" A box with a full physical volume inside should be always added "<<std::endl;
        return EXIT_FAILURE;
    }
    
    extVolume = makeFullBox(true);
    extVolume->add(makeFullBox(false));    
    if (!physVolSet.insert(extVolume).second) {
        std::cerr<<"testPhysVolSorter() "<<__LINE__<<" A box with a full physical volume inside should be always added "<<std::endl;
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
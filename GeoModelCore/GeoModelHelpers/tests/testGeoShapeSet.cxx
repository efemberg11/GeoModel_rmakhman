/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelHelpers/GeoShapeSorter.h"
#include "GeoModelHelpers/GeoShapeUtils.h"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoShapeShift.h"


#include <stdlib.h>
#include <iostream>

template<class T, typename... args> bool insertVolume(const bool printFailure,
                                                     GeoShapeSet<GeoShape>& set,
                                                     args... a) {
    GeoIntrusivePtr<T> newPtr{new T(a...)};
    
    const auto insert_itr = set.insert(newPtr);
    if (insert_itr.second && newPtr->refCount() == 2) return true;
    if (printFailure) {
        std::cerr<<"Shape insertion failed "<<printGeoShape(newPtr)<<std::endl;
        std::cerr<<"Shape "<<printGeoShape(*insert_itr.first)<<" is taking its place "<<std::endl;
    }
    return false;
}

int main() {
    GeoShapeSet<GeoShape> shapeColl{};
    GeoShapeSorter shapeSorter{};

    if (!insertVolume<GeoBox>(true, shapeColl, 10.,10.,10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (insertVolume<GeoBox>(false, shapeColl, 10.,10.,10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" GeoBox A has been inserted twice. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!insertVolume<GeoBox>(true, shapeColl, 11.,10., 10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!insertVolume<GeoBox>(true, shapeColl, 10., 11., 10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }

    if (!insertVolume<GeoBox>(true, shapeColl, 10., 10., 11.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }

    if (!insertVolume<GeoBox>(true, shapeColl, 10., 11., 11.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }
    ///##############################################################
    ///                 GeoTube
    ///##############################################################
    if (!insertVolume<GeoTube>(true, shapeColl, 10.,10.,10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (insertVolume<GeoTube>(false, shapeColl, 10.,10.,10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" GeoTube A has been inserted twice. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!insertVolume<GeoTube>(true, shapeColl, 11.,10., 10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!insertVolume<GeoTube>(true, shapeColl, 10., 11., 10.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }

    if (!insertVolume<GeoTube>(true, shapeColl, 10., 10., 11.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }

    if (!insertVolume<GeoTube>(true, shapeColl, 10., 11., 11.)) {
        std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
        return EXIT_FAILURE;
    }

    {
        GeoIntrusivePtr<GeoTube> tubeShift{new GeoTube(2,3,4)};
        GeoTrf::Transform3D trans1{GeoTrf::GeoTransformRT{GeoTrf::GeoRotation(0., 2., 2.), GeoTrf::Vector3D{1.,0.,3}}};
        GeoTrf::Transform3D trans2{GeoTrf::GeoTransformRT{GeoTrf::GeoRotation(2.,0., 2.), GeoTrf::Vector3D{2.,0.,3}}};
        if (!insertVolume<GeoShapeShift>(true, shapeColl, tubeShift, trans1)){
            std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
            return EXIT_FAILURE;
        }
        if (!insertVolume<GeoShapeShift>(true, shapeColl, tubeShift, trans2)){
            std::cerr<<"testGeoShapeSorter() "<<__LINE__<<" test failed. "<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
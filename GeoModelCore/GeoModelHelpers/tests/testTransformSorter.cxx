/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelHelpers/TransformToStringConverter.h"
#include "GeoModelKernel/Units.h"
#include <stdlib.h>
#include <iostream>
#include <set>

std::ostream& operator<<(std::ostream& ostr, const GeoTrf::Transform3D& trans) {
    ostr<<GeoTrf::toString(trans, true, 2);
    return ostr;
}

#define TEST_TRANSFORM(TRANS) \
    std::cout<<"testTransformSorter() "<<__LINE__<<"  Test transformation "<<TRANS<<std::endl; \
    if (sorter(TRANS, TRANS)) { \
        std::cerr<<"testTransformSorter() "<<__LINE__<<" transformation "<<TRANS<<" is less than itself "<<std::endl; \
        return EXIT_FAILURE; \
    }
#define COMPARE_SORTER(TRANSA, TRANSB)         \
    if (sorter(TRANSA, TRANSB) == sorter(TRANSB, TRANSA)) {                                   \
        std::cerr<<"testTransformSorter() "<<__LINE__<<" transformations "                    \
                  <<TRANSA<<" & "<<TRANSB<<" are identical. "<<std::endl;                     \
        const GeoTrf::Vector3D vecA{TRANSA.translation()}, vecB{TRANSB.translation()};        \
        std::cerr<<"testTransformSorter() comparison result (A,B): "<<sorter(TRANSA, TRANSB)  \
                 << " compare result: "<<sorter.compare(TRANSA, TRANSB)                       \
                 << " -- linear part: "<<sorter(TRANSA.linear(), TRANSB.linear()) << " / "    \
                 << sorter.compare(TRANSA.linear(), TRANSB.linear())                          \
                 << " -- translation part: "<<sorter(vecA, vecB)                              \
                 << " / "<<sorter.compare(vecA, vecB)<<std::endl;                             \
        std::cerr<<"testTransformSorter() comparison result (B,A): "<<sorter(TRANSB, TRANSA)  \
                 << " compare result: "<<sorter.compare(TRANSB, TRANSA)                       \
                 << " -- linear part: "<<sorter(TRANSB.linear(), TRANSA.linear()) << " / "    \
                 << sorter.compare(TRANSB.linear(), TRANSA.linear())                          \
                 << " -- translation part: "<<sorter(vecB, vecA)                              \
                 << " / "<<sorter.compare(vecB, vecA)<<std::endl;                             \
        return EXIT_FAILURE;                                                                  \
    }
int main() {
    constexpr double deg = GeoModelKernelUnits::deg;
    GeoTrf::TransformSorter sorter{};
    GeoTrf::CoordEulerAngles anglesA{45.*deg, 0., 0.};
    GeoTrf::Transform3D transA{GeoTrf::GeoTransformRT{GeoTrf::GeoRotation{anglesA}, 10. *GeoTrf::Vector3D::UnitX()}};
    TEST_TRANSFORM(transA);

    GeoTrf::CoordEulerAngles anglesB{0.,0.,0.};
    GeoTrf::Transform3D transB{GeoTrf::GeoTransformRT{GeoTrf::GeoRotation{anglesB}, 10. *GeoTrf::Vector3D::UnitX()}};
    TEST_TRANSFORM(transB);

    COMPARE_SORTER(transA, transB);
   

    GeoTrf::Transform3D transC{GeoTrf::Translate3D{1, -1, 0.}};
    TEST_TRANSFORM(transC)
    COMPARE_SORTER(transA, transC);
    COMPARE_SORTER(transB, transC);

    std::set<GeoIntrusivePtr<GeoTransform>, GeoTrf::TransformSorter> geoTrfCache{};
    {
        GeoIntrusivePtr<GeoTransform> geoTransC{new GeoTransform(transC)};
        if (!geoTrfCache.insert(geoTransC).second) {
            std::cerr<<"testTransformSorter() "<<__LINE__<<" Failed to insert transform into empty set"<<std::endl;
            return EXIT_FAILURE;
        }
        geoTransC.reset(new GeoTransform(transC));
        if (geoTrfCache.insert(geoTransC).second) {
            std::cerr<<"testTransformSorter() "<<__LINE__<<" The same transform should always point to the same object."<<std::endl;
            return EXIT_FAILURE;
        }
        geoTransC.reset(new GeoAlignableTransform(transC));
        if (!geoTrfCache.insert(geoTransC).second) {
            std::cerr<<"testTransformSorter() "<<__LINE__<<" Alignable transforms should be never deduplicated."<<std::endl;
            return EXIT_FAILURE;
        }

        geoTransC.reset(new GeoAlignableTransform(transC));
        if (!geoTrfCache.insert(geoTransC).second) {
            std::cerr<<"testTransformSorter() "<<__LINE__<<" Alignable transforms should be never deduplicated."<<std::endl;
            return EXIT_FAILURE;
        }

    }
   
    return EXIT_SUCCESS;
}
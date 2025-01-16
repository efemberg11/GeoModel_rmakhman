/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
 */
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"

#include <stdlib.h>
#include <iostream>

using namespace GeoTrf;

constexpr double tolerance = 1.e-9;
std::ostream& operator<<( std::ostream& ostr, const Vector3D& v) {
    ostr<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<")";
    return ostr;
}
std::ostream & operator<<(std::ostream& ostr, const Line3D& l) {
    ostr<<"line "<<l.position()<<" + "<<l.direction();
    return ostr;
}
std::ostream& operator<<(std::ostream& ostr, const Plane3D& p) {
    ostr<<"Plane with normal: "<<p.normal()<<" & offset: "<<p.offSet();
    return ostr;
}

#define ERROR(msg)                                                  \
    {                                                               \
        std::cerr<<__FILE__<<":"<<__LINE__<<" - "<<msg<<std::endl;  \
        return EXIT_FAILURE;                                        \
    }                                                               \

int main() {
    
    
    const Line3D lineA{ Vector3D{10.,0.,0.}, Vector3D::UnitX()};
    const Line3D lineB{ Vector3D{30.,0.,0.}, Vector3D::UnitY()};

    const std::optional<Vector3D> isectAtoB = lineA.intersect(lineB);
    
    if (!isectAtoB) ERROR("The two lines A "<<lineA<<" & B "<<lineB<<" are not intersecting");

    const std::optional<Vector3D> isectBtoA = lineB.intersect(lineA);
    if (!isectAtoB) ERROR("The two lines B "<<lineB<<" & A "<<lineA<<" are not intersecting");
    /// Check that the two lines intersect in the same point
    const GeoTrf::Vector3D iSectDiff = (*isectAtoB) - (*isectBtoA);
    if (iSectDiff.dot(iSectDiff) > tolerance) 
        ERROR("Intersections ended up at two different points "<<(*isectAtoB)<<" vs. "<<(*isectBtoA));
    const GeoTrf::Vector3D iSectToExpect = (*isectAtoB) - Vector3D{30.,0.,0.};
    if (iSectToExpect.dot(iSectToExpect) > tolerance) 
        ERROR("Intersections ended up at two different points "<<(*isectAtoB)<<" vs. "<<lineB.position());

    /// Now make it a bit more difficult
    for (const double dist : {-10., 10.,  45. ,100., 666., 1789.}) {
        for (double angle =0; angle<=180.*GeoModelKernelUnits::deg; angle+=15.*GeoModelKernelUnits::deg){
            const GeoTrf::Vector3D dir{0.,std::cos(angle), std::sin(angle)};
            const Line3D test{lineA.travel(dist) - dist*dir, dir};

            auto isectWithTest = lineA.intersect(test);
            if (!isectWithTest) ERROR("Intersection of "<<lineA<<" with "<<test<<" failed.");
            const Vector3D backTest = (*isectWithTest) - lineA.travel(lineA.distance(*isectWithTest)); 
            if (backTest.dot(backTest) > tolerance)
                ERROR("Intersection with "<<lineA<<" recuperated a different travel distance: "
                     <<lineA.distance(*isectWithTest)<<" vs. "<<dist);   
            if (std::abs(dist - test.distance(*isectWithTest))> tolerance)
                ERROR("Intersection with "<<test<<" recuperated a different travel distance: "
                     <<test.distance(*isectWithTest)<<" vs. "<<dist);   
        }
    }
    Plane3D plane{lineA.direction(), lineB.direction(), lineA.position()};

    if (!plane.inside(lineA.position()))
        ERROR("The point "<<lineA.position()<<" defining the plane "<<plane<<" is not inside");
    
    Line3D testLine{300.*plane.normal(), plane.normal()};
    auto planeIsect = testLine.intersect(plane);
    if(!planeIsect || !plane.inside(*planeIsect))
        ERROR("The intersection point "<<planeIsect.value_or(Vector3D::Zero())<<" is outside plane"
             <<plane);

    return EXIT_SUCCESS;
}


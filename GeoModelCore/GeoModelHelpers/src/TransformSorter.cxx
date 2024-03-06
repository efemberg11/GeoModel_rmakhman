/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelHelpers/throwExcept.h"

namespace GeoTrf {
    bool TransformSorter::operator()(const std::unique_ptr<Transform3D>& a, 
                                     const std::unique_ptr<Transform3D>& b) const {
        if (!a || !b) {
            THROW_EXCEPTION("Nullptr given to comparator");
        }
        return (*this)(*a, *b);
    }
    bool TransformSorter::operator()(const std::shared_ptr<Transform3D>& a,
                                     const std::shared_ptr<Transform3D>& b) const {
        if (!a || !b) {
            THROW_EXCEPTION("Nullptr given to comparator");
        }
        return (*this)(*a, *b);
    }
    bool TransformSorter::operator()(const Transform3D& a, const Transform3D& b) const {
        return compare(a, b) < 0;
    }
    bool TransformSorter::operator()(const RotationMatrix3D&a, const RotationMatrix3D& b) const{
        return compare(a, b) < 0;
    }

    int TransformSorter::compare(const Transform3D&a, const Transform3D& b) const{ 
        const Vector3D transA{a.translation()}, transB{b.translation()};
        const int transCmp = compare(transA, transB);
        if (transCmp) return transCmp;
        return compare(a.linear(), b.linear());
    }
    int TransformSorter::compare(const RotationMatrix3D&a, const RotationMatrix3D& b) const{ 
        return getGeoRotationAngles(a).compare(getGeoRotationAngles(b));
    }

    bool TransformSorter::operator()(const ::GeoTransform* a,
                                     const ::GeoTransform* b) const {
        if (!a || !b) {
            THROW_EXCEPTION("Nullptr given to comparator");
        }
        if (typeid(*a) == typeid(GeoAlignableTransform) ||
            typeid(*b) == typeid(GeoAlignableTransform)) {
            return a < b;
        }
        return (*this)(a->getTransform(), b->getTransform());
    }
}
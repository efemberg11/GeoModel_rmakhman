/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GeoShapeSorter_H
#define GeoModelHelpers_GeoShapeSorter_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include <set>

/// @brief  Helper struct to construct set of GeoShapes where the defining parameters of
///         each element are unique within 1 micrometer
struct GeoShapeSorter {
   template<class ShapeType1, class ShapeType2>
    bool operator()(const GeoIntrusivePtr<ShapeType1>& a, 
                    const GeoIntrusivePtr<ShapeType2>& b) const {
            return (*this)(a.get(), b.get());
    }

    bool operator()(const GeoShape* a, const GeoShape* b) const;
    /// @brief Compares 2 GeoShapes
    /// @param a : Pointer to shape A
    /// @param b : Pointer to shape B
    /// @return  Returns 0 if no defining shape property could be found that differs
    ///          Returns -1 if the first defining & differing property of A is smaller
    ///          Returns 1 otherwise
    int compare(const GeoShape* a, const GeoShape* b) const;

};

struct GeoComposedShapeSorter {
    template<class ShapeType1, class ShapeType2>
    bool operator()(const GeoIntrusivePtr<ShapeType1>& a, 
                    const GeoIntrusivePtr<ShapeType2>& b) const {
            return (*this)(a.get(), b.get());
    }
    bool operator()(const GeoShape* a, const GeoShape* b) const;

};
/// @brief 
/// @tparam ShapeType 
template<class ShapeType>
using GeoShapeSet = std::set<GeoIntrusivePtr<ShapeType>, GeoShapeSorter>;

#endif
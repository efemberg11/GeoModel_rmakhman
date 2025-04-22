/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GeoMaterialSorter_H
#define GeoModelHelpers_GeoMaterialSorter_H

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include <set>

/// @brief  Helper struct to deuplicate equivalent materials with different naming
struct GeoMaterialSorter {
    /** @brief Comparison operator returing whether Material A is smaller than Material B */
    bool operator()(const GeoMaterial* a, const GeoMaterial* b) const;
    /** @brief Comparison operator returing whether the Element A is smaller than Element B */
    bool operator()(const GeoElement* a, const GeoElement* b) const;
    /// @brief Compares 2  GeoMaterials
    /// @param a : Pointer to material A
    /// @param b : Pointer to material B
    /// @return  Returns 0 if no defining material property could be found that differs
    ///          Returns -1 if the first defining & differing property of A is smaller
    ///          Returns 1 otherwise
    int compare(const GeoMaterial* a, const GeoMaterial* b) const;
    int compare(const GeoElement* a, const GeoElement* b) const;

};

/// @brief 
/// @tparam ShapeType 
using GeoMaterialSet = std::set<GeoIntrusivePtr<const GeoMaterial>, GeoMaterialSorter>;

#endif
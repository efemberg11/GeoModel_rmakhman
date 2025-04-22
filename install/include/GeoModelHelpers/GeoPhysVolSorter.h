
/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GEOPHYSVOLSORTER_H
#define GeoModelHelpers_GEOPHYSVOLSORTER_H

#include "GeoModelKernel/GeoVPhysVol.h"

/**
 *  Helper struct to compare two physical volumes for the insertion into a set
 *  Two volumes are considered to be equal if they have:
 * 
 *      1) Equivalent shapes
 *      2) Same material pointer
 *      3) Same number of children
 *      4) Each child is placed at the same position inside the volume 
 *         & points 1-3 hold for them as well
 * 
 *   The comparisons are not performed if one of the objects is a full physical volume. In this case,
 *   it simply falls back to an ordinary pointer address comparator.          
*/
struct GeoPhysVolSorter {

    template <class VolType>
    bool operator()(const GeoIntrusivePtr<VolType>& a, 
                    const GeoIntrusivePtr<VolType>& b) const{ 
       return compare(a, b) < 0;
    }
    
    bool operator()(const GeoVPhysVol* a, const GeoVPhysVol* b) const {
        return compare(a, b) < 0;
    }

    int compare(const GeoVPhysVol*a, const GeoVPhysVol* b) const;
};
#endif
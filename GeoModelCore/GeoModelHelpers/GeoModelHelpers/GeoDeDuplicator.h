/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GEODEDUPLICATOR_H
#define GeoModelHelpers_GEODEDUPLICATOR_H

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GeoModelHelpers/GeoLogVolSorter.h"
#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/GeoShapeSorter.h"
#include "GeoModelHelpers/TransformSorter.h"

#include <set>
/***
 *  Helper class to deduplicate shapes, volumes and non-alignable transforms in the GeoModel tree that are equivalent
 *  but instantiated in different places. Every time when the cache function is invoked, it's tried to insert the object 
 *  into its respective store. If another object which is equivalent in terms of the GeoLogVolSorter, GeoPhysVolSorter or 
 *  GeoTransformSorter has been already added, the parsed object is deleted and the pointer to the already cached one is returned
 * 
 * 
 *  Usage for GeoTransforms:
 * 
 *  Instead of
 *        GeoTransform* xtrf = new GeoTransform(<Some transform to the volume> );
 * 
 *  just do
 *        GeoTransform* xtrf = makeTransform(<Some transform to the volume> );
 * 
 *  NOTE: GeoTransforms are shared across all instances of the GeoDeDuplicator class
 * 
 *  Useage for GeoShapes:
 *      Perform the usual assembly of the shape e.g.
 *       
 *          GeoShape* tube = new GeoTube(10,42, 66);
 *
 *     If you combine the GeoShape with the logical volume then do
 * 
 *           GeoLogVol* logVol = new GeoLogVol("TheCakeTastes", cacheShape(tube), cakeMaterial);
 *      
 *      In a similar fashion repeating components of GeoShapes in Boolean shapes can be made shared as well
 * 
 *  NOTE: GeoShapes are shares across all instances of the GeoDeuplicator class
 * 
 *   Useage for GeoFullPhysVol:
 *          GeoFullPhysVol are never shared and can neither be added to the GeoDeDuplicator. However, it's possible 
 *          to share the same logical volume across several physical volumes
 *        
 *      GeoFullPhysVol* fullPhyVol = new GeoFullPhyVol(cacheVolume(logVol));
 * 
 *   Useage for ordinary GeoPhysVol:
 * 
 *        Assemble the GeoPhysVol as it's needed
 *        
 *        GeoPhysVol* subVol = new GeoPhysVol(cacheVolume(logVol2));
 *        GeoPhysVol* subVol1 = new GeoPhysVol(cacheVolume(logVol3));
 *        
 *        subVol->add(makeTransform(GeoTrf::TranslateX3D(999)));
 *        subVol->add(cacheVolume(subVol1));
 * 
 *        fullPhysVol->add(cacheVolume(subVol));
 * 
*/
class GeoDeDuplicator {
    public:
        using GeoShapePtr = GeoIntrusivePtr<const GeoShape>;
        using GeoLogVolPtr = GeoIntrusivePtr<GeoLogVol>;
        using GeoTrfPtr =  GeoIntrusivePtr<GeoTransform>;
        using GeoPhysVolPtr = GeoIntrusivePtr<GeoPhysVol>;

        GeoDeDuplicator() = default;
        ~GeoDeDuplicator() = default;

        GeoPhysVolPtr cacheVolume(GeoPhysVolPtr vol) const;
        GeoTrfPtr makeTransform(const GeoTrf::Transform3D& trf) const;
        GeoLogVolPtr cacheVolume(GeoLogVolPtr vol) const;
        GeoShapePtr cacheShape(GeoShapePtr shape) const;

    private:
        using PhysVolSet = std::set<GeoIntrusivePtr<GeoPhysVol>, GeoPhysVolSorter>;
        using LogVolSet = std::set<GeoLogVolPtr, GeoLogVolSorter>;
        using TrfSet = std::set<GeoTrfPtr, GeoTrf::TransformSorter>;
        using ShapeSet = std::set<GeoShapePtr, GeoShapeSorter>;
        mutable PhysVolSet m_physVolStore{};
        mutable LogVolSet m_logVolStore{};

        static TrfSet s_trfStore;
        static ShapeSet s_shapeStore;
};


#endif

/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GEODEDUPLICATOR_H
#define GeoModelHelpers_GEODEDUPLICATOR_H

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoSerialIdentifier.h"
#include "GeoModelKernel/GeoNameTag.h"

#include "GeoModelHelpers/GeoLogVolSorter.h"
#include "GeoModelHelpers/GeoPhysVolSorter.h"
#include "GeoModelHelpers/GeoShapeSorter.h"
#include "GeoModelHelpers/TransformSorter.h"

#include <set>
#include <map>
#include <mutex>
#include <thread>
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
        using GeoSerialIdPtr = GeoIntrusivePtr<GeoSerialIdentifier>;
        using GeoNamePtr = GeoIntrusivePtr<GeoNameTag>;

        /** @brief Standard constructor */
        GeoDeDuplicator() = default;
        /** @brief Standard destructor */
        virtual ~GeoDeDuplicator() = default;
        /** @brief Takes a GeoPhysVol and adds it to the cache. If physVol deduplication 
         *          is turned on and an equivalent volume has been parsed before, 
         *          then that volume is returned instead */
        GeoPhysVolPtr cacheVolume(GeoPhysVolPtr vol) const;
        /** @brief Converts the transform into GeoTransform with(out) deduplication
         *         of equivalent transform nodes */
        GeoTrfPtr makeTransform(const GeoTrf::Transform3D& trf) const;
        /** @brief Take a GeoTransform node and adds it to the cahce. If transform deduplication 
         *          is turned on and an equivalent transform node has been parsed before, 
         *          then that mpde is returned instead */
        GeoTrfPtr cacheTransform(GeoTrfPtr transform) const;
        /** @brief Takes a GeoLogVol and adds it to the cache. If LogVol deduplication 
         *          is turned on and an equivalent volume has been parsed before, 
         *          then that volume is returned instead */
        GeoLogVolPtr cacheVolume(GeoLogVolPtr vol) const;
        /** @brief Takes a GeoShape and adds it to the cache. If shape deduplication 
         *          is turned on and an equivalent shape has been parsed before, 
         *          then that shape is returned instead */
        GeoShapePtr cacheShape(GeoShapePtr shape) const;
        /** @brief Returns a new GeoName tag */
        GeoNamePtr nameTag(const std::string& tagName) const;
        /** @brief Returns a new GeoSerial Id  */
        GeoSerialIdPtr serialId(const int id) const;

        /** @brief Toggles whether shape deduplication shall be enabled */
        void setShapeDeDuplication(bool enable);
        /** @brief Toggles whether logVol deduplication shall be enabled */
        void setLogVolDeDuplication(bool enable);
        /** @brief Toggles whether transform node deduplication shall be enabled */
        void setTransformDeDuplication(bool enable);
        /** @brief Toggles whether physVol node deduplication shall be enabled */
        void setPhysVolDeDuplication(bool enable);

        /** @brief Clears the shared Shape / Transform / SerialId & NameTag cache */
        static void clearSharedCaches();
    private:
        bool m_deDuplicateLogVol{true};
        bool m_deDuplicatePhysVol{true};
        bool m_deDuplicateTransform{true};
        bool m_deDuplicateShape{true};
        using PhysVolSet = std::set<GeoIntrusivePtr<GeoPhysVol>, GeoPhysVolSorter>;
        using LogVolSet = std::set<GeoLogVolPtr, GeoLogVolSorter>;
        using TrfSet = std::set<GeoTrfPtr, GeoTrf::TransformSorter>;
        using ShapeSet = std::set<GeoShapePtr, GeoShapeSorter>;
        using SerialIdMap = std::map<int, GeoSerialIdPtr>;
        using NameTagMap = std::map<std::string, GeoNamePtr>;

        mutable PhysVolSet m_physVolStore{};
        mutable LogVolSet m_logVolStore{};
        mutable std::vector<GeoIntrusivePtr<const RCBase>> m_genericCache{};

        static TrfSet s_trfStore;
        static ShapeSet s_shapeStore;
        static SerialIdMap s_serialIds;
        static NameTagMap s_nameTags;

        mutable std::mutex m_mutex{};
};


#endif

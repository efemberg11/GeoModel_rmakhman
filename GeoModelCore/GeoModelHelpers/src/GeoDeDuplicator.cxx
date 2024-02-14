
/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoDeDuplicator.h"

GeoDeDuplicator::TrfSet GeoDeDuplicator::s_trfStore{};
GeoDeDuplicator::ShapeSet GeoDeDuplicator::s_shapeStore{};

GeoDeDuplicator::GeoTrfPtr 
    GeoDeDuplicator::makeTransform(const GeoTrf::Transform3D& trf) const {
        return *s_trfStore.emplace(new GeoTransform(trf)).first;
    }
GeoDeDuplicator::GeoPhysVolPtr 
    GeoDeDuplicator::cacheVolume(GeoPhysVolPtr vol) const {
        return *m_physVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoLogVolPtr 
    GeoDeDuplicator::cacheVolume(GeoLogVolPtr vol) const {
        return *m_logVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoShapePtr 
    GeoDeDuplicator::cacheShape(GeoShapePtr shape) const {
        return *s_shapeStore.insert(shape).first;
    }

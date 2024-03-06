
/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoDeDuplicator.h"

GeoDeDuplicator::TrfSet GeoDeDuplicator::s_trfStore{};
GeoDeDuplicator::ShapeSet GeoDeDuplicator::s_shapeStore{};


void GeoDeDuplicator::setShapeDeduplication(bool enable){
    m_deDuplicateShape = enable;
}
void GeoDeDuplicator::setLogVolDeDuplication(bool enable) {
    m_deDuplicateLogVol = enable;
}
void GeoDeDuplicator::setTransformDeDuplication(bool enable) {
    m_deDuplicateTransform = enable;
}
void GeoDeDuplicator::setPhysVolDeDuplication(bool enable) {
    m_deDuplicatePhysVol = enable;
}

GeoDeDuplicator::GeoTrfPtr 
    GeoDeDuplicator::makeTransform(const GeoTrf::Transform3D& trf) const {
        GeoTrfPtr trfNode{new GeoTransform(trf)};
        if (!m_deDuplicateTransform) {
            m_genericCache.push_back(trfNode);
            return trfNode;
        }
        return *s_trfStore.emplace(trfNode).first;
    }
GeoDeDuplicator::GeoPhysVolPtr 
    GeoDeDuplicator::cacheVolume(GeoPhysVolPtr vol) const {
        if (!m_deDuplicatePhysVol) {
            m_genericCache.push_back(vol);
            return vol;
        }
        return *m_physVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoLogVolPtr 
    GeoDeDuplicator::cacheVolume(GeoLogVolPtr vol) const {
        if (!m_deDuplicateLogVol) {
            m_genericCache.push_back(vol);
            return vol;
        }
        return *m_logVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoShapePtr 
    GeoDeDuplicator::cacheShape(GeoShapePtr shape) const {
        if (!m_deDuplicateShape) {
            m_genericCache.push_back(shape);
            return shape;
        } 
        return *s_shapeStore.insert(shape).first;
    }

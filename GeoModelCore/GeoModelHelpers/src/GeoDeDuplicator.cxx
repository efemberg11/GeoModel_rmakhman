/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoDeDuplicator.h"

GeoDeDuplicator::TrfSet GeoDeDuplicator::s_trfStore{};
GeoDeDuplicator::ShapeSet GeoDeDuplicator::s_shapeStore{};
namespace {
    std::mutex s_mutex{};
}

void GeoDeDuplicator::setShapeDeDuplication(bool enable){
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
        GeoTrfPtr trfNode{make_intrusive<GeoTransform>(trf)};
        if (!m_deDuplicateTransform) {
            std::lock_guard guard{m_mutex};
            m_genericCache.push_back(trfNode);
            return trfNode;
        }
        std::lock_guard guard{s_mutex};
        return *s_trfStore.emplace(trfNode).first;
    }
GeoDeDuplicator::GeoPhysVolPtr 
    GeoDeDuplicator::cacheVolume(GeoPhysVolPtr vol) const {
        std::lock_guard guard{m_mutex};
        if (!m_deDuplicatePhysVol) {
            m_genericCache.push_back(vol);
            return vol;
        }
        return *m_physVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoLogVolPtr 
    GeoDeDuplicator::cacheVolume(GeoLogVolPtr vol) const {
        std::lock_guard guard{m_mutex};
        if (!m_deDuplicateLogVol) {
            m_genericCache.push_back(vol);
            return vol;
        }
        return *m_logVolStore.insert(vol).first;
    }
GeoDeDuplicator::GeoShapePtr 
    GeoDeDuplicator::cacheShape(GeoShapePtr shape) const {
        if (!m_deDuplicateShape) {
            std::lock_guard guard{m_mutex};
            m_genericCache.push_back(shape);
            return shape;
        }
        std::lock_guard guard{s_mutex}; 
        return *s_shapeStore.insert(shape).first;
    }

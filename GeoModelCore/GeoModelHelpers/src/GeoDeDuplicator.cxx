/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoDeDuplicator.h"

GeoDeDuplicator::TrfSet GeoDeDuplicator::s_trfStore{};
GeoDeDuplicator::ShapeSet GeoDeDuplicator::s_shapeStore{};
GeoDeDuplicator::SerialIdMap GeoDeDuplicator::s_serialIds{};
GeoDeDuplicator::NameTagMap GeoDeDuplicator::s_nameTags{};

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

void GeoDeDuplicator::clearSharedCaches() {
    s_trfStore.clear();
    s_shapeStore.clear();
    s_serialIds.clear();
    s_nameTags.clear();
}

GeoDeDuplicator::GeoTrfPtr 
    GeoDeDuplicator::makeTransform(const GeoTrf::Transform3D& trf) const {
        return cacheTransform(make_intrusive<GeoTransform>(trf));
    }
GeoDeDuplicator::GeoTrfPtr 
    GeoDeDuplicator::cacheTransform(GeoTrfPtr trfNode) const {
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

GeoDeDuplicator::GeoNamePtr GeoDeDuplicator::nameTag(const std::string& tagName) const{
    std::lock_guard guard{s_mutex};
    NameTagMap::const_iterator itr = s_nameTags.find(tagName);
    if (itr != s_nameTags.end()){
        return itr->second;
    }
    GeoNamePtr newTag = make_intrusive<GeoNameTag>(tagName);
    s_nameTags.insert(std::make_pair(tagName, newTag));
    return newTag;
}
GeoDeDuplicator::GeoSerialIdPtr GeoDeDuplicator::serialId(const int id) const {
    std::lock_guard guard{s_mutex};
    SerialIdMap::const_iterator itr = s_serialIds.find(id);
    if (itr != s_serialIds.end()){
        return itr->second;
    }
    GeoSerialIdPtr newId = make_intrusive<GeoSerialIdentifier>(id);
    s_serialIds.insert(std::make_pair(id, newId));
    return newId;
}
/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/GeoDeDuplicator.h"

GeoDeDuplicator::TrfSet GeoDeDuplicator::s_trfStore{};
GeoDeDuplicator::ShapeSet GeoDeDuplicator::s_shapeStore{};
GeoDeDuplicator::SerialIdMap GeoDeDuplicator::s_serialIds{};
GeoDeDuplicator::NameTagMap GeoDeDuplicator::s_nameTags{};
GeoDeDuplicator::GeoIdMap GeoDeDuplicator::s_geoIds{};

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
    s_geoIds.clear();
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
GeoDeDuplicator::GeoSerialIdPtr 
    GeoDeDuplicator::serialId(const int id) const {
    std::lock_guard guard{s_mutex};
    SerialIdMap::const_iterator itr = s_serialIds.find(id);
    if (itr != s_serialIds.end()){
        return itr->second;
    }
    GeoSerialIdPtr newId = make_intrusive<GeoSerialIdentifier>(id);
    s_serialIds.insert(std::make_pair(id, newId));
    return newId;
}
GeoDeDuplicator::GeoIdPtr 
    GeoDeDuplicator::geoId(const int id) const {
    std::lock_guard guard{s_mutex};
    GeoIdMap::const_iterator itr = s_geoIds.find(id);
    if (itr != s_geoIds.end()){
        return itr->second;
    }
    GeoIdPtr newId = make_intrusive<GeoIdentifierTag>(id);
    s_geoIds.insert(std::make_pair(id, newId));
    return newId;
}

PVLink GeoDeDuplicator::clone(PVConstLink vol) const {
    PVLink newVol{};
    bool tryPhysVolDeDup{m_deDuplicatePhysVol};
    GeoLogVolPtr logVol{const_cast<GeoLogVol*>(vol->getLogVol())};
    if (dynamic_cast<const GeoPhysVol*>(vol.get())) {
        newVol = make_intrusive<GeoPhysVol>(cacheVolume(logVol));
    } else {
        newVol = make_intrusive<GeoFullPhysVol>(cacheVolume(logVol));
        tryPhysVolDeDup = false;
    }
    for(unsigned int chNode =0; chNode < newVol->getNChildNodes(); ++chNode) {
        GeoIntrusivePtr<const GeoGraphNode>node{*newVol->getChildNode(chNode)};
        /** transform nodes */
        if (typeid(*node) == typeid(GeoAlignableTransform)) {
            const auto geoTrf = dynamic_pointer_cast<const GeoAlignableTransform>(node);
            newVol->add(make_intrusive<GeoAlignableTransform>(geoTrf->getDefTransform()));
        } else if (typeid(*node) == typeid(GeoTransform)) {
            const auto geoTrf = dynamic_pointer_cast<const GeoTransform>(node);
            auto geoTrfNonConst = const_pointer_cast(geoTrf);
            newVol->add(cacheTransform(geoTrfNonConst));
        } 
        /** physical volumes */
        else if (auto physVol = dynamic_pointer_cast<const GeoVPhysVol>(node); physVol) {
            newVol->add(clone(physVol));
        } else if (auto geoId = dynamic_pointer_cast<const GeoIdentifierTag>(node); geoId) {
            std::lock_guard guard{s_mutex};
            newVol->add(s_geoIds.insert(std::make_pair(geoId->getIdentifier(), const_pointer_cast(geoId))).first->second);
        } else if (auto serialId = dynamic_pointer_cast<const GeoSerialIdentifier>(node); serialId) {
            std::lock_guard guard{s_mutex};
            newVol->add(s_serialIds.insert(std::make_pair(serialId->getBaseId(), const_pointer_cast(serialId))).first->second);
        } else if (auto nameTag = dynamic_pointer_cast<const GeoNameTag>(node); nameTag) {
            std::lock_guard guard{s_mutex};
            newVol->add(s_nameTags.insert(std::make_pair(nameTag->getName(), const_pointer_cast(nameTag))).first->second);
        } else {
            /// Just copy what's left
            newVol->add(const_pointer_cast(node));
        }
    }
    if (tryPhysVolDeDup){
        newVol = cacheVolume(dynamic_pointer_cast<GeoPhysVol>(newVol));
    }
    return newVol;
}
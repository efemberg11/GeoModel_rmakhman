/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/Element2GeoItemRegistry.h"
#include "GeoModelXml/Element2GeoItem.h"

void Element2GeoItemRegistry::enableLogVolDeDuplication(bool enable){
    m_deDuplicateLogVols = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setLogVolDeDuplication(enable);
    }
}
void Element2GeoItemRegistry::enableShapeDeDuplication(bool enable){
    m_deDuplicateShapes = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setShapeDeDuplication(enable);
    }
}
void Element2GeoItemRegistry::enableTransformDeDuplication(bool enable) {
    m_deDuplicateTransforms = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setTransformDeDuplication(enable);
    }
}
void Element2GeoItemRegistry::enablePhysVolDeDuplication(bool enable){
    m_deDuplicatePhysVols = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setPhysVolDeDuplication(enable);
    }
}

void Element2GeoItemRegistry::enregister(const std::string& tagName, Element2GeoItem *processor) {
    m_processor[tagName] = processor;
    processor->setPhysVolDeDuplication(m_deDuplicatePhysVols);
    processor->setTransformDeDuplication(m_deDuplicateTransforms);
    processor->setShapeDeDuplication(m_deDuplicateShapes);
    processor->setLogVolDeDuplication(m_deDuplicateLogVols);
}

Element2GeoItem * Element2GeoItemRegistry::find(const std::string& name) {
    std::map<std::string, Element2GeoItem *>::iterator it = m_processor.find(name);
    return it == m_processor.end()? m_defaultProcessor.get(): it->second;
}

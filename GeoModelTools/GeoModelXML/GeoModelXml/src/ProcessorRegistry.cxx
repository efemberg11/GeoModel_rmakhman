/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/ProcessorRegistry.h"


void ProcessorRegistry::enableLogVolDeDuplication(bool enable){
    m_deDuplicateLogVols = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setLogVolDeDuplication(enable);
    }
}
void ProcessorRegistry::enableShapeDeDuplication(bool enable){
    m_deDuplicateShapes = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setShapeDeDuplication(enable);
    }
}
void ProcessorRegistry::enableTransformDeDuplication(bool enable) {
    m_deDuplicateTransforms = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setTransformDeDuplication(enable);
    }
}
void ProcessorRegistry::enablePhysVolDeDuplication(bool enable){
    m_deDuplicatePhysVols = enable;
    for (auto& [tagName, proc]: m_processor) {
        proc->setPhysVolDeDuplication(enable);
    }
}

void ProcessorRegistry::enregister(const std::string& tagName, ElementProcessor *processor) {
    m_processor[tagName] = processor;
    processor->setPhysVolDeDuplication(m_deDuplicatePhysVols);
    processor->setTransformDeDuplication(m_deDuplicateTransforms);
    processor->setShapeDeDuplication(m_deDuplicateShapes);
    processor->setLogVolDeDuplication(m_deDuplicateLogVols);
}

ElementProcessor * ProcessorRegistry::find(const std::string& name) {
    std::map<std::string, ElementProcessor *>::iterator it = m_processor.find(name);

    return it == m_processor.end()? m_defaultProcessor.get(): it->second;
}

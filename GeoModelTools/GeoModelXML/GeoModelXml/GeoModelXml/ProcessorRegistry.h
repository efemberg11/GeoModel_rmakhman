/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

//
//    Process an element followed by all its children.
//    Element processors have to be registered with me so I know who to call.
//    Any element tag with no registered processor will be handled by a default handler, which
//    does nothing and stops processing of its children.
//
#ifndef GEO_MODEL_XML_PROCESSOR_REGISTRY_H
#define GEO_MODEL_XML_PROCESSOR_REGISTRY_H


#include "GeoModelXml/ElementProcessor.h"
#include <string>
#include <map>


class ProcessorRegistry {
public:
    ProcessorRegistry() = default;
    
    void enregister(const std::string& tagName, ElementProcessor *processor);
    ElementProcessor *find(const std::string& name);

    /** Switches whether the log vol shall be shared across multiple physVols. 
     *  Propagates the flag to all registered ElementProcessors */
    void enableLogVolDeDuplication(bool enable);
    /** Switches whether the shapes shall be shared across the Geometry
     *  Propagates the flag to all regsitered ElementProcessors */
    void enableShapeDeDuplication(bool enable);
    /** Switches whether the non-alignable transform nodes shall be shared across the Geometry tree
     *  Propagtes the flag to all registered ElementProcessors */
    void enableTransformDeDuplication(bool enable);
    /** Switches whether the equivalent ordinary phyiscal volumes shall be shared across the Geometry tree
     *  Propagtes the flag to all registered ElementProcessors */
    void enablePhysVolDeDuplication(bool enable);

private:
    bool m_deDuplicateLogVols{false};
    bool m_deDuplicateShapes{false};
    bool m_deDuplicateTransforms{false};
    bool m_deDuplicatePhysVols{false};
    std::unique_ptr<ElementProcessor> m_defaultProcessor{std::make_unique<ElementProcessor>()};
    std::map<std::string, ElementProcessor *> m_processor;

};

#endif // PROCESSOR_REGISTRY_H

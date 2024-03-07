/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//    Process an element followed by all its children.
//    Element processors have to be registered with me so I know who to call.
//    Any element tag with no registered processor will be handled by a default handler, which
//    does nothing and stops processing of its children.
//
#ifndef GEO_MODEL_XML_ELEMENT2GEO_ITEM_REGISTRY_H
#define GEO_MODEL_XML_ELEMENT2GEO_ITEM_REGISTRY_H
#include <string>
#include <map>

#include "GeoModelXml/Element2GeoItem.h"

class Element2GeoItemRegistry {
public:
    Element2GeoItemRegistry() = default;
    ~Element2GeoItemRegistry() = default;
    void enregister(const std::string& tagName,  Element2GeoItem *processor); // register = keyword
    Element2GeoItem *find(const std::string& tagName);

    /** Switches whether the log vol shall be shared across multiple physVols. 
     *  Propagates the flag to all registered Element2GeoItem objects */
    void enableLogVolDeDuplication(bool enable);
    /** Switches whether the shapes shall be shared across the Geometry
     *  Propagates the flag to all regsitered Element2GeoItem objects */
    void enableShapeDeDuplication(bool enable);
    /** Switches whether the non-alignable transform nodes shall be shared across the Geometry tree
     *  Propagtes the flag to all registered Element2GeoItem objects */
    void enableTransformDeDuplication(bool enable);
    /** Switches whether the equivalent ordinary phyiscal volumes shall be shared across the Geometry tree
     *  Propagtes the flag to all registered Element2GeoItem objects */
    void enablePhysVolDeDuplication(bool enable);
private:
    std::unique_ptr<Element2GeoItem> m_defaultProcessor{std::make_unique<Element2GeoItem>()};
    std::map<std::string, Element2GeoItem *> m_processor{};

    bool m_deDuplicateLogVols{false};
    bool m_deDuplicateShapes{false};
    bool m_deDuplicateTransforms{false};
    bool m_deDuplicatePhysVols{false};
};

#endif // ELEMENT2GEO_ITEM_REGISTRY_H

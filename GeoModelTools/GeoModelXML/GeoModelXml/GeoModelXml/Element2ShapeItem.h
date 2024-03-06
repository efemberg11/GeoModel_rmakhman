/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEO_MODEL_XML_ELEMENT_2_SHAPEITEM_H
#define GEO_MODEL_XML_ELEMENT_2_SHAPEITEM_H
#include "GeoModelXml/Element2GeoItem.h"

#include <set>
class Element2ShapeItem: public Element2GeoItem {
    public:
        Element2ShapeItem();
        
        virtual ~Element2ShapeItem();

        static void deduplicateShapes(const bool enable);

    private:
        static std::set<Element2ShapeItem*> s_instances;
        static bool s_applyDepluication;
};


#endif
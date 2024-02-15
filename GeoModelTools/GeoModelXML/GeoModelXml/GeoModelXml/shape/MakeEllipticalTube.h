/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_MAKE_ELLIPTICAL_TUBE_H
#define GEO_MODEL_XML_MAKE_ELLIPTICAL_TUBE_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"

class MakeEllipticalTube: public Element2GeoItem {
public:
    MakeEllipticalTube() = default;;
    virtual RCBase * make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;

};

#endif // GEO_MODEL_XML_MAKE_ELLIPTICAL_TUBE_H

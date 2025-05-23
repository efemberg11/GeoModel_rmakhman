/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

//
//    Handle material elements.
//
#ifndef GEO_MODEL_XML_MAKE_TRANSFORMATION_H
#define GEO_MODEL_XML_MAKE_TRANSFORMATION_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"

class MakeTransformation: public Element2GeoItem {
public:
    MakeTransformation() = default;
    virtual GeoIntrusivePtr<RCBase> make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;
};

#endif // MAKE_TRANSFORMATION_H

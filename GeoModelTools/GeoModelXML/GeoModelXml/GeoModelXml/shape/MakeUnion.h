/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
// Automatically generated from scripts in /home/hessey/prog/gmx2geo/makeshape
//    Handle tube shape elements.
//
#ifndef GEO_MODEL_XML_MAKE_UNION_H
#define GEO_MODEL_XML_MAKE_UNION_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"

class MakeUnion: public Element2GeoItem {
public:
    MakeUnion() = default;
    virtual GeoIntrusivePtr<RCBase> make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;

};

#endif // GEO_MODEL_XML_MAKE_UNION_H

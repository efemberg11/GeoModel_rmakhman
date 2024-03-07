/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
// Jun 2021, Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
//
#ifndef GEO_MODEL_XML_MAKE_TORUS_H
#define GEO_MODEL_XML_MAKE_TORUS_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"


class MakeTorus: public Element2GeoItem {
public:
    MakeTorus() = default;
    virtual GeoIntrusivePtr<RCBase> make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;

};

#endif // GEO_MODEL_XML_MAKE_TORUS_H

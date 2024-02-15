/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_MAKE_TWISTED_TRAP_H
#define GEO_MODEL_XML_MAKE_TWISTED_TRAP_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"


class MakeTwistedTrap: public Element2GeoItem {
public:
    MakeTwistedTrap() = default;
    virtual RCBase * make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;

};

#endif // GEO_MODEL_XML_MAKE_TWISTED_TRAP_H

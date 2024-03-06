/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

//
// Automatically generated from scripts in /home/hessey/prog/gmx2geo/makeshape
//    Handle tube shape elements.
//
#ifndef GEO_MODEL_XML_MAKE_PGON_H
#define GEO_MODEL_XML_MAKE_PGON_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2ShapeItem.h"


class MakePgon: public Element2ShapeItem {
  public:
      MakePgon() = default;
      virtual RCBase * make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;

};

#endif // GEO_MODEL_XML_MAKE_PGON_H

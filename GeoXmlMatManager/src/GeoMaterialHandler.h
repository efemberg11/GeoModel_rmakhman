/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELSVC_GEOMATERIALHANDLER_H
#define GEOMODELSVC_GEOMATERIALHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlMatManager;

class GeoMaterialHandler : public XMLHandler {
 public:
  GeoMaterialHandler(std::string,GeoXmlMatManager*);
  void ElementHandle();
 private:
  GeoXmlMatManager* m_matManager;
};

#endif

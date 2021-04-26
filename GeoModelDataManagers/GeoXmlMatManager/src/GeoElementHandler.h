/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLMATMANAGER_GEOELEMENTHANDLER_H
#define GEOXMLMATMANAGER_GEOELEMENTHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlMatManager;

class GeoElementHandler : public XMLHandler {
 public:
  GeoElementHandler(std::string,GeoXmlMatManager*);
  void ElementHandle();
 private:
  GeoXmlMatManager* m_matManager;
};

#endif

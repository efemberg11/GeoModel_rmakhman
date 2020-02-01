/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELSVC_GEOELEMENTHANDLER_H
#define GEOMODELSVC_GEOELEMENTHANDLER_H

#include "XMLParser/XMLHandler.h"
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

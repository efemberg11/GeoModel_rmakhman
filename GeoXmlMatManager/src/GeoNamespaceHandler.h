/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELSVC_GEONAMESPACEHANDLER_H
#define GEOMODELSVC_GEONAMESPACEHANDLER_H

#include "XMLParser/XMLHandler.h"
#include <string>

class GeoXmlMatManager;

class GeoNamespaceHandler : public XMLHandler {
 public:
  GeoNamespaceHandler(std::string,GeoXmlMatManager*);
  void ElementHandle();
 private:
  GeoXmlMatManager* m_matManager;
};

#endif

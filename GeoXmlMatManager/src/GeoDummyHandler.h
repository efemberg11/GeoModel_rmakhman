/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELSVC_GEODUMMYHANDLER_H
#define GEOMODELSVC_GEODUMMYHANDLER_H

#include "XMLParser/XMLHandler.h"
#include <string>

class GeoDummyHandler : public XMLHandler {
 public:
  GeoDummyHandler(std::string);
  void ElementHandle();
};

#endif

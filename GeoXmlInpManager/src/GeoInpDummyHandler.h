/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPDUMMYHANDLER_H
#define GEOXMLINPMANAGER_GEOINPDUMMYHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoInpDummyHandler : public XMLHandler {
 public:
  GeoInpDummyHandler(std::string);
  void ElementHandle();
};

#endif

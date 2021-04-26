/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLMATMANAGER_GEOMATDUMMYHANDLER_H
#define GEOXMLMATMANAGER_GEOMATDUMMYHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoMatDummyHandler : public XMLHandler {
 public:
  GeoMatDummyHandler(std::string);
  void ElementHandle();
};

#endif

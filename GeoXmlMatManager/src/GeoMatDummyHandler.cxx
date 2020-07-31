/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoMatDummyHandler.h"
#include <iostream>

GeoMatDummyHandler::GeoMatDummyHandler(std::string s)
  : XMLHandler(s)
{
}

void GeoMatDummyHandler::ElementHandle()
{
  // Do nothing
}

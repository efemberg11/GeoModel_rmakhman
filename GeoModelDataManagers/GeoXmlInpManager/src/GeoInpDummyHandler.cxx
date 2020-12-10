/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInpDummyHandler.h"

GeoInpDummyHandler::GeoInpDummyHandler(std::string s)
  : XMLHandler(s)
{
}

void GeoInpDummyHandler::ElementHandle()
{
  // Do nothing
}

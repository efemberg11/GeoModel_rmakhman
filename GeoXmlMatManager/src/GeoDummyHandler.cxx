/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoDummyHandler.h"
#include <iostream>

GeoDummyHandler::GeoDummyHandler(std::string s)
  : XMLHandler(s)
{
}

void GeoDummyHandler::ElementHandle()
{
  // Do nothing
}

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPROWHANDLER_H
#define GEOXMLINPMANAGER_GEOINPROWHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlInpManager;

class GeoInpRowHandler : public XMLHandler {
public:
  GeoInpRowHandler(std::string s
		   , GeoXmlInpManager* man);
  void ElementHandle();

private:
  GeoXmlInpManager* m_inpManager;
};
#endif

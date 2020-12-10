/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPCOLDEFHANDLER_H
#define GEOXMLINPMANAGER_GEOINPCOLDEFHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlInpManager;

class GeoInpColDefHandler : public XMLHandler {
public:
  GeoInpColDefHandler(std::string s
		      , GeoXmlInpManager* man);
  void ElementHandle();

private:
  GeoXmlInpManager* m_inpManager;
};
#endif

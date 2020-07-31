/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPTABLEDEFHANDLER_H
#define GEOXMLINPMANAGER_GEOINPTABLEDEFHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlInpManager;

class GeoInpTableDefHandler : public XMLHandler {
public:
  GeoInpTableDefHandler(std::string s
			, GeoXmlInpManager* man);
  void ElementHandle();
private:
  GeoXmlInpManager* m_inpManager;
};

#endif

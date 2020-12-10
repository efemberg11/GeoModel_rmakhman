/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPTABLEHANDLER_H
#define GEOXMLINPMANAGER_GEOINPTABLEHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlInpManager;

class GeoInpTableHandler : public XMLHandler {
public:
  GeoInpTableHandler(std::string s
		     , GeoXmlInpManager* man);
  void ElementHandle();

private:
  GeoXmlInpManager* m_inpManager;
};
#endif

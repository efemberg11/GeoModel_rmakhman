/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef polyplaneHandler_H
#define polyplaneHandler_H

#include "GeoModelXMLParser/XMLHandler.h"
#include "AGDDHandlers/Polyplane.h"
#include <string>

class polyplaneHandler:public XMLHandler {
public:
	polyplaneHandler(std::string);
	void ElementHandle();
	static Polyplane CurrentPolyplane() {return s_pPlane;}
private:
	static Polyplane s_pPlane;
};

#endif

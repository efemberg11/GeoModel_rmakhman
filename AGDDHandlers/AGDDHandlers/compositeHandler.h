/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef compositeHandler_H
#define compositeHandler_H

#include "XMLParser/XMLHandler.h"
#include <string>

class compositeHandler:public XMLHandler {
public:
	compositeHandler(std::string);
	void ElementHandle();

};

#endif

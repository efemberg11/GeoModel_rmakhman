/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/shape/AddPlane.h"
#include "GeoModelXml/GmxUtil.h"
#include <string>
#include <sstream>
#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include "xercesc/util/XMLString.hpp"

using namespace xercesc;

void AddPlane::process(const xercesc::DOMElement *element, double &zPlane, double &rMinPlane, double &rMaxPlane) {

  if (!gmxUtil)
  {
	std::cout<<"This is AddPlane::process()!! gmxUtil is 0!!!!!"<<std::endl;
	return;
  }
  XMLCh * zplane_tmp = XMLString::transcode("zplane");
  zPlane=gmxUtil->evaluate(XMLString::transcode(element->getAttribute(zplane_tmp)));
  XMLCh * rminplane_tmp = XMLString::transcode("rminplane");
  rMinPlane=gmxUtil->evaluate(XMLString::transcode(element->getAttribute(rminplane_tmp)));
  XMLCh * rmaxplane_tmp = XMLString::transcode("rmaxplane");
  rMaxPlane=gmxUtil->evaluate(XMLString::transcode(element->getAttribute(rmaxplane_tmp)));
  XMLString::release(&zplane_tmp);
  XMLString::release(&rminplane_tmp);
  XMLString::release(&rmaxplane_tmp);
    return;
}

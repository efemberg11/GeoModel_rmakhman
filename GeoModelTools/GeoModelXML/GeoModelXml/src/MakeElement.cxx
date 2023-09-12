/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MakeElement.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoElement.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace xercesc;
using namespace GeoModelKernelUnits;

MakeElement::MakeElement() {}

RCBase * MakeElement::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
  //std::cout<<"this is MakeElement: make()"<<std::endl;
  char *name, *shortname, *z,*a;
  XMLCh *name_tmp,*shortname_tmp,*z_tmp,*a_tmp;
  double zVal, aVal;
//
//   Get my Z
//
  z_tmp = XMLString::transcode("Z");
  z = XMLString::transcode(element->getAttribute(z_tmp));
  zVal = gmxUtil.evaluate(z);
  XMLString::release(&z);
  XMLString::release(&z_tmp);
//
//   Get my A
//
  a_tmp = XMLString::transcode("A");
  a = XMLString::transcode(element->getAttribute(a_tmp));
  aVal = gmxUtil.evaluate(a);
  XMLString::release(&a);
  XMLString::release(&a_tmp);
  //
  //    Get my names
  //
  name_tmp = XMLString::transcode("name");
  name = XMLString::transcode(element->getAttribute(name_tmp));
  shortname_tmp = XMLString::transcode("shortname");
  shortname = XMLString::transcode(element->getAttribute(shortname_tmp));
  //
  //    Create it
  //
  //aVal *= gram/mole;

  GeoElement *el=0;
  if (gmxUtil.matManager)
  {
  	gmxUtil.matManager->addElement(name, shortname, zVal, aVal);
	  el=const_cast<GeoElement *>(gmxUtil.matManager->getElement(name));
    //std::cout<<"MaterialManager: element added "<<el->getName()<<std::endl;
  }
  else
  {
  	//std::cout<<" making new element "<<name<<" "<<shortname<<" "<<zVal<<" "<<aVal<<std::endl;
  	el = new GeoElement(name, shortname, zVal, aVal*gram/mole);
  }
  XMLString::release(&name);
  XMLString::release(&shortname);
  XMLString::release(&name_tmp);
  XMLString::release(&shortname_tmp);

  return (RCBase *) el;
}

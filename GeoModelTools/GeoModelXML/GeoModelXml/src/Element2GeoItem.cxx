/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/
#include <string>

#include "GeoModelXml/Element2GeoItem.h"
#include "OutputDirector.h"

#include "xercesc/util/XercesDefs.hpp"
#include <xercesc/dom/DOM.hpp>
#include "xercesc/util/XMLString.hpp"

#include "GeoModelXml/GmxUtil.h"
#include "GeoModelKernel/RCBase.h"

using namespace std;
using namespace xercesc;

Element2GeoItem::Element2GeoItem() {}

Element2GeoItem::~Element2GeoItem() {}

RCBase * Element2GeoItem::process(const xercesc::DOMElement *element, GmxUtil &gmxUtil) {

    char *name2release;
    XMLCh * name_tmp = XMLString::transcode("name");

    name2release = XMLString::transcode(element->getAttribute(name_tmp));
    string name(name2release);
    XMLString::release(&name2release);
    XMLString::release(&name_tmp);

    RCBase *item;
    map<string, RCBase *>::iterator entry;
    if (name == "") { // Unnamed item; cannot store in the map; make a new one 
        item = make(element, gmxUtil);
    }
    else if ((entry = m_map.find(name)) == m_map.end()) { // Not in; make a new one
        item = make(element, gmxUtil);
        m_map[name] = item; // And put it in the map
    }
    else { // Get it from the map
        item = entry->second; 
    }

    return item;
}

RCBase * Element2GeoItem::make(const xercesc::DOMElement *element, GmxUtil & /* gmxUtil */) const {
    char *name2release = XMLString::transcode(element->getNodeName());
    msglog << MSG::FATAL << "Oh oh: called base class make() method of Element2GeoType object; tag " << name2release << endmsg;
    XMLString::release(&name2release);

    std::abort();
}

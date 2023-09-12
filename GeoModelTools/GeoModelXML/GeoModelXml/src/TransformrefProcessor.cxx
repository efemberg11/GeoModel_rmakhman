/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//    Process transformref items: basically, just find the referenced transform and call its processor.
//
#include "GeoModelXml/TransformrefProcessor.h"
#include "OutputDirector.h"
#include <string>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelXml/GmxUtil.h"
#include "GeoModelXml/GeoNodeList.h"
#include "xercesc/util/XMLString.hpp"

using namespace std;
using namespace xercesc;

void TransformrefProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
XMLCh *ref = XMLString::transcode("ref");
const XMLCh *idref;
DOMDocument *doc = element->getOwnerDocument();
char *toRelease;
//
//    Get the referenced element
//
    idref = element->getAttribute(ref);
    DOMElement *elem = doc->getElementById(idref);
//
//    Check it is the right sort
//
    toRelease = XMLString::transcode(elem->getNodeName());
    string nodeName(toRelease);
    XMLString::release(&toRelease);
    if (nodeName != "transform") {
        msglog << MSG::FATAL<< "Error in xml/gmx file: transformref " << XMLString::transcode(idref) << " referenced a " << nodeName << 
                " instead of a logvol." << endmsg;
	std::abort();
    }
//
//    Process it
//
    gmxUtil.tagHandler.transform.process(elem, gmxUtil, toAdd);

    XMLString::release(&ref);

    return;
}

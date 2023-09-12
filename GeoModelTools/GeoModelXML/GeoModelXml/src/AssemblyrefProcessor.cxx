/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//    Process assemblyref items: basically, just find the referenced assembly and call its processor.
//
#include "OutputDirector.h"
#include "GeoModelXml/AssemblyrefProcessor.h"
#include <string>

#include "xercesc/util/XercesDefs.hpp"
#include <xercesc/dom/DOM.hpp>
#include "xercesc/util/XMLString.hpp"

#include "GeoModelXml/GmxUtil.h"
#include "GeoModelXml/GeoNodeList.h"

using namespace std;
using namespace xercesc;

void AssemblyrefProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
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
    if (nodeName != string("assembly") && nodeName != string("set")) {
        msglog << MSG::FATAL << "Error in xml/gmx file: assemblyref " << XMLString::transcode(idref) << " referenced a " << 
                              nodeName << " instead of an assembly.\n";
	std::abort();
    }
//
//    Process it
//
    XMLCh * zeroid_tmp = XMLString::transcode("zeroid");
    const XMLCh *zeroid = element->getAttribute(zeroid_tmp);
    if (XMLString::equals(zeroid, XMLString::transcode("true"))) {
        gmxUtil.tagHandler.assembly.zeroId(elem);
    }
    gmxUtil.tagHandler.assembly.process(elem, gmxUtil, toAdd);
    XMLString::release(&ref);
    XMLString::release(&zeroid_tmp);
    return;
}

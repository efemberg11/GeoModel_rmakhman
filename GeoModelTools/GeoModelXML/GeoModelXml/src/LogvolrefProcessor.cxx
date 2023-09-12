/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

//
//    Process logvolref items: basically, just find the referenced logvol and call its processor.
//
#include "GeoModelXml/LogvolrefProcessor.h"
#include "OutputDirector.h"
#include <string>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelXml/GeoNodeList.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace std;
using namespace xercesc;

void LogvolrefProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
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
    if (nodeName != string("logvol")) {
        msglog << MSG::FATAL << "Error in xml/gmx file: logvolref " << XMLString::transcode(idref) << " referenced a " << nodeName << 
                " instead of a logvol." << endmsg;
	std::abort();
    }
//
//    Process it
//
    XMLCh * zeroid_tmp = XMLString::transcode("zeroid");
    XMLCh * true_tmp = XMLString::transcode("true");
    const XMLCh *zeroid = element->getAttribute(zeroid_tmp);
    if (XMLString::equals(zeroid, true_tmp)) {
        gmxUtil.tagHandler.logvol.zeroId(elem);
    }

    //msglog << MSG::DEBUG << "\n\nLogvolrefProcessor -- Calling the processor for the LogVol element..." << endmsg;
    gmxUtil.tagHandler.logvol.process(elem, gmxUtil, toAdd);
    XMLString::release(&ref);
    XMLString::release(&zeroid_tmp);
    XMLString::release(&true_tmp);

    return;
}

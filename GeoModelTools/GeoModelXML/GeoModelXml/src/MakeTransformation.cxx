/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MakeTransformation.h"
#include <xercesc/dom/DOM.hpp>
//   #include <CLHEP/Geometry/Transform3D.h>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include "GeoModelKernel/GeoDefinitions.h"

using namespace xercesc;
using namespace std;

MakeTransformation::MakeTransformation() {}

RCBase * MakeTransformation::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
char *name2release;
 GeoTrf::Transform3D hepTransform=GeoTrf::Transform3D::Identity(); // Starts as Identity transform
//
//   Add my element contents
//
    for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
        if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
            name2release = XMLString::transcode(child->getNodeName());
            string name(name2release);
            DOMElement *el = dynamic_cast<DOMElement *>(child);
            if (name == string("translation")) {
                hepTransform = hepTransform * gmxUtil.tagHandler.translation.getTransform(el, gmxUtil); 
            }
            else if (name == "rotation") {
                hepTransform = hepTransform * gmxUtil.tagHandler.rotation.getTransform(el, gmxUtil); 
            }
            else if (name == "scaling") {
	    
// TODO: figure out what to do in this case 	    
//                hepTransform = hepTransform * gmxUtil.tagHandler.scaling.getTransform(el, gmxUtil); 
            }
            XMLString::release(&name2release);
        }
    }
//
//    Create and return GeoModel transform
//

    XMLCh * alignable_tmp = XMLString::transcode("alignable");
    char *toRelease = XMLString::transcode(element->getAttribute(alignable_tmp));
    string alignable(toRelease);
    XMLString::release(&toRelease);
    XMLString::release(&alignable_tmp);
    if (alignable.compare(string("true")) == 0) {
        return (RCBase *) new GeoAlignableTransform(hepTransform);
    }
    else {
        return (RCBase *) new GeoTransform(hepTransform);
    }
}

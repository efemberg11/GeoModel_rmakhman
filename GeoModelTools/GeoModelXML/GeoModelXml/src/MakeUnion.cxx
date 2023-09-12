/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/shape/MakeUnion.h"
#include <string>
#include "OutputDirector.h"
#include <xercesc/dom/DOM.hpp>
//   #include <CLHEP/Geometry/Transform3D.h>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace xercesc;
using namespace std;

MakeUnion::MakeUnion() {}

RCBase * MakeUnion::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
// 
//    Process child elements; first is first shaperef; then transform; then second shaperef.
//
    const GeoShape *first = 0;
    const GeoShape *second = 0;
    GeoTrf::Transform3D hepXf=GeoTrf::Transform3D::Identity();
    int elementIndex = 0;
    for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
        if (child->getNodeType() == DOMNode::ELEMENT_NODE) { // Skips text nodes
	  switch (elementIndex) {
	  case 0: { // First element is first shaperef
	    first = static_cast<GeoShape *>( gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement *> (child), gmxUtil));
	    break;
	  }
	  case 1: { // Second element is transformation or transformationref
	    char *toRelease = XMLString::transcode(child->getNodeName());
	    string nodeName(toRelease);
	    XMLString::release(&toRelease);
	    const GeoTransform *geoXf = (nodeName == "transformation")
	      ? static_cast<const GeoTransform *>( gmxUtil.tagHandler.transformation.process(dynamic_cast<DOMElement *>(child), gmxUtil))
	      : static_cast<const GeoTransform *>( gmxUtil.tagHandler.transformationref.process(dynamic_cast<DOMElement *>(child), gmxUtil));
	    hepXf = geoXf->getTransform();
	    break;
	  }
	  case 2: { // Third element is second shaperef
	    second = static_cast<const GeoShape *>( gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement *> (child), gmxUtil));
	    break;
	  }
	  default: // More than 3 elements?
	    msglog << MSG::FATAL << "MakeUnion: Incompatible DTD? got more than 3 child elements" << endmsg;
	  }
	  elementIndex++;
        }
    }

    if (!first || !second) std::abort();

    // FIXME: add() returns a new'd object --- should really be
    // returning a `unique_ptr<GeoShapeUnion>' not a
    // `const GeoShapeUnion'
    GeoShapeUnion *temp = const_cast<GeoShapeUnion*>(&(first->add(*(GeoShape *) &(*(second) << hepXf))));

    return (RCBase *) temp;
}

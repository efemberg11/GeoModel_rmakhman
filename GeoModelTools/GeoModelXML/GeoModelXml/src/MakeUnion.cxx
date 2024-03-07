/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/shape/MakeUnion.h"
#include <string>
#include "OutputDirector.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelHelpers/throwExcept.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace xercesc;
using namespace std;


GeoIntrusivePtr<RCBase>MakeUnion::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    // 
    //    Process child elements; first is first shaperef; then transform; then second shaperef.
    //
    GeoIntrusivePtr<const GeoShape> first{}, second{};
    GeoTrf::Transform3D hepXf=GeoTrf::Transform3D::Identity();
    int elementIndex = 0;
    for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
        if (child->getNodeType() == DOMNode::ELEMENT_NODE) { // Skips text nodes
          switch (elementIndex) {
            case 0: { // First element is first shaperef
              first = dynamic_pointer_cast<GeoShape>(gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement *> (child), gmxUtil));
              break;
            }
            case 1: { // Second element is transformation or transformationref
              char *toRelease = XMLString::transcode(child->getNodeName());
              string nodeName(toRelease);
              XMLString::release(&toRelease);
              GeoTrfPtr geoXf = (nodeName == "transformation") ? 
                    dynamic_pointer_cast<GeoTransform>(gmxUtil.tagHandler.transformation.process(dynamic_cast<DOMElement *>(child), gmxUtil))
                :   dynamic_pointer_cast<GeoTransform>(gmxUtil.tagHandler.transformationref.process(dynamic_cast<DOMElement *>(child), gmxUtil));
              hepXf = geoXf->getTransform();
              break;
            } case 2: { // Third element is second shaperef
              second = dynamic_pointer_cast<GeoShape>( gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement *> (child), gmxUtil));
              break;
            } default: // More than 3 elements?
              THROW_EXCEPTION("MakeUnion: Incompatible DTD? got more than 3 child elements");
          }
          elementIndex++;
        }
    }

    if (!first || !second) {
		    THROW_EXCEPTION("One of the two operands is not defined");
    }
    GeoIntrusivePtr<const GeoShapeUnion> unionShape{};

   static const GeoTrf::TransformSorter sorter{};
    if (true || sorter.compare(hepXf, GeoTrf::Transform3D::Identity())) {
        unionShape = make_intrusive<GeoShapeUnion>(first, make_intrusive<GeoShapeShift>(second,hepXf));
    } else {
        unionShape = make_intrusive<GeoShapeUnion>(first, second);
    }
 
    return  const_pointer_cast(cacheShape(unionShape));
}

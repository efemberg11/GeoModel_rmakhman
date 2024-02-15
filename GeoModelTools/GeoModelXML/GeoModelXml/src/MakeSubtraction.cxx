/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

// Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
// Then heavily modified
#include "GeoModelXml/shape/MakeSubtraction.h"
#include "OutputDirector.h"
#include <string>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelHelpers/throwExcept.h"

#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"
#include <array>
using namespace xercesc;
using namespace std;


RCBase * MakeSubtraction::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
// 
//    Process child elements; first is first shaperef; then transformation; then second shaperef.
//
    GeoIntrusivePtr<const GeoShape> first{}, second{};
    GeoTrf::Transform3D hepXf=GeoTrf::Transform3D::Identity();
    int elementIndex = 0;
    for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
        if (child->getNodeType() == DOMNode::ELEMENT_NODE) { // Skips text nodes
      switch (elementIndex) {
      case 0: { // First element is first shaperef
        first = static_cast<const GeoShape *>(gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement *> (child), gmxUtil));
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
        THROW_EXCEPTION("MakeSubtraction: Incompatible DTD? got more than 3 child elements");
      }
      elementIndex++;
        }
    }

    if (!first || !second) {
        THROW_EXCEPTION("Only one of the two subtraction operands was defined.");
    }

  
    GeoIntrusivePtr<GeoShapeSubtraction> subtract{};
    static const GeoTrf::TransformSorter sorter{};
    if (sorter.compare(hepXf, GeoTrf::Transform3D::Identity())) {
        subtract = new GeoShapeSubtraction(first, new GeoShapeShift(second, hepXf));
    } else {
        subtract = new GeoShapeSubtraction(first, second);
    }
    return const_cast<GeoShape*>(cacheShape(subtract).get());
}

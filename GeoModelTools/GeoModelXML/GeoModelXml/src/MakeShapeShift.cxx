/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelXml/shape/MakeShapeShift.h"

#include "OutputDirector.h"
#include <string>
#include <xercesc/dom/DOM.hpp>

#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoTransform.h"

#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include "GeoModelHelpers/TransformSorter.h"
#include "GeoModelHelpers/throwExcept.h"
#include "GeoModelHelpers/GeoShapeUtils.h"

#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace xercesc;

GeoIntrusivePtr<RCBase> MakeShapeShift::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    
    unsigned int elementIndex = 0;
    GeoShape* shape{nullptr};
    GeoTrf::Transform3D hepXf{GeoTrf::Transform3D::Identity()};
    for (DOMNode *child = element->getFirstChild(); child != nullptr; child = child->getNextSibling()) {
       if (child->getNodeType() != DOMNode::ELEMENT_NODE) { 
            continue;
       }
      switch (elementIndex) {
        case 0: { // First element is first shaperef
            shape = static_cast<GeoShape*>(gmxUtil.tagHandler.shaperef.process(dynamic_cast<DOMElement*> (child), gmxUtil));
            break;
        } case 1: { // Second element is transformation or transformationref
            char *toRelease = XMLString::transcode(child->getNodeName());
            std::string nodeName{toRelease};
            XMLString::release(&toRelease);
            const GeoTransform *geoXf = (nodeName == "transformation")
              ? static_cast<const GeoTransform *>( gmxUtil.tagHandler.transformation.process(dynamic_cast<DOMElement *>(child), gmxUtil))
              : static_cast<const GeoTransform *>( gmxUtil.tagHandler.transformationref.process(dynamic_cast<DOMElement *>(child), gmxUtil));
            hepXf = geoXf->getTransform();
            break;
        } default: // More than 3 elements?
            THROW_EXCEPTION("Only transofrmation & geoshaperef are accepted");
      }
      ++elementIndex;        
    }
    if (!shape) {
        THROW_EXCEPTION("No shape has been given");
    }
    
    static const GeoTrf::TransformSorter sorter{};
    if (!sorter.compare(GeoTrf::Transform3D::Identity(), hepXf)) {
        return shape;
    }    
    return const_cast(cacheShape(make_intrusive<GeoShapeShift>(shape, hepXf)));
}


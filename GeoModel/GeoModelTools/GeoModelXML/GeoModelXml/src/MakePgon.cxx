/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

// Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
// But then edited for AddPlane stuff
#include "GeoModelXml/shape/MakePgon.h"
#include "GeoModelKernel/GeoPgon.h"
#include <xercesc/dom/DOM.hpp>
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>
using namespace xercesc;


GeoIntrusivePtr<RCBase>MakePgon::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    constexpr int nParams = 3; 
    static const std::array<std::string, nParams> parName {"sphi", "dphi", "nsides"};
    std::array<double, nParams> p{};
    char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }

    GeoIntrusivePtr<GeoPgon> pgon = make_intrusive<GeoPgon>(p[0], p[1], p[2]);
    //
    //    Add planes
    //
    double zPlane{0.}, rMinPlane{0.}, rMaxPlane{0.};
    for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
        if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
            toRelease = XMLString::transcode(child->getNodeName());
            std::string name(toRelease);
            XMLString::release(&toRelease);
            if (name == "addplane") {
                gmxUtil.tagHandler.addplane.process(dynamic_cast<const DOMElement *>(child), zPlane, rMinPlane, rMaxPlane);
                pgon->addPlane(zPlane, rMinPlane, rMaxPlane);
            }
        }
    }
    return const_pointer_cast(cacheShape(pgon));
}

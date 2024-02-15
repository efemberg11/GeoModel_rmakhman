/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

// Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
// But then edited for AddPlane stuff
#include "GeoModelXml/shape/MakePcon.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelHelpers/GeoShapeUtils.h"
#include "GeoModelHelpers/throwExcept.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>
using namespace xercesc;


RCBase * MakePcon::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    constexpr int nParams = 2; 
    static const std::array<std::string, nParams> parName{"sphi", "dphi"};
    std::array<double, nParams> p{};
    char *toRelease;
    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }

    GeoIntrusivePtr<GeoPcon> pcon{new GeoPcon(p[0], p[1])};
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
                pcon->addPlane(zPlane, rMinPlane, rMaxPlane);
            }
        }
    }
    if (!pcon->isValid()) {
        THROW_EXCEPTION("Invalid Pcon defined "<<printGeoShape(pcon));
    }
    return const_cast<GeoShape*>(cacheShape(pcon).get());
}

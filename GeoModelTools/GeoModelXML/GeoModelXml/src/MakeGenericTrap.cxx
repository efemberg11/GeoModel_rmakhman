/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

// *NOT*!!! Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
// GenericTraps would have 17 params, so Vakho built up a vector instead.
// So skeleton was automatically generated, then edited by hand.
//
#include "GeoModelXml/shape/MakeGenericTrap.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>


using namespace xercesc;


RCBase * MakeGenericTrap::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    constexpr int nParams = 17; 
    std::array<std::string, 17> parName{"x0", "y0", "x1", "y1", "x2", "y2", "x3", "y3", 
                                        "x4", "y4", "x5", "y5", "x6", "y6", "x7", "y7", "zhalflength"};
    char *toRelease;
    std::vector<GeoTwoVector> vertices;
    double x{0.}, y{0.}, p{0.};

    for (int i = 0; i < (nParams - 1) / 2; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[2 * i].data())));
        x = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[2 * i + 1].data())));
        y = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
        vertices.emplace_back(x, y);
    }
    //
    //    z-half-length
    //
    toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[16].data())));
    p = gmxUtil.evaluate(toRelease);
    XMLString::release(&toRelease);

    return const_cast<GeoShape*>(cacheShape(new GeoGenericTrap(p, vertices)).get());
}

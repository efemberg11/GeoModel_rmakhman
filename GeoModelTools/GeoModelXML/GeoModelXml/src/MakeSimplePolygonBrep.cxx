/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <array>

#include "GeoModelXml/shape/MakeSimplePolygonBrep.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelHelpers/throwExcept.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"


using namespace xercesc;


GeoIntrusivePtr<RCBase> MakeSimplePolygonBrep::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    constexpr int nParams = 3; 
    static const std::array<std::string, nParams> parName {"xpoints", "ypoints", "zhalflength"};
    double z{0.};
    std::vector <double> x{}, y{};

    char *toRelease;

    toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[0].data())));
    std::string xPoints(toRelease);
    XMLString::release(&toRelease);

    toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[1].data())));
    std::string yPoints(toRelease);
    XMLString::release(&toRelease);

    toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[2].data())));
    z = gmxUtil.evaluate(toRelease);
    XMLString::release(&toRelease);

    GeoIntrusivePtr<GeoSimplePolygonBrep> poly = make_intrusive<GeoSimplePolygonBrep>(z);

    std::istringstream xSS(xPoints);
    while (!xSS.eof()) {
        std::string upToNextComma;
        getline( xSS, upToNextComma, ';' );
        x.push_back(gmxUtil.evaluate(upToNextComma.c_str()));
    }

    std::istringstream ySS(yPoints);
    while (!ySS.eof()) {
        std::string upToNextComma;
        getline( ySS, upToNextComma, ';' );
        y.push_back(gmxUtil.evaluate(upToNextComma.c_str()));
    }

    int nx = x.size();
    int ny = y.size();
  
    if (nx < 3 || ny < 3 || nx != ny) {
        THROW_EXCEPTION("Unequal number of x and y points, or less than 3\n\n"<<
         "xpoints was:\n" + xPoints + "\nypoints was:\n" + yPoints + "\n\n");
    }

    for (int i = 0; i < nx; ++i) {
        poly->addVertex(x[i], y[i]);
    }

    return const_pointer_cast(cacheShape(poly));

}

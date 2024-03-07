/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

// Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
#include "GeoModelXml/shape/MakeCons.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoCons.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>

using namespace xercesc;


GeoIntrusivePtr<RCBase> MakeCons::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    constexpr int nParams = 7; 
    static const std::array<std::string, nParams> parName{"rmin1", "rmin2", "rmax1", "rmax2", "dz", "sphi", "dphi"};
    std::array<double, nParams> p{};
    char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }
    return const_pointer_cast(cacheShape(make_intrusive<GeoCons>(p[0], p[1], p[2], p[3], p[4], p[5], p[6])));
}

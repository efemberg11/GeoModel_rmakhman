/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

// Automatically generated code from /home/hessey/prog/gmx2geo/makeshape
#include "GeoModelXml/shape/MakeTrap.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"
#include "GeoModelKernel/GeoTrap.h"

#include <array>

using namespace xercesc;


GeoIntrusivePtr<RCBase> MakeTrap::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
  constexpr int nParams = 11; 
  static const std::array<std::string, nParams> parName {"zhalflength", "theta", "phi", "dydzn", "dxdyndzn", "dxdypdzn", "angleydzn", "dydzp", "dxdyndzp", "dxdypdzp", "angleydzp"};
  std::array<double, nParams> p{};
  char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }

    return  const_pointer_cast(cacheShape(make_intrusive<GeoTrap>(p[0], p[1], p[2], p[3], 
                                                                  p[4], p[5], p[6], p[7], 
                                                                  p[8], p[9], p[10])));
}

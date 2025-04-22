/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/
//
// Jun 2021, Riccardo Maria BIANCHI, <riccardo.maria.bianchi@cern.ch>
//
#include "GeoModelXml/shape/MakeTorus.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoTorus.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>

using namespace xercesc;


GeoIntrusivePtr<RCBase> MakeTorus::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
  constexpr int nParams = 5; 
  static const std::array<std::string, nParams> parName {"rmin", "rmax", "rtor", "sphi", "dphi"};
  std::array<double, nParams> p{};
  char *toRelease;

  for (int i = 0; i < nParams; ++i) {
      toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i].data())));
      p[i] = gmxUtil.evaluate(toRelease);
      XMLString::release(&toRelease);
  }

  return const_pointer_cast(cacheShape(make_intrusive<GeoTorus>(p[0], p[1], p[2], p[3], p[4])));
}

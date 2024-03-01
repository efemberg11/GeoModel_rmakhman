/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MakeTranslation.h"
#include <xercesc/dom/DOM.hpp>
//#include <CLHEP/Geometry/Transform3D.h>
//#include <CLHEP/Geometry/Vector3D.h>
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>

#include "GeoModelKernel/GeoDefinitions.h"

MakeTranslation::MakeTranslation() {}

using namespace xercesc;
//using namespace HepGeom;

GeoTrf::Translate3D MakeTranslation::getTransform(const DOMElement *translation, GmxUtil &gmxUtil) {

const int nParams = 3; 
static const std::array<std::string, nParams> parName {"x", "y", "z"};
std::array<double, nParams> p{};
char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(translation->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }
    return GeoTrf::Translate3D(p[0], p[1], p[2]);
}

/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MakeRotation.h"
#include <xercesc/dom/DOM.hpp>
//  #include <CLHEP/Geometry/Transform3D.h>
//  #include <CLHEP/Geometry/Vector3D.h>
#include "GeoModelKernel/GeoDefinitions.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

#include <array>

MakeRotation::MakeRotation() {}

using namespace xercesc;
//  using namespace HepGeom;

GeoTrf::Rotation3D MakeRotation::getTransform(const DOMElement *rotation, GmxUtil &gmxUtil) {

const int nParams = 4; 
static const std::array<std::string, nParams> parName {"angle", "xcos", "ycos", "zcos"};
std::array<double, nParams> p{};
char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(rotation->getAttribute(XMLString::transcode(parName[i].data())));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }
    GeoTrf::Rotation3D temp;
    temp = GeoTrf::AngleAxis3D(p[0], GeoTrf::Vector3D(p[1], p[2], p[3]));

    return temp;

}

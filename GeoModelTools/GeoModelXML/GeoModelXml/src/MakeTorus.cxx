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

using namespace xercesc;

MakeTorus::MakeTorus() {}

RCBase * MakeTorus::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
const int nParams = 5; 
char const *parName[nParams] = {"rmin", "rmax", "rtor", "sphi", "dphi"};
double p[nParams];
char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i])));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }

    return new GeoTorus(p[0], p[1], p[2], p[3], p[4]);
}

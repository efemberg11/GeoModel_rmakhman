/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

/*
 * A TwistedTrap is a general twisted trapezoid: The faces perpendicular to the
 * z planes are trapezia, and their centres are not necessarily on
 * a line parallel to the z axis.
 *      pTwist  Phi twist angle
 *      pDz      Half-length along the z-axis
 *      pTheta Polar angle of the line joining the centres of the faces at -/+pDz
 *      pPhi     Azimuthal angle of the line joing the centre of the face at -pDz to the centre of the face at +pDz
 *      pDy1    Half-length along y of the face at -pDz
 *      pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
 *      pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
 *
 *      pDy2    Half-length along y of the face at +pDz
 *      pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
 *      pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
 *      pAlph   Angle with respect to the y axis from the centre of the side
 *
 */


#include "GeoModelXml/shape/MakeTwistedTrap.h"
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoTwistedTrap.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"

using namespace xercesc;

MakeTwistedTrap::MakeTwistedTrap() {}

RCBase * MakeTwistedTrap::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
const int nParams = 11; 
char const *parName[nParams] = {"twist", "dz", "theta","phi","dy1","dx1","dx2","dy2","dx3","dx4","alpha"};
double p[nParams];
char *toRelease;

    for (int i = 0; i < nParams; ++i) {
        toRelease = XMLString::transcode(element->getAttribute(XMLString::transcode(parName[i])));
        p[i] = gmxUtil.evaluate(toRelease);
        XMLString::release(&toRelease);
    }

    return new GeoTwistedTrap(p[0], p[1], p[2],p[3], p[4], p[5],p[6], p[7], p[8],p[9], p[10]);
}

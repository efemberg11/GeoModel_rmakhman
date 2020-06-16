/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoDefinitions.h"

namespace GeoTrf
{
  GeoRotation::GeoRotation(double phi, double theta, double psi)
  {
    double sinPhi   = std::sin( phi   ), cosPhi   = std::cos( phi   );
    double sinTheta = std::sin( theta ), cosTheta = std::cos( theta );
    double sinPsi   = std::sin( psi   ), cosPsi   = std::cos( psi   );
    
    this->operator()(0,0) =   cosPsi * cosPhi - cosTheta * sinPhi * sinPsi;
    this->operator()(0,1) =   cosPsi * sinPhi + cosTheta * cosPhi * sinPsi;
    this->operator()(0,2) =   sinPsi * sinTheta;
    
    this->operator()(1,0) = - sinPsi * cosPhi - cosTheta * sinPhi * cosPsi;
    this->operator()(1,1) = - sinPsi * sinPhi + cosTheta * cosPhi * cosPsi;
    this->operator()(1,2) =   cosPsi * sinTheta;
    
    this->operator()(2,0) =   sinTheta * sinPhi;
    this->operator()(2,1) = - sinTheta * cosPhi;
    this->operator()(2,2) =   cosTheta;
  }
}


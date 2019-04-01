/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "CentralScrutinizer.h"

CentralScrutinizer::CentralScrutinizer(const GeoVFullPhysVol *fullPhysVol)
  :GeoVDetectorElement(fullPhysVol)
{
}

CentralScrutinizer::~CentralScrutinizer()
{
}

int CentralScrutinizer::identify() const
{
  return 0;
}

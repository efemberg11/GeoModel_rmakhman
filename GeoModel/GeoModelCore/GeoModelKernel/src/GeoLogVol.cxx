/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoLogVol.h"

GeoLogVol::GeoLogVol (const std::string &Name, const GeoShape *Shape, const GeoMaterial *Material)
  : m_name (Name)
  , m_material (Material)
  , m_shape (Shape){}


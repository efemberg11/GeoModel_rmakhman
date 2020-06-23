/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO2G4_ExtParameterisedVolumeBuilder_H
#define GEO2G4_ExtParameterisedVolumeBuilder_H

#include "VolumeBuilder.h"
#include <string>

class Geo2G4AssemblyVolume;
class GeoMaterial;
class GeoElement;

class ExtParameterisedVolumeBuilder: public VolumeBuilder
{
public:
  ExtParameterisedVolumeBuilder(std::string n);
  ///
  G4LogicalVolume* Build(GeoPVConstLink pv) const;
  ///
  Geo2G4AssemblyVolume* BuildAssembly(GeoPVConstLink pv) const;
 private:
  /// Prints info when some PhysVol contains both types (PV and ST) of daughters
  void PrintSTInfo(std::string volume) const;
  ///
  void getMatEther() const;

  mutable bool               m_getMatEther;
  mutable const GeoMaterial* m_matEther;
  mutable const GeoMaterial* m_matHypUr;
};

#endif

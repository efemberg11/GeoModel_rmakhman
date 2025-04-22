/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO2G4_Geo2G4SolidFactory_h
#define GEO2G4_Geo2G4SolidFactory_h

#include <map>
#include <string>

class G4VSolid;
class GeoShape;
class GeoUnidentifiedShape;

class Geo2G4SolidFactory
{
public:
    

  Geo2G4SolidFactory();
  G4VSolid* Build(const GeoShape*, std::string name=std::string("")) const;
  

};

#endif

/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef MassCalculator_h
#define MassCalculator_h 1

#include "G4Types.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"


//#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
//#include "GeoModelKernel/GeoFullPhysVol.h"

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @file    MassCalculator.hh
 * @author  Marilena Bandieramonte
 * @date   0905.2022
 *
 */

class MassCalculator {
    
public:
    
    //static MassCalculator& Instance();
    MassCalculator(){};
    ~MassCalculator(){};
    MassCalculator(const MassCalculator&) = delete;
    MassCalculator& operator=(const MassCalculator&) = delete;
    
    double volume(const PVConstLink& pv);
    double exclusiveMass(const PVConstLink& pv);
    double inclusiveMass(const PVConstLink& pv);
    void calculateMass(G4LogicalVolume* logVol, G4VPhysicalVolume * physVol, std::vector<json>& jlist, double& exclusiveMass, bool writeRep);
    void iterateFromWorldMass(G4LogicalVolume* logVolume, std::vector<json>& jlist, double& inclusiveMass, double& exclusiveMass, G4String prefix, G4String material);
    void recursiveMassCalculation (G4VPhysicalVolume* worldg4, const GeoVPhysVol* /*worldgeoModel*/, std::vector<json>& jlist);
    void SetPrefixLogicalVolume(const G4String &prefixLV) { fPrefixLogicalVolume = prefixLV; }
    void SetMaterial(const G4String &material) { fMaterial = material; }
    void SetVerbosity(const int verbosity){ fVerbosityFlag = verbosity; }
    void printGeometryInfo(G4LogicalVolume* lv, G4int verbosity);

private:
    
    G4String fPrefixLogicalVolume;
    G4String fMaterial;
    const G4double fDensityThreshold = 0.02 * SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3;
    G4int    fVerbosityFlag;
    
}; // MassCalculator

#endif // MassCalculator_h 1

#ifndef FSLConfigurator_h
#define FSLConfigurator_h 1

#include "G4Types.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "GeoModelKernel/GeoPhysVol.h"

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @file    FSLConfigurator.hh
 * @author  Marilena Bandieramonte
 * @date   13.05.2022
 *
 */

class FSLConfigurator {
    
public:
    
    FSLConfigurator();
    ~FSLConfigurator(){};
    FSLConfigurator(const FSLConfigurator&) = delete;
    FSLConfigurator& operator=(const FSLConfigurator&) = delete;
    
    

private:
  
    
}; // FSLConfigurator

#endif // FSLConfigurator_h 1

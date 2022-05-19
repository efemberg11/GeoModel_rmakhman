#ifndef ClashDetector_h
#define ClashDetector_h 1

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
 * @file    ClashDetector.hh
 * @author  Marilena Bandieramonte
 * @date   0905.2022
 *
 */

class ClashDetector {
    
public:
    
    ClashDetector(G4VPhysicalVolume *world);
    ~ClashDetector(){};
    ClashDetector(const ClashDetector&) = delete;
    ClashDetector& operator=(const ClashDetector&) = delete;
    
    // Verifies if the placed volume is overlapping with existing
    // daughters or with the mother volume. Provides default resolution
    // for the number of points to be generated and verified.
    // A tolerance for the precision of the overlap check can be specified,
    // by default it is set to maximum precision.
    // Reports a maximum of overlaps errors according to parameter in input.
    // Returns true if the volume is overlapping.
    bool g4CheckOverlaps(G4VPhysicalVolume* volume, std::vector<json>& jlist, G4int res = 1000, G4double tol = 0., G4int maxErr = 1 );
    
    void recursivelyCheckOverlap(G4LogicalVolume* envelope,std::vector<json>& jlist);
    
    //Retrieves the corresponding point in global coordinates,
    //using the chain of G4VPhysicalVolumes stored in the fTree vector
    G4ThreeVector localToGlobal(G4ThreeVector& local, bool skipFirstIt);
    
    // Iterate from the volume envelope through all the daughter volumes, and look for the ancestors of
    // 'volume', populating the fTree vector of G4VPhysicalVolumes
    bool iterateFromWorld(G4LogicalVolume* envelope, G4VPhysicalVolume* volume, G4ThreeVector& local);
    
    void SetGMClashVerbosity(const G4bool flag)     { fGmclashVerbosity = flag; }
    void SetTolerance (const G4double tolerance){fTolerance=tolerance;}
    G4double GetTolerance (){return fTolerance;}

private:
  
    G4bool   fGmclashVerbosity;
    std::vector<G4VPhysicalVolume*> fTree;
    G4VPhysicalVolume *fWorld;
    G4double fTolerance;
    
}; // ClashDetector

#endif // ClashDetector_h 1

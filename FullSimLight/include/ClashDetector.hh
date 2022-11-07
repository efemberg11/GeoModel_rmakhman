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
 * @author  Evgueni Tcherniaev
 * @date    05.11.2022
 *
 */

class ClashDetector
{
public:

  ClashDetector(const G4VPhysicalVolume* world);
  ~ClashDetector() {};
  ClashDetector(const ClashDetector&) = delete;
  ClashDetector& operator=(const ClashDetector&) = delete;

  void CheckOverlapsInTree(const G4VPhysicalVolume* root = nullptr);

  void SetGMClashVerbosity(G4bool flag) { fVerbosity = flag; }
  void SetResolution(G4int resolution) { fResolution = resolution; }
  void SetTolerance(G4double tolerance) { fTolerance = tolerance; }
  G4int NumberOfChecks() const { return fNumberOfChecks; }
  G4int NumberOfClashes() const { return jlist.size(); }
  void PrintOutReport(const G4String& reportFileName) const;

private:

  // Find the path from World volume to target volume
  void setPath(const G4VPhysicalVolume* target);

  // Transform local point to the global coordinates,
  // using sequence of G4VPhysicalVolumes stored in the fPath vector
  G4ThreeVector localToGlobal(const G4ThreeVector& local);

  // Append the overlap to the JSON list.
  void reportOverlap(G4int type,
                     const G4VPhysicalVolume* volume1,
                     const G4VPhysicalVolume* volume2,
                     const G4ThreeVector& localPoint,
                     G4double size);

  // Verifies if the placed volume is overlapping with existing daughters
  // or with the mother volume.
  // Provides default resolution for the number of points to be generated
  // and verified.
  // A tolerance for the precision of the overlap check can be specified,
  // by default it is set to maximum precision.
  // Returns true if the volume is overlapping.
  bool CheckOverlaps(const G4VPhysicalVolume* volume);

private:

  std::vector<const G4VPhysicalVolume*> fPath;
  std::vector<json> jlist;
  const G4VPhysicalVolume* fWorld = nullptr;
  G4bool fVerbosity = false;
  G4int fResolution = 1000;
  G4double fTolerance = 0.;
  G4int fNumberOfChecks = 0;

}; // ClashDetector

#endif // ClashDetector_h 1

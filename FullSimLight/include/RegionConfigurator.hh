
#ifndef RegionConfigurator_h
#define RegionConfigurator_h 1

#include "G4Types.hh"
#include "G4String.hh"

#include <vector>

/**
 * @file    RegionConfigurator.hh
 * @author  Mihaly Novak
 * @date    24.11.2021
 *
 * Utility for storing and activating detector region configuration.
 *
 * This singleton stores the detector region configurations (e.g. name, list
 * of root logical volumes, secondary production cuts) that can be utilised by
 * calling its `CreateRegion` method. This method must be invoked at the end of
 * `MyDetectorConstruction::Construct` interface after the detector is built but
 * before the physics is initialised.
 */

class RegionConfigurator {

public:

  static RegionConfigurator& Instance();

  RegionConfigurator(const RegionConfigurator&) = delete;
  RegionConfigurator& operator=(const RegionConfigurator&) = delete;

  void CreateRegions(int verbose=0);

private:

  RegionConfigurator();

  struct RegionData {
    G4String                 fRegionName;
    std::vector<G4String>    fRootLVNames;
    G4double                 fGammaCut;
    G4double                 fElectronCut;
    G4double                 fPositronCut;
  };

  void InitRegionData();


private:

  std::vector<RegionData>   fRegionData;


}; // RegionConfigurator

#endif // RegionConfigurator_h 1

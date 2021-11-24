#include "RegionConfigurator.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"


RegionConfigurator& RegionConfigurator::Instance() {
  static RegionConfigurator instance;
  return instance;
}


// loop over all RegionData and create a region for each if any of the listed
// root logical volumes can be found
void RegionConfigurator::CreateRegions(int verbose) {
  std::vector<G4LogicalVolume*>* lvStore =  G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*> lvList;
  for(const RegionData& aRegionData: fRegionData) {
    G4Region*          reg = nullptr;
    const G4String regName = aRegionData.fRegionName;
    for (const G4String& rootLVName: aRegionData.fRootLVNames) {
      // collect logical volume ptrs with the given name (if any)
      lvList.clear();
      for (std::size_t ilv=0, nlv=lvStore->size(); ilv<nlv; ++ilv) {
        G4LogicalVolume* lv = (*lvStore)[ilv];
        if (lv->GetName()==rootLVName) {
          lvList.push_back(lv);
        }
      }
      std::size_t nlv = lvList.size();
      if (nlv>0) {
        // create the region if not yet
        if (reg==nullptr) {
          if (verbose>0) {
             G4cout << " ---> Creating a region with name = "  << regName << G4endl;
          }
          reg = new G4Region(regName);
          // create the production cut
          G4ProductionCuts* pcut = new G4ProductionCuts;
          pcut->SetProductionCut(aRegionData.fGammaCut   , 0);
          pcut->SetProductionCut(aRegionData.fElectronCut, 1);
          pcut->SetProductionCut(aRegionData.fPositronCut, 2);
          reg->SetProductionCuts(pcut);
        }
        if (verbose>0) {
           G4cout << "      = adding N = " << nlv
                  << " root logical volumes to it with the name of " << rootLVName
                  << G4endl;
        }
        for (std::size_t ilv=0; ilv<nlv; ++ilv) {
           reg->AddRootLogicalVolume(lvList[ilv]);
        }
      }
    }
    if (reg==nullptr && verbose>0) {
      // none of the rootLVName has been found in the G4LogicalVolume store...
      G4cout << " xxxx NO region has been created with the name = "  << regName << G4endl;
      G4cout << "      None of the given root logical volumes have been found in the G4LogicalVolumeStore. " << G4endl;
      G4cout << "      Root logical volume names: ";
      for (const G4String& rootLVName: aRegionData.fRootLVNames) {
        G4cout << rootLVName << "  ";
      }
      G4cout << G4endl;
    }
  }
}


RegionConfigurator::RegionConfigurator() {
  InitRegionData();
}


// One regin configuration, taken from Athena by using `G4PhysicsRegionConfigNew.py`
// and `G4GeomtryToolConfig.py` with a configuration setting that can produce regions
// and secondary production thresholds similar to that used in production.
void RegionConfigurator::InitRegionData() {
  fRegionData.resize(11);
  //
  //  Region: Pixel
  fRegionData[0].fRegionName  = "Pixel";
  fRegionData[0].fGammaCut    = 0.05;
  fRegionData[0].fElectronCut = 0.05;
  fRegionData[0].fPositronCut = 0.05;
  fRegionData[0].fRootLVNames = {"siLog", "siBLayLog", "dbmDiamondLog"};
  //
  //  Region: SCT
  fRegionData[1].fRegionName  = "SCT";
  fRegionData[1].fGammaCut    = 0.05;
  fRegionData[1].fElectronCut = 0.05;
  fRegionData[1].fPositronCut = 0.05;
  fRegionData[1].fRootLVNames = {"BRLSensor", "ECSensor0", "ECSensor1", "ECSensor2", "ECSensor3"};
  //
  //  Region: TRT
  fRegionData[2].fRegionName  = "TRT";
  fRegionData[2].fGammaCut    = 0.05;
  fRegionData[2].fElectronCut = 30.0;
  fRegionData[2].fPositronCut = 30.0;
  fRegionData[2].fRootLVNames = {"Gas", "GasMA"};
  //
  //  Region: TRT_Ar
  fRegionData[3].fRegionName  = "TRT_Ar";
  fRegionData[3].fGammaCut    = 0.05;
  fRegionData[3].fElectronCut = 30.0;
  fRegionData[3].fPositronCut = 30.0;
  fRegionData[3].fRootLVNames = {"Gas_Ar", "GasMA_Ar"};
  //
  //  Region: EMB
  fRegionData[4].fRegionName  = "EMB";
  fRegionData[4].fGammaCut    = 0.1;
  fRegionData[4].fElectronCut = 0.1;
  fRegionData[4].fPositronCut = 0.1;
  fRegionData[4].fRootLVNames = {"LAr::EMB::STAC"};
  //
  //  Region: EMEC
  fRegionData[5].fRegionName  = "EMEC";
  fRegionData[5].fGammaCut    = 0.1;
  fRegionData[5].fElectronCut = 0.1;
  fRegionData[5].fPositronCut = 0.1;
  fRegionData[5].fRootLVNames = {"LAr::EMEC::Mother"};
  //
  //  Region: HEC
  fRegionData[6].fRegionName  = "HEC";
  fRegionData[6].fGammaCut    = 1.0;
  fRegionData[6].fElectronCut = 1.0;
  fRegionData[6].fPositronCut = 1.0;
  fRegionData[6].fRootLVNames = {"LAr::HEC::LiquidArgon"};
  //
  //  Region: FCAL
  fRegionData[7].fRegionName  = "FCAL";
  fRegionData[7].fGammaCut    = 0.03;
  fRegionData[7].fElectronCut = 0.03;
  fRegionData[7].fPositronCut = 0.03;
  fRegionData[7].fRootLVNames = {"LAr::FCAL::LiquidArgonC"};
  //
  //  Region: DriftWall
  fRegionData[8].fRegionName  = "DriftWall";
  fRegionData[8].fGammaCut    = 1.0;
  fRegionData[8].fElectronCut = 0.05;
  fRegionData[8].fPositronCut = 0.05;
  fRegionData[8].fRootLVNames = {"MDTDriftWall"};
  //
  //  Region: DriftWall1
  fRegionData[9].fRegionName  = "DriftWall1";
  fRegionData[9].fGammaCut    = 1.0;
  fRegionData[9].fElectronCut = 1.0;
  fRegionData[9].fPositronCut = 1.0;
  fRegionData[9].fRootLVNames = {"Endplug"};
  //
  //  Region: DriftWall2
  fRegionData[10].fRegionName  = "DriftWall2";
  fRegionData[10].fGammaCut    = 1.0;
  fRegionData[10].fElectronCut = 1.0;
  fRegionData[10].fPositronCut = 1.0;
  fRegionData[10].fRootLVNames = {"SensitiveGas"};
}

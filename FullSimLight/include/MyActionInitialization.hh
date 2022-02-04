
#ifndef MyActionInitialization_h
#define MyActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"
#include "GeantinoMapsConfigurator.hh"

class MyActionInitialization : public G4VUserActionInitialization {

public:

  MyActionInitialization(bool isperformance=false);
 ~MyActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;

  void SetPerformanceModeFlag(bool val) { fIsPerformance = val; }
  void  SetSpecialScoringRegionName(const G4String& rname) { fSpecialScoringRegionName = rname; }

private:
  GeantinoMapsConfigurator* fGeantinoMapsConfig;
  bool     fIsPerformance;
  G4String fSpecialScoringRegionName;


};

#endif

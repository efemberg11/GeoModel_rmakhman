
#ifndef MyActionInitialization_h
#define MyActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
  MyActionInitialization(bool isperformance=false, bool createGeantinoMaps=false, G4String geantinoMapsFilename="geantinoMaps.root");
  virtual ~MyActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

  void SetPerformanceModeFlag(bool val) { fIsPerformance = val; }

private:
  bool fIsPerformance;
  bool fCreateGeantinoMaps;
  G4String fGeantinoMapsFilename;

};

#endif

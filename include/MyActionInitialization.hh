
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
  void SetRlimit(G4double r) { fRlimit = r; }
  void SetZlimit(G4double z) { fZlimit = z; }
  void SetXlimit(G4double x) { fXlimit = x; }
  void SetYlimit(G4double y) { fYlimit = y; }

private:
  bool fIsPerformance;
  bool fCreateGeantinoMaps;
  G4String fGeantinoMapsFilename;
  G4double fRlimit;
  G4double fZlimit;
  G4double fXlimit;
  G4double fYlimit;

};

#endif

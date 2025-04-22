#ifndef GeantinoMapsMessenger_h
#define GeantinoMapsMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GeantinoMapsConfigurator;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class GeantinoMapsMessenger : public G4UImessenger {
public:
  GeantinoMapsMessenger(GeantinoMapsConfigurator* gmaps_conf);
 ~GeantinoMapsMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
 GeantinoMapsConfigurator*  fGeantinoMapsConfig;

 G4UIdirectory*             fGMapsDirectory;
 G4UIcmdWithADoubleAndUnit* fRminCmd;
 G4UIcmdWithADoubleAndUnit* fRmaxCmd;
 G4UIcmdWithADoubleAndUnit* fZminCmd;
 G4UIcmdWithADoubleAndUnit* fZmaxCmd;
 G4UIcmdWithADoubleAndUnit* fXminCmd;
 G4UIcmdWithADoubleAndUnit* fXmaxCmd;
 G4UIcmdWithADoubleAndUnit* fYminCmd;
 G4UIcmdWithADoubleAndUnit* fYmaxCmd;
 G4UIcmdWithADouble* fEtaminCmd;
 G4UIcmdWithADouble* fEtamaxCmd;
 G4UIcmdWithAString* fVolumesListCmd;

};

#endif

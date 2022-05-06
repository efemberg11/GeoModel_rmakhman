
#ifndef FSLPrimaryGeneratorMessenger_h
#define FSLPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class FSLPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;

class FSLPrimaryGeneratorMessenger : public G4UImessenger {
public:
  FSLPrimaryGeneratorMessenger(FSLPrimaryGeneratorAction* gun);
 ~FSLPrimaryGeneratorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
 FSLPrimaryGeneratorAction*  fTheGun;

 G4UIdirectory*             fGunDirectory;
 G4UIcmdWithAnInteger*      fNumPrimaryPerEvtCmd;
 G4UIcmdWithAString*        fPrimaryTypeCmd;
 G4UIcmdWithADoubleAndUnit* fPrimaryEnergyCmd;
 G4UIcmdWith3Vector*        fPrimaryDirCmd;
 G4UIcmdWith3Vector*        fPrimaryPosCmd;

};

#endif

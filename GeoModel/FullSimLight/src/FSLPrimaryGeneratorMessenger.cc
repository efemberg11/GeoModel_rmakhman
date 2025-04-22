
#include "FSLPrimaryGeneratorMessenger.hh"

#include "FSLPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"


FSLPrimaryGeneratorMessenger::FSLPrimaryGeneratorMessenger(FSLPrimaryGeneratorAction *gun) : G4UImessenger(),
 fTheGun(gun), fGunDirectory(nullptr), fPrimaryTypeCmd(nullptr), fPrimaryEnergyCmd(nullptr), 
 fPrimaryDirCmd(nullptr),  fPrimaryPosCmd(nullptr) {
 fGunDirectory     = new G4UIdirectory("/FSLgun/");
 fGunDirectory->SetGuidance("gun control");

 fNumPrimaryPerEvtCmd = new G4UIcmdWithAnInteger("/FSLgun/primaryPerEvt",this);
 fNumPrimaryPerEvtCmd->SetGuidance("set number of primary particles per event");
 fNumPrimaryPerEvtCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 fPrimaryTypeCmd      = new G4UIcmdWithAString("/FSLgun/particle",this);
 fPrimaryTypeCmd->SetGuidance("set primary particle type");
 fPrimaryTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 fPrimaryEnergyCmd    = new G4UIcmdWithADoubleAndUnit("/FSLgun/energy",this);
 fPrimaryEnergyCmd->SetGuidance("set primary particle energy");
 fPrimaryEnergyCmd->SetParameterName("pEnergy",false);
 fPrimaryEnergyCmd->SetRange("pEnergy>0.");
 fPrimaryEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 fPrimaryDirCmd       = new G4UIcmdWith3Vector("/FSLgun/direction",this);
 fPrimaryDirCmd->SetGuidance("set primary particle direction");
 fPrimaryDirCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 fPrimaryPosCmd       = new G4UIcmdWith3Vector("/FSLgun/position",this);
 fPrimaryPosCmd->SetGuidance("set primary particle position");
 fPrimaryPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


FSLPrimaryGeneratorMessenger::~FSLPrimaryGeneratorMessenger() {
  delete fGunDirectory;
  delete fNumPrimaryPerEvtCmd;
  delete fPrimaryTypeCmd;
  delete fPrimaryEnergyCmd;
  delete fPrimaryDirCmd;
  delete fPrimaryPosCmd;
}

void FSLPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command==fNumPrimaryPerEvtCmd) {
    fTheGun->SetNumPrimaryPerEvt(fNumPrimaryPerEvtCmd->GetNewIntValue(newValue));
  }
  if (command==fPrimaryTypeCmd) {
    fTheGun->SetPrimaryName(newValue);
  }
  if (command==fPrimaryEnergyCmd) {
    fTheGun->SetPrimaryEnergy(fPrimaryEnergyCmd->GetNewDoubleValue(newValue));
  }
  if (command==fPrimaryDirCmd) {
    fTheGun->SetPrimaryDirection(fPrimaryDirCmd->GetNew3VectorValue(newValue));
  }
  if (command==fPrimaryPosCmd) {
    fTheGun->SetPrimaryPosition(fPrimaryPosCmd->GetNew3VectorValue(newValue));
  }
}

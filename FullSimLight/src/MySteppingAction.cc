
#include "MySteppingAction.hh"

#include "MyEventAction.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"


MySteppingAction::MySteppingAction(MyEventAction* evtact) : G4UserSteppingAction(), fEventAction(evtact) {}


MySteppingAction::~MySteppingAction() {}


void MySteppingAction::UserSteppingAction(const G4Step* theStep) {
  G4double edep      = theStep->GetTotalEnergyDeposit();
  G4double stepl     = theStep->GetStepLength();
  G4bool   isCharged = (theStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.);
  fEventAction->AddData(edep, stepl, isCharged);
}

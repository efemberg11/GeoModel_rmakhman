
#include "MySteppingAction.hh"

#include "MyEventAction.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4Region.hh"


MySteppingAction::MySteppingAction(MyEventAction* evtact)
: G4UserSteppingAction(), fEventAction(evtact), fScoringRegion(nullptr) {}


MySteppingAction::~MySteppingAction() {}

void MySteppingAction::SetScoringRegion(G4Region* reg) {
  fScoringRegion = reg;
  fEventAction->SetIsSpecialScoringRegion(reg!=nullptr);
}


void MySteppingAction::UserSteppingAction(const G4Step* theStep) {
  // scoring in the spcial given region if any
  const G4bool isSpecialScoring = (fScoringRegion!=nullptr && fScoringRegion==theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion());
  const G4double           edep = theStep->GetTotalEnergyDeposit();
  const G4double          stepl = theStep->GetStepLength();
  const G4bool        isCharged = (theStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.);
  fEventAction->AddData(edep, stepl, isCharged, isSpecialScoring);
}

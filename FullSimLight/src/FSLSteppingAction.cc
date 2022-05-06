
#include "FSLSteppingAction.hh"

#include "FSLEventAction.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4Region.hh"


FSLSteppingAction::FSLSteppingAction(FSLEventAction* evtact)
: G4UserSteppingAction(), fEventAction(evtact), fScoringRegion(nullptr) {}


FSLSteppingAction::~FSLSteppingAction() {}

void FSLSteppingAction::SetScoringRegion(G4Region* reg) {
  fScoringRegion = reg;
  fEventAction->SetIsSpecialScoringRegion(reg!=nullptr);
}


void FSLSteppingAction::UserSteppingAction(const G4Step* theStep) {
  // scoring in the spcial given region if any
  const G4bool isSpecialScoring = (fScoringRegion!=nullptr && fScoringRegion==theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion());
  const G4double           edep = theStep->GetTotalEnergyDeposit();
  const G4double          stepl = theStep->GetStepLength();
  const G4bool        isCharged = (theStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.);
  fEventAction->AddData(edep, stepl, isCharged, isSpecialScoring);
}

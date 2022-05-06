
#include "FSLTrackingAction.hh"

#include "G4Track.hh"
#include "G4Region.hh"
#include "FSLEventAction.hh"


FSLTrackingAction::FSLTrackingAction(FSLEventAction* evtact)
: G4UserTrackingAction(), fEventAction(evtact), fScoringRegion(nullptr) {}


FSLTrackingAction::~FSLTrackingAction() {}

void FSLTrackingAction::SetScoringRegion(G4Region* reg) {
  fScoringRegion = reg;
  fEventAction->SetIsSpecialScoringRegion(reg!=nullptr);
}


void FSLTrackingAction::PreUserTrackingAction(const G4Track* theTrack) {
  // count secondaries
  const G4bool isSpecialScoring = (fScoringRegion!=nullptr && fScoringRegion==theTrack->GetVolume()->GetLogicalVolume()->GetRegion());
  if (theTrack->GetParentID()!=0) {
    fEventAction->AddSecondaryTrack(theTrack, isSpecialScoring);
  }
}

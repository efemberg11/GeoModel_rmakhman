
#include "MyTrackingAction.hh"

#include "G4Track.hh"
#include "G4Region.hh"
#include "MyEventAction.hh"


MyTrackingAction::MyTrackingAction(MyEventAction* evtact)
: G4UserTrackingAction(), fEventAction(evtact), fScoringRegion(nullptr) {}


MyTrackingAction::~MyTrackingAction() {}

void MyTrackingAction::SetScoringRegion(G4Region* reg) {
  fScoringRegion = reg;
  fEventAction->SetIsSpecialScoringRegion(reg!=nullptr);
}


void MyTrackingAction::PreUserTrackingAction(const G4Track* theTrack) {
  // count secondaries
  const G4bool isSpecialScoring = (fScoringRegion!=nullptr && fScoringRegion==theTrack->GetVolume()->GetLogicalVolume()->GetRegion());
  if (theTrack->GetParentID()!=0) {
    fEventAction->AddSecondaryTrack(theTrack, isSpecialScoring);
  }
}

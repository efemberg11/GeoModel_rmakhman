
#include "MyTrackingAction.hh"

#include "G4Track.hh"
#include "MyEventAction.hh"


MyTrackingAction::MyTrackingAction(MyEventAction* evtact) : G4UserTrackingAction(), fEventAction(evtact) {}


MyTrackingAction::~MyTrackingAction() {}


void MyTrackingAction::PreUserTrackingAction(const G4Track* theTrack) {
  // count secondaries
  if (theTrack->GetParentID()!=0) {
    fEventAction->AddSecondaryTrack(theTrack);
  }
}

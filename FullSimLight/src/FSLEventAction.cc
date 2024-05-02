
#include "FSLEventAction.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
//#include "G4Geantino.hh"
//#include "G4ChargedGeantino.hh"

#include "G4ThreeVector.hh"
#include "G4PrimaryVertex.hh"

#include "FSLRunAction.hh"
#include "FSLSteppingAction.hh"
#include "FSLRun.hh"
#include "FSLPrimaryGeneratorAction.hh"


FSLEventAction::FSLEventAction() : G4UserEventAction(), fIsSpecialScoring(false) {
  fEventData.Clear();
  if (fIsSpecialScoring) {
    fEventDataSpecialRegion.Clear();
  }
}


FSLEventAction::~FSLEventAction() { }


void FSLEventAction::BeginOfEventAction(const G4Event* evt) {
  G4cout<<"FullSimLight, processing event: "<<evt->GetEventID()<<G4endl;
  fEventData.Clear();
  if (fIsSpecialScoring) {
    fEventDataSpecialRegion.Clear();
  }
}


void FSLEventAction::EndOfEventAction(const G4Event*) {
  //get the Run and add the data collected during the simulation of the event that has been completed
  FSLRun* run = static_cast<FSLRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->FillPerEvent(fEventData, false);
  if (fIsSpecialScoring) {
    run->FillPerEvent(fEventDataSpecialRegion, true);
  }
}


// called from the SteppingAction
void FSLEventAction::AddData(G4double edep, G4double length, G4bool ischarged, G4bool isspecial) {
  fEventData.fEdep += edep;
  if (ischarged) {
    fEventData.fTrackLCh    += length;
    fEventData.fChargedStep += 1.;
  } else {
    fEventData.fTrackLNe    += length;
    fEventData.fNeutralStep += 1.;
  }
  // return if this step was not done in the special region
  if (!isspecial) {
    return;
  }
  fEventDataSpecialRegion.fEdep += edep;
  if (ischarged) {
    fEventDataSpecialRegion.fTrackLCh    += length;
    fEventDataSpecialRegion.fChargedStep += 1.;
  } else {
    fEventDataSpecialRegion.fTrackLNe    += length;
    fEventDataSpecialRegion.fNeutralStep += 1.;
  }
}


// called from the TrackingAction
void FSLEventAction::AddSecondaryTrack(const G4Track* track, G4bool isspecial) {
  const G4ParticleDefinition* pdf = track->GetDefinition();
  if (pdf==G4Gamma::Gamma()) {
    fEventData.fNGamma += 1.;
  } else if (pdf==G4Electron::Electron()) {
    fEventData.fNElec += 1.;
  } else if (pdf==G4Positron::Positron()) {
    fEventData.fNPosit += 1.;
  }
  // return if this step was not done in the special region
  if (!isspecial) {
    return;
  }
  if (pdf==G4Gamma::Gamma()) {
    fEventDataSpecialRegion.fNGamma += 1.;
  } else if (pdf==G4Electron::Electron()) {
    fEventDataSpecialRegion.fNElec += 1.;
  } else if (pdf==G4Positron::Positron()) {
    fEventDataSpecialRegion.fNPosit += 1.;
  }
}

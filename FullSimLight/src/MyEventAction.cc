
#include "MyEventAction.hh"

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

#include "MyRunAction.hh"
#include "MySteppingAction.hh"
#include "MyRun.hh"
#include "MyPrimaryGeneratorAction.hh"


MyEventAction::MyEventAction() : G4UserEventAction() {
  fEventData.Clear();
}


MyEventAction::~MyEventAction() { }


void MyEventAction::BeginOfEventAction(const G4Event*) {
  fEventData.Clear();
}


void MyEventAction::EndOfEventAction(const G4Event*) {
  //get the Run and add the data collected during the simulation of the event that has been completed
  MyRun* run = static_cast<MyRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->FillPerEvent(fEventData);
}


// called from the SteppingAction
void MyEventAction::AddData(G4double edep, G4double length, G4bool ischarged) {
  fEventData.fEdep += edep;
  if (ischarged) {
    fEventData.fTrackLCh    += length;
    fEventData.fChargedStep += 1.;
  } else {
    fEventData.fTrackLNe    += length;
    fEventData.fNeutralStep += 1.;
  }
}


// called from the TrackingAction
void MyEventAction::AddSecondaryTrack(const G4Track* track) {
    const G4ParticleDefinition* pdf = track->GetDefinition();
    if (pdf==G4Gamma::Gamma()) {
        fEventData.fNGamma += 1.;
    } else if (pdf==G4Electron::Electron()) {
        fEventData.fNElec += 1.;
    } else if (pdf==G4Positron::Positron()) {
        fEventData.fNPosit += 1.;
    }
}

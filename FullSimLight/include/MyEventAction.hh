
#ifndef MyEventAction_h
#define MyEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "MyEventData.hh"

class G4Event;
class G4Track;

class MyEventAction: public G4UserEventAction {

public:

   MyEventAction();
  ~MyEventAction() override;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  void AddData(G4double edep, G4double length, G4bool ischarged);
  void AddSecondaryTrack(const G4Track* track);

private:

  MyEventData fEventData;

};

#endif

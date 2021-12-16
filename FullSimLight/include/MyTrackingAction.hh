
#ifndef MyTrackingAction_h
#define MyTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
class MyEventAction;
class G4Region;

class MyTrackingAction : public G4UserTrackingAction {

public:

   MyTrackingAction(MyEventAction*);
  ~MyTrackingAction() override;

   void SetScoringRegion(G4Region* reg);

   void PreUserTrackingAction(const G4Track*) override;

private:

  MyEventAction* fEventAction;
  G4Region*      fScoringRegion;

};

#endif

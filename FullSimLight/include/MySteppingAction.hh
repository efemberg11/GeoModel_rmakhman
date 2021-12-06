
#ifndef MySteppingAction_h
#define MySteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class MyEventAction;
class G4Step;
class G4Region;

class MySteppingAction : public G4UserSteppingAction {

public:

  MySteppingAction(MyEventAction*);
 ~MySteppingAction() override;

  void UserSteppingAction(const G4Step*) override;

  void SetScoringRegion(G4Region* reg);

private:

  MyEventAction*   fEventAction;
  G4Region*        fScoringRegion;

};

#endif

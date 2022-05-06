
#ifndef FSLSteppingAction_h
#define FSLSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class FSLEventAction;
class G4Step;
class G4Region;

class FSLSteppingAction : public G4UserSteppingAction {

public:

  FSLSteppingAction(FSLEventAction*);
 ~FSLSteppingAction() override;

  void UserSteppingAction(const G4Step*) override;

  void SetScoringRegion(G4Region* reg);

private:

  FSLEventAction*   fEventAction;
  G4Region*        fScoringRegion;

};

#endif

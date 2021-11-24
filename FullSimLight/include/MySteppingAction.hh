
#ifndef MySteppingAction_h
#define MySteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class MyEventAction;
class G4Step;

class MySteppingAction : public G4UserSteppingAction {

public:

  MySteppingAction(MyEventAction*);
 ~MySteppingAction() override;

  void UserSteppingAction(const G4Step*) override;

private:

  MyEventAction*   fEventAction;

};

#endif

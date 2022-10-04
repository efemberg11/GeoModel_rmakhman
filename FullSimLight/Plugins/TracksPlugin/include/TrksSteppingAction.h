#ifndef TRKSUSERACTION_HH
#define TRKSUSERACTION_HH


#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class G4Step;

class TrksSteppingAction: public G4UserSteppingAction
{
public:
    TrksSteppingAction();
    ~TrksSteppingAction();

    void UserSteppingAction(const G4Step* /*aStep*/);

};

#endif // TRKSUSERACTION_HH
#ifndef __TRKSTRACKINGACTION_HH__
#define __TRKSTRACKINGACTION_HH__

#include "G4UserTrackingAction.hh"

class TrksTrackingAction: public G4UserTrackingAction
{
public:
    TrksTrackingAction();
    ~TrksTrackingAction();

    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);

};


#endif //__TEKSTRACKINGACTION_HH__

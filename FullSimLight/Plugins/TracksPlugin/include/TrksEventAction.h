#ifndef __TrksEventAction_hh__
#define __TrksEventAction_hh__

#include "G4UserEventAction.hh"
#include "G4UnitsTable.hh"

#include <vector>
#include <map>

struct stepData
{
    float x;
    float y;
    float z;
};

class TrksEventAction: public G4UserEventAction
{
public:

    TrksEventAction();
    ~TrksEventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

    static G4ThreadLocal std::map<int, int> gPDGs;
    static G4ThreadLocal std::map<int, std::vector<stepData>> trksInfoMap;


};


#endif //__TrksEventAction_hh__
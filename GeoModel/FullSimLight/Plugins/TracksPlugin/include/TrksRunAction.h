#ifndef TRKSRUNACTION_HH
#define TRKSRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4UnitsTable.hh"

#include <vector>
#include <mutex>

class TrksRunAction: public G4UserRunAction
{
public:

    TrksRunAction();
    ~TrksRunAction();

    void BeginOfRunAction(const G4Run* /*aRun*/);
    void EndOfRunAction(const G4Run* /*aRun*/);

    static std::map<G4int, G4int> fgTrkNums;
    static std::mutex fgTrkNumsMutex;

};



#endif // TRKSRUNACTION_HH
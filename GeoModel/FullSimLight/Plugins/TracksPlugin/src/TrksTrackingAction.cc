#include "TrksTrackingAction.h"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "TrksRunAction.h"
#include "G4RunManager.hh"
#include "G4StepPoint.hh"
#include "TrksEventAction.h"

#include "H5Cpp.h"

#include <mutex>
#include <string>

using std::string;
using std::to_string;

TrksTrackingAction::TrksTrackingAction():G4UserTrackingAction()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksTrackingAction::~TrksTrackingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    int trkNum = aTrack->GetTrackID();

    std::map<int, std::vector<stepData>> &trksInfoMap = TrksEventAction::trksInfoMap;

    stepData tmpStepData;
    tmpStepData.x = aTrack->GetPosition().getX();
    tmpStepData.y = aTrack->GetPosition().getY();
    tmpStepData.z = aTrack->GetPosition().getZ();

    if (trksInfoMap.find(trkNum) != trksInfoMap.end()) //track ID already in the map
    {
        std::vector<stepData> &v = trksInfoMap[trkNum];
        v.push_back(tmpStepData);
    }
    else //There is no such track with given track ID
    {   
        std::vector<stepData> newV = {tmpStepData};
        trksInfoMap[trkNum] = newV;
    }

    G4int particlePDG = aTrack->GetParticleDefinition()->GetPDGEncoding();
    TrksEventAction::gPDGs[trkNum] = particlePDG;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksTrackingAction::PostUserTrackingAction(const G4Track* /*aTrack*/)
{

}
#include "TrksSteppingAction.h"
#include "G4Step.hh"
//#include "G4Track.hh"
//#include "G4UnitsTable.hh"
//#include "G4SystemOfUnits.hh"
#include "TrksEventAction.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksSteppingAction::TrksSteppingAction(): G4UserSteppingAction()
{

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksSteppingAction::~TrksSteppingAction()
{
    // ---
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksSteppingAction::UserSteppingAction(const G4Step * aStep)
{

    int trkNum = aStep->GetTrack()->GetTrackID();

    stepData tmpStepData;
    tmpStepData.x = aStep->GetPostStepPoint()->GetPosition().getX();
    tmpStepData.y = aStep->GetPostStepPoint()->GetPosition().getY();
    tmpStepData.z = aStep->GetPostStepPoint()->GetPosition().getZ();

    std::map<int, std::vector<stepData>> &trksInfoMap = TrksEventAction::trksInfoMap;

    trksInfoMap[trkNum].push_back(tmpStepData);

}

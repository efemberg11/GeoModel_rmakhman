#include "TrksEventAction.h"
#include "TrksHDF5Factory.h"
#include "G4Event.hh"
#include "TrksRunAction.h"

#include <string>

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal std::map<int, int> TrksEventAction::gPDGs;
G4ThreadLocal std::map<int, std::vector<stepData>> TrksEventAction::trksInfoMap;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksEventAction::TrksEventAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


TrksEventAction::~TrksEventAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksEventAction::BeginOfEventAction(const G4Event*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksEventAction::EndOfEventAction(const G4Event* anEvent)
{
    G4int eventNum = anEvent->GetEventID();

    TrksHDF5Factory* hdf5Factory = TrksHDF5Factory::GetInstance();

    std::vector<int> gPDGsVec;
    for (auto el : gPDGs)
    {
        gPDGsVec.push_back(el.second);
    }

    H5::H5File* file = hdf5Factory->GetFile();

    // write trachs PDG into outout file
    hsize_t trkNums = gPDGsVec.size();
    H5::DataSpace dataSpacePDG(1, &trkNums);
    std::string dataSetNamePDG = "event" + std::to_string(eventNum) + "PDGs";
    hdf5Factory->gWriteFileMutex.lock();
    H5::DataSet *dataSetPDG = new H5::DataSet(file->createDataSet(dataSetNamePDG,
                               H5::PredType::NATIVE_INT, dataSpacePDG));
    dataSetPDG->write(gPDGsVec.data(), H5::PredType::NATIVE_INT);
    hdf5Factory->gWriteFileMutex.unlock();
    gPDGs.clear();
    delete dataSetPDG;


    // write step points of each track into output file
    H5::CompType stepDataType(sizeof(stepData));
    stepDataType.insertMember("x", HOFFSET(stepData, x), H5::PredType::NATIVE_FLOAT);
    stepDataType.insertMember("y", HOFFSET(stepData, y), H5::PredType::NATIVE_FLOAT);
    stepDataType.insertMember("z", HOFFSET(stepData, z), H5::PredType::NATIVE_FLOAT);

    for( auto itPair : trksInfoMap)
    {
        const int trkNum = itPair.first;
        const std::vector<stepData> &vevcData = itPair.second;

        hsize_t numOfStepCoors = vevcData.size();
        H5::DataSpace space(1, &numOfStepCoors, nullptr);
        string datasetName = string("event")+to_string(eventNum)
                                                     + "-trk"+to_string(trkNum);
                                                     
        hdf5Factory->gWriteFileMutex.lock();
        H5::DataSet* dataset =  new H5::DataSet(file->createDataSet( 
                                                datasetName, stepDataType, space));
        dataset->write(vevcData.data(), stepDataType);
        hdf5Factory->gWriteFileMutex.unlock();
        delete dataset;
    }

    trksInfoMap.clear();

    TrksRunAction::fgTrkNumsMutex.lock();
    TrksRunAction::fgTrkNums[eventNum] = trkNums;
    TrksRunAction::fgTrkNumsMutex.unlock();
}

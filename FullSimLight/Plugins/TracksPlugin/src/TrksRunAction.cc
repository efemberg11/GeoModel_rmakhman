#include "TrksRunAction.h"
#include "TrksHDF5Factory.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::map<G4int, G4int> TrksRunAction::fgTrkNums;
std::mutex TrksRunAction::fgTrkNumsMutex;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksRunAction::TrksRunAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksRunAction::~TrksRunAction()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksRunAction::BeginOfRunAction(const G4Run* /*aRun*/)
{
    if  (IsMaster())
    {
        TrksHDF5Factory* hdf5Factory = TrksHDF5Factory::GetInstance();
        hdf5Factory->OpenFile("Tracks_data.h5");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksRunAction::EndOfRunAction(const G4Run* /*aRun*/)
{

    if (IsMaster())
    {

        TrksHDF5Factory* hdf5Factory = TrksHDF5Factory::GetInstance();
        // hdf5Factory->gWriteFileMutex.lock();

        std::vector<int> trkNumsVec;

        for(auto el : fgTrkNums)
        {
            trkNumsVec.push_back(el.second);
        }

        H5::H5File* file = hdf5Factory->GetFile();

        hsize_t eventNums = trkNumsVec.size();
        H5::DataSpace dataSpace(1, &eventNums);
        H5::DataSet* dataSet = new H5::DataSet(file->createDataSet("eventNums",
                                            H5::PredType::NATIVE_INT, dataSpace));
        dataSet->write(trkNumsVec.data(), H5::PredType::NATIVE_INT);
        // hdf5Factory->gWriteFileMutex.unlock();

        delete dataSet;

        file->close();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

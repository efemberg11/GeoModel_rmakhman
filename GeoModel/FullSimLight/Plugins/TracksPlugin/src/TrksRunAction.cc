#include "TrksRunAction.h"
#include "TrksHDF5Factory.h"

#include <string>
#include <filesystem>
#include <set>

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
        using namespace std;
        namespace fs = std::filesystem;

        set<string> fileNames;
        string outputFileName; 

        fs::path pwdPath = fs::current_path();
        for (const auto & entry : fs::directory_iterator(pwdPath))
        {
            fileNames.insert(entry.path());
        }

        for (int i=0; true; ++i)
        {
            outputFileName = "Tracks_data";
            outputFileName += i == 0 ? "" : string("_") + to_string(i);
            outputFileName+=".h5";

            if (fileNames.find( string(pwdPath) + "/" + outputFileName) == fileNames.end())
            {

                break;
            }

            cout << "A file " << outputFileName << " already exists in the working directory. " 
                 << "Looking for another file name.\n";
        }


        TrksHDF5Factory* hdf5Factory = TrksHDF5Factory::GetInstance();
        hdf5Factory->OpenFile(outputFileName.c_str());
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

        G4String outputFileName = file->getFileName();

        file->close();

        G4cout << "The file " << outputFileName << " has been created." << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

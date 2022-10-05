#include "TrksHDF5Factory.h"

#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksHDF5Factory* TrksHDF5Factory::fgInstance = nullptr;
H5::H5File* TrksHDF5Factory::fgFile;
std::mutex     TrksHDF5Factory::gWriteFileMutex;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksHDF5Factory::TrksHDF5Factory()
{
    fgInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksHDF5Factory::~TrksHDF5Factory()
{

    if (fgFile)
    {
        fgFile->close();
        delete fgFile;
        fgFile = nullptr;
    }

    if (fgInstance)
    {
        fgInstance = nullptr;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrksHDF5Factory* TrksHDF5Factory::GetInstance()
{
    if (fgInstance) return fgInstance;
    else return new TrksHDF5Factory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrksHDF5Factory::OpenFile(const char* name)
{
    fgFile = new H5::H5File( name, H5F_ACC_TRUNC );
}


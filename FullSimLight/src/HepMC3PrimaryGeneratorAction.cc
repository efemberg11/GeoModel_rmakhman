#include "HepMC3PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "HepMC3G4AsciiReader.hh"
#include "G4AutoLock.hh"

namespace {
 G4Mutex mutex_gen = G4MUTEX_INITIALIZER;
 
}


HepMC3PrimaryGeneratorAction::HepMC3PrimaryGeneratorAction(std::string file_path,std::string file_type) : G4VUserPrimaryGeneratorAction()
{
    HEPMC3_generator=HepMC3G4AsciiReader::GetInstance(file_path, file_type);
}
HepMC3PrimaryGeneratorAction::~HepMC3PrimaryGeneratorAction()
{
delete HEPMC3_generator;
}

void HepMC3PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)

{
    G4cout<<"HepMC3PrimaryGeneratorAction::GeneratePrimaries going to lock"<<G4endl;
    G4AutoLock lock(&mutex_gen);
    G4cout<<"HepMC3PrimaryGeneratorAction::GeneratePrimaries MUTEX IS MINE"<<G4endl;
    HEPMC3_generator->GeneratePrimaryVertex(anEvent);
    lock.unlock();
    G4cout<<"HepMC3PrimaryGeneratorAction::GeneratePrimaries MUTEX unlocked"<<G4endl;
}

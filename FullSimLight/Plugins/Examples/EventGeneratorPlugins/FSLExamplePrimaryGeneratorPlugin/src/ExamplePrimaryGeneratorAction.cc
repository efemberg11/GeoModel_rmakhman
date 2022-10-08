#include "ExamplePrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExamplePrimaryGeneratorAction::ExamplePrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun->SetParticleDefinition(
               particleTable->FindParticle(particleName="proton"));
  fParticleGun->SetParticleEnergy(70.5*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.02 * m, 0.0*m, 0));
  fParticleGun->SetParticlePolarization(G4ThreeVector(0.0, 0.0, 1));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExamplePrimaryGeneratorAction::~ExamplePrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExamplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double x = (G4UniformRand()-0.5)/8;
  G4double y = (G4UniformRand()-0.5)/8;

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,1));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

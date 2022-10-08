#ifndef _ExamplePrimaryGeneratorAction_HH_
#define _ExamplePrimaryGeneratorAction_HH_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "globals.hh"

class G4Event;
class G4ParticleGun;

class ExamplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    ExamplePrimaryGeneratorAction();
   ~ExamplePrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event* anEvent);

  private:

    G4ParticleGun* fParticleGun;
};

#endif

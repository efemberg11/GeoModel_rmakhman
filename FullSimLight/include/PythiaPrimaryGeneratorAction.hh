#ifndef PythiaPrimaryGeneratorAction_h
#define PythiaPrimaryGeneratorAction_h 1

#if USE_PYTHIA

#include <Pythia8/Pythia.h>
#include <G4VUserPrimaryGeneratorAction.hh>

class G4Event;

bool use_pythia();
void set_pythia_config(const char*);

class PythiaPrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {

public:
  PythiaPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*) override;

private:
  Pythia8::Pythia pythia;
};

#endif
#endif

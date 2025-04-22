#ifndef PythiaPrimaryGeneratorAction_h
#define PythiaPrimaryGeneratorAction_h 1

#if USE_PYTHIA

#include <Pythia8/Pythia.h>
#include <G4VUserPrimaryGeneratorAction.hh>

class G4Event;

bool use_pythia();
void set_pythia_config(const char*);
const char* get_pythia_config();

class PythiaPrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {

public:
  // seeding < 0 : fixed 1234 for all threads (used for performance measurements)
  // seeding = 0 : re-seeding by the event ID in order to guarantee reproducibilty
  PythiaPrimaryGeneratorAction(int seeding = -1);

  void GeneratePrimaries(G4Event*) override;

private:

  int             fSeeding;
  Pythia8::Pythia fPythia;

};

#endif
#endif

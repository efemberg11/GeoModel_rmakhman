#ifndef HepMC3PrimaryGeneratorAction_h
#define HepMC3PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "HepMC3G4AsciiReader.hh"

class G4Event;
class G4VPrimaryGenerator;

class HepMC3PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction

{

public:
    HepMC3PrimaryGeneratorAction(std::string file_path,std::string file_type);
    ~HepMC3PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event* anEvent);

private:
    HepMC3G4AsciiReader* HEPMC3_generator;


    
};

#endif

//
// This file defines a DummyRunAction plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserRunActionPlugin.h"
#include "G4UserRunAction.hh"
#include <iostream>

class DummyRunAction:public G4UserRunAction {
public:
    
    // Constructor:
    DummyRunAction();
    
    // Destructor:
    ~DummyRunAction();
    
    void   BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;
    
};

DummyRunAction::DummyRunAction():G4UserRunAction() {
    std::cout << "HELLO from DummyRunAction" << std::endl;
}

DummyRunAction::~DummyRunAction() {
    std::cout << "GOODBYE from DummyRunAction" << std::endl;
}

void DummyRunAction::BeginOfRunAction(const G4Run*){
    
    std::cout<<"DummyRunAction implementation of the BeginOfRunAction method"<<std::endl;
}

void DummyRunAction::EndOfRunAction(const G4Run*){
    
    std::cout<<"DummyRunAction implementation of the EndOfRunAction method"<<std::endl;
}

//----------------------------------------------------------------------//



class DummyRunActionPlugin:public FSLUserRunActionPlugin {
    
public:
    
    DummyRunActionPlugin();
    virtual G4UserRunAction *getUserRunAction() const final override;
};

DummyRunActionPlugin::DummyRunActionPlugin()
{
    std::cout<<"HELLO from the DummyRunActionPlugin"<<std::endl;
}

G4UserRunAction *DummyRunActionPlugin::getUserRunAction() const {
  return new DummyRunAction();
}


extern "C" DummyRunActionPlugin *createDummyRunActionPlugin() {
    return new DummyRunActionPlugin();
}


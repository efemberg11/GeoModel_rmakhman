//
// This file defines a DummyEventAction plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserEventActionPlugin.h"
#include "G4UserEventAction.hh"
#include <iostream>

class DummyEventAction:public G4UserEventAction {
public:
    
    // Constructor:
    DummyEventAction();
    
    // Destructor:
    ~DummyEventAction();
    
    void BeginOfEventAction(const G4Event* evt) override;
    void EndOfEventAction(const G4Event* evt) override;
    
};

DummyEventAction::DummyEventAction():G4UserEventAction() {
    std::cout << "HELLO from DummyEventAction" << std::endl;
}

DummyEventAction::~DummyEventAction() {
    std::cout << "GOODBYE from DummyEventAction" << std::endl;
}

void DummyEventAction::BeginOfEventAction(const G4Event* /*evt*/) {
    
    std::cout<<"DummyEventAction implementation of the BeginOfEventAction method"<<std::endl;
}

void DummyEventAction::EndOfEventAction(const G4Event* /*evt*/)     {
    
    std::cout<<"DummyEventAction implementation of the EndOfEventAction method"<<std::endl;
}

//----------------------------------------------------------------------//



class DummyEventActionPlugin:public FSLUserEventActionPlugin {
    
public:
    
    DummyEventActionPlugin();
    virtual  G4UserEventAction *getUserEventAction() const final override;
};

DummyEventActionPlugin::DummyEventActionPlugin()
{
    std::cout<<"HELLO from the DummyEventActionPlugin"<<std::endl;
}

 G4UserEventAction *DummyEventActionPlugin::getUserEventAction() const {
  return new DummyEventAction();
}


extern "C" DummyEventActionPlugin *createDummyEventActionPlugin() {
    return new DummyEventActionPlugin();
}


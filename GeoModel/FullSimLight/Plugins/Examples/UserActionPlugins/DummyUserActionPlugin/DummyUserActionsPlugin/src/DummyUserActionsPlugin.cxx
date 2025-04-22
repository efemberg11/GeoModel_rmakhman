//
// This file defines a DummyUserActionPlugin plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/UserActionPlugin.h"
#include "G4VUserActionInitialization.hh"
#include <iostream>

class DummyUserActions:public G4VUserActionInitialization {
public:
    
    // Constructor:
    DummyUserActions();
    
    // Destructor:
    ~DummyUserActions();
    
    void BuildForMaster() const override;
    void Build() const override;
    
};

DummyUserActions::DummyUserActions():G4VUserActionInitialization() {
    std::cout << "HELLO from DummyUserActions" << std::endl;
}

DummyUserActions::~DummyUserActions() {
    std::cout << "GOODBYE from DummyUserActions" << std::endl;
}

void DummyUserActions::BuildForMaster() const{
    
    std::cout<<"DummyUserActions implementation of the BuildForMaster method"<<std::endl;
}

void DummyUserActions::Build() const{
    
    std::cout<<"DummyUserActions implementation of the Build method"<<std::endl;
}

//----------------------------------------------------------------------//



class DummyUserActionsPlugin:public UserActionPlugin {
    
public:
    
    DummyUserActionsPlugin();
    virtual const G4VUserActionInitialization *getUserActionInitialization() const final override;
};

DummyUserActionsPlugin::DummyUserActionsPlugin()
{
    std::cout<<"Hello from the DummyUserActionsPlugin"<<std::endl;
}

const G4VUserActionInitialization *DummyUserActionsPlugin::getUserActionInitialization() const {
  return new DummyUserActions();
}


extern "C" DummyUserActionsPlugin *createDummyUserActionsPlugin() {
    return new DummyUserActionsPlugin();
}


//
// This file defines a DummySteppingAction plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserSteppingActionPlugin.h"
#include "G4UserSteppingAction.hh"
#include <iostream>

class DummySteppingAction:public G4UserSteppingAction {
public:
    
    // Constructor:
    DummySteppingAction();
    
    // Destructor:
    ~DummySteppingAction();
    
    void UserSteppingAction(const G4Step*) override;
    
};

DummySteppingAction::DummySteppingAction():G4UserSteppingAction() {
    std::cout << "HELLO from DummySteppingAction" << std::endl;
}

DummySteppingAction::~DummySteppingAction() {
    std::cout << "GOODBYE from DummySteppingAction" << std::endl;
}

void DummySteppingAction::UserSteppingAction(const G4Step*){
    
    std::cout<<"DummySteppingAction implementation of the UserSteppingAction method"<<std::endl;
}

//----------------------------------------------------------------------//



class DummySteppingActionPlugin:public FSLUserSteppingActionPlugin {
    
public:
    
    DummySteppingActionPlugin();
    virtual G4UserSteppingAction *getUserSteppingAction() const final override;
};

DummySteppingActionPlugin::DummySteppingActionPlugin()
{
    std::cout<<"HELLO from the DummySteppingActionPlugin"<<std::endl;
}

G4UserSteppingAction *DummySteppingActionPlugin::getUserSteppingAction() const {
  return new DummySteppingAction();
}


extern "C" DummySteppingActionPlugin *createDummySteppingActionPlugin() {
    return new DummySteppingActionPlugin();
}


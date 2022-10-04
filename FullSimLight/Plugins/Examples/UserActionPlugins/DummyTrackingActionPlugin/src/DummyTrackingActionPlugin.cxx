//
// This file defines a DummyTrackingAction plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserTrackingActionPlugin.h"
#include "G4UserTrackingAction.hh"
#include <iostream>

class DummyTrackingAction:public G4UserTrackingAction {
public:
    
    // Constructor:
    DummyTrackingAction();
    
    // Destructor:
    ~DummyTrackingAction();
    
    void PreUserTrackingAction(const G4Track*) override;
    
};

DummyTrackingAction::DummyTrackingAction():G4UserTrackingAction() {
    std::cout << "HELLO from DummyTrackingAction" << std::endl;
}

DummyTrackingAction::~DummyTrackingAction() {
    std::cout << "GOODBYE from DummyTrackingAction" << std::endl;
}

void DummyTrackingAction::PreUserTrackingAction(const G4Track*) {
    
    std::cout<<"DummyTrackingAction implementation of the PreUserTrackingAction method"<<std::endl;
}



//----------------------------------------------------------------------//



class DummyTrackingActionPlugin:public FSLUserTrackingActionPlugin {
    
public:
    
    DummyTrackingActionPlugin();
    virtual G4UserTrackingAction *getUserTrackingAction() const final override;
};

DummyTrackingActionPlugin::DummyTrackingActionPlugin()
{
    std::cout<<"HELLO from the DummyTrackingActionPlugin"<<std::endl;
}

G4UserTrackingAction *DummyTrackingActionPlugin::getUserTrackingAction() const {
  return new DummyTrackingAction();
}


extern "C" DummyTrackingActionPlugin *createDummyTrackingActionPlugin() {
    return new DummyTrackingActionPlugin();
}


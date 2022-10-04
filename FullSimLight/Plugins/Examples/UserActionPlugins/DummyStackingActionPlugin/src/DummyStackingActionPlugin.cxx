//
// This file defines a DummyStackingAction plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserStackingActionPlugin.h"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include <iostream>

class DummyStackingAction:public G4UserStackingAction {
public:
    
    // Constructor:
    DummyStackingAction();
    
    // Destructor:
    ~DummyStackingAction();
    
    
    virtual G4ClassificationOfNewTrack
    ClassifyNewTrack(const G4Track* aTrack);
    
    virtual void NewStage();
    //
    //    This method is called by G4StackManager when the urgentStack
    //  becomes empty and contents in the waitingStack are transtered
    //  to the urgentStack.
    //    Note that this method is not called at the begining of each
    //  event, but "PrepareNewEvent" is called.
    
    virtual void PrepareNewEvent();
    //
    //    This method is called by G4StackManager at the begining of
    //  each event.
    
};

DummyStackingAction::DummyStackingAction():G4UserStackingAction() {
    std::cout << "HELLO from DummyStackingAction" << std::endl;
}

DummyStackingAction::~DummyStackingAction() {
    std::cout << "GOODBYE from DummyStackingAction" << std::endl;
}

G4ClassificationOfNewTrack DummyStackingAction::ClassifyNewTrack(const G4Track* /*aTrack*/){
    
    std::cout<<"DummyStackingAction implementation of the ClassifyNewTrack method"<<std::endl;
}

void DummyStackingAction::NewStage(){
    
    std::cout<<"DummyStackingAction implementation of the NewStage method"<<std::endl;
    
}

void DummyStackingAction::PrepareNewEvent(){
    
    std::cout<<"DummyStackingAction implementation of the PrepareNewEvent method"<<std::endl;
    
}

//----------------------------------------------------------------------//



class DummyStackingActionPlugin:public FSLUserStackingActionPlugin {
    
public:
    
    DummyStackingActionPlugin();
    virtual G4UserStackingAction *getUserStackingAction() const final override;
};

DummyStackingActionPlugin::DummyStackingActionPlugin()
{
    std::cout<<"HELLO from the DummyStackingActionPlugin"<<std::endl;
}

 G4UserStackingAction *DummyStackingActionPlugin::getUserStackingAction() const {
    return new DummyStackingAction();
}


extern "C" DummyStackingActionPlugin *createDummyStackingActionPlugin() {
    return new DummyStackingActionPlugin();
}


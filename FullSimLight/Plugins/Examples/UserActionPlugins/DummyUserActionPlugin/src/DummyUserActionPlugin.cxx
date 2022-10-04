//
// This file defines a DummyUserActionPlugin plugin as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLUserActionPlugin.h"
#include "G4VUserActionInitialization.hh"
#include <iostream>


class DummyUserActionPlugin:public FSLUserActionPlugin {
    
public:
    
    DummyUserActionPlugin();
    ~DummyUserActionPlugin();

};

DummyUserActionPlugin::DummyUserActionPlugin() {
  std::cout << "HELLO from DummyUserActionPlugin" << std::endl;
}

DummyUserActionPlugin::~DummyUserActionPlugin() {
  std::cout << "GOODBYE from DummyUserActionPlugin" << std::endl;
}

extern "C" DummyUserActionPlugin *createDummyUserActionPlugin() {
  return new DummyUserActionPlugin;
}

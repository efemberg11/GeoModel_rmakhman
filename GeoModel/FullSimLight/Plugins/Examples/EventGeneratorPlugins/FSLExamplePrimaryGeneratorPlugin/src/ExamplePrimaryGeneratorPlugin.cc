#include "FullSimLight/FSLUserActionPlugin.h"
#include <iostream>

#include "ExamplePrimaryGeneratorAction.hh"

class ExamplePrimaryGeneratorPlugin:public FSLUserActionPlugin {
    
public:
    
    ExamplePrimaryGeneratorPlugin();

    G4VUserPrimaryGeneratorAction *getPrimaryGeneratorAction() const;
};

ExamplePrimaryGeneratorPlugin::ExamplePrimaryGeneratorPlugin()
{
    std::cout<<"Hello from the ExamplePrimaryGeneratorPlugin"<<std::endl;
}

G4VUserPrimaryGeneratorAction* ExamplePrimaryGeneratorPlugin::getPrimaryGeneratorAction() const
{
    return new ExamplePrimaryGeneratorAction;
}

extern "C" ExamplePrimaryGeneratorPlugin *createExamplePrimaryGeneratorPlugin() {
    return new ExamplePrimaryGeneratorPlugin();
}


#include "FullSimLight/FSLPhysicsListPlugin.h"

#include "G4VModularPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

#include <iostream>

class TestPhysicsListPlugin:public FSLPhysicsListPlugin {
    
public:
    
    TestPhysicsListPlugin();
    ~TestPhysicsListPlugin();

    G4VModularPhysicsList* GetPhysicsList() const;


};

TestPhysicsListPlugin::TestPhysicsListPlugin()
{
    std::cout<<"Hello from the TestPhysicsListPlugin"<<std::endl;
}

TestPhysicsListPlugin::~TestPhysicsListPlugin()
{
    std::cout<<"Goodbye from the TestPhysicsListPlugin"<<std::endl;
}

G4VModularPhysicsList* TestPhysicsListPlugin::GetPhysicsList() const
{

    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    physicsList->RegisterPhysics(opticalPhysics);

    return physicsList;
}

extern "C" TestPhysicsListPlugin *createTestPhysicsListPlugin() {
    return new TestPhysicsListPlugin();
}


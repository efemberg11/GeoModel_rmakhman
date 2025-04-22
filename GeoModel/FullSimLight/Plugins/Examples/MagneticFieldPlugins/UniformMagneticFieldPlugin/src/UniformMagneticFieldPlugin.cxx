#include <iostream>
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "FullSimLight/MagFieldPlugin.h"



class UniformMagneticFieldPlugin: public MagFieldPlugin

{

public:
	//Constructor
    UniformMagneticFieldPlugin();

	//Destructor
	~UniformMagneticFieldPlugin();

protected:
	//Overriding virtual function
    G4MagneticField* getField();

};

UniformMagneticFieldPlugin::UniformMagneticFieldPlugin() {

	std::cout << "HELLO from Uniform Magnetic Field Plugin." << std::endl;


}

UniformMagneticFieldPlugin::~UniformMagneticFieldPlugin() {

std::cout << "GOODBYE from Uniform Magnetic Field Plugin." << std::endl;
}               

G4MagneticField* UniformMagneticFieldPlugin::getField()
{ 
	float mfld = 0;
    std::cout << "Setting field to " << mfld << " Tesla" << std::endl;
    
  return new G4UniformMagField(G4ThreeVector(0.0, 0.0, mfld));
}




extern "C" UniformMagneticFieldPlugin* createUniformMagneticFieldPlugin()
{
 return new UniformMagneticFieldPlugin;
}




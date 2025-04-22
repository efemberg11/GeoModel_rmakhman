//
// This file defines a Sensitive Detector for the Beampipe as well
// as a mechanism for plugging it in: 
//
#include "FullSimLight/FSLSensitiveDetectorPlugin.h"
#include "G4VSensitiveDetector.hh"
#include <iostream>

class SensitiveDetector:public G4VSensitiveDetector {
public:

  // Constructor:
  SensitiveDetector();

  // Destructor:
  ~SensitiveDetector();

  // Process hits
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)  override  final;

  virtual void EndOfEvent(G4HCofThisEvent *) override {
    std::cout << "EndOfEvent" << std::endl;
  }

  virtual void Initialize(G4HCofThisEvent *) override {
    std::cout << "BeginOfEvent" << std::endl;
  }

};

SensitiveDetector::SensitiveDetector():G4VSensitiveDetector("Detector") {
  std::cout << "HELLO from SensitiveDetector" << std::endl;
}

SensitiveDetector::~SensitiveDetector() {
  std::cout << "GOODBYE from SensitiveDetector" << std::endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step *, G4TouchableHistory* ) {
  std::cout << "PROCESS HITS from SensitiveDetector" << std::endl;
  return true;
}

//----------------------------------------------------------------------//



class SDPlugin:public FSLSensitiveDetectorPlugin {

public:
  
  SDPlugin();
  
  virtual G4VSensitiveDetector *getSensitiveDetector() const final override;

  virtual std::string getHitCollectionName() const final override; 


};

SDPlugin::SDPlugin()
{

}

std::string SDPlugin::getHitCollectionName() const {
  return "HitCollection";
}

G4VSensitiveDetector *SDPlugin::getSensitiveDetector() const {
  return new SensitiveDetector();
}

extern "C" SDPlugin *createSDPlugin() {
  return new SDPlugin();
}





















#include "FSLDetectorMessenger.hh"

#include "FSLDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "globals.hh"


FSLDetectorMessenger::
FSLDetectorMessenger( FSLDetectorConstruction* FSLDet )
  : theDetector( FSLDet ) {

  theDetectorDir = new G4UIdirectory( "/FSLdet/" );
  theDetectorDir->SetGuidance( "Detector control." );

  theFieldCommand = new G4UIcmdWithADoubleAndUnit( "/FSLdet/setField", this );
  theFieldCommand->SetGuidance( "Define uniform magnetic field along Z." );
  theFieldCommand->SetGuidance( " -> in unit of  [Tesla]" );
  theFieldCommand->SetParameterName( "By", false );
  theFieldCommand->SetDefaultValue( 0.0 );
  theFieldCommand->SetUnitCategory( "Magnetic flux density" );
  theFieldCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

  theGDMLCommand = new G4UIcmdWithAString( "/FSLdet/setGdmlFile", this );
  theGDMLCommand->SetGuidance( "Set the GDML file." );
  theGDMLCommand->SetDefaultValue( "atlas2018.gdml" );
  theGDMLCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

  theRegionCommand = new G4UIcmdWithoutParameter( "/FSLdet/addRegions", this );
  theRegionCommand->SetGuidance( "Try to add detector regions." );
  theRegionCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

}


FSLDetectorMessenger::~FSLDetectorMessenger() {
  delete theFieldCommand;
  delete theDetectorDir;
  delete theRegionCommand;
}


void FSLDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if ( command == theFieldCommand ) {
    theDetector->SetMagFieldValue( theFieldCommand->GetNewDoubleValue(newValue) );
  }
  if ( command == theGDMLCommand ) {
    theDetector->SetGDMLFileName( newValue );
  }
  if ( command == theRegionCommand ) {
    theDetector->SetAddRegions( true );
  }

}

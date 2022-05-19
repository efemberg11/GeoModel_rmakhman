
#ifndef FSLDetectorMessenger_h
#define FSLDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class FSLDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

class FSLDetectorMessenger: public G4UImessenger {

public:

  FSLDetectorMessenger( FSLDetectorConstruction* );
  ~FSLDetectorMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:
  FSLDetectorConstruction*    theDetector;
  G4UIdirectory*             theDetectorDir;
  G4UIcmdWithADoubleAndUnit* theFieldCommand;
  G4UIcmdWithAString*        theGDMLCommand;
  G4UIcmdWithoutParameter*   theRegionCommand;

};

#endif

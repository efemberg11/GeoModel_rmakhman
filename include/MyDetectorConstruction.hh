
#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "G4GDMLParser.hh"
#include "G4String.hh"
#include "G4Timer.hh"

class G4VPhysicalVolume;
class G4FieldManager;
class G4UniformMagField;
class MyDetectorMessenger;
class GeoPhysVol;

class MyDetectorConstruction : public G4VUserDetectorConstruction {

public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  G4VPhysicalVolume *Construct();
  void ConstructSDandField();

  void SetGDMLFileName  (const G4String &gdmlfile)   { fGeometryFileName = gdmlfile;}
  void SetRunOverlapCheck(const bool runOvCheck) { fRunOverlapCheck = runOvCheck; }
  void SetGeometryFileName(const G4String &geometryFileName) { fGeometryFileName = geometryFileName; }

  void SetMagFieldValue(const G4double fieldValue)
  {
    fFieldValue = fieldValue;
    gFieldValue = fFieldValue;
  }
  //void SetBuildFromGDML(const G4bool b) {fBuildFromGDML=b;}

  static G4double GetFieldValue() { return gFieldValue; }
    
  void RecursivelyCheckOverlap(G4LogicalVolume* envelope);
  GeoPhysVol* CreateTheWorld(GeoPhysVol* world);

protected:
  G4Timer fTimer;
    
private:
  // this static member is for the print out
  static G4double gFieldValue;
  G4bool   fRunOverlapCheck;
  //G4bool   fBuildFromPlugin;
  //G4String fGDMLFileName;
  //G4String fSQLiteFileName;
  G4String fGeometryFileName;
  G4double fFieldValue;
  G4GDMLParser fParser;
  G4VPhysicalVolume *fWorld;
  MyDetectorMessenger *fDetectorMessenger;
};

#endif

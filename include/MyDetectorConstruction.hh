
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
  bool myCheckOverlaps(G4VPhysicalVolume* volume, G4int res = 1000, G4double tol = 0., G4bool verbose = true, G4int maxErr = 1);
    // Verifies if the placed volume is overlapping with existing
    // daughters or with the mother volume. Provides default resolution
    // for the number of points to be generated and verified.
    // A tolerance for the precision of the overlap check can be specified,
    // by default it is set to maximum precision.
    // Reports a maximum of overlaps errors according to parameter in input.
    // Returns true if the volume is overlapping.
  
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


#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "G4GDMLParser.hh"
#include "G4String.hh"
#include "G4Timer.hh"
#include <nlohmann/json.hpp>

#include "G4Cache.hh"
#include "G4MagneticField.hh"

using json = nlohmann::json;

class G4VPhysicalVolume;
class G4FieldManager;
class G4UniformMagField;
class G4MagneticField;
class G4VIntegrationDriver;
class G4MagIntegratorStepper;
class MyDetectorMessenger;
class GeoPhysVol;

class MyDetectorConstruction : public G4VUserDetectorConstruction {

public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  G4VPhysicalVolume *Construct();
  void ConstructSDandField();

  void SetGDMLFileName  (const G4String &gdmlfile)   { fGeometryFileName = gdmlfile;}
  void SetRunOverlapCheck(const bool runOvCheck)     { fRunOverlapCheck = runOvCheck; }
  void SetGeometryFileName(const G4String &geometryFileName) { fGeometryFileName = geometryFileName; }
  void SetReportFileName(const G4String &reportFileName)     { fReportFileName = reportFileName; }
  void SetOutputGDMLFileName(const G4String &outputGDMLFileName)     { fOutputGDMLFileName = outputGDMLFileName; }
  void SetDumpGDML(const bool dumpGDML)              {fDumpGDML=dumpGDML;}

  /// Common method to construct a driver with a stepper of requested type.
  G4VIntegrationDriver*
  createDriverAndStepper(std::string stepperType) const;

  void SetMagFieldValue(const G4double fieldValue)
  {
    fFieldValue = fieldValue;
    gFieldValue = fFieldValue;
    fFieldConstant = true;
  }

  static G4double GetFieldValue() { return gFieldValue; }
    
  void RecursivelyCheckOverlap(G4LogicalVolume* envelope, std::vector<json>& jlist);
  
  // Verifies if the placed volume is overlapping with existing
  // daughters or with the mother volume. Provides default resolution
  // for the number of points to be generated and verified.
  // A tolerance for the precision of the overlap check can be specified,
  // by default it is set to maximum precision.
  // Reports a maximum of overlaps errors according to parameter in input.
  // Returns true if the volume is overlapping.
  bool myCheckOverlaps(G4VPhysicalVolume* volume, std::vector<json>& jlist, G4int res = 1000, G4double tol = 0., G4bool verbose = true, G4int maxErr = 1 );
    
  //Retrieves the corresponding point in global coordinates,
  //using the chain of G4VPhysicalVolumes stored in the fTree vector
  G4ThreeVector localToGlobal(G4ThreeVector& mp, bool skipFirstIt);
  
  // Iterate from the volume envelope through all the daughter volumes, and look for the ancestors of
  // 'volume', populating the fTree vector of G4VPhysicalVolumes
  bool iterateFromWorld(G4LogicalVolume* envelope, G4VPhysicalVolume*volume, G4ThreeVector& local);
  
    GeoPhysVol* CreateTheWorld(GeoPhysVol* world);

protected:
  G4Timer fTimer;
    
private:
  // this static member is for the print out
  static G4double gFieldValue;
  G4bool   fRunOverlapCheck;
  G4bool   fDumpGDML;
  G4double fMinStep;
  G4String fGeometryFileName;
  G4String fReportFileName;
  G4String fOutputGDMLFileName;
  G4double fFieldValue;
  G4bool   fFieldConstant;
  G4GDMLParser fParser;
  G4VPhysicalVolume *fWorld;
  MyDetectorMessenger *fDetectorMessenger;
  std::vector<G4VPhysicalVolume*> fTree;
  G4Cache<G4MagneticField*> fField; //pointer to the thread-local fields
};

#endif

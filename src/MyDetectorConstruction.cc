
#include "MyDetectorConstruction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "MyDetectorMessenger.hh"
#include "G4PVPlacement.hh"

// **** INCLUDES for GeoModel
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModel2G4/ExtParameterisedVolumeBuilder.h"
#include "G4LogicalVolume.hh"

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"

#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoGeometryPluginLoader.h"

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QFileInfo>

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'
// ****

G4double MyDetectorConstruction::gFieldValue = 0.0;

MyDetectorConstruction::MyDetectorConstruction() : fWorld(nullptr), fDetectorMessenger(nullptr)
{
  fGeometryFileName    = "geometry-ATLAS-R2-2016-01-00-01.db";
  fFieldValue          = 0.0;
  fDetectorMessenger   = new MyDetectorMessenger(this);
  fRunOverlapCheck     = false;
}

MyDetectorConstruction::~MyDetectorConstruction()
{
  delete fDetectorMessenger;
}

void MyDetectorConstruction::RecursivelyCheckOverlap(G4LogicalVolume* envelope){
    
    int localNoDaughters = envelope->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<envelope->GetName()<<" is : "<<localNoDaughters<<std::endl;
    for (int sampleNo=0; sampleNo<localNoDaughters; sampleNo++){
        
        G4VPhysicalVolume *daughter=envelope->GetDaughter(sampleNo);
        if(daughter->GetLogicalVolume()->GetNoDaughters()>0)
                RecursivelyCheckOverlap(daughter->GetLogicalVolume());
        //std::cout<<"Starting Overlaps check on daughter: "<<daughter->GetName()<<std::endl;
        daughter->CheckOverlaps();
    }
}

void MyDetectorConstruction::CreateTheWorld(GeoPhysVol* world)
{
    if (world == nullptr)
    {
        // Setup the 'World' volume from which everything else will be suspended
        double densityOfAir=0.1;
        const GeoMaterial* worldMat = new GeoMaterial("std::Air", densityOfAir);
        const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
        const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, worldMat);
        world = new GeoPhysVol(worldLog);
    }
    //return world;
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    fTimer.Start();
   
    G4cout << "MyDetectorConstruction::Construct() :: starting the timer"<<G4endl;
    GeoPhysVol* world = nullptr;
    G4LogicalVolume* envelope;
    if (fGeometryFileName.contains(".dylib") || fGeometryFileName.contains(".so"))
    {
        std::cout<< "Bulding the detector from a plugin: "<<fGeometryFileName<<std::endl;
        GeoGeometryPluginLoader loader;
        GeoVGeometryPlugin *factory=loader.load(fGeometryFileName.data());
        if (!factory) {
            std::cout<<"Error!Cannot load geometry from factory. Exiting!"<<std::endl;
            exit(0);
            
        }
        std::cout<< "----> factory built: "<<fGeometryFileName<<std::endl;
        
        CreateTheWorld(world);
        std::cout<< "----> Fake World created: "<<fGeometryFileName<<std::endl;
        factory->create(world);
    
        std::cout<< "----> factory create world?: "<<fGeometryFileName<<std::endl;

        qDebug() << "ReadGeoModel::buildGeoModel() done.";
        fTimer.Stop();
        G4cout << "First step done. GeoModelTree built from the SQLite file."<<G4endl;
        G4cout << "*** Real time elapsed   : " <<fTimer.GetRealElapsed()   << G4endl;
        G4cout << "*** User time elapsed   : " <<fTimer.GetUserElapsed()   << G4endl;
        G4cout << "*** System time elapsed : " <<fTimer.GetSystemElapsed() << G4endl;
        
        fTimer.Start();
        // build the Geant4 geometry and get an hanlde to the world' volume
        ExtParameterisedVolumeBuilder* builder = new ExtParameterisedVolumeBuilder("ATLAS");
        
        std::cout << "Building G4 geometry."<<std::endl;
        envelope = builder->Build(world);
        G4VPhysicalVolume* physWorld= new G4PVPlacement(0,G4ThreeVector(),envelope,envelope->GetName(),0,false,0,false);
        
        fWorld = physWorld;
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
        
        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly check your setup selection criteria or input files!" << G4endl;
            G4Exception("MyDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
        }
        G4cout << "Second step done. Geant4 geometry created from GeoModeltree "<<G4endl;
        G4cout << "Detector Construction from the plugin file " << fGeometryFileName.data() <<", done!"<<G4endl;
        exit(0);
        
    }
    else if (fGeometryFileName.contains(".db")){
        G4cout << "Building the detector from the SQLite file: "<<fGeometryFileName<<G4endl;
        
        // open the DB
        GMDBManager* db = new GMDBManager(fGeometryFileName.data());
        /* Open database */
        if (db->isOpen()) {
            qDebug() << "OK! Database is open!";
        }
        else{
            qDebug() << "Database is not open!";
            // return;
            throw;
            
        }
    
        // -- testing the input database
        //std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
        //db->printAllMaterials();
        /* setup the GeoModel reader */
        GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
        qDebug() << "ReadGeoModel set.";
        
        
        /* build the GeoModel geometry */
        //GeoPhysVol* world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
        world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
        qDebug() << "ReadGeoModel::buildGeoModel() done.";
        fTimer.Stop();
        G4cout << "First step done. GeoModelTree built from the SQLite file."<<G4endl;
        G4cout << "*** Real time elapsed   : " <<fTimer.GetRealElapsed()   << G4endl;
        G4cout << "*** User time elapsed   : " <<fTimer.GetUserElapsed()   << G4endl;
        G4cout << "*** System time elapsed : " <<fTimer.GetSystemElapsed() << G4endl;
        
        fTimer.Start();
        // build the Geant4 geometry and get an hanlde to the world' volume
        ExtParameterisedVolumeBuilder* builder = new ExtParameterisedVolumeBuilder("ATLAS");
        
        std::cout << "Building G4 geometry."<<std::endl;
        envelope = builder->Build(world);
        G4VPhysicalVolume* physWorld= new G4PVPlacement(0,G4ThreeVector(),envelope,envelope->GetName(),0,false,0,false);
        
        fWorld = physWorld;
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
        
        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly check your setup selection criteria or GDML input!" << G4endl;
            G4Exception("MyDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
        }
        G4cout << "Second step done. Geant4 geometry created from GeoModeltree "<<G4endl;
        G4cout << "Detector Construction from the SQLite file " << fGeometryFileName.data() <<", done!"<<G4endl;
    }
    
    else if (fGeometryFileName.contains(".gdml")){
        G4cout << "Building the detector from the GDML file: "<<fGeometryFileName<<G4endl;
        //fParser.SetOverlapCheck(true);
        fParser.Read(fGeometryFileName, false); // turn off schema checker
        fWorld = (G4VPhysicalVolume *)fParser.GetWorldVolume();
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
        
        //RecursivelyCheckOverlap(fWorld->GetLogicalVolume());
        envelope = fWorld->GetLogicalVolume();
        
        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly! Check your setup selection criteria or the GDML input!" << G4endl;
            G4Exception("MyDetectorConstruction::Construct()", "FULLSIMLIGHT_0001", FatalException, ed);
        }
        G4cout << "Detector Construction from the GDML file " << fGeometryFileName.data() <<", done!"<<G4endl;
        // ConstructSDandField();
        
    }
    else{
        std::cout<< "Error! Geometry format file not supported! Please use one of the following format: .db/.gdml/.so/.dylib. Exiting. "<<std::endl;
        exit(-1);
    }
    
    fTimer.Stop();
    G4cout << "**** Real time elapsed   : " <<fTimer.GetRealElapsed()   << G4endl;
    G4cout << "**** User time elapsed   : " <<fTimer.GetUserElapsed()   << G4endl;
    G4cout << "**** System time elapsed : " <<fTimer.GetSystemElapsed() << G4endl;
    
    if (fRunOverlapCheck){
        RecursivelyCheckOverlap(envelope);
        G4cout<<"Recursive overlap check done! Exiting."<<G4endl;
        exit(0);
    }
    
    return fWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
  if (std::abs(fFieldValue) > 0.0) {
    // Apply a global uniform magnetic field along the Z axis.
    // Notice that only if the magnetic field is not zero, the Geant4
    // transportion in field gets activated.
    auto uniformMagField     = new G4UniformMagField(G4ThreeVector(0.0, 0.0, fFieldValue));
    G4FieldManager *fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(uniformMagField);
    fieldMgr->CreateChordFinder(uniformMagField);
    G4cout << G4endl << " *** SETTING MAGNETIC FIELD : fieldValue = " << fFieldValue / tesla << " Tesla *** " << G4endl
           << G4endl;

  } else {
    G4cout << G4endl << " *** NO MAGNETIC FIELD SET  *** " << G4endl << G4endl;
  }
}

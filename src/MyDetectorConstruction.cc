
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
  fGDMLFileName             = "atlas2018.gdml";
  fGeometryDatabaseFileName = "geometry.db";
  fFieldValue        = 0.0;
  fDetectorMessenger = new MyDetectorMessenger(this);
}

MyDetectorConstruction::~MyDetectorConstruction()
{
  delete fDetectorMessenger;
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // open the DB
    GMDBManager* db = new GMDBManager(fGeometryDatabaseFileName.data());
    /* Open database */
    if (db->isOpen()) {
        qDebug() << "OK! Database is open!";
    }
    else {
        qDebug() << "Database is not open!";
        // return;
        throw;
    }

    // -- testing the input database
    std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
    db->printAllMaterials();
    /* setup the GeoModel reader */
    GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
    qDebug() << "ReadGeoModel set.";


    /* build the GeoModel geometry */
    GeoPhysVol* world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
    qDebug() << "ReadGeoModel::buildGeoModel() done.";


    // --- testing the imported ATLAS Geometry

//    // get the GeoLogVol used for the 'world' volume
//    std::cout << "Getting the GeoLogVol used by the 'world' volume" << std::endl;
//    const GeoLogVol* logVol = world->getLogVol();
//    std::cout << "'world' GeoLogVol name: " << logVol->getName() << std::endl;
//    std::cout << "'world' GeoMaterial name: " << logVol->getMaterial()->getName() << std::endl;
//
//    // get number of children volumes
//    unsigned int nChil = world->getNChildVols();
//    std:: cout << "'world' number of children: " << nChil << std::endl;
//
//    // loop over all children nodes
//    std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and GeoFullPhysVol)..." << std::endl;
//    for (unsigned int idx=0; idx<nChil; ++idx) {
//        PVConstLink nodeLink = world->getChildVol(idx);
//
//        if ( dynamic_cast<const GeoVPhysVol*>( &(*( nodeLink ))) ) {
//            std::cout << "\t" << "the child n. " << idx << " ";
//            const GeoVPhysVol *childVolV = &(*( nodeLink ));
//            if ( dynamic_cast<const GeoPhysVol*>(childVolV) )
//
//            {
//                const GeoPhysVol* childVol = dynamic_cast<const GeoPhysVol*>(childVolV);
//                std::cout << "is a GeoPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName() << std::endl;
//                std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
//
//            } else if ( dynamic_cast<const GeoFullPhysVol*>(childVolV) ) {
//                const GeoFullPhysVol* childVol = dynamic_cast<const GeoFullPhysVol*>(childVolV);
//                std::cout << "is a GeoFullPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName() << std::endl;
//                std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
//            }
//        } else if ( dynamic_cast<const GeoNameTag*>( &(*( nodeLink ))) ) {
//            qDebug() << "\t" << "the child n. " << idx << " is a GeoNameTag";
//            const GeoNameTag *childVol = dynamic_cast<const GeoNameTag*>(&(*( nodeLink )));
//            std::cout << "\t\tGeoNameTag's name: " << childVol->getName() << std::endl;
//            //std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes\n";
//        }
//    }

    // build the Geant4 geometry and get an hanlde to the world' volume
    ExtParameterisedVolumeBuilder* builder = new ExtParameterisedVolumeBuilder("ATLAS");
    std::cout << "Building G4 geometry."<<std::endl;
    G4LogicalVolume* envelope = builder->Build(world);
    G4VPhysicalVolume* physWorld= new G4PVPlacement(0,G4ThreeVector(),envelope,envelope->GetName(),0,false,0,false);
    //fWorld = builder->Build(world);
    fWorld = physWorld;
    fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
    if (fWorld == 0) {
        G4ExceptionDescription ed;
        ed << "World volume not set properly check your setup selection criteria or GDML input!" << G4endl;
        G4Exception("MyDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
    }
    return fWorld;
}

//G4VPhysicalVolume *MyDetectorConstruction::Construct()
//{
//  //  parser.SetOverlapCheck(true);
//  fParser.Read(fGDMLFileName, false); // turn off schema checker
//  fWorld = (G4VPhysicalVolume *)fParser.GetWorldVolume();
//  fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
//  if (fWorld == 0) {
//    G4ExceptionDescription ed;
//    ed << "World volume not set properly check your setup selection criteria or GDML input!" << G4endl;
//    G4Exception("MyDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
//  }
//  // ConstructSDandField();
//  return fWorld;
//}

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

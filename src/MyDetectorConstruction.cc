
#include "MyDetectorConstruction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4UniformMagField.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "MyDetectorMessenger.hh"
#include "G4PVPlacement.hh"

// Geant4 steppers
#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4CashKarpRKF45.hh"
#include "G4ClassicalRK4.hh"
#include "G4DoLoMcPriRK34.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4FieldManager.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4NystromRK4.hh"
#include "G4RK547FEq1.hh"
#include "G4RK547FEq2.hh"
#include "G4RK547FEq3.hh"
#include "G4RKG3_Stepper.hh"
#include "G4TsitourasRK45.hh"
// Geant4 includes
#include "G4ChordFinder.hh"
#include "G4IntegrationDriver.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Version.hh"
#include "G4VIntegrationDriver.hh"
#if G4VERSION_NUMBER >= 1060
#include "G4InterpolationDriver.hh"
#endif


// **** INCLUDES for GeoModel
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModel2G4/ExtParameterisedVolumeBuilder.h"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4GDMLParser.hh"


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

#include "MagFieldServices/AtlasFieldSvc.h"
#include "StandardFieldSvc.h"


namespace clashdet {
    enum typeOfClash{ withMother=0, withSister, fullyEncapsSister, invalidSolid};
    // a simple struct to model a clash detection erro
    struct clash {
        typeOfClash clashType;
        std::string volume1Name;
        G4int       volume1CopyNo;
        std::string volume1EntityType;
        std::string volume2Name;
        G4int       volume2CopyNo;
        std::string volume2EntityType;
        double x,y,z;
        G4double distance;
    };
    
    void to_json(json& j, const clash& p) {
        j = json{{"typeOfClash", p.clashType}, {"volume1Name", p.volume1Name}, {"volume1CopyNo", p.volume1CopyNo}, {"volume1EntityType", p.volume1EntityType},{"volume2Name", p.volume2Name},{"volume2CopyNo", p.volume2CopyNo}, {"volume2EntityType", p.volume2EntityType},{"x", p.x},{"y", p.y},{"z", p.z},{"distance[mm]", p.distance} };
    }
    
    void from_json(const json& j, clash& p) {
        j.at("clashType").get_to(p.clashType);
        j.at("volume1Name").get_to(p.volume1Name);
        j.at("volume1CopyNo").get_to(p.volume1CopyNo);
        j.at("volume1EntityType").get_to(p.volume1EntityType);
        j.at("volume2Name").get_to(p.volume2Name);
        j.at("volume2CopyNo").get_to(p.volume2CopyNo);
        j.at("volume2EntityType").get_to(p.volume2EntityType);
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
        j.at("z").get_to(p.z);
        j.at("distance[mm]").get_to(p.distance);
 
    }
} // namespace clashdet





G4double MyDetectorConstruction::gFieldValue = 0.0;

MyDetectorConstruction::MyDetectorConstruction() : fWorld(nullptr), fDetectorMessenger(nullptr)
{
  fFieldValue          = 0.0;
  fDetectorMessenger   = new MyDetectorMessenger(this);
  fRunOverlapCheck     = false;
  fReportFileName      = "gmclash_report.json";
  fMinStep             = 1.0e-2;
  fField.Put(0);
}

MyDetectorConstruction::~MyDetectorConstruction()
{
  delete fDetectorMessenger;
}

bool MyDetectorConstruction::iterateFromWorld(G4LogicalVolume* envelope, G4VPhysicalVolume* volume, G4ThreeVector& local){

    int localNoDaughters = envelope->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<envelope->GetName()<<" is : "<<localNoDaughters<<std::endl;
    G4VPhysicalVolume *daughter;
    G4LogicalVolume *daughterLV;
    bool isFound;
 
    for (int n=0; n<localNoDaughters; n++)
    {
        
        daughter=envelope->GetDaughter(n);
        daughterLV = daughter->GetLogicalVolume();
//        std::cout<<"Checking daughter "<<n<<" out of "<<localNoDaughters<<std::endl;
//        std::cout<<"-----> Daughter Name is: "<<daughter->GetName()<<std::endl;
//        std::cout<<"-----> DaughterLV Name is: "<<daughterLV->GetName()<< " it has daughterLV->GetNoDaughters(): "<<daughterLV->GetNoDaughters()<<std::endl;
        if (daughterLV->GetNoDaughters()>0)
        {
            //std::cout<<"------------> More than 0 daughters"<<std::endl;
            if(daughterLV->IsAncestor(volume))
            {
                fTree.push_back(daughter);
                //std::cout<<"--------------> Found the ancestor! Daughter: "<< daughter->GetName() <<", the logical volume connected is: "<< daughterLV->GetName()<< std::endl;
                
                if(daughterLV->GetNoDaughters()>1){
//                    std::cout<<"---------------> MORE than 1 daughters"<<std::endl;
//                    std::cout<<"---------------> Iterating to the next level. Passing Volume: "<<daughterLV->GetName()<<". Elements in the tree: "<<fTree.size()<<std::endl;
                    isFound = iterateFromWorld(daughterLV, volume, local);
                    if(isFound) return true;
                    break;
                    
                }else
                {
//                    std::cout<<"---------------> ONLY 1 daughter"<<std::endl;
//                    std::cout<<daughterLV->GetDaughter(0)->GetName()<< " should be equal to == "<< volume->GetName()<< " and "<<daughterLV->GetDaughter(0)->GetCopyNo()<<" should be equal to :"<<volume->GetCopyNo()<<std::endl;
                    fTree.push_back(daughterLV->GetDaughter(0));
//                    std::cout<<"Found the volume! Daughter: "<< daughter->GetName() <<", the logical volume connected is: "<< daughterLV->GetName()<< std::endl;
                    return true;
                }
            }
//            std::cout<<"------------> Not the ANCESTOR"<<std::endl;
            if(daughterLV->GetName()== volume->GetLogicalVolume()->GetName() && daughter->GetName()== volume->GetName() && daughter->GetCopyNo()== volume->GetCopyNo())
            {
                std::cout<<" *** Volume FOUND - EVERYTHING CHECKED!"<<std::endl;
                std::cout<<daughterLV->GetName()<< " should be equal to == " <<volume->GetLogicalVolume()->GetName()<<std::endl;
                fTree.push_back(volume);//TO CHECK
                return true;
                
            }
//            else std::cout<<"------------> Names are different  "<<daughterLV->GetName()<< "  and  " <<volume->GetLogicalVolume()->GetName()<<std::endl;
        } else //the volume has zero daughters
        {
//            std::cout<<"------------> ZERO daughters!!!"<<std::endl;
            if(daughterLV->GetName()== volume->GetLogicalVolume()->GetName() && daughter->GetName()== volume->GetName() && daughter->GetCopyNo()== volume->GetCopyNo())
            {
                std::cout<<" *** Volume FOUND - EVERYTHING CHECKED!!!"<<std::endl;
                std::cout<<daughterLV->GetName()<< " should be equal to == "<< volume->GetLogicalVolume()->GetName()<<std::endl;
                fTree.push_back(daughter);
//                std::cout<<"Found the volume! Daughter: "<< daughter->GetName() <<", the logical volume connected is: "<< daughterLV->GetName()<< std::endl;
                return true;
                
            }
//            else {
//                std::cout<<"------------> Too bad, it is not the right one!!!"<<std::endl;
//                std::cout<<daughterLV->GetName()<< " should have been equal to == "<< volume->GetLogicalVolume()->GetName()<<std::endl;
//            }
        }
    }
    std::cout<<"***** Research ends here! ***** "<<std::endl;
    return true;
}

G4ThreeVector MyDetectorConstruction::localToGlobal(G4ThreeVector& local, bool skipFirstIt){
    
    std::cout<<"Converting coordinates from Local to Global: "<<std::endl;
    std::cout<<"G4VPhysicalVolumes chain is: \n"<<fWorld->GetName();
    for (auto & element : fTree) {
        std::cout<<" --> " << element->GetName()<<" ( "<< element->GetLogicalVolume()->GetName()<<" ) ";
    }
    std::cout<<std::endl;
    G4ThreeVector globalPoint = local;
    G4ThreeVector localPoint;
    for (std::vector<G4VPhysicalVolume*>::reverse_iterator element = fTree.rbegin();
         element != fTree.rend(); ++element )
    {
        // if the clash happens with the mother volumes the clashing point is already
        // in the mother coordinates - so we skip the first loop
        if(skipFirstIt){

            std::cout<<"IS mother, skipping the first iteration"<<std::endl;
            skipFirstIt=false;
            

        }
        else{
            localPoint = globalPoint;
            std::cout<<"Translating from "<<(*element)->GetName()<<" to "<<(*element)->GetMotherLogical()->GetName()<<std::endl;
            G4AffineTransform Tm((*element)->GetRotation(), (*element)->GetTranslation());
            globalPoint = Tm.TransformPoint(localPoint);
            std::cout<<"Local point: "<<localPoint<<" transformed in global: "<<globalPoint<<std::endl;
            
        }


    }
    return globalPoint;
}


void MyDetectorConstruction::RecursivelyCheckOverlap(G4LogicalVolume* envelope,std::vector<json>& jlist){
    
    int localNoDaughters = envelope->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<envelope->GetName()<<" is : "<<localNoDaughters<<std::endl;
    for (int sampleNo=0; sampleNo<localNoDaughters; sampleNo++){
        
        G4VPhysicalVolume *daughter=envelope->GetDaughter(sampleNo);
        if(daughter->GetLogicalVolume()->GetNoDaughters()>0)
                RecursivelyCheckOverlap(daughter->GetLogicalVolume(), jlist);
        //std::cout<<"Starting Overlaps check on daughter: "<<daughter->GetName()<<std::endl;
        //std::cout<<"... "<<sampleNo<<std::endl;
        myCheckOverlaps(daughter, jlist);
    }
}

bool MyDetectorConstruction::myCheckOverlaps(G4VPhysicalVolume* volume, std::vector<json>& jlist, G4int res, G4double tol,G4bool verbose, G4int maxErr)
{
        std::cout.precision(8);
        clashdet::clash singleClash;
        json jSingleClash;
        if (res <= 0) { return false; }
        G4VSolid* solid = volume->GetLogicalVolume()->GetSolid();
        G4LogicalVolume* motherLog = volume->GetMotherLogical();
        if (motherLog == nullptr) {
            std::cout<<"motherLog == nullptr!!!! :,(((( "<<std::endl;
            std::cout<<"solid->getName(): "<<solid->GetName()<<std::endl;
            exit(-1);
            return false; }
        
        G4int trials = 0;
        G4bool retval = false;
        
        if (verbose)
        {
            G4cout << "*************  Checking overlaps for volume " << volume->GetName()
            << " (" << solid->GetEntityType() << ") ... ";
        }

        // Check that random points are gererated correctly
        //
        G4ThreeVector ptmp = solid->GetPointOnSurface();
        if (solid->Inside(ptmp) != kSurface)
        {
            G4ThreeVector solidCenter(0,0,0);
            G4VSolid* motherSolid = motherLog->GetSolid();
            std::cout<<"**** GMClash cannot generate a point on the surface of the volume!" <<std::endl;
            iterateFromWorld(fWorld->GetLogicalVolume(), volume, solidCenter);
            G4ThreeVector globalPoint = localToGlobal (solidCenter, true);
            std::cout<<"**** Center of the solid in Global Coordinates: " <<globalPoint<<" \n"<<std::endl;
            fTree.clear();
            
            //fill the singleClash struct
            singleClash.clashType = clashdet::invalidSolid;
            singleClash.volume1Name=volume->GetName();
            singleClash.volume1CopyNo =volume->GetCopyNo();
            singleClash.volume1EntityType=solid->GetEntityType();
            singleClash.volume2Name =  motherLog->GetName();
            //singleClash.volume2CopyNo
            singleClash.volume2EntityType = motherSolid->GetEntityType();
            singleClash.x = globalPoint[0];
            singleClash.y = globalPoint[1];
            singleClash.z = globalPoint[2];
            singleClash.distance = -999;
            
            //write the singleClash in the json file
            to_json(jSingleClash, singleClash);
            // write prettified JSON to another file
            jlist.push_back(jSingleClash);
            
            
            G4String position[3] = { "outside", "surface", "inside" };
            std::ostringstream message;
            message << "Sample point is not on the surface !" << G4endl
            << "          The issue is detected for volume "
            << volume->GetName() << ':' << volume->GetCopyNo()
            << " (" << solid->GetEntityType() << ")" << G4endl
            << "          generated point " << ptmp
            << " is " << position[solid->Inside(ptmp)];
            G4Exception("G4PVPlacement::CheckOverlaps()",
                        "GeomVol1002", JustWarning, message);
            return false;
        }
    
        // Generate random points on the surface of the solid,
        // transform them into the mother volume coordinate system
        // and find the bonding box
        //
        std::vector<G4ThreeVector> points(res);
        G4double xmin =  kInfinity, ymin =  kInfinity, zmin =  kInfinity;
        G4double xmax = -kInfinity, ymax = -kInfinity, zmax = -kInfinity;
        G4AffineTransform Tm(volume->GetRotation(), volume->GetTranslation());
        for (G4int i = 0; i < res; ++i)
        {
            points[i] = Tm.TransformPoint(solid->GetPointOnSurface());
            xmin = std::min(xmin, points[i].x());
            ymin = std::min(ymin, points[i].y());
            zmin = std::min(zmin, points[i].z());
            xmax = std::max(xmax, points[i].x());
            ymax = std::max(ymax, points[i].y());
            zmax = std::max(zmax, points[i].z());
        }
        
        // Check overlap with the mother volume
        //
        G4VSolid* motherSolid = motherLog->GetSolid();
        for (G4int i = 0; i < res; ++i)
        {
            G4ThreeVector mp = points[i];
            if (motherSolid->Inside(mp) != kOutside) continue;
            G4double distin = motherSolid->DistanceToIn(mp);
            if (distin > tol)
            {
                ++trials;
                retval = true;
                std::ostringstream message;
                message << "Overlap with mother volume !" << G4endl
                << "          Overlap is detected for volume "
                << volume->GetName() << ':' << volume->GetCopyNo()
                << " (" << solid->GetEntityType() << ")" << G4endl
                << "          with its mother volume " << motherLog->GetName()
                << " (" << motherSolid->GetEntityType() << ")" << G4endl
                << "          at mother local point " << mp << ", "
                << "overlapping by at least: "
                << G4BestUnit(distin, "Length");
                
                if (trials >= maxErr)
                {
                    message << G4endl
                    << "NOTE: Reached maximum fixed number -" << maxErr
                    << "- of overlaps reports for this volume !";
                }
                G4Exception("G4PVPlacement::CheckOverlaps()",
                            "GeomVol1002", JustWarning, message);
                
                std::cout<<"**** GMClash detected a clash ::withMother - at local point: " <<mp<<std::endl;
                iterateFromWorld(fWorld->GetLogicalVolume(), volume, mp);
                G4ThreeVector globalPoint = localToGlobal (mp, true);
                std::cout<<"**** Global Point: " <<globalPoint<<" \n"<<std::endl;
                fTree.clear();
                
                //fill the singleClash struct
                singleClash.clashType = clashdet::withMother;
                singleClash.volume1Name=volume->GetName();
                singleClash.volume1CopyNo =volume->GetCopyNo();
                singleClash.volume1EntityType=solid->GetEntityType();
                singleClash.volume2Name =  motherLog->GetName();
                //singleClash.volume2CopyNo
                singleClash.volume2EntityType = motherSolid->GetEntityType();
                singleClash.x = globalPoint[0];
                singleClash.y = globalPoint[1];
                singleClash.z = globalPoint[2];
                singleClash.distance = distin;
                
                //write the singleClash in the json file
                to_json(jSingleClash, singleClash);
                // write prettified JSON to another file
                jlist.push_back(jSingleClash);
                
                
                
                
                if (trials >= maxErr)  { return true; }
                break;
            }
        }
        
        // Checking overlaps with each 'sister' volume
        //
        for (size_t k = 0; k < motherLog->GetNoDaughters(); ++k)
        {
            G4VPhysicalVolume* daughter = motherLog->GetDaughter(k);
            if (daughter == volume) continue;
            G4VSolid* daughterSolid = daughter->GetLogicalVolume()->GetSolid();
            G4AffineTransform Td(daughter->GetRotation(), daughter->GetTranslation());
            
            G4double distout = -kInfinity;
            G4ThreeVector plocal;
            if (!Td.IsRotated()) {
                G4ThreeVector offset = Td.NetTranslation();
                G4ThreeVector pmin, pmax;
                daughterSolid->BoundingLimits(pmin, pmax);
                pmin += offset;
                pmax += offset;
                if (pmin.x() >= xmax) continue;
                if (pmin.y() >= ymax) continue;
                if (pmin.z() >= zmax) continue;
                if (pmax.x() <= xmin) continue;
                if (pmax.y() <= ymin) continue;
                if (pmax.z() <= zmin) continue;
                for (G4int i = 0; i < res; ++i)
                {
                    G4ThreeVector p = points[i];
                    if (p.x() <= pmin.x()) continue;
                    if (p.x() >= pmax.x()) continue;
                    if (p.y() <= pmin.y()) continue;
                    if (p.y() >= pmax.y()) continue;
                    if (p.z() <= pmin.z()) continue;
                    if (p.z() >= pmax.z()) continue;
                    G4ThreeVector md = p - offset;
                    if (daughterSolid->Inside(md) == kInside)
                    {
                        G4double dtmp = daughterSolid->DistanceToOut(md);
                        if (dtmp <= tol) continue;
                        distout = dtmp;
                        plocal = md;
                        break;
                    }
                }
            } else {
                G4ThreeVector pmin, pmax;
                daughterSolid->BoundingLimits(pmin, pmax);
                G4ThreeVector pbox[8] = {
                    G4ThreeVector(pmin.x(), pmin.y(), pmin.z()),
                    G4ThreeVector(pmax.x(), pmin.y(), pmin.z()),
                    G4ThreeVector(pmin.x(), pmax.y(), pmin.z()),
                    G4ThreeVector(pmax.x(), pmax.y(), pmin.z()),
                    G4ThreeVector(pmin.x(), pmin.y(), pmax.z()),
                    G4ThreeVector(pmax.x(), pmin.y(), pmax.z()),
                    G4ThreeVector(pmin.x(), pmax.y(), pmax.z()),
                    G4ThreeVector(pmax.x(), pmax.y(), pmax.z())
                };
                G4double dxmin =  kInfinity, dymin =  kInfinity, dzmin =  kInfinity;
                G4double dxmax = -kInfinity, dymax = -kInfinity, dzmax = -kInfinity;
                for (G4int i = 0; i < 8; ++i)
                {
                    G4ThreeVector p = Td.TransformPoint(pbox[i]);
                    dxmin = std::min(dxmin, p.x());
                    dymin = std::min(dymin, p.y());
                    dzmin = std::min(dzmin, p.z());
                    dxmax = std::max(dxmax, p.x());
                    dymax = std::max(dymax, p.y());
                    dzmax = std::max(dzmax, p.z());
                }
                if (dxmin >= xmax) continue;
                if (dymin >= ymax) continue;
                if (dzmin >= zmax) continue;
                if (dxmax <= xmin) continue;
                if (dymax <= ymin) continue;
                if (dzmax <= zmin) continue;
                for (G4int i = 0; i < res; ++i)
                {
                    G4ThreeVector p = points[i];
                    if (p.x() >= dxmax) continue;
                    if (p.x() <= dxmin) continue;
                    if (p.y() >= dymax) continue;
                    if (p.y() <= dymin) continue;
                    if (p.z() >= dzmax) continue;
                    if (p.z() <= dzmin) continue;
                    G4ThreeVector md = Td.InverseTransformPoint(p);
                    if (daughterSolid->Inside(md) == kInside)
                    {
                        G4double dtmp = daughterSolid->DistanceToOut(md);
                        if (dtmp <= tol) continue;
                        distout = dtmp;
                        plocal = md;
                        break;
                    }
                }
            }
            
            if (distout > tol)
            {
                ++trials;
                retval = true;
                std::ostringstream message;
                
                message << "Overlap with volume already placed !" << G4endl
                << "          Overlap is detected for volume "
                << volume->GetName() << ':' << volume->GetCopyNo()
                << " (" << solid->GetEntityType() << ")" << G4endl
                << "          with " << daughter->GetName()
                << ':' << daughter->GetCopyNo()
                << " (" << daughterSolid->GetEntityType() << ")"
                << " volume's" << G4endl
                << "          local point " << plocal << ", "
                << "overlapping by at least: "
                << G4BestUnit(distout, "Length")<<std::endl;
                
                if (trials >= maxErr)
                {
                    message << G4endl
                    << "NOTE: Reached maximum fixed number -" << maxErr
                    << "- of overlaps reports for this volume !";
                }
                G4Exception("G4PVPlacement::CheckOverlaps()",
                            "GeomVol1002", JustWarning, message);
                
                std::cout<<"**** GMClash detected a clash ::withSister - at sister local point: " <<plocal<<std::endl;

                // Transform the generated point to the mother's coordinate system
                // and then to current volume's coordinate system
                //
                G4ThreeVector mp2 = Td.TransformPoint(plocal);
                G4ThreeVector msi = Tm.InverseTransformPoint(mp2);

                iterateFromWorld(fWorld->GetLogicalVolume(), volume, msi);
                

                G4ThreeVector globalPoint = localToGlobal (msi, false);
                std::cout<<"**** Global Point: " <<globalPoint<<" \n"<<std::endl;
                fTree.clear();
                
                //fill the singleClash struct
                singleClash.clashType         = clashdet::withSister;
                singleClash.volume1Name       = volume->GetName();
                singleClash.volume1CopyNo     = volume->GetCopyNo();
                singleClash.volume1EntityType = solid->GetEntityType();
                singleClash.volume2Name       = daughter->GetName();
                singleClash.volume2CopyNo     = daughter->GetCopyNo();
                singleClash.volume2EntityType = daughterSolid->GetEntityType();
                singleClash.x = globalPoint[0];
                singleClash.y = globalPoint[1];
                singleClash.z = globalPoint[2];
                singleClash.distance = distout;
                
                //write the singleClash in the json file
                to_json(jSingleClash, singleClash);
                jlist.push_back(jSingleClash);
                
                if (trials >= maxErr)  { return true; }
            }
            else if (distout == kInfinity)
            {
                // Now checking that 'sister' volume is not totally included
                // and overlapping. Generate a single point on the surface of
                // the 'sister' volume and verify that the point in NOT inside
                // the current volume
                //
                G4ThreeVector dPoint = daughterSolid->GetPointOnSurface();
                
                // Transform the generated point to the mother's coordinate system
                // and then to current volume's coordinate system
                //
                G4ThreeVector mp2 = Td.TransformPoint(dPoint);
                G4ThreeVector msi = Tm.InverseTransformPoint(mp2);
                
                if (solid->Inside(msi) == kInside)
                {
                    ++trials;
                    retval = true;
                    std::ostringstream message;
                    message << "Overlap with volume already placed !" << G4endl
                    << "          Overlap is detected for volume "
                    << volume->GetName() << ':' << volume->GetCopyNo()
                    << " (" << solid->GetEntityType() << ")" << G4endl
                    << "          apparently fully encapsulating volume "
                    << daughter->GetName() << ':' << daughter->GetCopyNo()
                    << " (" << daughterSolid->GetEntityType() << ")"
                    << " at the same level !";
            
                    if (trials >= maxErr)
                    {
                        message << G4endl
                        << "NOTE: Reached maximum fixed number -" << maxErr
                        << "- of overlaps reports for this volume !";
                    }
                    G4Exception("G4PVPlacement::CheckOverlaps()",
                                "GeomVol1002", JustWarning, message);
                    
                    std::cout<<"**** GMClash detected a clash ::fullyEncapsSister - at volume1 local point: " <<msi<<std::endl;
                    iterateFromWorld(fWorld->GetLogicalVolume(), volume, msi);
                    G4ThreeVector globalPoint = localToGlobal (msi, false);

                    std::cout<<"**** Global Point: " <<globalPoint<<" \n"<<std::endl;
                    fTree.clear();
                    
                    //fill the singleClash struct
                    singleClash.clashType         = clashdet::fullyEncapsSister;
                    singleClash.volume1Name       = volume->GetName();
                    singleClash.volume1CopyNo     = volume->GetCopyNo();
                    singleClash.volume1EntityType = solid->GetEntityType();
                    singleClash.volume2Name       = daughter->GetName();
                    singleClash.volume2CopyNo     = daughter->GetCopyNo();
                    singleClash.volume2EntityType = daughterSolid->GetEntityType();
                    singleClash.x = globalPoint[0];
                    singleClash.y = globalPoint[1];
                    singleClash.z = globalPoint[2];
                    //                    singleClash.distance = distout;
                    
                    //write the singleClash in the json file
                    to_json(jSingleClash, singleClash);
                    jlist.push_back(jSingleClash);
                    
                    if (trials >= maxErr)  { return true; }
                }
            }
        }
        
        if (verbose && trials == 0) { G4cout << "OK, done! " << G4endl; }
        return retval;
}

GeoPhysVol*  MyDetectorConstruction::CreateTheWorld(GeoPhysVol* world)
{
    if (world == nullptr)
    {
        // Setup the 'World' volume from which everything else will be suspended
        // Get the materials that we shall use.
        // -------------------------------------//
        GeoMaterial* Air=new GeoMaterial("Air", 1.290*SYSTEM_OF_UNITS::mg/SYSTEM_OF_UNITS::cm3);
        GeoElement* Oxigen = new GeoElement("Oxygen",  "O", 8.0, 16.0*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole);
        GeoElement* Nitrogen = new GeoElement("Nitrogen", "N", 7., 14.0067*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole);
        Air->add(Nitrogen, .8);
        Air->add(Oxigen, .2);
        Air->lock();
        const GeoBox* worldBox = new GeoBox(1300*SYSTEM_OF_UNITS::cm, 1300*SYSTEM_OF_UNITS::cm, 1300*SYSTEM_OF_UNITS::cm);
        const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, Air);
        world = new GeoPhysVol(worldLog);
    }
    return world;
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    fTimer.Start();

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

        world = CreateTheWorld(nullptr);
        factory->create(world);

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

        
//        G4GDMLParser parser;
//        parser.SetRegionExport(true);
//        //     parser.SetEnergyCutsExport(true);
//        parser.Write("accordion.gdml", fWorld->GetLogicalVolume());
//        G4cout << "Geometry exported in GDML, done!"<<G4endl;
        
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
        G4cout << "\n ===================  Starting Clashes Detection  =================== \n" << G4endl;
        fTimer.Start();
        std::vector<json> jlist;
        RecursivelyCheckOverlap(envelope, jlist);
        fTimer.Stop();
        G4cout << "\n**** Real time elapsed   : " <<fTimer.GetRealElapsed()   << G4endl;
        G4cout << "**** User time elapsed   : " <<fTimer.GetUserElapsed()   << G4endl;
        G4cout << "**** System time elapsed : " <<fTimer.GetSystemElapsed() << G4endl;
        
        json jReport={{"ClashesReport",jlist}};
        std::cout<<"\n**** Writing out the clashes report file: "<<fReportFileName<<std::endl;
        std::ofstream outJsonFile(fReportFileName);
        outJsonFile << std::setw(4) << jReport << std::endl;
        outJsonFile.close();
        
        G4cout<<"\n=================== Recursive overlap check done! =================== "<<G4endl;
        exit(0);
    }
    return fWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
  if (std::abs(fFieldValue) > 0.0) {
    // Apply a global uniform magnetic field along the Z axis.
    // Notice that only if the magnetic field is not zero, the Geant4
    // transportation in field gets activated.
    auto uniformMagField     = new G4UniformMagField(G4ThreeVector(0.0, 0.0, fFieldValue));
    G4FieldManager *fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(uniformMagField);
    fieldMgr->CreateChordFinder(uniformMagField);
    G4cout << G4endl << " *** SETTING UNIFORM MAGNETIC FIELD : fieldValue = " << fFieldValue / tesla << " Tesla *** " << G4endl
           << G4endl;

  } else
  {
      G4cout << G4endl << " *** MAGNETIC FIELD SET FROM FILE  *** " << G4endl << G4endl;
      if (fField.Get() == 0)
      {
          StandardFieldSvc* myMagField = new StandardFieldSvc("StandardFieldSvc");
          G4MagneticField* g4Field =  myMagField->getField();
          if(g4Field==nullptr) std::cout<<"Error, g4Field is null!"<<std::endl;
          fField.Put(g4Field);
          
//          std::cout.precision(8);
//          G4double point[3] = {0,0,0};
//          G4double bfield[3];
//          std::cout<<"4) Debug g4Field"<<std::endl;
//          for(int i=0; i<10; i++){
//              point[0]=i;
//              point[1]=i+10;
//              point[2]=i+100;
//              g4Field->GetFieldValue(point,bfield);
//
//              std::cout<<"g4Field->GetField(" <<point[0]<<" "<<point[1]<<" "<<point[2]<<"): "<<bfield[0]/tesla<<" "<<bfield[1]/tesla<<" "<<bfield[2]/tesla<<std::endl;
//          }
//          exit(-1);
        
          //This is thread-local
          G4FieldManager* fieldMgr =
          G4TransportationManager::GetTransportationManager()->GetFieldManager();
          G4cout<< "DeltaStep "<<fieldMgr->GetDeltaOneStep()/mm <<"mm" <<G4endl;
          //G4ChordFinder *pChordFinder = new G4ChordFinder(mymagField);
        
//#if G4VERSION_NUMBER < 1040
//
//        auto stepper = getStepper(m_integratorStepper, field);
//        G4MagInt_Driver* magDriver = fieldMgr->GetChordFinder()->GetIntegrationDriver();
//        magDriver->RenewStepperAndAdjust(stepper);
//#else
//
//        auto chordFinder = fieldMgr->GetChordFinder();
//        auto driver = createDriverAndStepper(m_integratorStepper, field);
//        chordFinder->SetIntegrationDriver(driver);
        
          fieldMgr->SetDetectorField(fField.Get());
          fieldMgr->CreateChordFinder(fField.Get());
//#endif
          
      }
      
  }
}
//=============================================================================
// Create the driver with a stepper
//=============================================================================
G4VIntegrationDriver*
MyDetectorConstruction::createDriverAndStepper(std::string stepperType) const
{

    G4Mag_EqRhs* eqRhs(nullptr);
//        if (!m_equationOfMotion.empty())
//        {
//            eqRhs = m_equationOfMotion->makeEquationOfMotion(field);
//            //ATH_MSG_INFO("Configuring alternative equation of motion using " <<
//                         m_equationOfMotion.name() );
//        }
//        else
//        {
//            //ATH_MSG_VERBOSE("Using G4Mag_UsualEqRhs as the equation of motion.");
//            eqRhs = new G4Mag_UsualEqRhs(field);
//        }
    G4VIntegrationDriver* driver = nullptr;
    if (stepperType=="HelixImplicitEuler") {
            G4HelixImplicitEuler* stepper = new G4HelixImplicitEuler(eqRhs);
            driver = new G4IntegrationDriver<G4HelixImplicitEuler>(
                                                                   fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="HelixSimpleRunge") {
            G4HelixSimpleRunge* stepper = new G4HelixSimpleRunge(eqRhs);
            driver = new G4IntegrationDriver<G4HelixSimpleRunge>(
                                                                 fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="HelixExplicitEuler") {
            G4HelixExplicitEuler* stepper = new G4HelixExplicitEuler(eqRhs);
            driver = new G4IntegrationDriver<G4HelixExplicitEuler>(
                                                                   fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="NystromRK4") {
            G4NystromRK4* stepper = new G4NystromRK4(eqRhs);
            driver = new G4IntegrationDriver<G4NystromRK4>(
                                                           fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="ClassicalRK4") {
            G4ClassicalRK4* stepper = new G4ClassicalRK4(eqRhs);
            driver = new G4IntegrationDriver<G4ClassicalRK4>(
                                                             fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="BogackiShampine23") {
            G4BogackiShampine23* stepper = new G4BogackiShampine23(eqRhs);
            driver = new G4IntegrationDriver<G4BogackiShampine23>(
                                                                  fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="BogackiShampine45") {
            G4BogackiShampine45* stepper = new G4BogackiShampine45(eqRhs);
            driver = new G4IntegrationDriver<G4BogackiShampine45>(
                                                                  fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="CashKarpRKF45") {
            G4CashKarpRKF45* stepper = new G4CashKarpRKF45(eqRhs);
            driver = new G4IntegrationDriver<G4CashKarpRKF45>(
                                                              fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="DoLoMcPriRK34") {
            G4DoLoMcPriRK34* stepper = new G4DoLoMcPriRK34(eqRhs);
            driver = new G4IntegrationDriver<G4DoLoMcPriRK34>(
                                                              fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="DormandPrince745") {
            G4DormandPrince745* stepper = new G4DormandPrince745(eqRhs);
            driver = new G4IntegrationDriver<G4DormandPrince745>(
                                                                 fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="DormandPrinceRK56") {
            G4DormandPrinceRK56* stepper = new G4DormandPrinceRK56(eqRhs);
            driver = new G4IntegrationDriver<G4DormandPrinceRK56>(
                                                                  fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="DormandPrinceRK78") {
            G4DormandPrinceRK78* stepper = new G4DormandPrinceRK78(eqRhs);
            driver = new G4IntegrationDriver<G4DormandPrinceRK78>(
                                                                  fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="RK547FEq1") {
            G4RK547FEq1* stepper = new G4RK547FEq1(eqRhs);
            driver = new G4IntegrationDriver<G4RK547FEq1>(
                                                          fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="RK547FEq2") {
            G4RK547FEq2* stepper = new G4RK547FEq2(eqRhs);
            driver = new G4IntegrationDriver<G4RK547FEq2>(
                                                          fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="RK547FEq3") {
            G4RK547FEq3* stepper = new G4RK547FEq3(eqRhs);
            driver = new G4IntegrationDriver<G4RK547FEq3>(
                                                          fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="RKG3_Stepper") {
            G4RKG3_Stepper* stepper = new G4RKG3_Stepper(eqRhs);
            driver = new G4IntegrationDriver<G4RKG3_Stepper>(
                                                             fMinStep, stepper, stepper->GetNumberOfVariables());
        } else if (stepperType=="TsitourasRK45") {
            G4TsitourasRK45* stepper = new G4TsitourasRK45(eqRhs);
            driver = new G4IntegrationDriver<G4TsitourasRK45>(
                                                              fMinStep, stepper, stepper->GetNumberOfVariables());
        }
#if G4VERSION_NUMBER >= 1060
        else if (stepperType=="DormandPrince745Int") {
            G4DormandPrince745* stepper = new G4DormandPrince745(eqRhs);
            driver = new G4InterpolationDriver<G4DormandPrince745>(
                                                                   fMinStep, stepper, stepper->GetNumberOfVariables());
        }
#endif
        else {
            std::cout<<"Stepper " << stepperType << " not available! returning NystromRK4!"<<std::endl;
            G4NystromRK4* stepper = new G4NystromRK4(eqRhs);
            driver = new G4IntegrationDriver<G4NystromRK4>(
                                                           fMinStep, stepper, stepper->GetNumberOfVariables());
        }
        return driver;
    }

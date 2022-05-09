#include "ClashDetector.hh"
#include "GeoModelKernel/GeoVolumeCursor.h"

#include "G4AffineTransform.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


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
        p.clashType=j.at("clashType").get<typeOfClash>();
        p.volume1Name=j.at("volume1Name").get<std::string>();
        p.volume1CopyNo=j.at("volume1CopyNo").get<int>();
        p.volume1EntityType=j.at("volume1EntityType").get<std::string>();
        p.volume2Name=j.at("volume2Name").get<std::string>();
        p.volume2CopyNo=j.at("volume2CopyNo").get<int>();
        p.volume2EntityType=j.at("volume2EntityType").get<std::string>();
        p.x=j.at("x").get<double>();
        p.y=j.at("y").get<double>();
        p.z=j.at("z").get<double>();
        p.distance=j.at("distance[mm]").get<double>();
    }
} // namespace clashdet

ClashDetector::ClashDetector(G4VPhysicalVolume *world):
fWorld(world)
{}
bool ClashDetector::iterateFromWorld(G4LogicalVolume* envelope, G4VPhysicalVolume* volume, G4ThreeVector& local){
    
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
                if (fGmclashVerbosity){
                    std::cout<<" *** Volume FOUND - EVERYTHING CHECKED!"<<std::endl;
                    std::cout<<daughterLV->GetName()<< " should be equal to == " <<volume->GetLogicalVolume()->GetName()<<std::endl;
                }
                fTree.push_back(volume);//TO CHECK
                return true;
                
            }
            //            else std::cout<<"------------> Names are different  "<<daughterLV->GetName()<< "  and  " <<volume->GetLogicalVolume()->GetName()<<std::endl;
        } else //the volume has zero daughters
        {
            //            std::cout<<"------------> ZERO daughters!!!"<<std::endl;
            if(daughterLV->GetName()== volume->GetLogicalVolume()->GetName() && daughter->GetName()== volume->GetName() && daughter->GetCopyNo()== volume->GetCopyNo())
            {
                if (fGmclashVerbosity){
                    std::cout<<" *** Volume FOUND - EVERYTHING CHECKED!!!"<<std::endl;
                    std::cout<<daughterLV->GetName()<< " should be equal to == "<< volume->GetLogicalVolume()->GetName()<<std::endl;
                    
                }
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
    if (fGmclashVerbosity)
        std::cout<<"***** Research ends here! ***** "<<std::endl;
    return true;
}

G4ThreeVector ClashDetector::localToGlobal(G4ThreeVector& local, bool skipFirstIt){
    
    if (fGmclashVerbosity){
        std::cout<<"Converting coordinates from Local to Global: "<<std::endl;
        std::cout<<"G4VPhysicalVolumes chain is: \n"<<fWorld->GetName();
        for (auto & element : fTree) {
            std::cout<<" --> " << element->GetName()<<" ( "<< element->GetLogicalVolume()->GetName()<<" ) ";
        }
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
            if (fGmclashVerbosity)
                std::cout<<"IS mother, skipping the first iteration"<<std::endl;
            skipFirstIt=false;
            
            
        }
        else{
            localPoint = globalPoint;
            if (fGmclashVerbosity)
                std::cout<<"Translating from "<<(*element)->GetName()<<" to "<<(*element)->GetMotherLogical()->GetName()<<std::endl;
            G4AffineTransform Tm((*element)->GetRotation(), (*element)->GetTranslation());
            globalPoint = Tm.TransformPoint(localPoint);
            if (fGmclashVerbosity)
                std::cout<<"Local point: "<<localPoint<<" transformed in global: "<<globalPoint<<std::endl;
            
        }
        
        
    }
    return globalPoint;
}


void ClashDetector::recursivelyCheckOverlap(G4LogicalVolume* envelope,std::vector<json>& jlist){
    
    int localNoDaughters = envelope->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<envelope->GetName()<<" is : "<<localNoDaughters<<std::endl;
    for (int sampleNo=0; sampleNo<localNoDaughters; sampleNo++){
        
        G4VPhysicalVolume *daughter=envelope->GetDaughter(sampleNo);
        if(daughter->GetLogicalVolume()->GetNoDaughters()>0)
            recursivelyCheckOverlap(daughter->GetLogicalVolume(), jlist);
        //std::cout<<"Starting Overlaps check on daughter: "<<daughter->GetName()<<std::endl;
        //std::cout<<"... "<<sampleNo<<std::endl;
        g4CheckOverlaps(daughter, jlist, 1000, fTolerance, 1);
    }
}

bool ClashDetector::g4CheckOverlaps(G4VPhysicalVolume* volume, std::vector<json>& jlist, G4int res, G4double tol, G4int maxErr)
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
    
    if (fGmclashVerbosity)
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
        
        // Convert from local to global coordinate system.
        // NB: we put the flag to false cause we are not in the mother volume
        // coordinate system, so we don't need to skip the first iteration
        G4ThreeVector globalPoint = localToGlobal (solidCenter, false);
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
            
            iterateFromWorld(fWorld->GetLogicalVolume(), volume, mp);
            G4ThreeVector globalPoint = localToGlobal (mp, true);
            std::cout<<"**** GMClash detected a clash ::withMother, at \nlocal point:\t" <<mp<<std::endl;
            std::cout<<"global point:\t" <<globalPoint<<" \n"<<std::endl;
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
#if G4VERSION_NUMBER>=1040
            daughterSolid->BoundingLimits(pmin, pmax);
#else
            std::cout<<"ERROR, this version of Geant4 doesn't support this check! Update to version >= Geant4.10.6"<<std::endl;
            exit(-1);
#endif
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
#if G4VERSION_NUMBER>=1040
            daughterSolid->BoundingLimits(pmin, pmax);
#else
            std::cout<<"ERROR, this version of Geant4 doesn't support this check! Update to version >= Geant4.10.6"<<std::endl;
            exit(-1);
#endif
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
#if G4VERSION_NUMBER>1050
                G4ThreeVector md = Td.InverseTransformPoint(p);
#else
                G4ThreeVector md;
                std::cout<<"InverseTransformPoint not available in this Geant4 version! Please update to Geant4.10.6 or greater versions. Exiting. "<<std::endl;
                exit(-1);
#endif
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
            
            // Transform the generated point to the mother's coordinate system
            // and then to current volume's coordinate system
            //
            G4ThreeVector mp2 = Td.TransformPoint(plocal);
            
#if G4VERSION_NUMBER>1050
            G4ThreeVector msi = Tm.InverseTransformPoint(mp2);
#else
            G4ThreeVector msi;
            std::cout<<"InverseTransformPoint not available in this Geant4 version! Please update to Geant4.10.6 or greater versions. Exiting. "<<std::endl;
            exit(-1);
#endif
            
            iterateFromWorld(fWorld->GetLogicalVolume(), volume, msi);
            G4ThreeVector globalPoint = localToGlobal (msi, false);
            
            std::cout<<"**** GMClash detected a clash ::withSister, at sister \nlocal point:\t" <<plocal<<std::endl;
            std::cout<<"global point:\t" <<globalPoint<<" \n"<<std::endl;
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
            
#if G4VERSION_NUMBER>1050
            G4ThreeVector msi = Tm.InverseTransformPoint(mp2);
#else
            G4ThreeVector msi;
            std::cout<<"InverseTransformPoint not available in this Geant4 version! Please update to Geant4.10.6 or greater versions. Exiting. "<<std::endl;
            exit(-1);
#endif
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
                
                
                iterateFromWorld(fWorld->GetLogicalVolume(), volume, msi);
                G4ThreeVector globalPoint = localToGlobal (msi, false);
                std::cout<<"**** GMClash detected a clash ::fullyEncapsSister, at volume1 \nlocal point:\t" <<msi<<std::endl;
                std::cout<<"global point:\t" <<globalPoint<<" \n"<<std::endl;
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
    
    if (fGmclashVerbosity && trials == 0) { G4cout << "OK, done! " << G4endl; }
    return retval;
}

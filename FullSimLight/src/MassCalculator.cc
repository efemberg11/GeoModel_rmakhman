/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "MassCalculator.hh"
#include "GeoModelKernel/GeoVolumeCursor.h"


namespace masscalc {
    
    // a simple struct to model a mass calculation report
    struct massReport {
        std::string logicalVolumeName;
        std::string physicalVolumeName;
        int         volumeCopyNo;
        std::string material;
        double density;
        std::string volumeEntityType;
        double inclusiveMass;  //mass of the volume, that includes the real mass of the daughters volumes
        double exclusiveMass;  //mass of the volume, from which the mass of the daughters is subtracted
    };
    
    // a simple struct to model a mass calculation report
    struct finalMassReport {
        std::string logicalVolumeName;
        //std::string physicalVolumeName;
        //int         volumeCopyNo;
        std::string material;
        double densityThreshold;
        std::string volumeEntityType;
        double inclusiveMass;  //total inclusice mass of the geometry, that includes the real mass of the daughters volumes
        double exclusiveMass;  //total exclusive mass of the geometry, from which the mass of the daughters is subtracted
        double apparentWeight; //apparent weight in Air of the whole geometry
        double exclusiveMassFiltered;  //total mass of the volumes with density > threshold
        double excludedMass;  //total mass of the volumes with density < threshold
    };
    
    void to_json(json& j, const massReport& p) {
        j = json{{"logicalVolumeName", p.logicalVolumeName}, json{"physicalVolumeName", p.physicalVolumeName}, {"volumeCopyNo", p.volumeCopyNo}, {"material", p.material},{"density[g/cm3]", p.density}, {"volumeEntityType", p.volumeEntityType}, {"inclusiveMass[kg]", p.inclusiveMass}, {"exclusiveMass[kg]", p.exclusiveMass} };
    }
    
    void from_json(const json& j, massReport& p) {
        p.logicalVolumeName=j.at("logicalVolumeName").get<std::string>();
        p.physicalVolumeName=j.at("physicalVolumeName").get<std::string>();
        p.volumeCopyNo=j.at("volumeCopyNo").get<int>();
        p.material=j.at("material").get<std::string>();
        p.density=j.at("density[g/cm3]").get<double>();
        p.volumeEntityType=j.at("volumeEntityType").get<std::string>();
        p.inclusiveMass=j.at("inclusiveMass[kg]").get<double>();
        p.exclusiveMass=j.at("exclusiveMass[kg]").get<double>();
        
    }
    
    void to_json(json& j, const finalMassReport& p) {
        j = json{{"logicalVolumeName", p.logicalVolumeName}, {"material", p.material},  {"densityThreshold[g/cm3]", p.densityThreshold}, {"volumeEntityType", p.volumeEntityType}, {"inclusiveMass[kg]", p.inclusiveMass}, {"exclusiveMass[kg]", p.exclusiveMass}, {"apparentWeightInAir[kg]", p.apparentWeight}, {"exclusiveFilteredMass[kg]", p.exclusiveMassFiltered}, {"excludedFilteredMass[kg]", p.excludedMass}   };
    }
    
    void from_json(const json& j, finalMassReport& p) {
        p.logicalVolumeName=j.at("logicalVolumeName").get<std::string>();
        p.material=j.at("material").get<std::string>();
        p.densityThreshold=j.at("densityThreshold[g/cm3]").get<double>();
        p.volumeEntityType=j.at("volumeEntityType").get<std::string>();
        p.inclusiveMass=j.at("inclusiveMass[kg]").get<double>();
        p.exclusiveMass=j.at("exclusiveMass[kg]").get<double>();
        p.apparentWeight=j.at("apparentWeightInAir[kg]").get<double>();
        p.exclusiveMassFiltered=j.at("exclusiveFilteredMass[kg]").get<double>();
        p.excludedMass=j.at("excludedFilteredMass[kg]").get<double>();
        
    }
} // namespace masscalc

double MassCalculator::volume(const PVConstLink& pv) {
    const GeoLogVol        * lv       = pv->getLogVol();
    const GeoShape         *shape    = lv->getShape();
    return shape->volume();
}


double MassCalculator::exclusiveMass(const PVConstLink& pv) {
    const GeoLogVol* lv       = pv->getLogVol();
    const GeoMaterial      *material = lv->getMaterial();
    double density = material->getDensity();
    return density*volume(pv);
}

double MassCalculator::inclusiveMass(const PVConstLink& pv) {
    
    const GeoLogVol*        lv       = pv->getLogVol();
    const GeoMaterial      *material = lv->getMaterial();
    // Exclude from the calculation the EMEC special shape
    // cause the corresponding methods are not implemented in GeoModel
    if (lv->getName() == "LAr::EMEC::Pos::InnerWheel" ||
        lv->getName() == "LAr::EMEC::Neg::InnerWheel" ||
        lv->getName() == "LAr::EMEC::Pos::OuterWheel" ||
        lv->getName() == "LAr::EMEC::Neg::OuterWheel" ||
        lv->getName() == "LAr::EMEC::Pos::InnerCone"  ||
        lv->getName() == "LAr::EMEC::Neg::InnerCone"  ||
        lv->getName() == "LAr::EMEC::Pos::OuterFrontCone" ||
        lv->getName() == "LAr::EMEC::Neg::OuterFrontCone" ||
        lv->getName() == "LAr::EMEC::Pos::OuterBackCone"  ||
        lv->getName() == "LAr::EMEC::Neg::OuterBackCone"  ||
        lv->getName() == "LAr::EMEC::Pos::InnerSlice00"   ||
        lv->getName() == "LAr::EMEC::Neg::InnerSlice00"   ||
        lv->getName() == "LAr::EMEC::Pos::OuterSlice00"   ||
        lv->getName() == "LAr::EMEC::Neg::OuterSlice00"   ||
        lv->getName() == "UnidentifiedShape"){
        // This is one to remove
        std::cout<<" !REMOVING Unidentified shapes: "<< lv->getName()<<", shape is not implemented in GeoModel, it will account zero in the mass calculation!"<<std::endl;
        return 0;
    }
    double density = material->getDensity();
    double mass = exclusiveMass(pv);
    
    GeoVolumeCursor av(pv);
    while (!av.atEnd()) {
        mass += inclusiveMass(av.getVolume());
        mass -= volume(av.getVolume())*density;
        av.next();
    }
    
    return mass;
}


void MassCalculator::calculateMass(G4LogicalVolume* logVol, G4VPhysicalVolume * physVol, std::vector<json>& jlist, double& exclusiveMass, bool writeRep){
    
    double tmpInclusive,tmpExclusive;
    
    //By setting the 'propagate' boolean flag - the second one -  to 'false' the
    //       method returns the mass of the present logical volume only
    //       (subtracted for the volume occupied by the daughter volumes).
    
    tmpInclusive = logVol->GetMass(true, true);  //real mass of the LV, inclusive of the masses of the daughters
    tmpExclusive = logVol->GetMass(true, false); //mass of the LV substracted for the volume occupied by the daughters
    
    //if the method is called iteratively, the only mass cumulative mass that makes sense
    //to retrieve is the sum of the exclusive masses of all the volumes
    //satisfying the filters criteria
    exclusiveMass+= tmpExclusive;
    
    if(writeRep){
        masscalc::massReport singleMassReport;
        json jSingleMassReport;
        //fill the singleMassReport struct
        singleMassReport.logicalVolumeName=logVol->GetName();
        singleMassReport.physicalVolumeName=physVol->GetName();
        singleMassReport.volumeCopyNo =physVol->GetCopyNo();
        singleMassReport.material =logVol->GetMaterial()->GetName();
        singleMassReport.density =logVol->GetMaterial()->GetDensity()/SYSTEM_OF_UNITS::g*SYSTEM_OF_UNITS::cm3;
        singleMassReport.volumeEntityType=logVol->GetSolid()->GetEntityType();
        singleMassReport.inclusiveMass = tmpInclusive/(CLHEP::kg);
        singleMassReport.exclusiveMass = tmpExclusive/(CLHEP::kg);
        
        //write the singleMassReport in the json file
        to_json(jSingleMassReport, singleMassReport);
        // write prettified JSON to another file
        jlist.push_back(jSingleMassReport);
    }
    
}

void MassCalculator::iterateFromWorldMass(G4LogicalVolume* logVolume, std::vector<json>& jlist, double& inclusiveMass, double& exclusiveMass, G4String prefix, G4String material){
    
    int nDaughters = logVolume->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<logVolume->GetName()<<" is : "<<nDaughters<<std::endl;
    G4VPhysicalVolume *daughterPV = nullptr;
    G4LogicalVolume *daughterLV   = nullptr;
    G4double density;
    
    for (int n=0; n<nDaughters; n++)
    {
        daughterPV=logVolume->GetDaughter(n);
        daughterLV = daughterPV->GetLogicalVolume();
        density=daughterLV->GetMaterial()->GetDensity();
        //fAnalysisManager->FillH1(fHistoID, density/(SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3), 1);
        
        //1. look only for the logVol
        if(prefix!="" && material==""){
            if (daughterLV->GetName().contains(prefix) || daughterPV->GetName().contains(prefix)){
                //                std::cout<<"Found the LV "<<prefix<<" and its full name is "<<daughterLV->GetName()<<std::endl;
                //                std::cout<<"Found the Daughter "<<prefix<<" and its full name is "<<daughterPV->GetName()<<std::endl;
                std::cout<<"Cubic Volume of "<<daughterLV->GetName()<<" is "<<daughterLV->GetSolid()->GetCubicVolume()/SYSTEM_OF_UNITS::cm3<<" [cm3]"<<std::endl;
                
                calculateMass(daughterLV, daughterPV, jlist, exclusiveMass, true );
            }
            
        }
        //2. look only for the material
        else if(prefix=="" && material!=""){
            //            std::cout<<"Looking for a specific material: "<<material<<std::endl;
            //            std::cout<<"daughterLV->GetMaterial()->GetName(): "<<daughterLV->GetMaterial()->GetName()<<std::endl;
            if (daughterLV->GetMaterial()->GetName().contains(material)){
                calculateMass(daughterLV, daughterPV, jlist, exclusiveMass, true );
            }
            
        }
        //3. look for both
        else if(prefix!="" && material!=""){
            if ((daughterLV->GetName().contains(prefix) || daughterPV->GetName().contains(prefix)) && daughterLV->GetMaterial()->GetName().contains(material)){
                calculateMass(daughterLV, daughterPV, jlist, exclusiveMass, true );
            }
            
        }
        //4. loop on the whole geometry tree, filtering w.r.t density of the material
        else
        {
            //Calculate the mass of volumes that are above a certain density threshold
            if (density > fDensityThreshold)
            {
                //sum of the exclusive masses of all the volumes that have density > threshold
                inclusiveMass+=daughterLV->GetMass(true, false);
                
            }
            else
            {
                //Sum of the ignored mass
                exclusiveMass+=daughterLV->GetMass(true, false);
            }
            
        }
        if (daughterLV->GetNoDaughters()>0){
            iterateFromWorldMass(daughterLV, jlist, inclusiveMass, exclusiveMass,  prefix, material);
        }
        
    }
    
}

void MassCalculator::recursiveMassCalculation (G4VPhysicalVolume* worldg4, GeoVPhysVol* /*worldgeoModel*/, std::vector<json>& jlist){
    
    masscalc::massReport singleMassReport;
    json jSingleMassReport;
    int localNoDaughters = worldg4->GetLogicalVolume()->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<worldg4->GetLogicalVolume()->GetName()<<" is : "<<localNoDaughters<<std::endl;
    G4VPhysicalVolume *daughter;
    G4LogicalVolume *daughterLV;
    G4double cubicVolumeWorld, globalDensityWorld, globalMassWorld;
    
    //Instance of the G4AnalysisManager used to fill the density_histogram
    //    fAnalysisManager = G4AnalysisManager::Instance();
    //    if (!fAnalysisManager->OpenFile("density_histogram")){
    //        G4cout<<"RecursiveMassCalculation ERROR: File cannot be opened!"<<G4endl;
    //        exit(-1);
    //    } else
    //        G4cout<<"\n...output File density_histogram opened!"<<G4endl;
    
    if (fVerbosityFlag>0) {
        std::cout<<"\n========== Printing geometry info ============\n "<<std::endl;
        printGeometryInfo (worldg4->GetLogicalVolume(), fVerbosityFlag);
        std::cout<<"\n========== Printing geometry info: DONE! ============ \n"<<std::endl;
        
    }
    
    //    fHistoID = fAnalysisManager->CreateH1("density", "density", 300, 10e-3, 30, "none", "none", "log");
    //    fAnalysisManager->SetH1Ascii(fHistoID,true);  // misi: always ascii
    //    fAnalysisManager->SetH1XAxisTitle(fHistoID, "density [g/cm3]]");
    
    //Debug information about World volume, Name, Material, Solid name of the LV, density and cubic volume
    std::cout<<"Checking World volume "<<std::endl;
    std::cout<<"-----> World Name is: "<<worldg4->GetName()<<std::endl;
    std::cout<<"-----> WorldLV Name is: "<<worldg4->GetLogicalVolume()->GetName()<< " it has  "<<localNoDaughters<<" daughters."<<std::endl;
    std::cout<<"-----> World Material is: "<<worldg4->GetLogicalVolume()->GetMaterial()<<std::endl;
    std::cout<<"-----> World Solid name is: "<<worldg4->GetLogicalVolume()->GetSolid()->GetName()<<std::endl;
    cubicVolumeWorld = worldg4->GetLogicalVolume()->GetSolid()->GetCubicVolume();
    std::cout<<"-----> World Solid cubic volume is: "<<cubicVolumeWorld/CLHEP::m3<<" m3"<<std::endl;
    std::cout<<"-----> World Solid entity type: "<<worldg4->GetLogicalVolume()->GetSolid()->GetEntityType()<<std::endl;
    globalDensityWorld = worldg4->GetLogicalVolume()->GetMaterial()->GetDensity();
    std::cout<<"-----> World Solid density is: "<<globalDensityWorld/ (CLHEP::g / CLHEP::cm3)<<" [g/cm3]"<<std::endl;
    std::cout<<"-----> World Solid density is: "<<globalDensityWorld/ (CLHEP::kg / CLHEP::m3)<<" [kg/m3]"<<std::endl;
    //    std::cout<<"-----> Minimum density is: "<<0/ (CLHEP::g / CLHEP::cm3)<<" [g/cm3]"<<std::endl;
    //    std::cout<<"-----> Maximum density is: "<<30<<" [g/cm3]"<<std::endl;
    globalMassWorld = globalDensityWorld * cubicVolumeWorld;
    std::cout<<"-----> World mass is: "<< globalMassWorld / (CLHEP::kg) <<" [Kg]"<<std::endl;
    std::cout<<"\n *** --------------- ***\n"<<std::endl;
    
    double inclusiveMassG4 = 0., exclusiveMassG4=0., apparentWeightG4=0.;
    double totalInclusiveMassG4 = 0., totalExclusiveMassG4 =0.;
    //double inclusiveMassGeoModel = 0., exclusiveMassGeoModel = 0.;
    
    
    //CASE 1: DEFAULT BEHAVIOUR
    //No filter is applied -- calculate the inclusiveMass, exclusiveMass and apparentWeigth of the whole geometry
    //For the inclusive and exclusive mass calculation uses a filter on the density of the material
    //Ignoring the materials for which density is < 0.02 g/cm3 (density of the less dense solid - aerogel)
    if(fPrefixLogicalVolume=="" && fMaterial=="")
    {
        //Loop over the daughters of the worls volume - Tree top daughters - only
        for (int n=0; n<localNoDaughters; n++)
        {
            daughter=worldg4->GetLogicalVolume()->GetDaughter(n);
            daughterLV = daughter->GetLogicalVolume();
            std::cout<<"\n****** Checking daughter "<< n+1 <<" out of "<<localNoDaughters<<std::endl;
            std::cout<<"---> Daughter Name is: "<<daughter->GetName()<<std::endl;
            std::cout<<"---> DaughterLV Name is: "<<daughterLV->GetName()<< " it has  "<<daughterLV->GetNoDaughters()<< " daughters" <<std::endl;
            std::cout<<"---> DaughterLV Material is: "<<daughterLV->GetMaterial()<<std::endl;
            std::cout<<"---> DaughterLV Solid name is: "<<daughterLV->GetSolid()->GetName()<<std::endl;
            std::cout<<"---> DaughterLV Solid cubic volume is: "<<daughterLV->GetSolid()->GetCubicVolume()/CLHEP::m3<<" m3"<<std::endl;
            std::cout<<"---> DaughterLV Solid entity type: "<<daughterLV->GetSolid()->GetEntityType()<<std::endl;
            G4double globalDensity = daughterLV->GetMaterial()->GetDensity();
            std::cout<<"---> DaughterLV Solid density is: "<<globalDensity/ (CLHEP::g / CLHEP::cm3)<<" [gr/cm3]"<<std::endl;
            
            //            //GeoModel mass calculation invoked only if the geometry is not in GDML format
            //            //NB exclusive mass concept in GeoModel is different than the Geant4 one
            //            //So do not expect that the exclusive masses of the 2 calculations are the same
            //            if (!fGeometryFileName.contains(".gdml")){
            //                const PVConstLink FSLpv = worldgeoModel->getChildVol(n);
            //                inclusiveMassGeoModel+= inclusiveMass(FSLpv); //real mass of the whole volume, including the real masses of the daughters
            //                //exclusiveMassGeoModel+= exclusiveMass(FSLpv); //mass of the whole volume, as it would not have daughters
            //
            //            }
            
            //Here is already excluding the world volume from the calculation,
            //Being it made by AIR by default
            //calculateMass of each daughter and since the last flag is true, write out results in the json file
            //I call this only once -- no iteration
            //calculateMass(daughterLV, daughter, jlist, inclusiveMassG4, exclusiveMassG4, true);
            
            inclusiveMassG4 = daughterLV->GetMass(true, true);  //real mass of the LV, inclusive of the masses of the daughters
            exclusiveMassG4 = daughterLV->GetMass(true, false); //mass of the LV substracted for the volume occupied by the daughters
            totalInclusiveMassG4+=inclusiveMassG4;
            totalExclusiveMassG4+=exclusiveMassG4;
            
            //fill the singleMassReport struct
            singleMassReport.logicalVolumeName=daughterLV->GetName();
            singleMassReport.physicalVolumeName=daughter->GetName();
            singleMassReport.volumeCopyNo =daughter->GetCopyNo();
            singleMassReport.material =daughterLV->GetMaterial()->GetName();
            singleMassReport.density =daughterLV->GetMaterial()->GetDensity()/SYSTEM_OF_UNITS::g*SYSTEM_OF_UNITS::cm3;
            singleMassReport.volumeEntityType=daughterLV->GetSolid()->GetEntityType();
            singleMassReport.inclusiveMass = inclusiveMassG4/(CLHEP::kg);
            singleMassReport.exclusiveMass = exclusiveMassG4/(CLHEP::kg);
            //std::cout<<"-----> DaughterLV inclusive mass is: "<<singleMassReport.inclusiveMass<<" [kg]"<<std::endl;
            
            //write the singleMassReport in the json file
            to_json(jSingleMassReport, singleMassReport);
            // write prettified JSON to another file
            jlist.push_back(jSingleMassReport);
            
        }
        
        //Calculate the apparentWeight for the whole geometry
        //mass of the LV substracted for the volume occupied by the daughters
        double exclusiveWorld = worldg4->GetLogicalVolume()->GetMass(true, false);
        apparentWeightG4 = exclusiveWorld + totalInclusiveMassG4 - globalMassWorld;
        
        masscalc::finalMassReport finalMassReport;
        json jsonFinalMassReport;
        //fill the finalMassReport struct
        finalMassReport.logicalVolumeName = worldg4->GetLogicalVolume()->GetName();
        finalMassReport.densityThreshold = 0.02;
        finalMassReport.material = worldg4->GetLogicalVolume()->GetMaterial()->GetName();
        finalMassReport.volumeEntityType="World Volume";
        finalMassReport.inclusiveMass = totalInclusiveMassG4/(CLHEP::kg);
        finalMassReport.exclusiveMass = totalExclusiveMassG4/(CLHEP::kg);
        finalMassReport.apparentWeight = apparentWeightG4/(CLHEP::kg);
        
        std::cout<<"\n=== Calculated total masses ==="<<std::endl;
        std::cout<<"Total inclusive mass of the detector is ... "<<totalInclusiveMassG4 / (CLHEP::kg) <<" [kg]."<<std::endl;
        std::cout<<"Total exclusive mass of the detector is ... "<<totalExclusiveMassG4 / (CLHEP::kg) <<" [kg]."<<std::endl;
        std::cout<<"Total apparent weight in Air of the detector is ... "<<apparentWeightG4 / (CLHEP::kg) <<" [kg]."<<std::endl;
        
        //Do the same calculation but with a filter on the density
        double exclusiveFilteredMass=0.;
        double excludedFilteredMass=0.;
        iterateFromWorldMass(worldg4->GetLogicalVolume(),jlist, exclusiveFilteredMass, excludedFilteredMass, fPrefixLogicalVolume, fMaterial);
        
        finalMassReport.exclusiveMassFiltered = exclusiveFilteredMass/(CLHEP::kg);
        finalMassReport.excludedMass = excludedFilteredMass/(CLHEP::kg);
        
        std::cout<<"\n==== Filters by density ===="<<std::endl;
        std::cout<<"Total exclusive mass for Geometry filtered by density ... "<<exclusiveFilteredMass / (CLHEP::kg) <<" [kg]."<<std::endl;
        std::cout<<"Total ignored mass cause below density threshold ... "<<excludedFilteredMass / (CLHEP::kg) <<" [kg]."<<std::endl;
        
        //        //This might be misleading, better to comment it out
        //        if (!fGeometryFileName.contains(".gdml")){
        //            std::cout<<"\nGeoModel: Total inclusive mass of the detector is ... "<<inclusiveMassGeoModel / (SYSTEM_OF_UNITS::kg) <<" [kg]."<<std::endl;
        //            std::cout<<"GeoModel: Total exclusive mass of the detector is ... "<<exclusiveMassGeoModel / (SYSTEM_OF_UNITS::kg) <<" [kg]."<<std::endl;
        //        }
        //
        //write the finalMassReport in the json file
        to_json(jsonFinalMassReport, finalMassReport);
        // write prettified JSON to another file
        jlist.push_back(jsonFinalMassReport);
        
        
    }
    //CASE 2: One of the 2 filters or both are used
    // Iterate on the whole geometry tree, looking for the logical volumes that correspond to the
    // filter research and for those calculate the inclusive and exclusive masses, and write the
    // corresponding filled struct in the json file
    else
    {
        iterateFromWorldMass(worldg4->GetLogicalVolume(),jlist, inclusiveMassG4, exclusiveMassG4, fPrefixLogicalVolume, fMaterial);
        
        //fill the finalMassReport struct
        singleMassReport.logicalVolumeName = fPrefixLogicalVolume;
        //singleMassReport.physicalVolumeName = fPrefixLogicalVolume;
        singleMassReport.volumeCopyNo = worldg4->GetCopyNo();
        singleMassReport.material = fMaterial;
        singleMassReport.volumeEntityType="Total for filtered Geometry";
        singleMassReport.inclusiveMass = -999; //inclusiveMassG4/(CLHEP::kg);
        singleMassReport.exclusiveMass = exclusiveMassG4/(CLHEP::kg);
        
        
        //std::cout<<"\nTotal inclusive mass for the requested Geometry is ... "<<inclusiveMassG4 / (CLHEP::kg) <<" [kg]."<<std::endl;
        std::cout<<"Total exclusive mass for the filtered Geometry is ... "<<exclusiveMassG4 / (CLHEP::kg) <<" [kg]."<<std::endl;
        
        
        //write the finalMassReport in the json file
        to_json(jSingleMassReport, singleMassReport);
        // write prettified JSON to another file
        jlist.push_back(jSingleMassReport);
    }
    
    
    //    fAnalysisManager->Write();
    //    fAnalysisManager->CloseFile();
    
}


void MassCalculator::printGeometryInfo(G4LogicalVolume* lv, G4int verbosity){
    
    int localNoDaughters = lv->GetNoDaughters();
    G4VPhysicalVolume *daughter;
    G4LogicalVolume *daughterLV;
    for (int n=0; n<localNoDaughters; n++)
    {
        daughter=lv->GetDaughter(n);
        daughterLV = daughter->GetLogicalVolume();
        
        std::cout<< "LV_name: ";
        std::cout.width(40); std::cout << std::left <<daughter->GetLogicalVolume()->GetName();
        std::cout << std::left << "\tMaterial:  "<<daughter->GetLogicalVolume()->GetMaterial()->GetName()<<std::endl;
        if (verbosity>1)
            std::cout <<daughter->GetLogicalVolume()->GetMaterial()<<std::endl;
        
        if(daughterLV->GetNoDaughters()>0)
            printGeometryInfo(daughterLV, verbosity);
    }
    
}


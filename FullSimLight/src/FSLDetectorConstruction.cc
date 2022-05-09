#include "FSLDetectorConstruction.hh"

#include <iomanip>

#include "G4Version.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4UniformMagField.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "G4PVPlacement.hh"
//#include "G4LogicalVolumeStore.hh"

#include "FSLDetectorMessenger.hh"
#include "RegionConfigurator.hh"

// Geant4 steppers

#if G4VERSION_NUMBER>=1040
#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4DoLoMcPriRK34.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4RK547FEq1.hh"
#include "G4RK547FEq2.hh"
#include "G4RK547FEq3.hh"
#include "G4TsitourasRK45.hh"
#include "G4VIntegrationDriver.hh"
#include "G4IntegrationDriver.hh"
#endif

#if G4VERSION_NUMBER>=1060
#include "G4InterpolationDriver.hh"
#endif

#include "G4CashKarpRKF45.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4NystromRK4.hh"
#include "G4RKG3_Stepper.hh"

// Geant4 includes
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Version.hh"


// **** INCLUDES for GeoModel
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

#include "MagFieldServices/AtlasFieldSvc.h"
#include "StandardFieldSvc.h"

#include "GeoModelKernel/GeoVolumeCursor.h"

G4AnalysisManager* FSLDetectorConstruction::fAnalysisManager = nullptr;

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



//_____________________________________________________________________________________
double volume(const PVConstLink& pv) {
  const GeoLogVol        * lv       = pv->getLogVol();
  const GeoShape         *shape    = lv->getShape();
  return shape->volume();
}


double exclusiveMass(const PVConstLink& pv) {
  const GeoLogVol* lv       = pv->getLogVol();
  const GeoMaterial      *material = lv->getMaterial();
  double density = material->getDensity();
  return density*volume(pv);
}

double inclusiveMass(const PVConstLink& pv) {

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



G4double FSLDetectorConstruction::gFieldValue = 0.0;

FSLDetectorConstruction::FSLDetectorConstruction() : fWorld(nullptr), fDetectorMessenger(nullptr)
{
  fFieldValue          = 0.0;
  fVerbosityFlag       = -1;
  fGmclashVerbosity    = false;
  fTolerance           = 0.0;
  fFieldConstant       = false;
  fDetectorMessenger   = new FSLDetectorMessenger(this);
  fRunOverlapCheck     = false;
  fRunMassCalculator   = false;
  fAddRegions          = false;
  fDumpGDML            = false;
  fReportFileName      = "gmclash_report.json";
  fMinStep             = 1.0e-2;
  fField.Put(0);
  fOutputGDMLFileName = "geometry.gdml";

}

FSLDetectorConstruction::~FSLDetectorConstruction()
{
  delete fDetectorMessenger;
}

void FSLDetectorConstruction::calculateMass(G4LogicalVolume* logVol, G4VPhysicalVolume * physVol, std::vector<json>& jlist, double& exclusiveMass, bool writeRep){

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

void FSLDetectorConstruction::iterateFromWorldMass(G4LogicalVolume* logVolume, std::vector<json>& jlist, double& inclusiveMass, double& exclusiveMass, G4String prefix, G4String material){

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

bool FSLDetectorConstruction::iterateFromWorld(G4LogicalVolume* envelope, G4VPhysicalVolume* volume, G4ThreeVector& local){

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

G4ThreeVector FSLDetectorConstruction::localToGlobal(G4ThreeVector& local, bool skipFirstIt){

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

void FSLDetectorConstruction::RecursiveMassCalculation (G4VPhysicalVolume* worldg4, GeoPhysVol* /*worldgeoModel*/, std::vector<json>& jlist){

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

void FSLDetectorConstruction::RecursivelyCheckOverlap(G4LogicalVolume* envelope,std::vector<json>& jlist){

    int localNoDaughters = envelope->GetNoDaughters();
    //std::cout<<"Total n. of Daughters of "<<envelope->GetName()<<" is : "<<localNoDaughters<<std::endl;
    for (int sampleNo=0; sampleNo<localNoDaughters; sampleNo++){

        G4VPhysicalVolume *daughter=envelope->GetDaughter(sampleNo);
        if(daughter->GetLogicalVolume()->GetNoDaughters()>0)
                RecursivelyCheckOverlap(daughter->GetLogicalVolume(), jlist);
        //std::cout<<"Starting Overlaps check on daughter: "<<daughter->GetName()<<std::endl;
        //std::cout<<"... "<<sampleNo<<std::endl;
        FSLCheckOverlaps(daughter, jlist, 1000, fTolerance, 1);
    }
}

bool FSLDetectorConstruction::FSLCheckOverlaps(G4VPhysicalVolume* volume, std::vector<json>& jlist, G4int res, G4double tol, G4int maxErr)
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

GeoPhysVol*  FSLDetectorConstruction::CreateTheWorld(GeoPhysVol* world)
{
    if (world == nullptr)
    {
        // Setup the 'World' volume from which everything else will be suspended
        // Get the materials that we shall use.
        // -------------------------------------//
        GeoMaterial* Air=new GeoMaterial("Air", 1.290*SYSTEM_OF_UNITS::mg/SYSTEM_OF_UNITS::cm3);
        GeoElement* Oxigen = new GeoElement("Oxygen",  "O", 8.0, 16.0*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole);
        GeoElement* Nitrogen = new GeoElement("Nitrogen", "N", 7., 14.0067*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole);

//        GeoMaterial* Ether=new GeoMaterial("Ether", 1e-25*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3);
//        GeoElement* vacuum = new GeoElement("vacuum", "Mt", 1, 1);
//        Ether->add(vacuum, 1.0);
//        Ether->lock();

        Air->add(Nitrogen, .8);
        Air->add(Oxigen, .2);
        Air->lock();
        const GeoBox* worldBox = new GeoBox(2000*SYSTEM_OF_UNITS::cm, 2000*SYSTEM_OF_UNITS::cm, 4000*SYSTEM_OF_UNITS::cm);
        const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, Air);
        //const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, Ether);
        world = new GeoPhysVol(worldLog);
    }
    return world;
}

G4VPhysicalVolume *FSLDetectorConstruction::Construct()
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

        G4cout << "ReadGeoModel::buildGeoModel() done." << G4endl;
        fTimer.Stop();
        G4cout << "First step done. GeoModelTree built from the SQLite file." << G4endl;
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
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::GetInvisible());

        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly check your setup selection criteria or input files!" << G4endl;
            G4Exception("FSLDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
        }
        
//        std::vector<G4LogicalVolume*>* lvStore =  G4LogicalVolumeStore::GetInstance();
//        std::size_t nlv=lvStore->size();
//        std::cout<<"G4LogicalVolume store size: "<<nlv<<std::endl;
//        for (std::size_t i=0;  i<nlv; ++i) {
//            G4LogicalVolume* lv = (*lvStore)[i];
//            std::cout<<"G4LogicalVolume store:: "<<lv->GetName()<<std::endl;
//
//        }
        
        G4cout << "Second step done. Geant4 geometry created from GeoModeltree "<<G4endl;
        G4cout << "Detector Construction from the plugin file " << fGeometryFileName.data() <<", done!"<<G4endl;

    }
    else if (fGeometryFileName.contains(".db")){
        G4cout << "Building the detector from the SQLite file: "<<fGeometryFileName<<G4endl;

        // open the DB
        GMDBManager* db = new GMDBManager(fGeometryFileName.data());
        /* Open database */
        if (db->checkIsDBOpen()) {
            G4cout << "Database is open!" << G4endl;
        }
        else{
            G4cout << "ERROR! Database is not open." << G4endl;
            // return;
            throw;

        }

        // -- testing the input database
        //std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
        //db->printAllMaterials();
        /* setup the GeoModel reader */
        GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
        G4cout << "ReadGeoModel set.";


        /* build the GeoModel geometry */
        //GeoPhysVol* world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
        world = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory and get an handle to the 'world' volume
        G4cout << "ReadGeoModel::buildGeoModel() done." << G4endl;
        fTimer.Stop();
        G4cout << "First step done. GeoModelTree built from the SQLite file." << G4endl;
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
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::GetInvisible());

        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly check your setup selection criteria or GDML input!" << G4endl;
            G4Exception("FSLDetectorConstruction::Construct()", "FULLSIMLIGHT_0000", FatalException, ed);
        }
        G4cout << "Second step done. Geant4 geometry created from GeoModeltree "<<G4endl;
        G4cout << "Detector Construction from the SQLite file " << fGeometryFileName.data() <<", done!"<<G4endl;
        
//        std::vector<G4LogicalVolume*>* lvStore =  G4LogicalVolumeStore::GetInstance();
//        std::size_t nlv=lvStore->size();
//        std::cout<<"G4LogicalVolume store size: "<<nlv<<std::endl;
//        for (std::size_t i=0;  i<nlv; ++i) {
//            G4LogicalVolume* lv = (*lvStore)[i];
//            std::cout<<"G4LogicalVolume store:: "<<lv->GetName()<<std::endl;
//
//        }
    }

    else if (fGeometryFileName.contains(".gdml")){
        G4cout << "Building the detector from the GDML file: "<<fGeometryFileName<<G4endl;
        //fParser.SetOverlapCheck(true);
        fParser.Read(fGeometryFileName, false); // turn off schema checker
        fWorld = (G4VPhysicalVolume *)fParser.GetWorldVolume();
        fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::GetInvisible());

        //RecursivelyCheckOverlap(fWorld->GetLogicalVolume());
        envelope = fWorld->GetLogicalVolume();

        if (fWorld == 0) {
            G4ExceptionDescription ed;
            ed << "World volume not set properly! Check your setup selection criteria or the GDML input!" << G4endl;
            G4Exception("FSLDetectorConstruction::Construct()", "FULLSIMLIGHT_0001", FatalException, ed);
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

    if(fRunMassCalculator)
    {
        G4cout<<"\n=================== Starting mass calculation...   =================== "<<G4endl;

        std::cout<<"Calculating the mass of the total detector... "<<fGeometryFileName<<std::endl;
        fTimer.Start();
        std::vector<json> jlist;
        RecursiveMassCalculation(fWorld,world, jlist);


        fTimer.Stop();
        G4cout << "\n**** Real time elapsed for mass calculation  : "<<fTimer.GetRealElapsed()/60   << " min. "<< G4endl;
        G4cout << "**** User time elapsed for mass calculation   : " <<fTimer.GetUserElapsed()/60   << " min. "<<G4endl;
        G4cout << "**** System time elapsed for mass calculation : " <<fTimer.GetSystemElapsed()/60 << " min. "<<G4endl;

        json jReport={{"MassReport",jlist}};
        std::cout<<"\n**** Writing out the mass report file: "<<fReportFileName<<std::endl;
        std::ofstream outJsonFile(fReportFileName);
        outJsonFile << std::setw(4) << jReport << std::endl;
        outJsonFile.close();
        G4cout<<"\n=================== Mass calculation...DONE!   =================== "<<G4endl;
    }

    // trying to add detector regions with the configurations given in the RegionConfigurator
    if (fAddRegions)  {
        G4cout << "\n ===================  Trying to add detector regions ... ================== \n" << G4endl;
        RegionConfigurator::Instance().CreateRegions(1);
        G4cout << "\n ===================  Adding detector regions is DONE!  ================== \n" << G4endl;
    }

    if (fDumpGDML){

        G4cout << "\n ===================  Dump geometry in GDML format  =================== \n" << G4endl;
        //G4GDMLParser parser;
        //fParser.SetRegionExport(true);
        //fParser.SetEnergyCutsExport(true);
        PullUnidentifiedVolumes(fWorld->GetLogicalVolume());
        G4cout << "\n" << G4endl;
        fParser.Write(fOutputGDMLFileName, fWorld->GetLogicalVolume());
        G4cout << "\n =================== Geometry exported in GDML, DONE!  =================== \n" << G4endl;
        exit(0);
    }

    return fWorld;
}

void FSLDetectorConstruction::ConstructSDandField()
{
 // if (std::abs(fFieldValue) > 0.0) {

    if (fFieldConstant && std::abs(fFieldValue) > 0.0){
    // Apply a global uniform magnetic field along the Z axis.
    // Notice that only if the magnetic field is not zero, the Geant4
    // transportation in field gets activated.
    auto uniformMagField     = new G4UniformMagField(G4ThreeVector(0.0, 0.0, fFieldValue));
    G4FieldManager *fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(uniformMagField);
    fieldMgr->CreateChordFinder(uniformMagField);
    G4cout << G4endl << " *** SETTING UNIFORM MAGNETIC FIELD : fieldValue = " << fFieldValue / tesla << " Tesla *** " << G4endl
           << G4endl;

    }
    else if (fFieldConstant && fFieldValue == 0.0 ){
        G4cout << G4endl << " *** MAGNETIC FIELD IS OFF  *** " << G4endl << G4endl;
    }
    else // if (!fFieldConstant)
    {
      G4cout << G4endl << " *** MAGNETIC FIELD SET FROM FILE  *** " << G4endl << G4endl;
      if (fField.Get() == 0)
      {
          StandardFieldSvc* FSLMagField = new StandardFieldSvc("StandardFieldSvc");
          G4MagneticField* g4Field =  FSLMagField->getField();
          if(g4Field==nullptr) std::cout<<"Error, g4Field is null!"<<std::endl;
          fField.Put(g4Field);

          //This is thread-local
          G4FieldManager* fieldMgr =
          G4TransportationManager::GetTransportationManager()->GetFieldManager();
          G4cout<< "DeltaStep "<<fieldMgr->GetDeltaOneStep()/mm <<"mm" <<G4endl;
          //G4ChordFinder *pChordFinder = new G4ChordFinder(FSLmagField);

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
#if G4VERSION_NUMBER>=1040
//=============================================================================
// Create the driver with a stepper
//=============================================================================
G4VIntegrationDriver*
FSLDetectorConstruction::createDriverAndStepper(std::string stepperType) const
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
        }  else if (stepperType=="CashKarpRKF45") {
            G4CashKarpRKF45* stepper = new G4CashKarpRKF45(eqRhs);
            driver = new G4IntegrationDriver<G4CashKarpRKF45>(
                                                              fMinStep, stepper, stepper->GetNumberOfVariables());
        }
        else if (stepperType=="DoLoMcPriRK34") {
            G4DoLoMcPriRK34* stepper = new G4DoLoMcPriRK34(eqRhs);
        driver = new G4IntegrationDriver<G4DoLoMcPriRK34>(
                                                          fMinStep, stepper, stepper->GetNumberOfVariables());

        } else if (stepperType=="BogackiShampine23") {
            G4BogackiShampine23* stepper = new G4BogackiShampine23(eqRhs);
            driver = new G4IntegrationDriver<G4BogackiShampine23>(fMinStep, stepper, stepper->GetNumberOfVariables());

        } else if (stepperType=="BogackiShampine45") {
            G4BogackiShampine45* stepper = new G4BogackiShampine45(eqRhs);
            driver = new G4IntegrationDriver<G4BogackiShampine45>(
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
        }
        else if (stepperType=="TsitourasRK45") {
            G4TsitourasRK45* stepper = new G4TsitourasRK45(eqRhs);
            driver = new G4IntegrationDriver<G4TsitourasRK45>(
                                                          fMinStep, stepper, stepper->GetNumberOfVariables());

        }
        else if (stepperType=="RKG3_Stepper") {
            G4RKG3_Stepper* stepper = new G4RKG3_Stepper(eqRhs);
            driver = new G4IntegrationDriver<G4RKG3_Stepper>(
                                                             fMinStep, stepper, stepper->GetNumberOfVariables());
        }
#if G4VERSION_NUMBER>=1060
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
#endif

//=============================================================================
// Create the stepper (Geant4 < 10.4)
//=============================================================================
G4MagIntegratorStepper*
FSLDetectorConstruction::CreateStepper(std::string name, G4MagneticField* field) const
{
    std::cout<<"Stepper type is : "<<name<<std::endl;
    //ATH_MSG_DEBUG("getStepper");
    G4Mag_EqRhs* eqRhs(nullptr);
    //if (!m_equationOfMotion.empty())

   //{
    //    eqRhs = m_equationOfMotion->makeEquationOfMotion(field);
    //    ATH_MSG_INFO("Configuring alternative equation of motion using " <<
    //                 m_equationOfMotion.name() );
    //}
    //else
    //{
    //    ATH_MSG_VERBOSE("Using G4Mag_UsualEqRhs as the equation of motion.");
    // @TODO Check that this is the  default equation of motion in ATLAS
    eqRhs = new G4Mag_UsualEqRhs(field);
    //}
    // @TODO Add some way of confirming that the choices of Equation of
    // motion and stepper are compatible.
    // @TODO consider moving the stepper choice into a tool as well?
    // Only worthwhile if we envisage adding more stepper choices in the
    // future.
    if (name=="HelixImplicitEuler") return new G4HelixImplicitEuler(eqRhs);
    else if (name=="HelixSimpleRunge") return new G4HelixSimpleRunge(eqRhs);
    else if (name=="HelixExplicitEuler") return new G4HelixExplicitEuler(eqRhs);
    else if (name=="NystromRK4") return new G4NystromRK4(eqRhs);
    else if (name=="ClassicalRK4") return new G4ClassicalRK4(eqRhs);
    else if (name=="AtlasRK4") {
        std::string g4tag = G4VERSION_TAG;
        if (g4tag.find("atlas") != std::string::npos){

            std::cout<<"Setting AtlasRK4 stepper, in the OLD style"<<std::endl;
            std::cout<<"CAVEAT: only available with atlas patches of Geant4!"<<std::endl;
#ifdef G4ATLAS
            return new G4AtlasRK4(eqRhs);
#endif
        } else {

            std::cout<<"ERROR! AtlasRK4 is only available with atlas patches of Geant4!"<<std::endl;
            std::cout<<"Returning ClassicalRK4!"<<std::endl;
            return new G4ClassicalRK4(eqRhs);

        }
    }
#if G4VERSION_NUMBER>=1040
    else if (name=="BogackiShampine23") return new G4BogackiShampine23(eqRhs);
    else if (name=="BogackiShampine45") return new G4BogackiShampine45(eqRhs);
    else if (name=="CashKarpRKF45") return new G4CashKarpRKF45(eqRhs);
    else if (name=="DoLoMcPriRK34") return new G4DoLoMcPriRK34(eqRhs);
    else if (name=="DormandPrince745") return new G4DormandPrince745(eqRhs);
    else if (name=="DormandPrinceRK56") return new G4DormandPrinceRK56(eqRhs);
    else if (name=="DormandPrinceRK78") return new G4DormandPrinceRK78(eqRhs);
    else if (name=="RK547FEq1") return new G4RK547FEq1(eqRhs);
    else if (name=="RK547FEq2") return new G4RK547FEq2(eqRhs);
    else if (name=="RK547FEq3") return new G4RK547FEq3(eqRhs);
    else if (name=="RKG3_Stepper") return new G4RKG3_Stepper(eqRhs);
    else if (name=="TsitourasRK45") return new G4TsitourasRK45(eqRhs);
#endif
    std::cout<<"ERROR: Stepper " << name << " not available! returning NystromRK4!"<<std::endl;
    return new G4NystromRK4(eqRhs);
}


void FSLDetectorConstruction::PullUnidentifiedVolumes( G4LogicalVolume* v ){

    if (v==0) return;
    std::vector<G4VPhysicalVolume*> pv_to_remove;
    for (size_t i=0;i<v->GetNoDaughters();++i){

        G4VPhysicalVolume * n_v = v->GetDaughter(i);

        if (n_v->GetName() == "LAr::EMEC::Pos::InnerWheel" ||
            n_v->GetName() == "LAr::EMEC::Neg::InnerWheel" ||
            n_v->GetName() == "LAr::EMEC::Pos::OuterWheel" ||
            n_v->GetName() == "LAr::EMEC::Neg::OuterWheel" ||
            n_v->GetName() == "LAr::EMEC::Pos::InnerCone"  ||
            n_v->GetName() == "LAr::EMEC::Neg::InnerCone"  ||
            n_v->GetName() == "LAr::EMEC::Pos::OuterFrontCone" ||
            n_v->GetName() == "LAr::EMEC::Neg::OuterFrontCone" ||
            n_v->GetName() == "LAr::EMEC::Pos::OuterBackCone"  ||
            n_v->GetName() == "LAr::EMEC::Neg::OuterBackCone"  ||
            n_v->GetName() == "LAr::EMEC::Pos::InnerSlice00"   ||
            n_v->GetName() == "LAr::EMEC::Neg::InnerSlice00"   ||
            n_v->GetName() == "LAr::EMEC::Pos::OuterSlice00"   ||
            n_v->GetName() == "LAr::EMEC::Neg::OuterSlice00"   ||
            n_v->GetName() == "UnidentifiedShape"){
            // This is one to remove
            std::cout<<" !REMOVING: "<< n_v->GetName()<<", shape is not supported in GDML!"<<std::endl;
            pv_to_remove.push_back(n_v);
        } else {
            // Recurse
            PullUnidentifiedVolumes( n_v->GetLogicalVolume() );
        }
    }
    for (unsigned int j=0;j<pv_to_remove.size();++j){
        v->RemoveDaughter( pv_to_remove[j] );
    }


}

void FSLDetectorConstruction::printGeometryInfo(G4LogicalVolume* lv, G4int verbosity){

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

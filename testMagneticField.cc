
//--------------------------------------------------------
// testMagneticField application: 8 May 2020
// @author: Marilena Bandieramonte
// @email: marilena.bandieramonte@cern.ch
//--------------------------------------------------------

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>
#include "G4Timer.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4MagneticField.hh"
#include "Randomize.hh"
#include "g4analysis.hh"
#include "fstream"

#include "MagFieldServices/AtlasFieldSvc.h"
#include "StandardFieldSvc.h"

//#include "g4root.hh"

static bool parSolenoidOff = false;
static bool parToroidsOff = false;
static bool parIsAscii = true;
static std::string parMagFieldFile = "";

void GetInputArguments(int argc, char** argv);
void Help();

bool debug = false;

int main(int argc, char** argv) {
    
    // Get input arguments
    GetInputArguments(argc, argv);
    
    G4cout << "========================================================" << G4endl;
    G4cout << "======           testMagneticField              ========" << G4endl;
    G4cout << "========================================================" << G4endl;
    
    //choose the Random engine: set to MixMax explicitely (default form 10.4)
    G4Random::setTheEngine(new CLHEP::MixMaxRng);
    // set seed and print info
    G4Random::setTheSeed(12345678);
    G4cout << G4endl
    << " ===================================================== "      << G4endl
    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
    << " ===================================================== "      << G4endl
    << G4endl;
    
    G4String mname("G4_Random");              // material
    G4String hname = "hist_" + mname;
    //G4int stepsPhi = 1000;
    G4int stepsZ = 1000;
    //G4int stepsR = 100;
    G4int stepsX = 1000;
    G4int stepsY = 1000;
    G4double m_maxZ = 15*m, m_minZ = -15 *m;
    //G4double m_minR = 0.  , m_maxR = 12.5*m;
    G4double m_minX = -10*m, m_maxX = 10*m;
    G4double m_minY = -10*m, m_maxY = 10*m;
    G4double xyzt[3];
    G4double field[3];
    G4double deriv[3];
    G4bool useFullField = true;
    //G4bool useFastField = true;
    G4bool derivatives = false;
    
    //
    //    // Create (or get) analysis reader
    //    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    //    analysisReader->SetVerboseLevel(1);
    //
    //    // Define a base file name
    //    analysisReader->SetFileName("full_bfieldmap_7730_20400_14m_version5.root ");
    //    //analysisReader->SetFileName("ATLAS_BField_toroidsOff.root");
    //    int counter = 0;
    //    // Read ntuple
    //    std::cout.precision(8);
    //    std::cout<<"Getting BFieldMap"<<std::endl;
    //    G4int ntupleId =analysisReader->GetNtuple("BFieldMap");
    //    //exit(-1);
    //
    //   // G4int GetNtuple(const G4String& ntupleName, const G4String& fileName = "", const G4String& dirName = "");
    //
    //    std::cout<<"DONE!"<<std::endl;
    //    if ( ntupleId >= 0 ) {
    //        G4int id;
    //        G4double zmin,zmax,rmin,rmax;
    //
    //        analysisReader->SetNtupleIColumn("id", id);
    //        analysisReader->SetNtupleDColumn("zmin", zmin);
    //        analysisReader->SetNtupleDColumn("zmax", zmax);
    //        analysisReader->SetNtupleDColumn("rmin", rmin);
    //        analysisReader->SetNtupleDColumn("rmax",rmax);
    //
    //        G4cout << "Ntuple x, reading selected column Labs" << G4endl;
    //        while ( analysisReader->GetNtupleRow() ) {
    //            //G4cout << counter++ << "th entry: "
    //            G4cout << "  id: " << id << "\tzmin: " << zmin <<"\tzmax: " << zmax <<std::endl;
    //            G4cout << "  rmin: " << rmin << "\trmax: " << rmax <<std::endl;        }
    //    }
    //    exit(-1);
    
    
    G4String baseName = "ATLAS_BField";
    MagField::AtlasFieldSvc* myMagField = nullptr;
    
    if(parMagFieldFile!="")
    {
        myMagField = new MagField::AtlasFieldSvc(parMagFieldFile, parIsAscii);
        baseName = baseName + "_custom";
    }
    else{
        myMagField = new MagField::AtlasFieldSvc(parIsAscii);
        baseName = baseName + "_default";
    }
    if(parIsAscii && (parSolenoidOff||parToroidsOff))
    {
        std::cout<<"WARNING: it's not possible to switch off the solenoid or toroids when using ascii file. \nActivating Solenoid and Toroids.\n"<<std::endl;
        
    }else
    {
        if (parSolenoidOff) {
            baseName = baseName+"_solenoidOff";
            myMagField->SetUseSolenoidCurrent(0);
            std::cout<<"Solenoid current set to zero"<<std::endl;
            
        }
        if (parToroidsOff) {
            baseName = baseName+"_toroidsOff";
            myMagField->SetUseToroidsCurrent(0);
            std::cout<<"Toroids current set to zero"<<std::endl;
        }
        
    }
    
    G4String root_fileName = baseName + ".root";
    G4AnalysisManager* analysisManager = G4Analysis::ManagerInstance("root");
    analysisManager->SetVerboseLevel(1);
//    G4String txt_fileName = baseName + ".txt";
//    std::ofstream output(txt_fileName, std::ios::out);
//
//    std::cout<<"\nThe magnetic field map will be written in: \n\t"<<root_fileName<<std::endl;
//    std::cout<<"\t"<<txt_fileName<<"\n"<<std::endl;
    
    if (!analysisManager->OpenFile(root_fileName)){
        std::cout<<"\nERROR: File cannot be opened!"<<std::endl;
        exit(-1);
    }
//    if ( ! output ) {
//        std::cout << "\nERROR: File cannot be opened! File name is not defined."<<std::endl;
//        exit(-1);
//    }
//
//    output.setf( std::ios::scientific, std::ios::floatfield );
//    output << "\t    X \t\t   Y \t\t   Z \t\t   R \t\t   BField" << G4endl;
    
    analysisManager->CreateNtuple("xyzr", "xyzfieldmap");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("r");
    analysisManager->CreateNtupleDColumn("Bx");
    analysisManager->CreateNtupleDColumn("By");
    analysisManager->CreateNtupleDColumn("Bz");
    analysisManager->FinishNtuple();
    
    //create 2D xy histograms at z = 0,5,10 meters
    G4int h2ID_z0 = analysisManager->CreateH2("TH2_xy_z0", "TH2_xy_z0",stepsX, m_minX/m,m_maxX/m,stepsY, m_minY/m, m_maxY/m);
    G4int h2ID_z5 = analysisManager->CreateH2("TH2_xy_z5", "TH2_xy_z5",stepsX, m_minX/m,m_maxX/m,stepsY, m_minY/m, m_maxY/m);
    G4int h2ID_z10 = analysisManager->CreateH2("TH2_xy_z10", "TH2_xy_z10",stepsX, m_minX/m,m_maxX/m,stepsY, m_minY/m, m_maxY/m);
    //create 2D zy histograms at x = 0,3 meters
    G4int h2ID_x0 = analysisManager->CreateH2("TH2_zy_x0", "TH2_zy_x0",stepsZ, m_minZ/m, m_maxZ/m, stepsY, m_minY/m,m_maxY/m);
    G4int h2ID_x3 = analysisManager->CreateH2("TH2_zy_x3", "TH2_zy_x3",stepsZ, m_minZ/m, m_maxZ/m, stepsY, m_minY/m,m_maxY/m);
    
    //G4int h3ID = analysisManager->CreateH3("TH3_xyz", "TH3_xyz",stepsX, m_minX/m,m_maxX/m,stepsY, m_minY/m, m_maxY/m,stepsZ, m_minZ/m,m_maxZ/m);
    
//    CLHEP::HepRandomEngine*     rndmEngineMod;
//    rndmEngineMod = G4Random::getTheEngine();
    
    //Initialize the magnetic field
    myMagField->handle();
    
    //Write the v6 magnetic fiels maps root files in a format that can be read back
    //by the G4AnalysisReader
    //    if (!parIsAscii){
    //        if (!parSolenoidOff && !parToroidsOff)
    //            myMagField->writeMap("full_bfieldmap_7730_20400_14m_version5.root");
    //        else if(parToroidsOff)
    //            myMagField->writeMap("solenoid_bfieldmap_7730_0_14m_version5.root");
    //        else if (parSolenoidOff)
    //            myMagField->writeMap("toroid_bfieldmap_0_20400_14m_version5.root");
    //        exit(-1);
    //    }
    
    double fieldIntensity;
    
    std::cout<<"\nGenerating Magnetic Field maps ... hold on\n"<<std::endl;
    //iterate over phi,Z and R
    //    for ( int k = 0; k < stepsPhi; k++ ) { // loop over phi
    //        if(k%(stepsPhi/10)==0){
    //
    //            std::cout<<"[Progress "<<k/(stepsPhi/100)+10<<"%] ...."<<std::endl;
    //
    //        }
    //        double phi = 2.*M_PI*double(k)/double(stepsPhi);
    //        for ( int j = 0; j < stepsZ; j++ ) { // loop over Z
    //            xyzt[2] = (m_maxZ-m_minZ)*double(j)/double(stepsZ-1) + m_minZ;
    //            for ( int i = 0; i < stepsR; i++ ) { // loop over R
    //                double r = (m_maxR-m_minR)*double(i)/double(stepsR-1) + m_minR;
    //                xyzt[0] = r*cos(phi);
    //                xyzt[1] = r*sin(phi);
    //
    //                // compute the field
    //                if ( useFullField ) {
    //                    if ( derivatives )
    //                        myMagField->getField( xyzt, field, deriv );
    //                    else
    //                        myMagField->getField( xyzt, field, 0 );
    //                }
    ////                if ( useFastField ) {
    ////                    if ( derivatives )
    ////                        myMagField->getFieldZR( xyzt, fieldZR, derivZR );
    ////                    else
    ////                        myMagField->getFieldZR( xyzt, fieldZR, 0 );
    ////                }
    //
    //                fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
    //                analysisManager->FillNtupleDColumn(0, xyzt[0]);
    //                analysisManager->FillNtupleDColumn(1, xyzt[1]);
    //                analysisManager->FillNtupleDColumn(2, xyzt[2]);
    //                analysisManager->FillNtupleDColumn(3, r);
    //                analysisManager->FillNtupleDColumn(4, field[0]);
    //                analysisManager->FillNtupleDColumn(5, field[1]);
    //                analysisManager->FillNtupleDColumn(6, field[2]);
    //                analysisManager->AddNtupleRow();
    //
    //                output << "  " << xyzt[0]/m << "\t"
    //                << xyzt[1]/m << "\t"
    //                << xyzt[2]/m << "\t"
    //                << r/m << "\t"
    //                << fieldIntensity/tesla << G4endl;
    //
    //                if (debug){
    //                    std::cout<<" x:  "<<xyzt[0]/m<<" y:  "<<xyzt[1]/m<<" z:  "<<xyzt[2]/m<<" field: "<<fieldIntensity/tesla<<std::endl;
    //                    std::cout<<" r:  "<<r/m<<" z:  "<<xyzt[2]/m<<" phi:  "<<phi<<" field: "<<fieldIntensity/tesla<<std::endl;
    //                }
    //            }
    //        }
    //    }
    
    //iterate over x,y and z
    //    for ( int k = 0; k < stepsZ; k++ )
    //    { // loop over z
    //        if(k%(stepsZ/10)==0){
    //            std::cout<<"[Progress with loop over xyz "<<k/(stepsZ/100)+10<<"%] ...."<<std::endl;
    //
    //        }
    
    //fill 2D xy histogram at z = 0 meters
    xyzt[2] = 0;
    for ( int j = 0; j < stepsY; j++ )
    { // loop over Y
        xyzt[1] = (m_maxY-m_minY)*double(j)/double(stepsY-1) + m_minY;
        for ( int i = 0; i < stepsX; i++ )
        {
            // loop over X
            xyzt[0] = (m_maxX-m_minX)*double(i)/double(stepsX-1) + m_minX;
            // compute the field
            if ( useFullField ) {
                if ( derivatives )
                    myMagField->getField( xyzt, field, deriv );
                else
                    myMagField->getField( xyzt, field, 0 );
            }
            fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
            
            analysisManager->FillH2(h2ID_z0, xyzt[0]/m, xyzt[1]/m,fieldIntensity/tesla);
            //  analysisManager->FillH3(h3ID, xyzt[0]/m, xyzt[1]/m,xyzt[2]/m,fieldIntensity/tesla);
        }
        
    }
    //fill 2D xy histogram at z = 5 meters
    xyzt[2] = 5*m;
    for ( int j = 0; j < stepsY; j++ )
    { // loop over Y
        xyzt[1] = (m_maxY-m_minY)*double(j)/double(stepsY-1) + m_minY;
        for ( int i = 0; i < stepsX; i++ )
        {
            // loop over X
            xyzt[0] = (m_maxX-m_minX)*double(i)/double(stepsX-1) + m_minX;
            // compute the field
            if ( useFullField ) {
                if ( derivatives )
                    myMagField->getField( xyzt, field, deriv );
                else
                    myMagField->getField( xyzt, field, 0 );
            }
            fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
            
            analysisManager->FillH2(h2ID_z5, xyzt[0]/m, xyzt[1]/m,fieldIntensity/tesla);
            // analysisManager->FillH3(h3ID, xyzt[0]/m, xyzt[1]/m,xyzt[2]/m,fieldIntensity/tesla);
        }
        
    }
    //fill 2D xy histogram at z = 0 meters
    xyzt[2] = 10*m;
    for ( int j = 0; j < stepsY; j++ )
    { // loop over Y
        xyzt[1] = (m_maxY-m_minY)*double(j)/double(stepsY-1) + m_minY;
        for ( int i = 0; i < stepsX; i++ )
        {
            // loop over X
            xyzt[0] = (m_maxX-m_minX)*double(i)/double(stepsX-1) + m_minX;
            // compute the field
            if ( useFullField ) {
                if ( derivatives )
                    myMagField->getField( xyzt, field, deriv );
                else
                    myMagField->getField( xyzt, field, 0 );
            }
            fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
            
            analysisManager->FillH2(h2ID_z10, xyzt[0]/m, xyzt[1]/m,fieldIntensity/tesla);
            // analysisManager->FillH3(h3ID, xyzt[0]/m, xyzt[1]/m,xyzt[2]/m,fieldIntensity/tesla);
        }
        
    }
    //create 2D zy histogram at x = 0 meters
    xyzt[0] = 0.*m;
    for ( int j = 0; j < stepsY; j++ )
    { // loop over Y
        xyzt[1] = (m_maxY-m_minY)*double(j)/double(stepsY-1) + m_minY;
        for ( int i = 0; i < stepsZ; i++ )
        {
            // loop over X
            xyzt[2] = (m_maxZ-m_minZ)*double(i)/double(stepsZ-1) + m_minZ;
            // compute the field
            if ( useFullField ) {
                if ( derivatives )
                    myMagField->getField( xyzt, field, deriv );
                else
                    myMagField->getField( xyzt, field, 0 );
            }
            fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
            
            analysisManager->FillH2(h2ID_x0, xyzt[2]/m, xyzt[1]/m,fieldIntensity/tesla);
            //analysisManager->FillH3(h3ID, xyzt[0]/m, xyzt[1]/m,xyzt[2]/m,fieldIntensity/tesla);
        }
        
    }
    //fill 2D zy histogram at x = 3 meters
    xyzt[0] = 3*m;
    for ( int j = 0; j < stepsY; j++ )
    {   // loop over Y
        xyzt[1] = (m_maxY-m_minY)*double(j)/double(stepsY-1) + m_minY;
        for ( int i = 0; i < stepsZ; i++ )
        {
            // loop over X
            xyzt[2] = (m_maxZ-m_minZ)*double(i)/double(stepsZ-1) + m_minZ;
            // compute the field
            if ( useFullField ) {
                if ( derivatives )
                    myMagField->getField( xyzt, field, deriv );
                else
                    myMagField->getField( xyzt, field, 0 );
            }
            fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
            
            analysisManager->FillH2(h2ID_x3, xyzt[2]/m, xyzt[1]/m,fieldIntensity/tesla);
            //analysisManager->FillH3(h3ID, xyzt[0]/m, xyzt[1]/m,xyzt[2]/m,fieldIntensity/tesla);
        }
        
    }
    
    //}
    
    analysisManager->Write();
    analysisManager->CloseFile();
    
//    G4cout << "... write txt file : "<<txt_fileName<<" - done" << G4endl;
//    output.close();
//    G4cout << "... close txt file : "<<txt_fileName<<" - done" << G4endl;
    
    G4cout << "\n###### End of test #####" << G4endl;
    return 0;
}

static struct option options[] = {
    {"magneticFieldFile", required_argument, 0, 'f'},
    {"isRoot"           , no_argument      , 0, 'r'},
    {"solenoidOff"      , no_argument      , 0, 's'},
    {"toroidsOff "      , no_argument      , 0, 't'},
    {"help"             , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};


void Help() {
    std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
    G4cout <<"  testMagneticField application.    \n"
    << std::endl
    <<"  **** Parameters: \n\n"
    <<"      -f :  (optional) magnetic field filename [.data/.root]   (default : use ATLAS magnetic field maps)\n"
    <<"      -r :  (flag) use root field map (default : false, use ascii file)\n"
    <<"      -s :  (flag) set Solenoid Off \n"
    <<"      -t :  (flag) set Toroids Off \n"
    << std::endl;
    
    std::cout <<"\nUsage: ./testMagneticField [OPTIONS]\n" <<std::endl;
    for (int i=0; options[i].name!=NULL; i++) {
        printf("\t-%c  --%s\t\n", options[i].val, options[i].name);
    }
    std::cout<<"\n "<<std::setw(100)<<std::setfill('=')<<""<<std::setfill(' ')<<std::endl;
}


void GetInputArguments(int argc, char** argv) {
    while (true) {
        int c, optidx = 0;
        c = getopt_long(argc, argv, "f:strh", options, &optidx);
        if (c == -1)
            break;
        //
        switch (c) {
            case 0:
                c = options[optidx].val;
                break;
            case 'f':
                parMagFieldFile = optarg;
                break;
            case 's':
                parSolenoidOff = true;
                break;
            case 't':
                parToroidsOff  = true;
                break;
            case 'r':
                parIsAscii     = false;
                break;
            case 'h':
                Help();
                exit(0);
            default:
                Help();
                errx(1, "unknown option %c", c);
        }
    }
}


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

static bool parSolenoidOff = false;
static bool parToroidsOff = false;

void GetInputArguments(int argc, char** argv);
void Help();

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
    G4int stepsPhi = 1000;
    G4int stepsZ = 100;
    G4int stepsR = 100;
    G4double m_maxZ = 23*m, m_minZ = -23*m, m_minR = 0., m_maxR = 12.5*m;
    G4double xyzt[3];
    G4double field[3];
    G4double deriv[3];
    G4bool useFullField = true;
    //G4bool useFastField = true;
    G4bool derivatives = false;
    
    G4String baseName = "ATLAS_BField";
    
    if(parSolenoidOff) baseName = baseName+"_solenoidOff";
    if(parToroidsOff)  baseName = baseName+"_toroidsOff";
    
    G4String root_fileName = baseName + ".root";
    G4AnalysisManager* analysisManager = G4Analysis::ManagerInstance("root");
    analysisManager->SetVerboseLevel(1);
    if (analysisManager->OpenFile(root_fileName)){

        std::cout<<"File opened!"<<std::endl;

    } else std::cout<<"ERROR File cannot be opened!"<<std::endl;
    
    G4String txt_fileName = baseName + ".txt";
    std::ofstream output(txt_fileName, std::ios::out);
    if ( ! output ) {
        std::cout << "Cannot open file. File name is not defined."<<std::endl;
        exit(-1);
    }
    output.setf( std::ios::scientific, std::ios::floatfield );
    output << "\t    X \t\t   Y \t\t   Z \t\t   R \t\t   BField" << G4endl;
    
    analysisManager->CreateNtuple("xyz", "xyzfieldmap");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("r");
    analysisManager->CreateNtupleDColumn("Bx");
    analysisManager->CreateNtupleDColumn("By");
    analysisManager->CreateNtupleDColumn("Bz");
    analysisManager->FinishNtuple();
 
    CLHEP::HepRandomEngine*     rndmEngineMod;
    rndmEngineMod = G4Random::getTheEngine();
    
    MagField::AtlasFieldSvc* myMagField = new MagField::AtlasFieldSvc("StandardFieldSvc");
    if (parSolenoidOff) {
        myMagField->SetUseSolenoidCurrent(0);
        std::cout<<"Solenoid current set to zero"<<std::endl;
    }
    if (parToroidsOff) {
        myMagField->SetUseToroidsCurrent(0);
        std::cout<<"Toroids current set to zero"<<std::endl;
    }
    
    myMagField->handle();
    double fieldIntensity;
    for ( int k = 0; k < stepsPhi; k++ ) { // loop over phi
        double phi = 2.*M_PI*double(k)/double(stepsPhi);
        for ( int j = 0; j < stepsZ; j++ ) { // loop over Z
            xyzt[2] = (m_maxZ-m_minZ)*double(j)/double(stepsZ-1) + m_minZ;
            for ( int i = 0; i < stepsR; i++ ) { // loop over R
                double r = (m_maxR-m_minR)*double(i)/double(stepsR-1) + m_minR;
                xyzt[0] = r*cos(phi);
                xyzt[1] = r*sin(phi);
                
                // compute the field
                if ( useFullField ) {
                    if ( derivatives )
                        myMagField->getField( xyzt, field, deriv );
                    else
                        myMagField->getField( xyzt, field, 0 );
                }
//                if ( useFastField ) {
//                    if ( derivatives )
//                        myMagField->getFieldZR( xyzt, fieldZR, derivZR );
//                    else
//                        myMagField->getFieldZR( xyzt, fieldZR, 0 );
//                }
                
                fieldIntensity =std::sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]);
                analysisManager->FillNtupleDColumn(0, xyzt[0]);
                analysisManager->FillNtupleDColumn(1, xyzt[1]);
                analysisManager->FillNtupleDColumn(2, xyzt[2]);
                analysisManager->FillNtupleDColumn(3, r);
                analysisManager->FillNtupleDColumn(4, field[0]);
                analysisManager->FillNtupleDColumn(5, field[1]);
                analysisManager->FillNtupleDColumn(6, field[2]);
                analysisManager->AddNtupleRow();
                
                output << "  " << xyzt[0]/m << "\t"
                << xyzt[1]/m << "\t"
                << xyzt[2]/m << "\t"
                << r/m << "\t"
                << fieldIntensity/tesla << G4endl;
                
                std::cout<<" x:  "<<xyzt[0]/m<<" y:  "<<xyzt[1]/m<<" z:  "<<xyzt[2]/m<<" field: "<<fieldIntensity/tesla<<std::endl;
                std::cout<<" r:  "<<r/m<<" z:  "<<xyzt[2]/m<<" phi:  "<<phi<<" field: "<<fieldIntensity/tesla<<std::endl;
                
                //analysisManager->FillH2(0, r, xyzt[2],fieldIntensity );
//                analysisManager->FillNtupleDColumn(1, 0, r);
//                analysisManager->FillNtupleDColumn(1, 1, xyzt[2]);
//                analysisManager->FillNtupleDColumn(1, 2, fieldZR[0]);
//                analysisManager->FillNtupleDColumn(1, 2, fieldZR[1]);
//                analysisManager->AddNtupleRow(1);
//                 std::cout<<" r:  "<<r/m<<" z:  "<<xyzt[2]/m<<" phi:  "<<phi<<" field: "<<fieldIntensity/tesla<<std::endl;
//
//                std::cout<<" *************** \n"<<std::endl;
//
//                outputRZ << "\t"
//                << r/m << "\t"
//                << xyzt[2]/m << "\t"
//                << fieldIntensity/tesla << G4endl;
                
            }
        }
    }
    
    analysisManager->Write();
    analysisManager->CloseFile();
    output.close();
    
    G4cout << "###### End of example #####" << G4endl;
    return 0;
}

static struct option options[] = {
    {"solenoidOff"  , no_argument, 0, 's'},
    {"toroidsOff "  , no_argument, 0, 't'},
    {"help"         , no_argument, 0, 'h'},
    {0, 0, 0, 0}
};


void Help() {
    std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
    G4cout <<"  testMagneticField application.    \n"
    << std::endl
    <<"  **** Parameters: \n\n"
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
        c = getopt_long(argc, argv, "sth", options, &optidx);
        if (c == -1)
            break;
        //
        switch (c) {
            case 0:
                c = options[optidx].val;
                break;
            case 's':
                parSolenoidOff = true;
                break;
            case 't':
                parToroidsOff   = true;
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


//--------------------------------------------------------
// gmtogdml application: 8th June 2020 (README)
//--------------------------------------------------------

#include "G4Version.hh"
#include "Randomize.hh"
#include "MyDetectorConstruction.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

static G4String inputGeometryFileName   ;
static G4String outputGDMLFileName     = "geometry.gdml";

void GetInputArguments(int argc, char** argv);
void Help();

int main(int argc, char** argv) {
    
    // Get input arguments
    GetInputArguments(argc, argv);
    
    G4cout
    << " ===================  Running GeoModeltoGDML =================== "      << G4endl
    << "   Input geometry file name          =    " << inputGeometryFileName    << G4endl
    << "   Output GDML geometry file name    =    " << outputGDMLFileName       << G4endl
    << " ============================================================== "       << G4endl;
    
    // version banner
    G4String vs = G4Version;
    vs = vs.substr(1,vs.size()-2);
    G4String versionString = " Geant4 version ";
    versionString += vs;
    versionString += "   ";
    versionString += G4Date;
    G4cout << G4endl
    << "**************************************************************" << G4endl
    << versionString << G4endl
    << "                       Copyright : Geant4 Collaboration" << G4endl
    << "                      References : NIM A 506 (2003), 250-303" << G4endl
    << "                                 : IEEE-TNS 53 (2006), 270-278" << G4endl
    << "                                 : NIM A 835 (2016), 186-225" << G4endl
    << "                             WWW : http://geant4.org/" << G4endl
    << "**************************************************************" << G4endl
    << G4endl;
    
    
    // choose the Random engine: set to MixMax explicitely
    // (default from 10.4)
    G4Random::setTheEngine(new CLHEP::MixMaxRng);
    // set seed and print info
    G4Random::setTheSeed(12345678);
    
    G4cout << G4endl
    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
    << G4endl;
    
    // Detector construction
    MyDetectorConstruction* detector = new MyDetectorConstruction;
    detector->SetDumpGDML(true);
    detector->SetGeometryFileName (inputGeometryFileName);
    detector->SetOutputGDMLFileName (outputGDMLFileName);
    detector->Construct();
    
    delete detector;
    return 0;
}

static struct option options[] = {
    {"input geometry file name              "  , required_argument, 0, 'g'},
    {"output geometry gdml file name        "  , required_argument, 0, 'o'},
    {"help"                                    , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};


void Help() {
  std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
  G4cout <<"  GeoModeltoGDML Geant4 application.    \n"
            << std::endl
            <<"  **** Parameters: \n\n"
            <<"      -g :   [MANDATORY] input  geometry file name [.db/.gdml/.dylib/.so] \n"
            <<"      -o :   [OPTIONAL]  output GDML geometry file name (default: geometry.gdml)\n"
            << std::endl;
  std::cout <<"\nUsage: ./gmtogdml [OPTIONS]\n" <<std::endl;
  for (int i=0; options[i].name!=NULL; i++) {
    printf("\t-%c  --%s\t\n", options[i].val, options[i].name);
  }
  std::cout<<"\n "<<std::setw(100)<<std::setfill('=')<<""<<std::setfill(' ')<<std::endl;
}


void GetInputArguments(int argc, char** argv) {
  // process arguments
 if (argc == 1)
 {
     Help();
     exit(0);
     
 }
 while (true) {
   int c, optidx = 0;
   c = getopt_long(argc, argv, "g:o:h", options, &optidx);
   if (c == -1)
     break;
   //
   switch (c) {
   case 0:
     c = options[optidx].val;
     break;
   case 'g':
     inputGeometryFileName = optarg;
     break;
   case 'o':
     outputGDMLFileName = optarg;
     break;
   case 'h':
     Help();
     exit(0);
   default:
     Help();
     errx(1, "unknown option %c", c);
   }
  }
  // check if mandatory Geometry file was provided
  if (inputGeometryFileName=="") {
      G4cout << "\n  *** ERROR : Input geometry file is required! " << G4endl;
      Help();
      exit(-1);
      
  }
  // check that the output file as .gdml extention
  if (!outputGDMLFileName.contains(".gdml")) {
        G4cout << "\n  *** ERROR : Output geometry file must have .gdml extension! " << G4endl;
        Help();
        exit(-1);
        
    }
}

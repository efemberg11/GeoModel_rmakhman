//--------------------------------------------------------
// gmclash application: November 2022 (README)
//--------------------------------------------------------

#include "G4Version.hh"
#include "G4ExceptionHandler.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "ClashDetector.hh"
#include "FSLDetectorConstruction.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

G4bool gmclash_verbose = false;
G4String geometryFileName = "";
G4String rootVolumeName = "";
G4String reportFileName = "gmclash_report.json";
G4double tolerance = 0.0;
G4int numberOfPoints = 1000;
G4int numberOfVolumes = 1;
G4int numberOfLevels = 1;

struct option options[] = {
    {"geometry file name "               , required_argument, 0, 'g'},
    {"root volume name "                 , required_argument, 0, 'r'},
    {"output clashes report file name "  , required_argument, 0, 'o'},
    {"verbose output "                   , no_argument      , 0, 'v'},
    {"tolerance threshold value "        , required_argument, 0, 't'},
    {"numebr of random points "          , required_argument, 0, 'n'},
    {"help "                             , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};

void GetInputArguments(int argc, char** argv);
void Help();
void FindNumberOfVolumes(const G4VPhysicalVolume* volume, G4int level);
void PrintSummary(const G4VPhysicalVolume* world, const G4VPhysicalVolume* root);

int main(int argc, char** argv) {

  // Get input arguments
  GetInputArguments(argc, argv);

  // Print banner
  G4cout << G4endl;
  G4cout
    << "===================  Running GeoModelClash ==================="                                   << G4endl
    << "  Geometry file name               =  " << geometryFileName                                       << G4endl
    << "  Check clashes in                 =  " << (rootVolumeName == "" ? "whole tree" : rootVolumeName) << G4endl
    << "  Output clashes report file name  =  " << (reportFileName == "" ? "no report"  : reportFileName) << G4endl
    << "  Number of random points          =  " << numberOfPoints                                         << G4endl
    << "  Tolerance threshold value (mm)   =  " << tolerance                                              << G4endl
    << "  Verbose output                   =  " << (gmclash_verbose ? "on" : "off")                       << G4endl
    << "=============================================================="                                   << G4endl;

  // Version banner
  G4String version = G4Version;
  version = version.substr(1, version.size() - 2) + "   " + G4Date;
  G4cout << G4endl;
  G4cout
    << "**************************************************************" << G4endl
    << " Geant4 version " + version                                     << G4endl
    << "                       Copyright : Geant4 Collaboration"        << G4endl
    << "                      References : NIM A 506 (2003), 250-303"   << G4endl
    << "                                 : IEEE-TNS 53 (2006), 270-278" << G4endl
    << "                                 : NIM A 835 (2016), 186-225"   << G4endl
    << "                             WWW : http://geant4.org/"          << G4endl
    << "**************************************************************" << G4endl;

  // Define Exception handler
  G4ExceptionHandler* exceptionHandler = new G4ExceptionHandler();

  // Choose the Random engine: set to MixMax explicitely (default from Geant4 10.4)
  G4Random::setTheEngine(new CLHEP::MixMaxRng);
  G4Random::setTheSeed(12345678);

  G4cout << G4endl;
  G4cout
    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
    << G4endl;

  // Detector construction
  FSLDetectorConstruction* detector = new FSLDetectorConstruction;
  detector->SetGeometryFileName(geometryFileName);
  G4VPhysicalVolume* world = detector->Construct();

  // Set root volume
  const G4VPhysicalVolume* rootVolume = (rootVolumeName == "") ? world :
    G4PhysicalVolumeStore::GetInstance()->GetVolume(rootVolumeName);
  if (!rootVolume)
  {
    G4cout << "  *** ERROR : failed to set root volume!!! \n" << G4endl;
    exit(-1);
  }

  // Detect clashes
  G4cout << G4endl;
  G4cout << "=================== Starting Clashes Detection, root volume : " << rootVolume->GetName() << " "
         << "===================" << G4endl;
  G4cout << G4endl;

  ClashDetector clashDetector(world);
  clashDetector.SetResolution(numberOfPoints);
  clashDetector.SetTolerance(tolerance);
  clashDetector.SetGMClashVerbosity(gmclash_verbose);

  G4Timer fTimer;
  fTimer.Start();
  clashDetector.CheckOverlapsInTree(rootVolume);
  fTimer.Stop();

  G4cout << G4endl;
  G4cout << "**** Real time elapsed   : " << fTimer.GetRealElapsed()   << G4endl;
  G4cout << "**** User time elapsed   : " << fTimer.GetUserElapsed()   << G4endl;
  G4cout << "**** System time elapsed : " << fTimer.GetSystemElapsed() << G4endl;

  PrintSummary(world, rootVolume);
  G4cout << "   Number of volumes checked : " << clashDetector.NumberOfChecks() << G4endl;
  G4cout << "   Number of clashes detected: " << clashDetector.NumberOfClashes() << G4endl;

  G4cout << G4endl;
  if (reportFileName != "")
  {
    G4cout << "**** Writing out the clashes report file: " << reportFileName << G4endl;
    clashDetector.PrintOutReport(reportFileName);
  }

  G4cout << G4endl;
  G4cout << "=================== Clashes Detection done! ===================" << G4endl;

  delete detector;
  delete exceptionHandler;
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// Recursively count number of physical volumes in the tree,
// before to call 'numberOfVolumes' & 'numberOfLevels' should be set to 1

void FindNumberOfVolumes(const G4VPhysicalVolume* volume, G4int level)
{
  if (level > numberOfLevels) numberOfLevels = level;
  G4int nv = volume->GetLogicalVolume()->GetNoDaughters();
  numberOfVolumes += nv;
  for (G4int i = 0; i < nv; ++i)
  {
    G4VPhysicalVolume* daughter = volume->GetLogicalVolume()->GetDaughter(i);
    FindNumberOfVolumes(daughter, level + 1);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Print summary on the performed check

void PrintSummary(const G4VPhysicalVolume* world, const G4VPhysicalVolume* root)
{
  // Print some information about the geometry
  G4cout << G4endl;
  G4cout << "   Size of G4SolidStore          : " << G4SolidStore::GetInstance()->size() << G4endl;
  G4cout << "   Size of G4LogicalVolumeStore  : " << G4LogicalVolumeStore::GetInstance()->size() << G4endl;
  G4cout << "   Size of G4PhysicalVolumeStore : " << G4PhysicalVolumeStore::GetInstance()->size() << G4endl;
  FindNumberOfVolumes(world, 1);

  G4cout << G4endl;
  G4cout << "   Number of volumes : " << numberOfVolumes
         << " (" << numberOfLevels << " levels)   in tree: " << world->GetName() << G4endl;

  if (root != world)
  {
    numberOfVolumes = numberOfLevels = 1;
    FindNumberOfVolumes(root, 1);
    G4cout << "   Number of volumes : " << numberOfVolumes
           << " (" << numberOfLevels << " levels)   in tree: " << root->GetName() << G4endl;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Print help message

void Help() {
  G4cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << G4endl;
  G4cout << "  GeoModelClash Geant4 application.\n\n"
         << "  **** Parameters: \n\n"
         << "      -g :   [MANDATORY] the Geometry file name [.db/.gdml/.dylib/.so] \n"
         << "      -r :   [OPTIONAL] root volume name (default: the Geometry top volume)\n"
         << "      -o :   [OPTIONAL] clashes report file name (default: gmclash_report.json)\n"
         << "      -t :   [OPTIONAL] tolerance threshold value in mm (default: 0)\n"
         << "      -n :   [OPTIONAL] number of random points (default: 1000)\n"
         << "      -v :   [OPTIONAL] verbose output (default: off)\n"
         << G4endl;
  G4cout << "\nUsage: ./gmclash [OPTIONS]\n" << G4endl;
  for (int i=0; options[i].name!=NULL; i++) {
    printf("\t-%c  --%s\t\n", options[i].val, options[i].name);
  }
  G4cout << "\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << G4endl;
}

//////////////////////////////////////////////////////////////////////////
//
// Get input aguments and set corresponding global variables

void GetInputArguments(int argc, char** argv) {
  // Process arguments
  if (argc == 1) {
    Help();
    exit(0);
  }
  while (true) {
    int c, optidx = 0;
    c = getopt_long(argc, argv, "g:r:o:t:n:vh", options, &optidx);
    if (c == -1) break;

    switch (c) {
    case 0:
      c = options[optidx].val;
      break;
    case 'g':
      geometryFileName = optarg;
      break;
    case 'o':
      reportFileName = optarg;
      break;
    case 'v':
      gmclash_verbose = true;
      break;
    case 't':
      tolerance = atof(optarg)*mm;
      break;
    case 'n':
      numberOfPoints = atoi(optarg);
      if (numberOfPoints < 100) numberOfPoints = 100;
      if (numberOfPoints > 100000) numberOfPoints = 100000;
      break;
    case 'r':
      rootVolumeName = optarg;
      break;
    case 'h':
      Help();
      exit(0);
    default:
      Help();
      errx(1, "unknown option %c", c);
    }
  }
  // Check if mandatory Geometry file was provided
  if (geometryFileName == "") {
    G4cout << "\n  *** ERROR : Geometry file is required !!!" << G4endl;
    Help();
    exit(-1);
  }
}

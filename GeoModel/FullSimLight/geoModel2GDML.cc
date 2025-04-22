//--------------------------------------------------------
// gm2gdml application: 8th June 2020 (README)
//--------------------------------------------------------

#include "G4Version.hh"
#include "FSLDetectorConstruction.hh"
#include "G4Timer.hh"

#include "G4DisplacedSolid.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

G4String inputGeometryFileName;
G4String outputGDMLFileName = "geometry.gdml";
std::vector<G4VPhysicalVolume*> modified_volumes;

void GetInputArguments(int argc, char** argv);
void Help();
void EliminateDisplacedSolids();

int main(int argc, char** argv) {

    // Get input arguments
    GetInputArguments(argc, argv);

    G4cout
    << " ===================  Running GeoModel2GDML =================== "      << G4endl
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

    // Detector construction
    FSLDetectorConstruction* detector = new FSLDetectorConstruction;
    detector->SetGeometryFileName(inputGeometryFileName);
    G4VPhysicalVolume* world = detector->Construct();

    // Dump geometry
    G4Timer timer;
    timer.Start();
    G4cout << "\n ===================  Dump geometry in GDML format  =================== \n" << G4endl;

    // Remove volumes with custom shapes
    detector->PullUnidentifiedVolumes(world->GetLogicalVolume());

    // Eliminate G4DisplacedSolids, if any
    std::vector<G4LogicalVolume*>* lv_store = G4LogicalVolumeStore::GetInstance();
    for (int i = 0; i < int(lv_store->size()); ++i)
    {
      G4LogicalVolume* logical = lv_store->at(i);
      G4VSolid* solid = logical->GetSolid();
      if (solid->GetEntityType() != "G4DisplacedSolid") continue;
      G4cout << " !ELIMINATING G4DisplacedSolid objects, shape is not supported in GDML! \n" << G4endl;
      EliminateDisplacedSolids();
      break;
    }
    G4GDMLParser parser;
    parser.Write(outputGDMLFileName, world->GetLogicalVolume());

    timer.Stop();
    G4cout << G4endl;
    G4cout << "**** Real time elapsed   : " << timer.GetRealElapsed()   << G4endl;
    G4cout << "**** User time elapsed   : " << timer.GetUserElapsed()   << G4endl;
    G4cout << "**** System time elapsed : " << timer.GetSystemElapsed() << G4endl;
    G4cout << "\n =================== Geometry exported in GDML, DONE!  =================== \n" << G4endl;

    // Free allocated memory
    for (int i = 0; i < int(modified_volumes.size()); ++i)
    {
      G4VPhysicalVolume* v = modified_volumes[i];
      G4RotationMatrix* r = v->GetRotation();
      if (r == nullptr) continue;
      delete r;
      v->SetRotation(nullptr);
    }

    delete detector;
    return 0;
}

struct option options[] = {
    {"input geometry file name              "  , required_argument, 0, 'g'},
    {"output geometry gdml file name        "  , required_argument, 0, 'o'},
    {"help"                                    , no_argument      , 0, 'h'},
    {0, 0, 0, 0}
};


void Help() {
  std::cout <<"\n " << std::setw(100) << std::setfill('=') << "" << std::setfill(' ') << std::endl;
  G4cout <<"  GeoModel2GDML Geant4 application.    \n"
            << std::endl
            <<"  **** Parameters: \n\n"
            <<"      -g :   [MANDATORY] input  geometry file name [.db/.gdml/.dylib/.so] \n"
            <<"      -o :   [OPTIONAL]  output GDML geometry file name (default: geometry.gdml)\n"
            << std::endl;
  std::cout <<"\nUsage: ./gm2gdml [OPTIONS]\n" <<std::endl;
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

///////////////////////////////////////////////////////////////////////////////
//
// Auxiliary function for EliminateDisplacedSolids()

void SetTransformation(G4VPhysicalVolume* v, G4Transform3D& t)
{
  G4RotationMatrix* old_rotation = v->GetRotation();
  G4RotationMatrix new_rotation = t.getRotation().inverse();
  if (old_rotation != nullptr) {
    *old_rotation = new_rotation;
  } else {
    if (!new_rotation.isIdentity()) {
      G4RotationMatrix* r = new G4RotationMatrix(new_rotation);
      v->SetRotation(r);
      modified_volumes.push_back(v);
    }
  }
  v->SetTranslation(t.getTranslation());
}

///////////////////////////////////////////////////////////////////////////////
//
// Replace displaced solids in volumes

void EliminateDisplacedSolids() {
  std::vector<G4LogicalVolume*> lv_vector;
  std::vector<G4VPhysicalVolume*>* pv_store = G4PhysicalVolumeStore::GetInstance();

  // Find logical volumes constructed from displaced solids and
  // modify transformation in corresponding physical volumes
  for (int i = 0; i < int(pv_store->size()); ++i)
  {
    G4VPhysicalVolume* physical = pv_store->at(i);
    G4LogicalVolume* logical = physical->GetLogicalVolume();
    G4VSolid* solid = logical->GetSolid();
    if (solid->GetEntityType() != "G4DisplacedSolid") continue;
    // select logical volume
    lv_vector.push_back(logical);
    // modify transformation in physical volume
    G4DisplacedSolid* displaced = (G4DisplacedSolid*)solid;
    G4Transform3D t1(displaced->GetObjectRotation(), displaced->GetObjectTranslation());
    G4Transform3D t2(physical->GetObjectRotationValue(), physical->GetObjectTranslation());
    G4Transform3D tt = t2*t1;
    SetTransformation(physical, tt);
  }

  // Replace solid in selected logical volumes and
  // modify transformation in daughter physical volumes
  int nlv = lv_vector.size();
  for (int i = 0; i < nlv; ++i)
  {
    G4LogicalVolume* logical = lv_vector[i];
    G4VSolid* solid = logical->GetSolid();
    if (solid->GetEntityType() != "G4DisplacedSolid") continue;
    // replace solid
    G4DisplacedSolid* displaced = (G4DisplacedSolid*)solid;
    logical->SetSolid(displaced->GetConstituentMovedSolid());
    // modify transformation in daughter physical volumes
    G4Transform3D t1(displaced->GetObjectRotation(), displaced->GetObjectTranslation());
    t1 = t1.inverse();
    int nd = logical->GetNoDaughters();
    for (int k = 0; k < nd; ++k)
    {
      G4VPhysicalVolume* daughter = logical->GetDaughter(k);
      G4Transform3D t2(daughter->GetObjectRotationValue(), daughter->GetObjectTranslation());
      G4Transform3D tt = t1*t2;
      SetTransformation(daughter, tt);
    }
  }
}

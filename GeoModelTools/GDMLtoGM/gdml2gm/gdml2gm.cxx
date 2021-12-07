#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelWrite/WriteGeoModel.h"

#include "GeoModelXMLParser/XercesParser.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoPhysVol.h"

#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoNameTag.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'

int main(int argc, char ** argv) {
  //
  // Usage message:
  //

  std::string gdml2gm= argv[0];
  std::string usage= "usage: " + gdml2gm + " -f path_to/gdml_file -o outputFile]";
  //
  // Print usage message if no args given:
  //
  if (argc==1) {
    std::cerr << usage << std::endl;
    return 0;
  }
  //
  // Parse the command line:
  //
  std::string inputFile;
  std::string outputFile;
  for (int argi=1;argi<argc;argi++) {
    std::string argument=argv[argi];
    if (argument.find("-o")!=std::string::npos) {
      argi++;
      if (argi>=argc) {
	std::cerr << usage << std::endl;
	return 1;
      }
      outputFile=argv[argi];
    }
    else if (argument.find("-f")!=std::string::npos) {
      argi++;
      if (argi>=argc) {
	std::cerr << usage << std::endl;
	return 1;
      }
      inputFile=argv[argi];
    }
    else {
      std::cerr << "Unrecognized argument " << argument << std::endl;
      std::cerr << usage << std::endl;
      return 2;
    }
  }

  //
  // Check that we can access the output file
  //
  if (access(outputFile.c_str(),F_OK)==0) {
    if (!access(outputFile.c_str(),W_OK)) {
      if (system(("rm -f "+ outputFile).c_str())) {
	std::cerr << "Error, cannot overwrite existing file " << outputFile << std::endl;
	return 3;
      }
    }
    else {
      std::cerr << "Error, cannot overwrite existing file " << outputFile << " (permission denied)" << std::endl;
      return 4;
    }
  }
 
  // parse GDML file, create volume hierarchy and put it into world
  
  std::cout<< "GDMLtoGeo: GDML to GeoModel Interface. Parsing gdml file "<<inputFile<<" and setting world volume"<<std::endl;
  GDMLController controller("GDMLController");
  GeoModelTools::XercesParser xercesParser;
  xercesParser.ParseFileAndNavigate(inputFile);
  //std::cout << "done parsing "<<std::endl;
  GeoPhysVol* w=controller.getWorld();

  w->ref();
  
  //
  // Open a new database:
  //
  GMDBManager db(outputFile);
  //
  // check the DB connection
  //
  if (!db.checkIsDBOpen()) {
    std::cerr << "Error opening output file " << outputFile << std::endl;
    return 7;
  }

  GeoModelIO::WriteGeoModel dumpGeoModelGraph(db);
  w->exec(&dumpGeoModelGraph);
  dumpGeoModelGraph.saveToDB();

  w->unref();


  return 0;
}

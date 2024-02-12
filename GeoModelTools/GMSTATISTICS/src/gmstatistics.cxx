#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModelWrite/WriteGeoModel.h"

#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelFuncSnippets/defineWorld.h"
#include "GeoInventoryGraphAction.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <libgen.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <execinfo.h>
#ifndef __APPLE__
#include <malloc.h>
#endif
using namespace std;

#ifndef __APPLE__
unsigned int heapsize() {
  struct mallinfo mi=mallinfo();
  return (unsigned int)(mi.uordblks + mi.hblkhd);
}
#endif

#ifdef __APPLE__
const std::string shared_obj_extension=".dylib";
double factor=1000000.0;
#else
const std::string shared_obj_extension=".so";
double factor=1000.0;
#endif

int  snoop() {
 
  struct rusage usage;
  getrusage(RUSAGE_SELF,&usage);
  return usage.ru_maxrss;

}

int main(int argc, char ** argv) {

   //
  // Usage message:
  //
  std::string xtraOpts="";
  bool printTree=false;
  std::string gmstat= argv[0];
  std::string usage= "usage: " + gmstat + " [-p] " + xtraOpts +  "[plugin1"+shared_obj_extension
    + "] [plugin2" + shared_obj_extension
    + "] ";
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
  std::vector<std::string> inputPlugins;
  std::string outputFile;
  bool outputFileSet = false;
  for (int argi=1;argi<argc;argi++) {
    std::string argument=argv[argi];
    if (argument.find(shared_obj_extension)!=std::string::npos) {
      inputPlugins.push_back(argument);
    }
    else if (argument=="-p") {
      printTree=true;
    }
    else {
      std::cerr << "Unrecognized argument " << argument << std::endl;
      std::cerr << usage << std::endl;
      return 2;
    }
  }

  std::ofstream file(("/tmp/gmstatistics-"+std::to_string(getpid())).c_str());
  std::streambuf *coutBuff=std::cout.rdbuf();
  std::streambuf *fileBuff=file.rdbuf();
  std::cout.rdbuf(fileBuff); 


#ifdef __APPLE__
  {
    GeoIntrusivePtr<GeoVPhysVol> world{createGeoWorld()};
    for (const std::string & plugin : inputPlugins) {
      GeoGeometryPluginLoader loader;
      GeoVGeometryPlugin *factory=loader.load(plugin);
      if (!factory) {
	std::cerr << "Could not load plugin " << plugin << std::endl;
	std::cout.rdbuf(coutBuff);
	return 5;
      }
      factory->create(world);
      if (printTree) {
	GeoInventoryGraphAction action(std::cout);
	world->exec(&action);
      }
      delete factory;
      std::cout.rdbuf(fileBuff);
    }
    std::cout.rdbuf(coutBuff);
    //
    // Ceci n'est pas une pipe:
    //
    FILE *ceci=popen(("heap --guessNonObjects " + std::to_string(getpid()) + " | grep -e GeoModelKernel -e GeoGenericFunctions " ).c_str(),"r");
    //
    char *line;
    size_t linecap=0;
    int sum=0.0;
    while ( getline(&line, &linecap, ceci) > 0) {
      std::istringstream stream(line);
      unsigned int total, number;
      double average;
      std::string object;
      
      stream >> number >> total >> average >> object;
      std::cout << std::setw(30) << object << " " << std::setw(15) << number << " instances " << std::setw(20) <<  total << " bytes" << std::endl;
      sum +=total;
    }
    std::cout << "Total GeoModel object allocation:  " << sum/1000000.0 << "MB" << std::endl;
  }
    
    
#else
  {

    for (const std::string & plugin : inputPlugins) {
      GeoGeometryPluginLoader loader;
      GeoVGeometryPlugin *factory=loader.load(plugin);
      if (!factory) {
	std::cerr << "Could not load plugin " << plugin << std::endl;
	std::cout.rdbuf(coutBuff);
	return 5;
      }
      unsigned int expand{0};
      unsigned int net{0};
      {
	GeoIntrusivePtr<GeoVPhysVol> world{createGeoWorld()};

	int before=snoop();
	factory->create(world);
	net=snoop()-before;
	std::cout.rdbuf(coutBuff);
	
	if (printTree) {
	  GeoInventoryGraphAction action(std::cout);
	  world->exec(&action);
	}
	expand=heapsize();
      }
      unsigned int contract=expand-heapsize();
      std::cout << basename((char *) plugin.c_str()) << " allocates " << net/factor << " MB" << " net GeoModel " << contract/1000000.0 << " MB" << std::endl;
      delete factory;
      std::cout.rdbuf(fileBuff);
    }
    std::cout.rdbuf(coutBuff);
    
  }
#endif  
}

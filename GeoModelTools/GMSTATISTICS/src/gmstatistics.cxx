#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModelWrite/WriteGeoModel.h"

#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoPVLink.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCountVolAction.h"
#include "GeoModelKernel/GeoAccessVolumeAction.h"
#include "GeoModelKernel/GeoNameTag.h"
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
using namespace std;
// global operator new
size_t gsize=0;
bool heap=false;

#define BT_BUF_SIZE 100
bool tGeo(void)
{
  int j, nptrs;
  void *buffer[BT_BUF_SIZE];
  char **strings;
  
  nptrs = backtrace(buffer, BT_BUF_SIZE);

  strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }
  
  for (j = 0; j < nptrs; j++) {
    if (strstr(strings[j],"GeoModelKernel")) {
      
      //   for (int k=0;k<nptrs;k++) printf("%s\n",strings[k]);
      // printf("\n\n");
      
      free(strings);
      return true;
    }
  }
  free(strings);
  return false;
}


void * operator new(size_t size)
{
#ifndef __APPLE__
  if (heap) {
    if (tGeo()) gsize += size;
  }
#endif
  void * ptr;
    ptr = malloc(size); // attempt to allocate memory

    if (!ptr) // if memory is not allocated, then an exception is thrown
    {
        bad_alloc ba;
        throw ba;
    }
    else
    {
        return ptr;
    }
}

// global operator delete
void operator delete(void * ptr)
{
    free(ptr);
}




#ifdef __APPLE__
const std::string shared_obj_extension=".dylib";
double factor=1000000.0;
#else
const std::string shared_obj_extension=".so";
double factor=1000.0;
#endif

#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'
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
  std::string usage= "usage: " + gmstat + " [-p] [-h] " + xtraOpts +  "[plugin1"+shared_obj_extension
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
    else if (argument=="-h") {
      heap=true;
#ifndef __APPLE__
      std::cout << "Warning, heap analysis on Linux takes far greater time" << std::endl;  
      std::cout << "and is less informative than on MacOS            " << std::endl;
#endif      
    }
    else {
      std::cerr << "Unrecognized argument " << argument << std::endl;
      std::cerr << usage << std::endl;
      return 2;
    }
  }

  //
  // Create elements and materials:
  //

  const double  gr =   SYSTEM_OF_UNITS::gram;
  const double  mole = SYSTEM_OF_UNITS::mole;
  const double  cm3 =  SYSTEM_OF_UNITS::cm3;

  // Define the chemical elements
  GeoElement*  Nitrogen = new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0067 *gr/mole);
  GeoElement*  Oxygen   = new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9995 *gr/mole);
  GeoElement*  Argon    = new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.948  *gr/mole);
  GeoElement*  Hydrogen = new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00797 *gr/mole);

  double densityOfAir=0.001214 *gr/cm3;
  GeoMaterial *air = new GeoMaterial("Air", densityOfAir);
  air->add(Nitrogen  , 0.7494);
  air->add(Oxygen, 0.2369);
  air->add(Argon, 0.0129);
  air->add(Hydrogen, 0.0008);
  air->lock();

  //
  // Create a huge world volume made of Air:
  //

  const GeoBox* worldBox = new GeoBox(2000*SYSTEM_OF_UNITS::cm, 2000*SYSTEM_OF_UNITS::cm, 2500*SYSTEM_OF_UNITS::cm);
  const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  GeoPhysVol *world=new GeoPhysVol(worldLog);
  world->ref();
  //
  // Loop over plugins, create the geometry and put it under the world:
  //

  std::ofstream file(("/tmp/gmstatistics-"+std::to_string(getpid())).c_str());
  std::streambuf *coutBuff=std::cout.rdbuf();
  std::streambuf *fileBuff=file.rdbuf();
  std::cout.rdbuf(fileBuff); 

  
  for (const std::string & plugin : inputPlugins) {
    GeoGeometryPluginLoader loader;
    GeoVGeometryPlugin *factory=loader.load(plugin);
    if (!factory) {
      std::cerr << "Could not load plugin " << plugin << std::endl;
      std::cout.rdbuf(coutBuff);
      return 5;
    }
    
    int before=snoop();
    factory->create(world);
    int net=snoop()-before;
    std::cout.rdbuf(coutBuff);
    std::cout << basename((char *) plugin.c_str()) << " allocates " << net/factor << " MB" << std::endl;

    if (printTree) {
      GeoInventoryGraphAction action(std::cout);
      world->exec(&action);
    }
    std::cout.rdbuf(fileBuff);

   }
  std::cout.rdbuf(coutBuff);
  
  if (heap) {
#ifdef __APPLE__
    //
    // Ceci n'est pas une pipe:
    //
    FILE *ceci=popen(("heap --guessNonObjects " + std::to_string(getpid()) + " | grep GeoModel" ).c_str(),"r");
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
#else
    std::cout << "Total GeoModel object allocation:  " << gsize/1000000.0 << std::endl;
#endif
  }

}

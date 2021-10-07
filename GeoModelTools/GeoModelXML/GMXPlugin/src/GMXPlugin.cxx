/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"


#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

#include "GeoModelXml/GmxInterface.h"
#include "GeoModelXml/Gmx2Geo.h"

#include <iostream>
#include <fstream>
#include <sstream>

class GMXPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  GMXPlugin();

  // Destructor:
  ~GMXPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish);

 private:

  // Illegal operations:
  const GMXPlugin & operator=(const GMXPlugin &right)=delete;
  GMXPlugin(const GMXPlugin &right) = delete;

  bool exists (const std::string& name)
  {
    std::ifstream f(name.c_str());
    return f.good();
  }

  std::vector<std::string> parseFiles(const std::string files, const char separator=';')
  {
  	 std::string parsed;
	 std::istringstream iFiles(files);
	 std::vector<std::string> fileList;
	 while (iFiles>>parsed)
	 {
		std::cout<<" parsed string "<<parsed<<std::endl;
		fileList.push_back(parsed);
	 }
	 return fileList;
  }

};





GMXPlugin::GMXPlugin()
{
}


GMXPlugin::~GMXPlugin()
{
}


//## Other Operations (implementation)
void GMXPlugin::create(GeoPhysVol *world, bool publish)
{

  std::cout<< "This is GMXPlugin: creating a GeoModelXml detector "<<std::endl;
  std::vector<std::string> filesToParse;
  char* fPath=getenv("GMX_FILES");
  std::string fileName;
  if (fPath!=NULL) {
    std::cout<<" Environment variable GMX_FILES set to "<<fPath<<std::endl;
    fileName=std::string(fPath);
    filesToParse=parseFiles(fileName,':');
  }
  else {
  	fileName="gmx.xml";
	filesToParse.push_back(fileName);
  }

  char* matManager=getenv("GMX_USE_MATMANAGER");
  bool matman=0;
  if (matManager!=nullptr)
  {
  	std::cout<<" Environment variable GMX_USE_MATMANAGER set to "<<matManager<<std::endl;
	std::istringstream ss(matManager);
	ss>>matman;
	std::cout<<"matman set to "<<matman<<std::endl;
  }

  for (auto f: filesToParse)
  {
    if (!exists(f)) {
    	std::cout <<"GDMLtoGeo: input file "<<f<<
      	" does not exist. quitting and returning nicely! "<<std::endl;
   	return;
    }
    processorList proc;
    GmxInterface gmxInterface;
    Gmx2Geo gmx2Geo(f, world, gmxInterface,proc, 0 , matman);
  }

  if (matman) MaterialManager::getManager()->printAll();

}

extern "C" GMXPlugin *createGMXPlugin() {
  return new GMXPlugin;
}

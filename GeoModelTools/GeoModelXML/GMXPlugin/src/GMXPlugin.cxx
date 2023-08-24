/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"


#include "GeoModelKernel/GeoPhysVol.h"
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

  // Constructor with name to provide to publisher  
  GMXPlugin(std::string name):GeoVGeometryPlugin(name){}

  // Destructor:
  ~GMXPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish) override;

 private:

  // Illegal operations:
  const GMXPlugin & operator=(const GMXPlugin &right)=delete;
  GMXPlugin(const GMXPlugin &right) = delete;

  bool exists (const std::string& name)
  {
    std::ifstream f(name.c_str());
    return f.good();
  }

  std::vector<std::string> parseFiles(const std::string s, std::string delimiter=";")
  {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
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
    filesToParse=parseFiles(fileName,":");
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

  char* levelmaps=getenv("GMX_DUMP_LEVEL_MAPS");

  for (auto f: filesToParse)
  {
    if (!exists(f)) {
    	std::cout <<"GDMLtoGeo: input file "<<f<<
      	" does not exist. quitting and returning nicely! "<<std::endl;
   	return;
    }
    GmxInterface gmxInterface;
    //If we want to write the SQLite, pass a publisher through to fill Aux tables
    //(needed for ReadoutGeometry)
    if(publish) gmxInterface.setPublisher(getPublisher());

    if (levelmaps!=nullptr){
        std::string mapname = f.substr(0, f.length() - 4);
        mapname+="_levelMap.txt";
        std::cout<<"Dumping Copy Number Level Map to "<<mapname<<std::endl;
        Gmx2Geo gmx2Geo(f, world, gmxInterface, 0 , matman, mapname);
    }
    else Gmx2Geo gmx2Geo(f, world, gmxInterface, 0 , matman);
  }

  if (matman) MaterialManager::getManager()->printAll();

}

extern "C" GMXPlugin *createGMXPlugin() {
  return new GMXPlugin("GeoModelXML");
}

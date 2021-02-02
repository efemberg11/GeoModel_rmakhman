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

class MuonPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  MuonPlugin();

  // Destructor:
  ~MuonPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish);

 private:

  // Illegal operations:
  const MuonPlugin & operator=(const MuonPlugin &right)=delete;
  MuonPlugin(const MuonPlugin &right) = delete;

  bool exists (const std::string& name)
  {
    std::ifstream f(name.c_str());
    return f.good();
  }
  static const std::vector<std::string> inputFiles;
};


//
//
//
const std::vector<std::string> MuonPlugin::inputFiles={"Muons/Muons00.xml"};
//
//
//


MuonPlugin::MuonPlugin()
{
}


MuonPlugin::~MuonPlugin()
{
}


//## Other Operations (implementation)
void MuonPlugin::create(GeoPhysVol *world, bool publish)
{
  char * geomodel_xml_dir=getenv("GEOMODEL_XML_DIR");
  if (!geomodel_xml_dir) {
    std::cout << "Please define GEOMODEL_XML_DIR. It should point to a local copy " << std::endl;
    std::cout << "of https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelData/ "     << std::endl;
    return;
  }
  std::cout<< "This is MuonPlugin: creating a GeoModelXml detector "<<std::endl;

  for (const std::string & f: inputFiles)
  {
    std::string path=std::string(geomodel_xml_dir)+"/"+f;
    if (!exists(path)) {
    	std::cout <<"GDMLtoGeo: input file "<<f<<
      	" does not exist. quitting and returning nicely! "<<std::endl;
   	return;
    }
    GmxInterface gmxInterface;
    Gmx2Geo gmx2Geo(path, world, gmxInterface, 0);
  }

}

extern "C" MuonPlugin *createMuonPlugin() {
  return new MuonPlugin;
}

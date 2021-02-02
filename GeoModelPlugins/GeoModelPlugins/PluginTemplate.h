#ifndef _PLUGINTEMPLATE_H_
#define _PLUGINTEMPLATE_H_
#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelXml/GmxInterface.h"
#include "GeoModelXml/Gmx2Geo.h"

#include <iostream>
#include <fstream>
#include <sstream>
class PLUGINNAME : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  PLUGINNAME();

  // Destructor:
  ~PLUGINNAME();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world, bool publish);

 private:

  // Illegal operations:
  const PLUGINNAME & operator=(const PLUGINNAME &right)=delete;
  PLUGINNAME(const PLUGINNAME &right) = delete;

  // Input file list.  
  static const std::vector<std::string> inputFiles;
};


inline PLUGINNAME::PLUGINNAME()
{
}


inline PLUGINNAME::~PLUGINNAME()
{
}


//## Other Operations (implementation)
inline void PLUGINNAME::create(GeoPhysVol *world, bool publish)
{
  char * geomodel_xml_dir=getenv("GEOMODEL_XML_DIR");
  if (!geomodel_xml_dir) {
    std::cout << "Please define GEOMODEL_XML_DIR. It should point to a local copy " << std::endl;
    std::cout << "of https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelData/ "     << std::endl;
    return;
  }

  for (const std::string & f: inputFiles)
  {
    std::string path=std::string(geomodel_xml_dir)+"/"+f;
    if (!std::ifstream(path).good()) {
    	std::cout <<"GDMLtoGeo: input file "<<f<<
      	" does not exist. quitting and returning nicely! "<<std::endl;
   	return;
    }
    GmxInterface gmxInterface;
    Gmx2Geo gmx2Geo(path, world, gmxInterface, 0);
  }

}
#endif

#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include <string>
#include <iostream>
#include <dlfcn.h>
#include <libgen.h>

GeoGeometryPluginLoader::GeoGeometryPluginLoader () {
}

GeoGeometryPluginLoader::~GeoGeometryPluginLoader () {
}


GeoVGeometryPlugin *GeoGeometryPluginLoader::load(const std::string & pString) const {
  std::string bNameString=basename ((char *) pString.c_str());       // Strip off the directory
  bNameString=bNameString.substr(3);                        // Strip off leading "lib"
  bNameString=bNameString.substr(0,bNameString.find("."));  // Strip off extensions
  
  std::string createFunctionName = std::string("create")+bNameString;
  
  //
  // Loads the library:
  //
  void *handle = dlopen(pString.c_str(),RTLD_NOW);
  if (!handle) {
    std::cerr << dlerror() << std::endl;
    return nullptr;
  }
  
  //
  // Gets the function
  //
  void *f = dlsym(handle,createFunctionName.c_str());
  if (!f) {
    std::cerr << dlerror() << std::endl;
    return nullptr;
  }
  
  typedef void * (*CreationMethod) ();
  CreationMethod F = (CreationMethod) f;
  
  //
  // 
  //
  GeoVGeometryPlugin * factory = (GeoVGeometryPlugin *) F();
  return factory;

}

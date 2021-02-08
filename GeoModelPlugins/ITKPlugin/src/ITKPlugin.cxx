#define  PLUGINNAME ITKPlugin

#include "PluginTemplate.h"

const std::vector<std::string> ITKPlugin::inputFiles={"ITK/StripBlobRev_5433bf5.xml"};
extern "C" ITKPlugin *createITKPlugin() {
  return new ITKPlugin;
}

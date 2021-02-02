#define  PLUGINNAME MuonPlugin

#include "PluginTemplate.h"

const std::vector<std::string> MuonPlugin::inputFiles={"Muons/Muons00.xml"};
extern "C" MuonPlugin *createMuonPlugin() {
  return new MuonPlugin;
}

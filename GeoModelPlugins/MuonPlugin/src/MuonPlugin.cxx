#define  PLUGINNAME MuonPlugin

#include "PluginTemplate.h"

const std::vector<std::string> MuonPlugin::inputFiles={
  "Muons/Muons00.xml",
  "Muons/NSW/LM1.xml",
  "Muons/NSW/LM2.xml",
  "Muons/NSW/SM1.xml",
  "Muons/NSW/SM2.xml",
  "Muons/NSW/QL1.xml",
  "Muons/NSW/QL2.xml",
  "Muons/NSW/QL3.xml",
  "Muons/NSW/QS1.xml",
  "Muons/NSW/QS2.xml",
  "Muons/NSW/QS3.xml"
};
extern "C" MuonPlugin *createMuonPlugin() {
  return new MuonPlugin;
}

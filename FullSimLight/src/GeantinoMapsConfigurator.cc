#include "GeantinoMapsConfigurator.hh"

GeantinoMapsConfigurator* GeantinoMapsConfigurator::s_instance{nullptr};

GeantinoMapsConfigurator* GeantinoMapsConfigurator::getGeantinoMapsConf()
{
  if(!s_instance) {
    try {
      s_instance = new GeantinoMapsConfigurator();
    }
    catch(std::runtime_error& ex) {
      std::cerr << ex.what() << std::endl;
      return nullptr;
    }
  }
  return s_instance;
}

GeantinoMapsConfigurator::GeantinoMapsConfigurator() {}

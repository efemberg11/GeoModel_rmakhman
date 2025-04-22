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

GeantinoMapsConfigurator::GeantinoMapsConfigurator() :
fGMapsMessenger(nullptr)
{
    fGMapsMessenger   = new GeantinoMapsMessenger(this);
}

GeantinoMapsConfigurator::~ GeantinoMapsConfigurator()
{
    delete fGMapsMessenger;
}

void GeantinoMapsConfigurator::SetVolumesList(G4String volumeslist){
    
    using namespace std;
    
    string parsed;
    stringstream input_stringstream(volumeslist);
    
    while (getline(input_stringstream,parsed,' '))
    {
        fVolumesList.push_back(parsed);
        std::cout<<"Volume found: "<<parsed<<std::endl;
        
    }
    
}


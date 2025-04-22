#ifndef FSLConfigurator_h
#define FSLConfigurator_h 1

#include "G4Types.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "GeoModelKernel/GeoPhysVol.h"

// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>


using json = nlohmann::json;

/**
 * @file    FSLConfigurator.hh
 * @author  Marilena Bandieramonte
 * @date   13.05.2022
 *
 */

namespace simConfig {



struct regionConfig {
    std::string   regionName;
    std::vector<G4String> rootLVNames;
    double  gammaCut;
    double  electronCut;
    double  positronCut;
    double  protonCut;
};

struct fslConfig{
    
    std::string geometry;
    std::string physicsList;
    int nEvents;
    
    std::string eventGeneratorName;
    std::string eventInputFile;
    std::string typeOfEvent;
    
    std::string hepmc3InputFile;
    std::string hepmc3TypeOfFile;
    
    std::string generatorPlugin;
    
    std::vector<std::string> sensitiveDetectors;
 //   std::string outputHitsFile;
 //   std::string outputHistoFile;
    
    std::vector<regionConfig> regionsData;
    
    std::string magFieldType;
  //  std::string magFieldIntensity;
   // std::string magFieldMap;
    std::string magFieldPlugin;
    
    std::vector<std::string> userActions;


    std::vector<std::string> g4UiCommands;
    std::vector<std::string> g4VisUiCommands;
};

fslConfig fsl;
regionConfig rc;
json jf;

inline void to_json(json& j, const fslConfig& p) {
    j = json{ {"Geometry", p.geometry},
        {"Physics list name", p.physicsList},
        {"Number of events", p.nEvents},
        {"Generator", p.eventGeneratorName},
        {"Pythia event input file", p.eventInputFile},
        {"Pythia type of event", p.typeOfEvent},
        {"Sensitive Detector Extensions", p.sensitiveDetectors},
        {"Magnetic Field Type", p.magFieldType},
        {"Magnetic Field Plugin", p.magFieldPlugin},
        {"User Action Extensions", p.userActions},
        {"g4ui_commands", p.g4UiCommands},
        {"g4visui_commands", p.g4VisUiCommands},
        {"HepMC3 file", p.hepmc3InputFile}, 
        {"HepMC3 type of file", p.hepmc3TypeOfFile},
        {"Generator Plugin", p.generatorPlugin}
    };
    
}
inline void to_json(json& j, const regionConfig& r) {
    j = json{{"RegionName", r.regionName},{"RootLVNames", r.rootLVNames},{"GammaCut", r.gammaCut},{"ElectronCut", r.electronCut},{"PositronCut", r.positronCut},{"ProtonCut", r.protonCut}};
}
inline void from_json(const json& j, fslConfig& p) {
    p.geometry=j.at("Geometry").get<std::string>();
    p.physicsList=j.at("Physics list name").get<std::string>();
    p.nEvents=j.at("Number of events").get<int>();
    p.magFieldType=j.at("Magnetic Field Type").get<std::string>();
    p.eventGeneratorName=j.at("Generator").get<std::string>();
    p.eventInputFile=j.at("Pythia event input file").get<std::string>();
    p.typeOfEvent=j.at("Pythia type of event").get<std::string>();
    p.sensitiveDetectors=j.at("Sensitive Detector Extensions").get<std::vector<std::string>>();
    p.magFieldPlugin=j.at("Magnetic Field Plugin").get<std::string>();
    p.userActions=j.at("User Action Extensions").get<std::vector<std::string>>();
    p.g4UiCommands=j.at("g4ui_commands").get<std::vector<std::string>>();
    p.g4VisUiCommands=j.at("g4visui_commands").get<std::vector<std::string>>();
    p.hepmc3InputFile = j.at("HepMC3 file").get<std::string>();
    p.hepmc3TypeOfFile = j.at("HepMC3 type of file").get<std::string>();
    p.generatorPlugin = j.at("Generator Plugin").get<std::string>();
}

inline void from_json(const json& j, regionConfig& r) {
    r.regionName=j.at("RegionName").get<std::string>();
    r.rootLVNames=j.at("RootLVNames").get<std::vector<G4String>>();
    r.gammaCut=j.at("GammaCut").get<double>();
    r.electronCut=j.at("ElectronCut").get<double>();
    r.positronCut=j.at("PositronCut").get<double>();
    r.protonCut=j.at("ProtonCut").get<double>();

}

auto parse_json_file(std::string config_file_name)
{
    std::ifstream ifs(config_file_name);
    jf=json::parse(ifs);
    
    //read and store the configuration into the fslConfig struct
    simConfig::from_json(jf, fsl);

}



} // namespace fslconf

//class FSLConfigurator {
//
//public:
//
//    FSLConfigurator(){};
//    ~FSLConfigurator(){};
//    FSLConfigurator(const FSLConfigurator&) = delete;
//    FSLConfigurator& operator=(const FSLConfigurator&) = delete;
//
//
//
//private:
//
//    simConfig::fslConfig fSimConfigurator;
//}; // FSLConfigurator

#endif // FSLConfigurator_h 1

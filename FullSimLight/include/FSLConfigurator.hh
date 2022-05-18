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
    std::vector<std::string> rootLVNames;
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
    
    std::vector<std::string> sensitiveDetectors;
    std::string outputHitsFile;
    std::string outputHistoFile;
    
    std::vector<regionConfig> regionsData;
    
    std::string magFieldIntensity;
    std::string magFieldMap;
    std::string magFieldPlugin;
    
    std::vector<std::string> userActionExtensions;
    std::vector<std::string> g4UiCommands;
};

inline void to_json(json& j, const fslConfig& p) {
    j = json{{"Geometry", p.geometry},{"Physics list name", p.physicsList},{"Number of events", p.nEvents},{"Magnetic field intensity", p.magFieldIntensity},{"Generator", p.eventGeneratorName},{"Event input file", p.eventInputFile},{"Type of event", p.typeOfEvent},{"Sensitive Detector Extensions", p.sensitiveDetectors},{"Output Hits file", p.outputHitsFile},{"Output Histo file", p.outputHistoFile},{"Magnetic Field Map", p.magFieldMap},{"Magnetic Field Plugin", p.magFieldPlugin},{"User Action Extensions", p.userActionExtensions},{"g4ui_commands", p.g4UiCommands}};
    
}
inline void to_json(json& j, const regionConfig& r) {
    j = json{{"RegionName", r.regionName},{"RootLVNames", r.rootLVNames},{"GammaCut", r.gammaCut},{"ElectronCut", r.electronCut},{"PositronCut", r.positronCut},{"ProtonCut", r.protonCut}};
}
inline void from_json(const json& j, fslConfig& p) {
    p.geometry=j.at("Geometry").get<std::string>();
    p.physicsList=j.at("Physics list name").get<std::string>();
    p.nEvents=j.at("Number of events").get<int>();
    p.magFieldIntensity=j.at("Magnetic field intensity").get<std::string>();
    p.eventGeneratorName=j.at("Generator").get<std::string>();
    p.eventInputFile=j.at("Event input file").get<std::string>();
    p.typeOfEvent=j.at("Type of event").get<std::string>();
    p.sensitiveDetectors=j.at("Sensitive Detector Extensions").get<std::vector<std::string>>();
    p.outputHitsFile=j.at("Output Hits file").get<std::string>();
    p.outputHistoFile=j.at("Output Histo file").get<std::string>();
    p.magFieldMap=j.at("Magnetic Field Map").get<std::string>();
    p.magFieldPlugin=j.at("Magnetic Field Plugin").get<std::string>();
    p.userActionExtensions=j.at("User Action Extensions").get<std::vector<std::string>>();
    p.g4UiCommands=j.at("g4ui_commands").get<std::vector<std::string>>();
    
}

inline void from_json(const json& j, regionConfig& r) {
    r.regionName=j.at("RegionName").get<std::string>();
    r.rootLVNames=j.at("RootLVNames").get<std::vector<std::string>>();
    r.gammaCut=j.at("GammaCut").get<double>();
    r.electronCut=j.at("ElectronCut").get<double>();
    r.positronCut=j.at("PositronCut").get<double>();
    r.protonCut=j.at("ProtonCut").get<double>();

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

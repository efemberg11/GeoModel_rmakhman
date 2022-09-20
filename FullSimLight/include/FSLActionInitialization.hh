
#ifndef FSLActionInitialization_h
#define FSLActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"
#include "GeantinoMapsConfigurator.hh"

class FSLActionInitialization : public G4VUserActionInitialization {

public:

  FSLActionInitialization(bool isperformance=false, bool customUserActions = false);
 ~FSLActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;

  void SetPerformanceModeFlag(bool val) { fIsPerformance = val; }
  void SetSpecialScoringRegionName(const G4String& rname) { fSpecialScoringRegionName = rname; }
    
  void SetActions(std::vector<std::string> &usrActions)
    {
      userActions = usrActions;
    };
  void SetGenerator(std::string gen)
    {
      generator = gen;
    }
  void SetHepMC3FilePath(std::string file_path)
    {
      hepmc3_file_path = file_path;
    }
  void SetHepMC3FileType(std::string file_type)
    {
      hepmc3_file_type = file_type;
    }
  void SetGeneratorPlugin(std::string gen_plug)
    {
      generator_plugin = gen_plug;
    }
    
private:
  GeantinoMapsConfigurator* fGeantinoMapsConfig;
  bool     fIsPerformance;
  bool     fCustomUserActions;
  G4String fSpecialScoringRegionName;
  std::string generator;
  std::string hepmc3_file_path = "";
  std::string hepmc3_file_type = "";
  std::string generator_plugin = "";

    
  std::vector<std::string> userActions;
    


};

#endif

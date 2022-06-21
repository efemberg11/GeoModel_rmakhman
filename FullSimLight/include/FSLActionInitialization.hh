
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
    
  void SetActions(std::vector<std::string> &evtActions,
                    std::vector<std::string> &rActions,
                    std::vector<std::string> &trActions,
                    std::vector<std::string> &steActions,
                    std::vector<std::string> &staActions)
    {
        eventActions = evtActions;
        runActions = rActions;
        trackingActions = trActions;
        steppingActions = steActions;
        stackingActions = staActions;
    };
    
private:
  GeantinoMapsConfigurator* fGeantinoMapsConfig;
  bool     fIsPerformance;
  bool     fCustomUserActions;
  G4String fSpecialScoringRegionName;
    
    
  std::vector<std::string> eventActions;
  std::vector<std::string> runActions;
  std::vector<std::string> trackingActions;
  std::vector<std::string> steppingActions;
  std::vector<std::string> stackingActions;
    


};

#endif

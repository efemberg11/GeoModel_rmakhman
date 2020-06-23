/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef G4UserActions_MyLengthIntegratorSteppingAction_H
#define G4UserActions_MyLengthIntegratorSteppingAction_H

//#include "GaudiKernel/ITHistSvc.h"
//#include "GaudiKernel/ServiceHandle.h"

#include "G4Pow.hh"
//#include "TString.h"

#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"

#include <string>
#include <map>

//G4AnalysisManager
#include "MyAnalysis.hh"

// Forward declarations
//class TProfile;
//class TProfile2D;
class MyRunAction;


namespace G4UA
{

  /// @class MyLengthIntegratorSteppingAction
  /// @brief A user action used to evaluate thickness of all detectors
  ///        traversed by outgoing particles.
  ///
  /// This user action is currently used only in special runs with geantinos.
  /// Thickness is recorded in terms of both rad length and int length.
  ///
  /// NOTE: the current design is safe for multi-threading, but _not_
  /// performant due to sharing of the histograms and excessive locking. If
  /// this action needs to be used in multi-threaded jobs, we can rewrite it so
  /// that each instance has its own copy of the histograms which get merged in
  /// finalization of the LengthIntegratorTool.
  ///
  class MyLengthIntegratorSteppingAction final : public G4UserSteppingAction
  {

    public:

      /// Constructor takes the name of the histogram service as argument.
      MyLengthIntegratorSteppingAction(MyRunAction* myrun, G4double rlimit =12500 ,G4double zlimit=23000, G4double xlimit=12500, G4double ylimit=12500, bool createDetectorsMaps = false, bool createMaterialsMaps = false, bool createElementsMaps = false);
      /// Destructor
      ~MyLengthIntegratorSteppingAction();

      /// Called at every particle step to accumulate thickness.
      virtual void UserSteppingAction(const G4Step*) override;
      
      void SetRlimit(G4double r){fRlimit = r;}
      void SetZlimit(G4double z){fZlimit = z;}
      void SetXlimit(G4double x){fXlimit = x;}
      void SetYlimit(G4double y){fYlimit = y;}
      void SetCreateDetectorsMaps(bool val) { fCreateDetectorsMaps = val; }
      void SetCreateMaterialsMaps(bool val) { fCreateMaterialsMaps = val; }
      void SetCreateElementsMaps (bool val) { fCreateElementsMaps = val; }

    private:

      // Holder for G4 math tools
      G4Pow* m_g4pow;

      // Add elements and values into the map
      void addToDetThickMap(std::string, double, double);

      /// this method checks if a histo is on THsvc already and caches a local pointer to it
      /// if the histo is not present, it creates and registers it
//      TProfile2D* getOrCreateProfile(std::string regName, TString histoname, TString xtitle, int nbinsx, float xmin, float xmax,TString ytitle, int nbinsy,float ymin, float ymax,TString ztitle);
      //Geant4 version
      G4int getOrCreateProfile_g4(G4String regName, G4String histoname, G4String xtitle, int nbinsx, float xmin, float xmax,G4String ytitle, int nbinsy,float ymin, float ymax,G4String ztitle);


  public:
      // TO DO: make it private again and add GetDetThickMap method
      /// Map of detector thickness measurements for current event
      std::map<std::string, std::pair<double, double> > m_detThickMap;
  private:
      /// Pointer to the MyRunAction, needed to create new Profiles
      MyRunAction* m_run;
      
      ///R,Z,X,Y limits for geantino maps
      G4double fRlimit;
      G4double fZlimit;
      G4double fXlimit;
      G4double fYlimit;
      /// Flag to create detectors geantino maps
      bool fCreateDetectorsMaps;
      /// Flag to create materials geantino maps
      bool fCreateMaterialsMaps;
      /// Flag to create elements geantino maps
      bool fCreateElementsMaps;
    
      
      
//      /// Rad-length profile hist in R-Z - ROOT
//      TProfile2D* m_rzProfRL;
//      /// Int-length profile hist in R-Z - ROOT
//      TProfile2D* m_rzProfIL;
      
      /// Rad-length profile hist in R-Z - Geant4  -- taken from the runAction
      //G4int m_rzProfRL_id;
      /// Int-length profile hist in R-Z  - Geant4 -- taken from the runAction
      //G4int m_rzProfIL_id;
      
//      // 2D plots of rad-length and int-length - ROOT
//      std::map<std::string,TProfile2D*,std::less<std::string> > m_rzMapRL;
//      std::map<std::string,TProfile2D*,std::less<std::string> > m_xyMapRL;
//      
//      std::map<std::string,TProfile2D*,std::less<std::string> > m_rzMapIL;
//      std::map<std::string,TProfile2D*,std::less<std::string> > m_xyMapIL;
      
      // 2D plots of rad-length and int-length - Geant4
      std::map<std::string,G4int,std::less<std::string> > m_rzMapRL_g4;
      std::map<std::string,G4int,std::less<std::string> > m_xyMapRL_g4;
      
      std::map<std::string,G4int,std::less<std::string> > m_rzMapIL_g4;
      std::map<std::string,G4int,std::less<std::string> > m_xyMapIL_g4;

  }; // class MyLengthIntegratorSteppingAction

} // namespace G4UA

#endif


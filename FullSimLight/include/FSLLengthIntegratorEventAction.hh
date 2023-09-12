/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef G4UserActions_FSLLengthIntegratorEventAction_H
#define G4UserActions_FSLLengthIntegratorEventAction_H

//#include "GaudiKernel/ITHistSvc.h"
//#include "GaudiKernel/ServiceHandle.h"

#include "G4Pow.hh"
//#include "TString.h"

#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"

#include "FSLLengthIntegratorSteppingAction.hh"
#include "FSLRunAction.hh"
#include "GeantinoMapsConfigurator.hh"

#include <string>
#include <map>

// Forward declarations
//class TProfile;
//class TProfile2D;


namespace G4UA
{

  /// @class FSLLengthIntegratorEventAction
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
  class FSLLengthIntegratorEventAction final : public G4UserEventAction
  {

    public:

      /// Constructor takes the name of the histogram service as argument.
      FSLLengthIntegratorEventAction(FSLLengthIntegratorSteppingAction*, FSLRunAction* run);
      /// Destructor
      ~FSLLengthIntegratorEventAction();

      /// Called at beginning of G4 event to cache some details about the
      /// current primary vertex and particle. Also resets some measurements.
      virtual void BeginOfEventAction(const G4Event*) override;

      /// Called at end of G4 event to finalize measurements and fill hists
      virtual void EndOfEventAction(const G4Event*) override;
      
      /// Pointer to the RunAction
      FSLRunAction*  m_run;

    private:
      /// Pointer to FSLLengthIntegratorSteppingAction
      FSLLengthIntegratorSteppingAction* m_stepAct;
      GeantinoMapsConfigurator* fGeantinoMapsConfig;
      
//      /// Setup one set of measurement hists for a detector name - ROOT
//      void regAndFillHist(const std::string&, const std::pair<double, double>&);
      
      /// Setup one set of measurement hists for a detector name - G4
      void regAndFillHist_g4(const std::string&, const std::pair<double, double>&);

      /// Cached eta of the current primary
      double m_etaPrimary;
      /// Cached phi of the current primary
      double m_phiPrimary;
      

//      /// Rad-length profile hist in eta - ROOT
//      std::map<std::string, TProfile*> m_etaMapRL;
//      /// Rad-length profile hist in phi - ROOT
//      std::map<std::string, TProfile*> m_phiMapRL;
      
      /// Rad-length profile hist in eta - Geant4
      std::map<std::string, G4int> m_etaMapRL_g4;
      /// Rad-length profile hist in phi - Geant4
      std::map<std::string, G4int> m_phiMapRL_g4;

//      /// Int-length profile hist in eta - ROOT
//      std::map<std::string, TProfile*> m_etaMapIL;
//      /// Int-length profile hist in phi - ROOT
//      std::map<std::string, TProfile*> m_phiMapIL;
      
      /// Int-length profile hist in eta - Geant4
      std::map<std::string, G4int> m_etaMapIL_g4;
      /// Int-length profile hist in phi - Geant4
      std::map<std::string, G4int> m_phiMapIL_g4;
      

  }; // class FSLLengthIntegratorEventAction

} // namespace G4UA

#endif


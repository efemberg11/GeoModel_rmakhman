/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef G4UserActions_MyLengthIntegratorEventAction_H
#define G4UserActions_MyLengthIntegratorEventAction_H

//#include "GaudiKernel/ITHistSvc.h"
//#include "GaudiKernel/ServiceHandle.h"

#include "G4Pow.hh"
#include "TString.h"

#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"

#include "MyLengthIntegratorSteppingAction.hh"

#include <string>
#include <map>

// Forward declarations
class TProfile;
class TProfile2D;


namespace G4UA
{

  /// @class MyLengthIntegratorEventAction
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
  class MyLengthIntegratorEventAction final : public G4UserEventAction
  {

    public:

      /// Constructor takes the name of the histogram service as argument.
      MyLengthIntegratorEventAction(MyLengthIntegratorSteppingAction*);
      /// Destructor
      ~MyLengthIntegratorEventAction();

      /// Called at beginning of G4 event to cache some details about the
      /// current primary vertex and particle. Also resets some measurements.
      virtual void BeginOfEventAction(const G4Event*) override;

      /// Called at end of G4 event to finalize measurements and fill hists
      virtual void EndOfEventAction(const G4Event*) override;


    private:
      /// Pointer to MyLengthIntegratorSteppingAction
      MyLengthIntegratorSteppingAction* m_stepAct;
      
      /// Setup one set of measurement hists for a detector name.
      void regAndFillHist(const std::string&, const std::pair<double, double>&);

      /// Cached eta of the current primary
      double m_etaPrimary;
      /// Cached phi of the current primary
      double m_phiPrimary;

      /// Rad-length profile hist in eta
      std::map<std::string, TProfile*> m_etaMapRL;
      /// Rad-length profile hist in phi
      std::map<std::string, TProfile*> m_phiMapRL;

      /// Int-length profile hist in eta
      std::map<std::string, TProfile*> m_etaMapIL;
      /// Int-length profile hist in phi
      std::map<std::string, TProfile*> m_phiMapIL;


  }; // class MyLengthIntegratorEventAction

} // namespace G4UA

#endif


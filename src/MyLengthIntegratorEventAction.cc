/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#include "MyLengthIntegratorEventAction.hh"


#include "TProfile.h"
#include "TProfile2D.h"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "G4Material.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4Pow.hh"
#include "G4Types.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TDirectory.h"

// System includes
#include <mutex>


// Anonymous namespace for file-global mutexes and helper functions
namespace
{
  /// Mutex used to protect access to the thread-unsafe THistSvc
  std::mutex gHistSvcMutex;
}

namespace G4UA
{

  //---------------------------------------------------------------------------
  // Constructor
  //---------------------------------------------------------------------------
  MyLengthIntegratorEventAction::MyLengthIntegratorEventAction(MyLengthIntegratorSteppingAction* stepAct)
    : m_g4pow(0),
      m_stepAct(stepAct),
      m_etaPrimary(0), m_phiPrimary(0),
      m_rzProfRL(nullptr), m_rzProfIL(nullptr)
{
    // Protect concurrent access to the non-thread-safe hist svc
    std::lock_guard<std::mutex> lock(gHistSvcMutex);
    std::cout<<" MyLengthIntegratorEventAction Register the RZ profiles "<<std::endl;
    // Register the RZ profiles. The other profiles need to wait until the end
    // of the events as they are created only if used.
    //const char* radName = "/lengths/radLen/RZRadLen";
    //if(!getHist(m_hSvc, radName, m_rzProfRL)) {
      m_rzProfRL = new TProfile2D("RZRadLen","RZRadLen",1000,-25000.,25000.,2000,0.,15000.);
    //  regHist(m_hSvc, radName, m_rzProfRL);
    //}

    //const char* intName = "/lengths/intLen/RZIntLen";
    //if(!getHist(m_hSvc, intName, m_rzProfIL)) {
      m_rzProfIL = new TProfile2D("RZIntLen","RZIntLen",1000,-25000.,25000.,2000,0.,15000.);
    //  regHist(m_hSvc, intName, m_rzProfIL);
    //}

    m_g4pow = G4Pow::GetInstance();

  }
  MyLengthIntegratorEventAction::~MyLengthIntegratorEventAction()
  {

    // Clear histogram maps
    m_etaMapRL.clear();
    m_phiMapRL.clear();
    m_etaMapIL.clear();
    m_phiMapIL.clear();

    std::cout<<"END of MyLengthIntegratorEventAction" << std::endl;
  }
        
    

  //---------------------------------------------------------------------------
  // Cache primary info at beginning of event
  //---------------------------------------------------------------------------
  void MyLengthIntegratorEventAction::BeginOfEventAction(const G4Event* event)
  {
    std::cout<<" ****** MyLengthIntegratorEventAction::BeginOfEventAction  ****** " <<std::endl;
    //m_detThickMap.clear();
    G4PrimaryVertex* vert = event->GetPrimaryVertex(0);
    G4PrimaryParticle* part = vert->GetPrimary();
    G4ThreeVector mom = part->GetMomentum();
    std::cout<<" ****** Primary Particle: "<< part->GetParticleDefinition()->GetParticleName()<<" ****** " <<std::endl;
    std::cout<<" ****** Momentum: "<< mom<<" ****** " <<std::endl;
    m_etaPrimary = mom.eta();
    m_phiPrimary = mom.phi();
    std::cout.precision(6);
    std::cout<<" ****** Eta: "<< m_etaPrimary<<" ****** " <<std::endl;
    std::cout<<" ****** Phi: "<< m_phiPrimary<<" ****** " <<std::endl;
    std::cout<<" ****** ****** ****** ****** ****** ******  ****** "<<std::endl;
      
  }

  //---------------------------------------------------------------------------
  // Finalize event measurements
  //---------------------------------------------------------------------------
  void MyLengthIntegratorEventAction::EndOfEventAction(const G4Event*)
  {
    // Lazily protect this whole code from concurrent access
    std::lock_guard<std::mutex> lock(gHistSvcMutex);
    std::cout<<" ****** MyLengthIntegratorEventAction::EndOfEventAction  ****** " <<std::endl;
    //m_stepAct
      if (m_stepAct->m_detThickMap.size()==0){std::cout<<" size is zero"<<std::endl;
          exit(-1);
      }

    // Loop over volumes
    for (auto& it : m_stepAct->m_detThickMap) {

       std::cout<<" ****** Loop over volumes  ****** " <<it.first<<std::endl;
      // If histos already exist, then fill them
      if (m_etaMapRL.find(it.first) != m_etaMapRL.end()) {
        std::cout<<"Histos already exist, fill them"<<std::endl;
        m_etaMapRL[it.first]->Fill(m_etaPrimary, it.second.first, 1.);
        m_phiMapRL[it.first]->Fill(m_phiPrimary, it.second.first, 1.);

        m_etaMapIL[it.first]->Fill(m_etaPrimary, it.second.second, 1.);
        m_phiMapIL[it.first]->Fill(m_phiPrimary, it.second.second, 1.);
      }
      // New detector volume; register it
      else {
          std::cout<<"New detector volume; register it: "<<it.first<<std::endl;
        regAndFillHist(it.first, it.second);
      }

    } // Loop over detectors

    /////////////////////////////////////////////////
    // Putting this here, as normally I'd add the following code into a finalize function (as has been done above for the Athena(non-MP) code), but I'm not sure if overloading the finalize function in AthenaMP will break the histogram merging at the end of the job. If it wont, then the following code can be put in a finalize function, which will speed up AthenaMP jobs.

    // Adding zeros to TProfile bins, so that each bin contains the same number of entries,
    // so that a THStack of all the material TProfile plots (or all the element plots) will equal the Total_X0 TProfile plot
    // It's because each plot (for each material, say) is only filled if a Geantion hits it, not if it's not hit in an event
    std::cout<<"Doing stuff"<<std::endl;
    TProfile* totalEtaRL = m_etaMapRL["Total_X0"];

      if(totalEtaRL==NULL) std::cout<<" NULL"<<std::endl;
    int nbins = totalEtaRL->GetNbinsX();

    std::cout<<"m_etaMapRL nbins: "<<nbins<<std::endl;
    for (auto it : m_etaMapRL){

      if( it.first != "Total_X0" ){
          TProfile* plot = m_etaMapRL[it.first];
          for(int i=0 ; i<=nbins ; i++){
              double x_value = plot->GetBinCenter(i);
              int n_enties_plot = plot->GetBinEntries(i);
              int n_enties_ref = totalEtaRL->GetBinEntries(i);
              int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
              for(int j=0 ; j<n_zeros_to_be_added ; j++){
                  plot->Fill(x_value,0.0);
              }
          }
      }
    }

    TProfile* totalPhiRL = m_phiMapRL["Total_X0"];
    nbins = totalPhiRL->GetNbinsX();
    for (auto it : m_phiMapRL){
        if( it.first != "Total_X0" ){
            TProfile* plot = m_phiMapRL[it.first];
            for(int i=0 ; i<=nbins ; i++){
                double x_value = plot->GetBinCenter(i);
                int n_enties_plot = plot->GetBinEntries(i);
                int n_enties_ref = totalPhiRL->GetBinEntries(i);
                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                for(int j=0 ; j<n_zeros_to_be_added ; j++){
                    plot->Fill(x_value,0.0);
                }
            }
        }
    }

    TProfile* totalEtaIL = m_etaMapIL["Total_X0"];
    nbins = totalEtaIL->GetNbinsX();
    for (auto it : m_etaMapIL){
        if( it.first != "Total_X0" ){
            TProfile* plot = m_etaMapIL[it.first];
            for(int i=0 ; i<=nbins ; i++){
                double x_value = plot->GetBinCenter(i);
                int n_enties_plot = plot->GetBinEntries(i);
                int n_enties_ref = totalEtaIL->GetBinEntries(i);
                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                for(int j=0 ; j<n_zeros_to_be_added ; j++){
                    plot->Fill(x_value,0.0);
                }
            }
        }
    }

    TProfile* totalPhiIL = m_phiMapIL["Total_X0"];
    nbins = totalPhiIL->GetNbinsX();
    for (auto it : m_phiMapIL){
        if( it.first != "Total_X0" ){
            TProfile* plot = m_phiMapIL[it.first];
            for(int i=0 ; i<=nbins ; i++){
                double x_value = plot->GetBinCenter(i);
                int n_enties_plot = plot->GetBinEntries(i);
                int n_enties_ref = totalPhiIL->GetBinEntries(i);
                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                for(int j=0 ; j<n_zeros_to_be_added ; j++){
                    plot->Fill(x_value,0.0);
                }
            }
        }
    }
      
      std::cout<<" ****** MyLengthIntegratorEventAction::EndOfEventAction - DONE ****** " <<std::endl;
  }

  //---------------------------------------------------------------------------
  // Setup hists for one detector
  //---------------------------------------------------------------------------
  void MyLengthIntegratorEventAction::regAndFillHist(const std::string& detName,
                                        const std::pair<double, double>& thicks)
  {
    TProfile* profEtaRL = nullptr;
    TProfile* profEtaIL = nullptr;
    TProfile* profPhiRL = nullptr;
    TProfile* profPhiIL = nullptr;

    auto pathEtaRL = "/lengths/radLen/" + detName + "_RL";
    auto pathEtaIL = "/lengths/intLen/" + detName + "_IL";
    auto pathPhiRL = "/lengths/radLen/" + detName + "Phi_RL";
    auto pathPhiIL = "/lengths/intLen/" + detName + "Phi_IL";

    // Eta rad profile
    //if(!getHist(m_hSvc, pathEtaRL, profEtaRL))
      {
      const std::string name(detName+"_RL");
      profEtaRL = new TProfile(name.c_str(), name.c_str(), 1000, -6., 6.);
      profEtaRL->GetXaxis()->SetTitle("#eta");
      profEtaRL->GetYaxis()->SetTitle("%X0");
      //regHist(m_hSvc, pathEtaRL, profEtaRL);
      }
    // Eta int profile
    //if(!getHist(m_hSvc, pathEtaIL, profEtaIL))
      {
      const std::string name(detName+"_IL");
      profEtaIL = new TProfile(name.c_str(), name.c_str(), 1000, -6., 6.);
      profEtaIL->GetXaxis()->SetTitle("#eta");
      profEtaIL->GetYaxis()->SetTitle("#lambda");
      //regHist(m_hSvc, pathEtaIL, profEtaIL);
    }
    // Phi rad profile
    //if(!getHist(m_hSvc, pathPhiRL, profPhiRL))
      {
      const std::string name(detName+"Phi_RL");
      profPhiRL = new TProfile(name.c_str(), name.c_str(), 500, -M_PI, M_PI);
      profPhiRL->GetXaxis()->SetTitle("#phi");
      profPhiRL->GetYaxis()->SetTitle("%X0");
      //regHist(m_hSvc, pathPhiRL, profPhiRL);
    }
    // Phi int profile
    //if(!getHist(m_hSvc, pathPhiIL, profPhiIL))
      {
      const std::string name(detName+"Phi_IL");
      profPhiIL = new TProfile(name.c_str(), name.c_str(), 500, -M_PI, M_PI);
      profPhiIL->GetXaxis()->SetTitle("#phi");
      profPhiIL->GetYaxis()->SetTitle("#lambda");
      //regHist(m_hSvc, pathPhiIL, profPhiIL);
    }

    m_etaMapRL[detName] = profEtaRL;
    m_etaMapIL[detName] = profEtaIL;
    m_phiMapRL[detName] = profPhiRL;
    m_phiMapIL[detName] = profPhiIL;

    profEtaRL->Fill(m_etaPrimary, thicks.first, 1.);
    profEtaIL->Fill(m_etaPrimary, thicks.second, 1.);
    profPhiRL->Fill(m_phiPrimary, thicks.first, 1.);
    profPhiIL->Fill(m_phiPrimary, thicks.second, 1.);
  }

} // namespace G4UA


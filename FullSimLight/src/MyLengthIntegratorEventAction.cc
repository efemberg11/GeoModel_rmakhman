/*
 Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
 */
#if G4VERSION_NUMBER>=1040

#include "MyLengthIntegratorEventAction.hh"

//#include "TProfile.h"
//#include "TProfile2D.h"
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

//#include "TFile.h"
//#include "TDirectory.h"

// System includes
#include <mutex>

//G4AnalysisManager
#include "MyAnalysis.hh"


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
    MyLengthIntegratorEventAction::MyLengthIntegratorEventAction(MyLengthIntegratorSteppingAction* stepAct, MyRunAction* run, bool createEtaPhiMaps)
    :m_run(run),
    m_stepAct(stepAct),
    m_etaPrimary(0),
    m_phiPrimary(0),
    m_createEtaPhiMaps(createEtaPhiMaps){}
    
    MyLengthIntegratorEventAction::~MyLengthIntegratorEventAction(){
        
        m_etaMapRL_g4.clear();
        m_phiMapRL_g4.clear();
        m_etaMapIL_g4.clear();
        m_phiMapIL_g4.clear();
        
        //      //ROOT
        //      static std::mutex mutex_instance;
        //      std::lock_guard<std::mutex> lock(mutex_instance);
        //      G4cout<<"Finalizing MyLengthIntegratorEventAction" << G4endl;
        //      // Open output root file
        //      std::string fileName = "EtaPhi_RadLen.root";
        //      TFile* fOut = new TFile(fileName.c_str(), "RECREATE");
        //      std::string directory = "cells_"+fileName;
        //      TDirectory* dir = fOut->mkdir(directory.c_str());
        //      dir->cd();
        //
        //      // Save histograms and close file
        //      for (auto hist : m_etaMapRL) {
        //          hist.second->Write();
        //      }
        //      for (auto hist : m_etaMapIL) {
        //          hist.second->Write();
        //
        //      }
        //      for (auto hist : m_phiMapRL) {
        //          hist.second->Write();
        //
        //      }
        //      for (auto hist : m_phiMapIL) {
        //          hist.second->Write();
        //
        //      }
        //
        //      fOut->Write();
        //      fOut->Close();
        //      G4cout<<"ROOT File EtaPhi_RadLen.root closed" << G4endl;
        
        //    // Clear histogram maps
        //    m_etaMapRL.clear();
        //    m_phiMapRL.clear();
        //    m_etaMapIL.clear();
        //    m_phiMapIL.clear();
        //    //~ROOT
        
        
    }
    
    
    //---------------------------------------------------------------------------
    // Cache primary info at beginning of event
    //---------------------------------------------------------------------------
    void MyLengthIntegratorEventAction::BeginOfEventAction(const G4Event* event)
    {
        std::cout << "\n ========================================================= "      <<std::endl;
        G4cout  << " ****** BeginOfEventAction  ****** " <<G4endl;
        //clear the detThickMap for the event that begins
        m_stepAct->m_detThickMap.clear();
        G4PrimaryVertex* vert = event->GetPrimaryVertex(0);
        G4PrimaryParticle* part = vert->GetPrimary();
        G4ThreeVector mom = part->GetMomentum();
        G4cout<<" ****** Primary Particle: "<< part->GetParticleDefinition()->GetParticleName()<<" ****** " <<G4endl;
        G4cout<<" ****** Momentum: "<< mom <<" ****** " <<G4endl;
        m_etaPrimary = mom.eta();
        m_phiPrimary = mom.phi();
        G4cout.precision(6);
        G4cout<<" ****** Eta: "<< m_etaPrimary<<" ****** " <<G4endl;
        G4cout<<" ****** Phi: "<< m_phiPrimary<<" ****** " <<G4endl;
        
    }
    
    //---------------------------------------------------------------------------
    // Finalize event measurements
    //---------------------------------------------------------------------------
    void MyLengthIntegratorEventAction::EndOfEventAction(const G4Event*)
    {
        G4cout    <<" ****** EndOfEventAction  ****** "  << G4endl;
        auto analysisManager = G4AnalysisManager::Instance();
        // Lazily protect this whole code from concurrent access
        std::lock_guard<std::mutex> lock(gHistSvcMutex);
        
        //m_stepAct
        if (m_stepAct->m_detThickMap.size()==0){
            G4cout<<" m_detThickMap size is zero! "<<G4endl;
            exit(-1);
        }
        // Loop over volumes
        for (auto& it : m_stepAct->m_detThickMap) {
            if(it.first=="Total_X0"){
                analysisManager->FillP1(m_run->fEtaRad_id, m_etaPrimary, it.second.first, 1.);
                analysisManager->FillP1(m_run->fEtaInt_id, m_etaPrimary, it.second.second, 1.);
            }
        }
        
        
        if(m_createEtaPhiMaps){
            
            // Loop over volumes
            for (auto& it : m_stepAct->m_detThickMap) {
                
                //G4cout<<" ****** Loop over volumes  ****** " <<it.first<<G4endl;
                //      //ROOT
                //      // If histos already exist, then fill them
                //      if (m_etaMapRL.find(it.first) != m_etaMapRL.end()) {
                //        G4cout<<"ROOT: Histos already exist, fill them"<<G4endl;
                //        m_etaMapRL[it.first]->Fill(m_etaPrimary, it.second.first, 1.);
                //        m_phiMapRL[it.first]->Fill(m_phiPrimary, it.second.first, 1.);
                //
                //        m_etaMapIL[it.first]->Fill(m_etaPrimary, it.second.second, 1.);
                //        m_phiMapIL[it.first]->Fill(m_phiPrimary, it.second.second, 1.);
                //      }
                //      // New detector volume; register it
                //      else {
                //        G4cout<<"ROOT: New detector volume; register it: "<<it.first<<G4endl;
                //        regAndFillHist(it.first, it.second);
                //      }
                //      //~ROOT
                
                
                
                //Geant4
                // If histos already exist, then fill them
                if (m_etaMapRL_g4.find(it.first) != m_etaMapRL_g4.end()){
                    //G4cout<<"GEANT4: Histos already exist, fill them"<<G4endl;
                    analysisManager->FillP1(m_etaMapRL_g4[it.first], m_etaPrimary, it.second.first,  1.);
                    //m_etaMapRL_g4[it.first]->Fill(m_etaPrimary, it.second.first, 1.);
                    analysisManager->FillP1(m_phiMapRL_g4[it.first], m_phiPrimary, it.second.first,  1.);
                    //m_phiMapRL_g4[it.first]->Fill(m_phiPrimary, it.second.first, 1.);
                    analysisManager->FillP1(m_etaMapIL_g4[it.first], m_etaPrimary, it.second.second, 1.);
                    //m_etaMapIL_g4[it.first]->Fill(m_etaPrimary, it.second.second, 1.);
                    analysisManager->FillP1(m_phiMapIL_g4[it.first], m_phiPrimary, it.second.second, 1.);
                    //m_phiMapIL_g4[it.first]->Fill(m_phiPrimary, it.second.second, 1.);
                    
                }
                // New detector volume; register it
                else {
                    //G4cout<<"GEANT4: New detector volume; register it: "<<it.first<<G4endl;
                    regAndFillHist_g4(it.first, it.second);    //Geant4
                    
                }
                //~Geant4
                
            } // Loop over detectors
            
            /////////////////////////////////////////////////
            // Putting this here, as normally I'd add the following code into a finalize function (as has been done above for the Athena(non-MP) code), but I'm not sure if overloading the finalize function in AthenaMP will break the histogram merging at the end of the job. If it wont, then the following code can be put in a finalize function, which will speed up AthenaMP jobs.
            
            // Adding zeros to TProfile bins, so that each bin contains the same number of entries,
            // so that a THStack of all the material TProfile plots (or all the element plots) will equal the Total_X0 TProfile plot
            // It's because each plot (for each material, say) is only filled if a Geantion hits it, not if it's not hit in an event
            
            
            //   //ROOT
            //   TProfile* totalEtaRL = m_etaMapRL["Total_X0"];
            //   int nbins = totalEtaRL->GetNbinsX();
            //   G4cout<<"ROOT: m_etaMapRL nbins: "<<nbins<<G4endl;
            //   for (auto it : m_etaMapRL){
            //
            //      if( it.first != "Total_X0" ){
            //          TProfile* plot = m_etaMapRL[it.first];
            //          for(int i=0 ; i<=nbins ; i++){
            //              double x_value = plot->GetBinCenter(i);
            //              int n_enties_plot = plot->GetBinEntries(i);
            //              int n_enties_ref = totalEtaRL->GetBinEntries(i);
            //              int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
            //              for(int j=0 ; j<n_zeros_to_be_added ; j++){
            //                  plot->Fill(x_value,0.0);
            //              }
            //          }
            //      }
            //    }//~ROOT
            
            //Geant4
            G4int totalEtaRL_g4 = m_etaMapRL_g4["Total_X0"];
            int nbins_g4 = analysisManager->GetP1(totalEtaRL_g4)->axis().bins();
            //G4cout<<"Geant4: m_etaMapRL_g4 nbins: "<<nbins_g4<<G4endl;
            for (auto it : m_etaMapRL_g4){
                
                if( it.first != "Total_X0" ){
                    G4int plot = m_etaMapRL_g4[it.first];
                    for(int i=0 ; i<=nbins_g4 ; i++){
                        double x_value = analysisManager->GetP1(plot)->bin_center(i);
                        int n_enties_plot = analysisManager->GetP1(plot)->bin_entries(i);
                        int n_enties_ref  = analysisManager->GetP1(totalEtaRL_g4)->bin_entries(i);
                        int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                        for(int j=0 ; j<n_zeros_to_be_added ; j++){
                            analysisManager->FillP1(plot,x_value,0.0);
                        }
                    }
                }
            }//~Geant4
            
            //    //ROOT
            //    TProfile* totalPhiRL = m_phiMapRL["Total_X0"];
            //    nbins = totalPhiRL->GetNbinsX();
            //    G4cout<<"ROOT: m_phiMapRL nbins: "<<nbins<<G4endl;
            //    for (auto it : m_phiMapRL){
            //        if( it.first != "Total_X0" ){
            //            TProfile* plot = m_phiMapRL[it.first];
            //            for(int i=0 ; i<=nbins ; i++){
            //                double x_value = plot->GetBinCenter(i);
            //                int n_enties_plot = plot->GetBinEntries(i);
            //                int n_enties_ref = totalPhiRL->GetBinEntries(i);
            //                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
            //                for(int j=0 ; j<n_zeros_to_be_added ; j++){
            //                    plot->Fill(x_value,0.0);
            //                }
            //            }
            //        }
            //    }//~ROOT
            
            //Geant4
            G4int totalPhiRL_g4 = m_phiMapRL_g4["Total_X0"];
            nbins_g4 = analysisManager->GetP1(totalPhiRL_g4)->axis().bins();
            //G4cout<<"Geant4: m_phiMapRL_g4 nbins: "<<nbins_g4<<G4endl;
            for (auto it : m_phiMapRL_g4){
                
                if( it.first != "Total_X0" ){
                    G4int plot = m_phiMapRL_g4[it.first];
                    for(int i=0 ; i<=nbins_g4 ; i++){
                        double x_value = analysisManager->GetP1(plot)->bin_center(i);
                        int n_enties_plot = analysisManager->GetP1(plot)->bin_entries(i);
                        int n_enties_ref  = analysisManager->GetP1(totalPhiRL_g4)->bin_entries(i);
                        int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                        for(int j=0 ; j<n_zeros_to_be_added ; j++){
                            analysisManager->FillP1(plot,x_value,0.0);
                        }
                    }
                }
            }//~Geant4
            
            //    //ROOT
            //    TProfile* totalEtaIL = m_etaMapIL["Total_X0"];
            //    nbins = totalEtaIL->GetNbinsX();
            //    G4cout<<"ROOT: m_etaMapIL nbins: "<<nbins<<G4endl;
            //    for (auto it : m_etaMapIL){
            //        if( it.first != "Total_X0" ){
            //            TProfile* plot = m_etaMapIL[it.first];
            //            for(int i=0 ; i<=nbins ; i++){
            //                double x_value = plot->GetBinCenter(i);
            //                int n_enties_plot = plot->GetBinEntries(i);
            //                int n_enties_ref = totalEtaIL->GetBinEntries(i);
            //                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
            //                for(int j=0 ; j<n_zeros_to_be_added ; j++){
            //                    plot->Fill(x_value,0.0);
            //                }
            //            }
            //        }
            //    }//~ROOT
            
            //Geant4
            G4int totalEtaIL_g4 = m_etaMapIL_g4["Total_X0"];
            nbins_g4 = analysisManager->GetP1(totalEtaIL_g4)->axis().bins();
            //G4cout<<"Geant4: m_etaMapIL_g4 nbins: "<<nbins_g4<<G4endl;
            for (auto it : m_etaMapIL_g4){
                
                if( it.first != "Total_X0" ){
                    G4int plot = m_etaMapIL_g4[it.first];
                    for(int i=0 ; i<=nbins_g4 ; i++){
                        double x_value = analysisManager->GetP1(plot)->bin_center(i);
                        int n_enties_plot = analysisManager->GetP1(plot)->bin_entries(i);
                        int n_enties_ref  = analysisManager->GetP1(totalEtaIL_g4)->bin_entries(i);
                        int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                        for(int j=0 ; j<n_zeros_to_be_added ; j++){
                            analysisManager->FillP1(plot,x_value,0.0);
                        }
                    }
                }
            }//~Geant4
            
            //    //ROOT
            //    TProfile* totalPhiIL = m_phiMapIL["Total_X0"];
            //    nbins = totalPhiIL->GetNbinsX();
            //    G4cout<<"ROOT: m_phiMapIL nbins: "<<nbins<<G4endl;
            //    for (auto it : m_phiMapIL){
            //        if( it.first != "Total_X0" ){
            //            TProfile* plot = m_phiMapIL[it.first];
            //            for(int i=0 ; i<=nbins ; i++){
            //                double x_value = plot->GetBinCenter(i);
            //                int n_enties_plot = plot->GetBinEntries(i);
            //                int n_enties_ref = totalPhiIL->GetBinEntries(i);
            //                int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
            //                for(int j=0 ; j<n_zeros_to_be_added ; j++){
            //                    plot->Fill(x_value,0.0);
            //                }
            //            }
            //        }
            //    }//~ROOT
            
            //Geant4
            G4int totalPhiIL_g4 = m_phiMapIL_g4["Total_X0"];
            nbins_g4 = analysisManager->GetP1(totalPhiIL_g4)->axis().bins();
            //G4cout<<"Geant4: m_phiMapIL_g4 nbins: "<<nbins_g4<<G4endl;
            for (auto it : m_phiMapIL_g4){
                
                if( it.first != "Total_X0" ){
                    G4int plot = m_phiMapIL_g4[it.first];
                    for(int i=0 ; i<=nbins_g4 ; i++){
                        double x_value = analysisManager->GetP1(plot)->bin_center(i);
                        int n_enties_plot = analysisManager->GetP1(plot)->bin_entries(i);
                        int n_enties_ref  = analysisManager->GetP1(totalPhiIL_g4)->bin_entries(i);
                        int n_zeros_to_be_added = n_enties_ref - n_enties_plot;
                        for(int j=0 ; j<n_zeros_to_be_added ; j++){
                            analysisManager->FillP1(plot,x_value,0.0);
                        }
                    }
                }
            }//~Geant4
        }
        std::cout << "\n ========================================================= "  << std::endl;
    }
    
    //---------------------------------------------------------------------------
    // Setup hists for one detector
    //---------------------------------------------------------------------------
    //  //ROOT Version
    //  void MyLengthIntegratorEventAction::regAndFillHist(const std::string& detName,
    //                                        const std::pair<double, double>& thicks)
    //  {
    //    TProfile* profEtaRL = nullptr;
    //    TProfile* profEtaIL = nullptr;
    //    TProfile* profPhiRL = nullptr;
    //    TProfile* profPhiIL = nullptr;
    //
    ////    auto pathEtaRL = "/lengths/radLen/" + detName + "_RL";
    ////    auto pathEtaIL = "/lengths/intLen/" + detName + "_IL";
    ////    auto pathPhiRL = "/lengths/radLen/" + detName + "Phi_RL";
    ////    auto pathPhiIL = "/lengths/intLen/" + detName + "Phi_IL";
    //
    //    // Eta rad profile
    //    //if(!getHist(m_hSvc, pathEtaRL, profEtaRL))
    //      {
    //      const std::string name(detName+"_RL");
    //      profEtaRL = new TProfile(name.c_str(), name.c_str(), 1000, -6., 6.);
    //      profEtaRL->GetXaxis()->SetTitle("#eta");
    //      profEtaRL->GetYaxis()->SetTitle("%X0");
    //      //regHist(m_hSvc, pathEtaRL, profEtaRL);
    //      }
    //    // Eta int profile
    //    //if(!getHist(m_hSvc, pathEtaIL, profEtaIL))
    //      {
    //      const std::string name(detName+"_IL");
    //      profEtaIL = new TProfile(name.c_str(), name.c_str(), 1000, -6., 6.);
    //      profEtaIL->GetXaxis()->SetTitle("#eta");
    //      profEtaIL->GetYaxis()->SetTitle("#lambda");
    //      //regHist(m_hSvc, pathEtaIL, profEtaIL);
    //    }
    //    // Phi rad profile
    //    //if(!getHist(m_hSvc, pathPhiRL, profPhiRL))
    //      {
    //      const std::string name(detName+"Phi_RL");
    //      profPhiRL = new TProfile(name.c_str(), name.c_str(), 500, -M_PI, M_PI);
    //      profPhiRL->GetXaxis()->SetTitle("#phi");
    //      profPhiRL->GetYaxis()->SetTitle("%X0");
    //      //regHist(m_hSvc, pathPhiRL, profPhiRL);
    //    }
    //    // Phi int profile
    //    //if(!getHist(m_hSvc, pathPhiIL, profPhiIL))
    //      {
    //      const std::string name(detName+"Phi_IL");
    //      profPhiIL = new TProfile(name.c_str(), name.c_str(), 500, -M_PI, M_PI);
    //      profPhiIL->GetXaxis()->SetTitle("#phi");
    //      profPhiIL->GetYaxis()->SetTitle("#lambda");
    //      //regHist(m_hSvc, pathPhiIL, profPhiIL);
    //    }
    //
    //    m_etaMapRL[detName] = profEtaRL;
    //    m_etaMapIL[detName] = profEtaIL;
    //    m_phiMapRL[detName] = profPhiRL;
    //    m_phiMapIL[detName] = profPhiIL;
    //
    //    profEtaRL->Fill(m_etaPrimary, thicks.first, 1.);
    //    profEtaIL->Fill(m_etaPrimary, thicks.second, 1.);
    //    profPhiRL->Fill(m_phiPrimary, thicks.first, 1.);
    //    profPhiIL->Fill(m_phiPrimary, thicks.second, 1.);
    //  }

    void MyLengthIntegratorEventAction::regAndFillHist_g4(const std::string& detName,
                                                          const std::pair<double, double>& thicks)
    {
        auto pathEtaRL = detName + "_RL";
        auto pathEtaIL = detName + "_IL";
        auto pathPhiRL = detName + "Phi_RL";
        auto pathPhiIL = detName + "Phi_IL";
        
        //MOM
        auto analysisManager = G4AnalysisManager::Instance();
        // Eta rad profile
        //
        int id_EtaRL = m_run->fMasterAnalysisManager->GetP1Id(pathEtaRL, false);
        if(id_EtaRL<0) {
            
            //G4cout<<"Geant4: Eta rad profile of "<<pathEtaRL<<" didn't exist, creating P1 now on MASTER:  "<<m_run->fMasterAnalysisManager<<G4endl;
            const std::string name(detName+"_RL");
            id_EtaRL = m_run->fMasterAnalysisManager->CreateP1(pathEtaRL, name.c_str(), 500, -6., 6.);
            m_run->fMasterAnalysisManager->SetP1XAxisTitle(id_EtaRL, "#eta");
            m_run->fMasterAnalysisManager->SetP1YAxisTitle(id_EtaRL, "%X0");
            
        }
        id_EtaRL=analysisManager->GetP1Id(pathEtaRL, false);
        if(id_EtaRL<0)
        {
            //G4cout<<"Geant4: Eta rad profile of "<<pathEtaRL<<" didn't exist, creating P1 now on WORKER: "<<analysisManager<<G4endl;
            const std::string name(detName+"_RL");
            id_EtaRL = analysisManager->CreateP1(pathEtaRL, name.c_str(), 500, -6., 6.);
            analysisManager->SetP1XAxisTitle(id_EtaRL, "#eta");
            analysisManager->SetP1YAxisTitle(id_EtaRL, "%X0");
            
        }
        // Eta int profile
        //
        int id_EtaIL=m_run->fMasterAnalysisManager->GetP1Id(pathEtaIL, false);
        if(id_EtaIL<0)
        {
            //G4cout<<"Geant4: Eta int profile of "<<pathEtaIL<<" didn't exist, creating P1 now on MASTER:  "<<m_run->fMasterAnalysisManager<<G4endl;
            const std::string name(detName+"_IL");
            id_EtaIL = m_run->fMasterAnalysisManager->CreateP1(pathEtaIL, name.c_str(), 500, -6., 6.);
            m_run->fMasterAnalysisManager->SetP1XAxisTitle(id_EtaIL, "#eta");
            m_run->fMasterAnalysisManager->SetP1YAxisTitle(id_EtaIL, "#lambda");
            
        }
        id_EtaIL=analysisManager->GetP1Id(pathEtaIL, false);
        if(id_EtaIL<0)
        {
            //G4cout<<"Geant4: Eta int profile of "<<pathEtaIL<<" didn't exist, creating P1 now on WORKER: "<<analysisManager<<G4endl;
            const std::string name(detName+"_IL");
            id_EtaIL = analysisManager->CreateP1(pathEtaIL, name.c_str(), 500, -6., 6.);
            analysisManager->SetP1XAxisTitle(id_EtaIL, "#eta");
            analysisManager->SetP1YAxisTitle(id_EtaIL, "#lambda");
            
        }
        // Phi rad profile
        //
        int id_PhiRL=m_run->fMasterAnalysisManager->GetP1Id(pathPhiRL, false);
        if(id_PhiRL<0)
        {
            //G4cout<<"Geant4: Phi rad profile of "<<pathPhiRL<<" didn't exist, creating P1 now on MASTER: "<<m_run->fMasterAnalysisManager<<G4endl;
            const std::string name(detName+"Phi_RL");
            id_PhiRL = m_run->fMasterAnalysisManager->CreateP1(pathPhiRL, name.c_str(), 500, -M_PI, M_PI);
            m_run->fMasterAnalysisManager->SetP1XAxisTitle(id_PhiRL, "#phi");
            m_run->fMasterAnalysisManager->SetP1YAxisTitle(id_PhiRL, "%X0");
        }
        id_PhiRL=analysisManager->GetP1Id(pathPhiRL, false);
        if(id_PhiRL<0)
        {
            //G4cout<<"Geant4: Phi rad profile of "<<pathPhiRL<<" didn't exist, creating P1 now on WORKER: "<<analysisManager<<G4endl;
            const std::string name(detName+"Phi_RL");
            id_PhiRL = analysisManager->CreateP1(pathPhiRL, name.c_str(), 500, -M_PI, M_PI);
            analysisManager->SetP1XAxisTitle(id_PhiRL, "#phi");
            analysisManager->SetP1YAxisTitle(id_PhiRL, "%X0");
        }
        // Phi int profile
        //
        int id_PhiIL=m_run->fMasterAnalysisManager->GetP1Id(pathPhiIL, false);
        if(id_PhiIL<0)
        {
            //G4cout<<"Geant4: Phi int profile of "<<pathPhiIL<<" didn't exist, creating P1 now on MASTER: "<<m_run->fMasterAnalysisManager<<G4endl;
            const std::string name(detName+"Phi_IL");
            id_PhiIL = m_run->fMasterAnalysisManager->CreateP1(pathPhiIL, name.c_str(), 500, -M_PI, M_PI);
            m_run->fMasterAnalysisManager->SetP1XAxisTitle(id_PhiIL, "#phi");
            m_run->fMasterAnalysisManager->SetP1YAxisTitle(id_PhiIL, "#lambda");
            
        }
        //
        id_PhiIL=analysisManager->GetP1Id(pathPhiIL, false);
        if(id_PhiIL<0)
        {
            //G4cout<<"Geant4: Phi int profile of "<<pathPhiIL<<" didn't exist, creating P1 now on WORKER: "<<analysisManager<<G4endl;
            const std::string name(detName+"Phi_IL");
            id_PhiIL = analysisManager->CreateP1(pathPhiIL, name.c_str(), 500, -M_PI, M_PI);
            analysisManager->SetP1XAxisTitle(id_PhiIL, "#phi");
            analysisManager->SetP1YAxisTitle(id_PhiIL, "#lambda");
            
        }
        //G4cout<<"Geant4: Assigning  id_EtaRL: "<<id_EtaRL<<", id_EtaIL: "<<id_EtaIL<<", id_PhiRL: "<<id_PhiRL<<", id_PhiIL: "<<id_PhiIL<<" to detector : "<<detName<<G4endl;
        m_etaMapRL_g4[detName] = id_EtaRL;
        m_etaMapIL_g4[detName] = id_EtaIL;
        m_phiMapRL_g4[detName] = id_PhiRL;
        m_phiMapIL_g4[detName] = id_PhiIL;
        
        //profEtaRL->Fill(m_etaPrimary, thicks.first, 1.);
        analysisManager->FillP1(id_EtaRL, m_etaPrimary, thicks.first,  1.);
        //profEtaIL->Fill(m_etaPrimary, thicks.second, 1.);
        analysisManager->FillP1(id_EtaIL, m_etaPrimary, thicks.second, 1.);
        //profPhiRL->Fill(m_phiPrimary, thicks.first, 1.);
        analysisManager->FillP1(id_PhiRL, m_phiPrimary, thicks.first,  1.);
        //profPhiIL->Fill(m_phiPrimary, thicks.second, 1.);
        analysisManager->FillP1(id_PhiIL, m_phiPrimary, thicks.second, 1.);

    }
    
} // namespace G4UA

#endif

/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#include "LengthIntegrator.hh"
//#include "SimHelpers/ServiceAccessor.h"

#include "TProfile.h"
#include "TProfile2D.h"

//#include "GaudiKernel/ISvcLocator.h"
//#include "GaudiKernel/Bootstrap.h"


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

#include "g4analysis.hh"

// System includes
#include <mutex>

// Framework includes
//#include "GaudiKernel/GaudiException.h"

// Anonymous namespace for file-global mutexes and helper functions
namespace
{
  /// Mutex used to protect access to the thread-unsafe THistSvc
  //std::mutex gHistSvcMutex;

  /// Helper function to attempt a retrieve a hist from the hist svc
//  template<class HistType>
//  bool getHist(const ServiceHandle<ITHistSvc>& hSvc,
//               const std::string& histPath, HistType*& ptr)
//  {
//    TH1* h = nullptr;
//    if(hSvc->exists(histPath)) {
//      if(hSvc->getHist(histPath, h).isSuccess()) {
//        if((ptr = dynamic_cast<HistType*>(h))) return true;
//      }
//      else throw GaudiException("Failed to getHist", "GetHistErr", StatusCode::FAILURE);
//    }
//    return false;
//  }

  /// Helper function to register a hist to the hist svc
//  template<class HistType>
//  void regHist(const ServiceHandle<ITHistSvc>& hSvc,
//               const std::string& histPath, HistType*& ptr)
//  {
//    if(hSvc->regHist(histPath, ptr).isFailure()) {
//      delete ptr; // Cleanup here, for convenience.
//      throw GaudiException("Failed to regHist", "RegHistErr", StatusCode::FAILURE);
//    }
//  }
}

namespace G4UA
{

  //---------------------------------------------------------------------------
  // Constructor
  //---------------------------------------------------------------------------
  LengthIntegrator::LengthIntegrator(const std::string& histSvcName)
    : m_g4pow(0),
      //m_hSvc(histSvcName, "LengthIntegrator"),
      m_etaPrimary(0), m_phiPrimary(0),
      m_rzProfRL(nullptr), m_rzProfIL(nullptr)
  {
    // Protect concurrent access to the non-thread-safe hist svc
    //std::lock_guard<std::mutex> lock(gHistSvcMutex);
    std::cout<<"LengthIntegrator constructor: "<<std::endl;
    
    // MOM
    //Geant4
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    // Register the RZ profiles. The other profiles need to wait until the end
    // of the events as they are created only if used.
    const char* radName = "/lengths/radLen/RZRadLen";
    if(analysisManager->GetP2Id(radName, false) < 0){
        m_rzProfRL_id = analysisManager->CreateP2(radName,"RZRadLen",1000,-25000.,25000.,2000,0.,15000.);
        std::cout<<"Geant4 : Created RZRadLen 2DProfile with name: "<<radName<< " and  with id: "<<m_rzProfRL_id<<std::endl;

    }
    const char* intName = "/lengths/intLen/RZIntLen";
    if(analysisManager->GetP2Id(intName, false)< 0)
    {
        m_rzProfIL_id = analysisManager->CreateP2(intName,"RZIntLen",1000,-25000.,25000.,2000,0.,15000.);
        std::cout<<"Geant4 :Created RZIntLen 2DProfile with name: "<<intName<< " and with id: "<<m_rzProfIL_id<<std::endl;

    }
    //~Geant4
    
    // ROOT
    // Register the RZ profiles. The other profiles need to wait until the end
    // of the events as they are created only if used.
    
    //const char* radName = "/lengths/radLen/RZRadLen";
    //if(!getHist(m_hSvc, radName, m_rzProfRL)) {
      m_rzProfRL = new TProfile2D("RZRadLen","RZRadLen",1000,-25000.,25000.,2000,0.,15000.);
      std::cout<<"ROOT : Created RZRadLen 2DProfile with name: "<<radName<< std::endl;
    //  regHist(m_hSvc, radName, m_rzProfRL);
    //}

    //const char* intName = "/lengths/intLen/RZIntLen";
    //if(!getHist(m_hSvc, intName, m_rzProfIL)) {
      m_rzProfIL = new TProfile2D("RZIntLen","RZIntLen",1000,-25000.,25000.,2000,0.,15000.);
      std::cout<<"ROOT : Created RZIntLen 2DProfile with name: "<<intName<< std::endl;
    //  regHist(m_hSvc, intName, m_rzProfIL);
    //}
    // ~ROOT

    m_g4pow = G4Pow::GetInstance();

  }
  LengthIntegrator::~LengthIntegrator()
  {
      
    std::cout<<"Finalizing ~LengthIntegrator, create root file with ROOT " << std::endl;
    std::string fileName = "RZRadLen.root";
    TFile* fOut = new TFile(fileName.c_str(), "RECREATE");
    std::string directory = "cells_"+fileName;
    TDirectory* dir = fOut->mkdir(directory.c_str());
    dir->cd();
    
    m_rzProfRL->Write();
    m_rzProfIL->Write();
    // Save histograms and close file
    for (auto hist : m_rzMapRL) {
    hist.second->Write();
    }
    for (auto hist : m_xyMapRL) {
        hist.second->Write();

    }
    for (auto hist : m_rzMapIL) {
        hist.second->Write();
        
    }
    for (auto hist : m_xyMapIL) {
          hist.second->Write();
          
    }
    for (auto hist : m_etaMapIL) {
          hist.second->Write();
          
    }
    for (auto hist : m_etaMapRL) {
          hist.second->Write();
          
    }
    for (auto hist : m_phiMapRL) {
          hist.second->Write();
          
    }
    for (auto hist : m_phiMapIL) {
          hist.second->Write();
          
    }
    fOut->Write();
    fOut->Close();
    std::cout<<"ROOT: File successfully saved and closed! " << std::endl;
    // Clear histogram maps
    
      std::cout<<"Clear histogram maps! " << std::endl;
    m_rzMapRL.clear();
    m_xyMapRL.clear();
    m_rzMapIL.clear();
    m_xyMapIL.clear();
      
    m_rzMapRL_g4.clear();
    m_xyMapRL_g4.clear();
    m_rzMapIL_g4.clear();
    m_xyMapIL_g4.clear();
      
    m_etaMapIL.clear();
    m_phiMapIL.clear();
    m_etaMapRL.clear();
    m_phiMapRL.clear();
    m_etaMapIL_g4.clear();
    m_phiMapIL_g4.clear();
    m_etaMapRL_g4.clear();
    m_phiMapRL_g4.clear();
      
    std::cout<<"Done! " << std::endl;
      
  }
        
    

  //---------------------------------------------------------------------------
  // Cache primary info at beginning of event
  //---------------------------------------------------------------------------
  void LengthIntegrator::BeginOfEventAction(const G4Event* event)
  {
    std::cout<<" ****** LengthIntegrator::BeginOfEventAction  ****** " <<std::endl;
    m_detThickMap.clear();
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
  void LengthIntegrator::EndOfEventAction(const G4Event*)
  {
    // Lazily protect this whole code from concurrent access
    //std::lock_guard<std::mutex> lock(gHistSvcMutex);
    std::cout<<" ****** "<<std::endl;
    std::cout<<" ****** LengthIntegrator::EndOfEventAction  - START ****** " <<std::endl;
    std::cout<<" ****** "<<std::endl;
    auto analysisManager = G4AnalysisManager::Instance();
    // Loop over volumes
    for (auto& it : m_detThickMap) {

      // ROOT
      // If histos already exist, then fill them
      if (m_etaMapRL.find(it.first) != m_etaMapRL.end()) {
        std::cout<<"ROOT: Histos already exist, fill them"<<std::endl;
        m_etaMapRL[it.first]->Fill(m_etaPrimary, it.second.first, 1.);
        m_phiMapRL[it.first]->Fill(m_phiPrimary, it.second.first, 1.);

        m_etaMapIL[it.first]->Fill(m_etaPrimary, it.second.second, 1.);
        m_phiMapIL[it.first]->Fill(m_phiPrimary, it.second.second, 1.);
      }
      // New detector volume; register it
      else {
          std::cout<<"ROOT: New detector volume; register it: "<<it.first<<std::endl;
        regAndFillHist(it.first, it.second);    //ROOT
      }
      // ~ROOT

      //Geant4
      // If histos already exist, then fill them
      if (m_etaMapRL_g4.find(it.first) != m_etaMapRL_g4.end()){
          std::cout<<"GEANT4: Histos already exist, fill them"<<std::endl;
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
          std::cout<<"GEANT4: New detector volume; register it: "<<it.first<<std::endl;
          regAndFillHist_g4(it.first, it.second);    //Geant4

      }
        //~Geant4


    } // Loop over detectors

    /////////////////////////////////////////////////
    // Putting this here, as normally I'd add the following code into a finalize function (as has been done above for the Athena(non-MP) code), but I'm not sure if overloading the finalize function in AthenaMP will break the histogram merging at the end of the job. If it wont, then the following code can be put in a finalize function, which will speed up AthenaMP jobs.

    // Adding zeros to TProfile bins, so that each bin contains the same number of entries,
    // so that a THStack of all the material TProfile plots (or all the element plots) will equal the Total_X0 TProfile plot
    // It's because each plot (for each material, say) is only filled if a Geantion hits it, not if it's not hit in an event

    std::cout<<"Adding zeros: ROOT totalEtaRL, this operation will not be done in Geant4! "<<std::endl;
    TProfile* totalEtaRL = m_etaMapRL["Total_X0"];
    int nbins = totalEtaRL->GetNbinsX();
    std::cout<<"ROOT: m_etaMapRL nbins: "<<nbins<<std::endl;
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
    std::cout<<" ****** "<<std::endl;
    std::cout<<" ****** LengthIntegrator::EndOfEventAction  - END ****** \n\n" <<std::endl;
    std::cout<<" ****** "<<std::endl;
  }

  //---------------------------------------------------------------------------
  // Accumulate results from one step
  //---------------------------------------------------------------------------
  void LengthIntegrator::UserSteppingAction(const G4Step* aStep)
  {
    std::cout<<"\n ****** LengthIntegrator::UserSteppingAction Accumulate results from one step  - START ****** " <<std::endl;
    G4TouchableHistory* touchHist =
      (G4TouchableHistory*) aStep->GetPreStepPoint()->GetTouchable();
    G4LogicalVolume* lv = touchHist->GetVolume()->GetLogicalVolume();
    std::string volName = lv->GetName();
    G4Material* mat = lv->GetMaterial();
    double radl = mat->GetRadlen();
    double intl = mat->GetNuclearInterLength();
    double stepl = aStep->GetStepLength();

    // FIXME: using DBL_MAX just ensures double overflow below.
    double thickstepRL = radl != 0 ? stepl/radl *100 : DBL_MAX;
    double thickstepIL = intl != 0 ? stepl/intl : DBL_MAX;

    std::string detName;
    auto colsPos = volName.find("::");
    if (colsPos != std::string::npos)
          detName = volName.substr(0, colsPos);
    else
      detName=volName;
    //detName="Generic";

    std::string matName = "M_" + lv->GetMaterial()->GetName();
    std::string detName_plus_matName = "DM_" + detName + "_" + lv->GetMaterial()->GetName();
    std::string detName_d = "D_" + detName;
    double zLimit = 3512.0; // For ITk studies: 3512mm is the z-limit of the ID End-Plate Cryostat.
    double zHit = aStep->GetPreStepPoint()->GetPosition().z();
    double rLimit = 1152.0; // For ITk studies: 1150mm is the R-limit of the ITk. Note, the solenoid coil is outside of this (at an R of ~1300mm, I think)
    double rHit = aStep->GetPreStepPoint()->GetPosition().perp();

    if(fabs(zHit) < zLimit && rHit < rLimit)
    {

      LengthIntegrator::addToDetThickMap(detName_d,            thickstepRL, thickstepIL);
      LengthIntegrator::addToDetThickMap(matName,              thickstepRL, thickstepIL);
      LengthIntegrator::addToDetThickMap(detName_plus_matName, thickstepRL, thickstepIL);
      LengthIntegrator::addToDetThickMap("Total_X0",           thickstepRL, thickstepIL);

      const G4ElementVector* eVec = mat->GetElementVector();
      for (size_t i=0 ; i < mat->GetNumberOfElements() ; ++i)
      {
          std::string elementName = "E_" + (*eVec)[i]->GetName();
          std::string matName_plus_elementName = "ME_" + lv->GetMaterial()->GetName() + "_" + (*eVec)[i]->GetName();
          std::string detName_plus_elementName = "DE_" + detName + "_" + (*eVec)[i]->GetName();
          double el_thickstepRL = stepl * (mat->GetVecNbOfAtomsPerVolume())[i] * (*eVec)[i]->GetfRadTsai() * 100.0;
          G4double lambda0 = 35*g/cm2;
          double el_thickstepIL = stepl * amu/lambda0 * (mat->GetVecNbOfAtomsPerVolume())[i] * m_g4pow->Z23( G4int( (*eVec)[i]->GetN() + 0.5 ) );
          LengthIntegrator::addToDetThickMap(elementName,              el_thickstepRL, el_thickstepIL);
          LengthIntegrator::addToDetThickMap(matName_plus_elementName, el_thickstepRL, el_thickstepIL);
          LengthIntegrator::addToDetThickMap(detName_plus_elementName, el_thickstepRL, el_thickstepIL);
      }

    }

    //G4ThreeVector midPoint = (aStep->GetPreStepPoint()->GetPosition()+aStep->GetPostStepPoint()->GetPosition())*0.5;
    //m_rzProfRL->Fill( midPoint.z() , midPoint.perp() , thickstepRL , 1. );
    //m_rzProfIL->Fill( midPoint.z() , midPoint.perp() , thickstepIL , 1. );

    G4ThreeVector hitPoint = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector endPoint = aStep->GetPostStepPoint()->GetPosition();
//// MOM
    auto analysisManager = G4AnalysisManager::Instance();

    // Protect concurrent histo filling
    // Filling m_rzProfRL_id and m_rzProfIL_id,
    {
      //static std::mutex mutex_instance;
      //std::lock_guard<std::mutex> lock(mutex_instance);
      //ROOT:
      std::cout<<"ROOT: fill m_rzProfRL with "<<hitPoint.z()<<" "<< hitPoint.perp()<<" "<< thickstepRL <<""<< 1.<<std::endl;
      m_rzProfRL->Fill( hitPoint.z() , hitPoint.perp() , thickstepRL , 1. );
      m_rzProfIL->Fill( hitPoint.z() , hitPoint.perp() , thickstepIL , 1. );
      m_rzProfRL->Fill( endPoint.z() , endPoint.perp() , thickstepRL , 1. );
      m_rzProfIL->Fill( endPoint.z() , endPoint.perp() , thickstepIL , 1. );
      //GEANT4:
      std::cout<<"GEANT4: fill m_rzProfRL_id: "<<m_rzProfRL_id<<" with "<<hitPoint.z()<<" "<< hitPoint.perp()<<" "<< thickstepRL <<""<< 1.<<std::endl;
      analysisManager->FillP2(m_rzProfRL_id, hitPoint.z() , hitPoint.perp() , thickstepRL , 1.);
      analysisManager->FillP2(m_rzProfIL_id, hitPoint.z() , hitPoint.perp() , thickstepIL , 1.);
      analysisManager->FillP2(m_rzProfRL_id, endPoint.z() , endPoint.perp() , thickstepRL , 1.);
      analysisManager->FillP2(m_rzProfIL_id, endPoint.z() , endPoint.perp() , thickstepIL , 1.);
    }
      //TO DO : commented out for now, following only m_rzProfRL_id and m_rzProfRL_id
    std::vector<std::string> L;
    L.push_back(detName_d);
    //L.push_back(matName);
    //L.push_back(detName_plus_matName);
    L.push_back("Total_X0");

    std::string specialname = "";
    if(matName.find("Support") != std::string::npos) specialname = "CarbonFiber";
    if(matName.find("Carbon") != std::string::npos) specialname = "CarbonFiber";
    if(matName.find("Steel") != std::string::npos) specialname = "Steel";
    if(matName.find("BarrelStrip") != std::string::npos) specialname = "Services";
    if(matName.find("Brl") != std::string::npos) specialname = "Services";
    if(matName.find("Svc") != std::string::npos) specialname = "Services";
    if(matName.find("InnerIST") != std::string::npos) specialname = "Services";
    if(matName.find("InnerPST") != std::string::npos) specialname = "Services";
    if(matName.find("BarrelPixel") != std::string::npos) specialname = "Services";
    if(matName.find("EndcapPixel") != std::string::npos) specialname = "Services";
    if(matName.find("InnerPixel") != std::string::npos) specialname = "Services";
    if(matName.find("OuterPixel") != std::string::npos) specialname = "Services";
    if(matName.find("pix::Chip") != std::string::npos) specialname = "PixelChips";
    if(matName.find("pix::Hybrid") != std::string::npos) specialname = "PixelChips";
    if(specialname != ""){
      L.push_back("M_"+specialname);
    }else{
      L.push_back(matName);
    }

    std::string plotstring = "";

    // 1. UPDATE m_rzMapRL and m_xyMapRL per Detector and Materials
    for (auto it : L)
    {
        static std::mutex mutex_register;
        std::lock_guard<std::mutex> lock(mutex_register);
        plotstring = it;

        G4cout<<"processing string "<<plotstring<<G4endl;
        if(!m_rzMapRL[plotstring]){
            TString rzname = "RZRadLen_"+plotstring;
            std::string rznameReg = "/lengths/radLen/RZRadLen_"+plotstring;

            TString xyname = "XYRadLen_"+plotstring;
            std::string xynameReg = "/lengths/radLen/XYRadLen_"+plotstring;

            m_rzMapRL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
            m_xyMapRL[plotstring]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");

      }

      m_rzMapRL[plotstring]->Fill( hitPoint.z() , hitPoint.perp() , thickstepRL , 1. );
      m_rzMapRL[plotstring]->Fill( endPoint.z() , endPoint.perp() , thickstepRL , 1. );
      m_xyMapRL[plotstring]->Fill( hitPoint.x() , hitPoint.y() , thickstepRL , 1. );
      m_xyMapRL[plotstring]->Fill( endPoint.x() , endPoint.y() , thickstepRL , 1. );


        // Geant4
        if(!m_rzMapRL_g4[plotstring]){
          std::string rznameReg = "/lengths/radLen/RZRadLen_"+plotstring;
          std::string xynameReg = "/lengths/radLen/XYRadLen_"+plotstring;
          G4String rzname_g4 = "RZRadLen_"+plotstring;
          G4String xyname_g4 = "XYRadLen_"+plotstring;
          m_rzMapRL_g4[plotstring]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
          m_xyMapRL_g4[plotstring]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");

        }
        analysisManager->FillP2(m_rzMapRL_g4[plotstring],  hitPoint.z() , hitPoint.perp() , thickstepRL , 1.);
        analysisManager->FillP2(m_rzMapRL_g4[plotstring],  endPoint.z() , endPoint.perp() , thickstepRL , 1.);
        analysisManager->FillP2(m_xyMapRL_g4[plotstring],  hitPoint.x() , hitPoint.y()    , thickstepRL , 1.);
        analysisManager->FillP2(m_xyMapRL_g4[plotstring],  endPoint.x() , endPoint.y()    , thickstepRL , 1.);
        // ~Geant4

    }
    // 2. UPDATE m_rzMapIL and m_xyMapIL per Detector and Materials
    for (auto it : L) {

      static std::mutex mutex_instance;
      std::lock_guard<std::mutex> lock(mutex_instance);

      plotstring = it;

      if(!m_rzMapIL[plotstring]){
          std::string rznameReg = "/lengths/intLen/RZIntLen_"+plotstring;
          TString rzname = "RZIntLen_"+plotstring;
          std::string xynameReg = "/lengths/intLen/XYIntLen_"+plotstring;
          TString xyname = "XYIntLen_"+plotstring;

          m_rzMapIL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");

          m_xyMapIL[plotstring]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");
      }

      m_rzMapIL[plotstring]->Fill( hitPoint.z() , hitPoint.perp() , thickstepIL , 1. );
      m_rzMapIL[plotstring]->Fill( endPoint.z() , endPoint.perp() , thickstepIL , 1. );
      m_xyMapIL[plotstring]->Fill( hitPoint.x() , hitPoint.y() , thickstepIL , 1. );
      m_xyMapIL[plotstring]->Fill( endPoint.x() , endPoint.y() , thickstepIL , 1. );

        if(!m_rzMapIL_g4[plotstring])
        {
          std::string rznameReg = "/lengths/intLen/RZIntLen_"+plotstring;
          std::string xynameReg = "/lengths/intLen/XYIntLen_"+plotstring;
          G4String rzname_g4 = "RZIntLen_"+plotstring;
          G4String xyname_g4 = "XYIntLen_"+plotstring;
          m_rzMapIL_g4[plotstring]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
          m_xyMapIL_g4[plotstring]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");
        }
        analysisManager->FillP2(m_rzMapIL_g4[plotstring],hitPoint.z() , hitPoint.perp() , thickstepIL , 1.);
        analysisManager->FillP2(m_rzMapIL_g4[plotstring],endPoint.z() , endPoint.perp() , thickstepIL , 1.);
        analysisManager->FillP2(m_xyMapIL_g4[plotstring],hitPoint.x() , hitPoint.y()    , thickstepIL , 1.);
        analysisManager->FillP2(m_xyMapIL_g4[plotstring],endPoint.x() , endPoint.y()    , thickstepIL , 1.);

    }

    // 3. UPDATE m_rzMapRL and m_xyMapRL per ELEMENTS
    const G4ElementVector* eVec = mat->GetElementVector();
    for (size_t i=0 ; i < mat->GetNumberOfElements() ; ++i) {

      static std::mutex mutex_instance;
      std::lock_guard<std::mutex> lock(mutex_instance);

      std::string elementName = "E_" + (*eVec)[i]->GetName();
      double el_thickstep = stepl * (mat->GetVecNbOfAtomsPerVolume())[i] * (*eVec)[i]->GetfRadTsai() * 100.0;

      if(!m_rzMapRL[elementName]){

          std::string rznameReg = "/lengths/radLen/RZRadLen_"+elementName;
          TString rzname = "RZRadLen_"+elementName;
          TString xyname = "XYRadLen_"+elementName;
          std::string xynameReg = "/lengths/radLen/XYRadLen_"+elementName;

          m_rzMapRL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
          m_xyMapRL[elementName]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");


      }

      m_rzMapRL[elementName]->Fill( hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
      m_rzMapRL[elementName]->Fill( endPoint.z() , endPoint.perp() , el_thickstep , 1. );
      m_xyMapRL[elementName]->Fill( hitPoint.x() , hitPoint.y() , el_thickstep , 1. );
      m_xyMapRL[elementName]->Fill( endPoint.x() , endPoint.y() , el_thickstep , 1. );

        if(!m_rzMapRL_g4[elementName]){
            std::string rznameReg = "/lengths/radLen/RZRadLen_"+elementName;
            std::string xynameReg = "/lengths/radLen/XYRadLen_"+elementName;
            //
            G4String rzname_g4 = "RZRadLen_"+elementName;
            G4String xyname_g4 = "XYRadLen_"+elementName;
            m_rzMapRL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
            m_xyMapRL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");

        }
        analysisManager->FillP2(m_rzMapRL_g4[elementName],hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
        analysisManager->FillP2(m_rzMapRL_g4[elementName],endPoint.z() , endPoint.perp() , el_thickstep , 1. );
        analysisManager->FillP2(m_xyMapRL_g4[elementName],hitPoint.x() , hitPoint.y()    , el_thickstep , 1. );
        analysisManager->FillP2(m_xyMapRL_g4[elementName],endPoint.x() , endPoint.y()    , el_thickstep , 1.);

    }
    // 4. UPDATE m_rzMapIL and m_xyMapIL per ELEMENTS
    for (size_t i=0 ; i < mat->GetNumberOfElements() ; ++i) {

      static std::mutex mutex_instance;
      std::lock_guard<std::mutex> lock(mutex_instance);

      std::string elementName = "E_" + (*eVec)[i]->GetName();
      G4double lambda0 = 35*g/cm2;
      //G4Pow* m_g4pow = G4Pow::GetInstance();
      double el_thickstep = stepl * amu/lambda0 * (mat->GetVecNbOfAtomsPerVolume())[i] * m_g4pow->Z23( G4int( (*eVec)[i]->GetN() + 0.5 ) );

      if(!m_rzMapIL[elementName]){

          TString rzname = "RZIntLen_"+elementName;
          std::string rznameReg = "/lengths/intLen/RZIntLen_"+elementName;
          TString xyname = "XYIntLen_"+elementName;
          std::string xynameReg = "/lengths/intLen/XYIntLen_"+elementName;

          m_rzMapIL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
          m_xyMapIL[elementName]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");



      }
      //
      m_rzMapIL[elementName]->Fill( hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
      m_rzMapIL[elementName]->Fill( endPoint.z() , endPoint.perp() , el_thickstep , 1. );
      m_xyMapIL[elementName]->Fill( hitPoint.x() , hitPoint.y() , el_thickstep , 1. );
      m_xyMapIL[elementName]->Fill( endPoint.x() , endPoint.y() , el_thickstep , 1. );

        if(!m_rzMapIL_g4[elementName]){
            std::string rznameReg = "/lengths/intLen/RZIntLen_"+elementName;
            std::string xynameReg = "/lengths/intLen/XYIntLen_"+elementName;
            G4String rzname_g4 = "RZIntLen_"+elementName;
            G4String xyname_g4 = "XYIntLen_"+elementName;
            m_rzMapIL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
            m_xyMapIL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");

        }
        analysisManager->FillP2(m_rzMapIL_g4[elementName],hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
        analysisManager->FillP2(m_rzMapIL_g4[elementName],endPoint.z() , endPoint.perp() , el_thickstep , 1. );
        analysisManager->FillP2(m_xyMapIL_g4[elementName],hitPoint.x() , hitPoint.y()    , el_thickstep , 1. );
        analysisManager->FillP2(m_xyMapIL_g4[elementName],endPoint.x() , endPoint.y()    , el_thickstep , 1.);

    }
      
    std::cout<<" ****** LengthIntegrator::UserSteppingAction  - END ****** \n" <<std::endl;

  }

  /// note that this should be called from a section protected by a mutex, since it talks to the THitSvc

  TProfile2D* LengthIntegrator::getOrCreateProfile(std::string regName, TString histoname, TString xtitle, int nbinsx, float xmin, float xmax, TString ytitle, int nbinsy,float ymin, float ymax,TString ztitle){

    //G4cout<<"histo "<<histoname<<" not found. checking for  "<<regName<<G4endl;

//    if(m_hSvc->exists(regName)){
//      TH2* histo=nullptr;
//    //TO DO  if(m_hSvc->getHist(regName,histo).isSuccess())
//    return dynamic_cast<TProfile2D*>(histo);
//    } else
      {
      //G4cout<<"...... creating  "<<regName<<G4endl;
      TProfile2D* result= new TProfile2D(histoname,histoname,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
      result->GetXaxis()->SetTitle(xtitle);
      result->GetYaxis()->SetTitle(ytitle);
      result->GetZaxis()->SetTitle(ztitle);

      //if (m_hSvc && m_hSvc->regHist(regName,result).isFailure()){
      //    std::cout<<"FATAL: Registration of histogram " << rznameReg << " failed"<<std::endl;
	//ATH_MSG_FATAL( "Registration of histogram " << rznameReg << " failed" );
	//throw GaudiException("Registration of histogram " + regName + " failed", "RegHistErr", StatusCode::FAILURE);
      //}
      return result;
    }

    // should never be here
    G4cout<<"ERROR something went wrong in handling of THistSvc "<<regName <<" "<<histoname<<G4endl;
    return nullptr;
  }
    
    //TO DO: sistemare indentation
  G4int LengthIntegrator::getOrCreateProfile_g4(G4String regName,G4String histoname, G4String xtitle, int nbinsx, float xmin, float xmax,G4String ytitle, int nbinsy,float ymin, float ymax,G4String ztitle)
    {
        
        auto analysisManager = G4AnalysisManager::Instance();
        G4int ok = analysisManager->GetP2Id(regName, false);
        std::cout<<"!!!!!!Looking for 2dProfile with name: "<<regName<<" and result id is: "<<ok<<std::endl;
        if(ok<0){
            G4int id = analysisManager->CreateP2(regName,histoname,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
            std::cout<<"!!!!!!Created 2dProfile with name: "<<regName<<" and result id is: "<<id<<std::endl;
            analysisManager->SetP2XAxisTitle(id,xtitle);
            analysisManager->SetP2YAxisTitle(id,ytitle);
            analysisManager->SetP2ZAxisTitle(id,ztitle);
            return id;
        }
        return ok;
    }

  //---------------------------------------------------------------------------
  // Add elements and values to the map
  //---------------------------------------------------------------------------
  void LengthIntegrator::addToDetThickMap(std::string name, double thickstepRL, double thickstepIL)
  {
    auto it=m_detThickMap.find(name);
    if(it!=m_detThickMap.end()){
      (*it).second.first+=thickstepRL;
      (*it).second.second+=thickstepIL;
    } else {
      m_detThickMap.insert(std::map<std::string,std::pair<double,double>,std::less<std::string> >::value_type( name, std::pair<double,double>( thickstepRL, thickstepIL) ) );
    }
  }

  //---------------------------------------------------------------------------
  // Setup hists for one detector
  //---------------------------------------------------------------------------
  //ROOT VERSION
  void LengthIntegrator::regAndFillHist(const std::string& detName,
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
      std::cout<<"ROOT: Eta rad profile of "<<pathEtaRL<<" didn't exist, creating P1 now"<<std::endl;
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
      std::cout<<"ROOT: Eta int profile of "<<pathEtaIL<<" didn't exist, creating P1 now"<<std::endl;
      profEtaIL = new TProfile(name.c_str(), name.c_str(), 1000, -6., 6.);
      profEtaIL->GetXaxis()->SetTitle("#eta");
      profEtaIL->GetYaxis()->SetTitle("#lambda");
      //regHist(m_hSvc, pathEtaIL, profEtaIL);
    }
    // Phi rad profile
    //if(!getHist(m_hSvc, pathPhiRL, profPhiRL))
      {
      std::cout<<"ROOT: Phi rad profile of "<<pathPhiRL<<" didn't exist, creating P1 now"<<std::endl;
      const std::string name(detName+"Phi_RL");
      profPhiRL = new TProfile(name.c_str(), name.c_str(), 500, -M_PI, M_PI);
      profPhiRL->GetXaxis()->SetTitle("#phi");
      profPhiRL->GetYaxis()->SetTitle("%X0");
      //regHist(m_hSvc, pathPhiRL, profPhiRL);
    }
    // Phi int profile
    //if(!getHist(m_hSvc, pathPhiIL, profPhiIL))
      {
      std::cout<<"ROOT: Phi int profile of "<<pathPhiIL<<" didn't exist, creating P1 now"<<std::endl;
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
    
    std::cout<<"ROOT: Filling the corresponding P1 profiles\n" <<std::endl;
    profEtaRL->Fill(m_etaPrimary, thicks.first, 1.);
    profEtaIL->Fill(m_etaPrimary, thicks.second, 1.);
    profPhiRL->Fill(m_phiPrimary, thicks.first, 1.);
    profPhiIL->Fill(m_phiPrimary, thicks.second, 1.);
  }
  //Geant4 VERSION
  void LengthIntegrator::regAndFillHist_g4(const std::string& detName,
                                          const std::pair<double, double>& thicks)
    {
        auto pathEtaRL = "/lengths/radLen/" + detName + "_RL";
        auto pathEtaIL = "/lengths/intLen/" + detName + "_IL";
        auto pathPhiRL = "/lengths/radLen/" + detName + "Phi_RL";
        auto pathPhiIL = "/lengths/intLen/" + detName + "Phi_IL";
        
        int id_EtaRL = -1;
        int id_EtaIL = -1;
        int id_PhiRL = -1;
        int id_PhiIL = -1;
        
        //MOM
        auto analysisManager = G4AnalysisManager::Instance();
        // Eta rad profile
        //
        if(analysisManager->GetP1Id(pathEtaRL, false)<0)
        {
            std::cout<<"Geant4: Eta rad profile of "<<pathEtaRL<<" didn't exist, creating P1 now"<<std::endl;
            const std::string name(detName+"_RL");
            id_EtaRL = analysisManager->CreateP1(pathEtaRL, name.c_str(), 1000, -6., 6.);
            analysisManager->SetP1XAxisTitle(id_EtaRL, "#eta");
            analysisManager->SetP1YAxisTitle(id_EtaRL, "%X0");
    
        }
        // Eta int profile
        //
        if(analysisManager->GetP1Id(pathEtaIL, false)<0)
        {
            std::cout<<"Geant4: Eta int profile of "<<pathEtaIL<<" didn't exist, creating P1 now"<<std::endl;
            const std::string name(detName+"_IL");
            id_EtaIL = analysisManager->CreateP1(pathEtaIL, name.c_str(), 1000, -6., 6.);
            analysisManager->SetP1XAxisTitle(id_EtaIL, "#eta");
            analysisManager->SetP1YAxisTitle(id_EtaIL, "#lambda");
            
        }
        // Phi rad profile
        //
        if(analysisManager->GetP1Id(pathPhiRL, false)<0)
        {
            std::cout<<"Geant4: Phi rad profile of "<<pathPhiRL<<" didn't exist, creating P1 now"<<std::endl;
            const std::string name(detName+"Phi_RL");
            id_PhiRL = analysisManager->CreateP1(pathPhiRL, name.c_str(), 500, -M_PI, M_PI);
            analysisManager->SetP1XAxisTitle(id_PhiRL, "#phi");
            analysisManager->SetP1YAxisTitle(id_PhiRL, "%X0");
        }
        // Phi int profile
        //
        if(analysisManager->GetP1Id(pathPhiIL, false)<0)
        {
            std::cout<<"Geant4: Phi int profile of "<<pathPhiIL<<" didn't exist, creating P1 now"<<std::endl;
            const std::string name(detName+"Phi_IL");
            id_PhiIL = analysisManager->CreateP1(pathPhiIL, name.c_str(), 500, -M_PI, M_PI);
            analysisManager->SetP1XAxisTitle(id_PhiIL, "#phi");
            analysisManager->SetP1YAxisTitle(id_PhiIL, "#lambda");
     
        }
        std::cout<<"Geant4: Assigning  id_EtaRL: "<<id_EtaRL<<", id_EtaIL: "<<id_EtaIL<<", id_PhiRL: "<<id_PhiRL<<", id_PhiIL: "<<id_PhiIL<<" to detector : "<<detName<<std::endl;
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
        std::cout<<"Geant4: Filling the corresponding P1 profiles\n" <<std::endl;
        
    }
} // namespace G4UA


/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/
#include "G4Version.hh"
#if G4VERSION_NUMBER>=1040

#include "MyLengthIntegratorSteppingAction.hh"

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

// System includes
#include <mutex>


#include "MyRunAction.hh"


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
  MyLengthIntegratorSteppingAction::MyLengthIntegratorSteppingAction(MyRunAction* run)
    : m_g4pow(0),
      m_run(run)
  
  {

      fGeantinoMapsConfig= GeantinoMapsConfigurator::getGeantinoMapsConf();
      m_g4pow = G4Pow::GetInstance();
      
//    //ROOT
//    // Protect concurrent access to the non-thread-safe hist svc
//    std::lock_guard<std::mutex> lock(gHistSvcMutex);
//
//    // Register the RZ profiles. The other profiles need to wait until the end
//    // of the events as they are created only if used.
//    //const char* radName = "/lengths/radLen/RZRadLen";
//    //if(!getHist(m_hSvc, radName, m_rzProfRL)) {
//      m_rzProfRL = new TProfile2D("RZRadLen","RZRadLen",1000,-25000.,25000.,2000,0.,15000.);
//    //  regHist(m_hSvc, radName, m_rzProfRL);
//    //}
//
//    //const char* intName = "/lengths/intLen/RZIntLen";
//    //if(!getHist(m_hSvc, intName, m_rzProfIL)) {
//      m_rzProfIL = new TProfile2D("RZIntLen","RZIntLen",1000,-25000.,25000.,2000,0.,15000.);
//    //  regHist(m_hSvc, intName, m_rzProfIL);
//    //}
//    //~ROOT
    

  }
  MyLengthIntegratorSteppingAction::~MyLengthIntegratorSteppingAction()
  {
      m_detThickMap.clear();
      
//    //ROOT
//    static std::mutex mutex_instance;
//    std::lock_guard<std::mutex> lock(mutex_instance);
//    G4cout<<"Finalizing MyLengthIntegratorSteppingAction" << G4endl;
//    // Open output root file
//    std::string fileName = "RZRadLen.root";
//    TFile* fOut = new TFile(fileName.c_str(), "RECREATE");
//    std::string directory = "cells_"+fileName;
//    TDirectory* dir = fOut->mkdir(directory.c_str());
//    dir->cd();
//
//    m_rzProfRL->Write();
//    m_rzProfIL->Write();
//
//    // Save histograms and close file
//    for (auto hist : m_rzMapRL) {
//    hist.second->Write();
//    }
//    for (auto hist : m_xyMapRL) {
//        hist.second->Write();
//
//    }
//    for (auto hist : m_rzMapIL) {
//        hist.second->Write();
//
//    }
//    for (auto hist : m_xyMapIL) {
//          hist.second->Write();
//
//    }
//
//    fOut->Write();
//    fOut->Close();
//    G4cout<<"ROOT File RZRadLen.root closed" << G4endl;
    
//    // Clear histogram maps
//    m_rzMapRL.clear();
//    m_xyMapRL.clear();
//    m_rzMapIL.clear();
//    m_xyMapIL.clear();
//    //~ROOT

  }

    //---------------------------------------------------------------------------
    // Accumulate results from one step
    //---------------------------------------------------------------------------
    void MyLengthIntegratorSteppingAction::UserSteppingAction(const G4Step* aStep)
    {
        //G4cout<<" ****** MyLengthIntegratorSteppingAction::UserSteppingAction Accumulate results from one step  ****** " <<G4endl;
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
        
        double zHit = aStep->GetPreStepPoint()->GetPosition().z();
        double rHit = aStep->GetPreStepPoint()->GetPosition().perp();
        
//        std::cout<<"zHit: "<<zHit<<" ::["<<fGeantinoMapsConfig->GetZmin()<<","<<fGeantinoMapsConfig->GetZmax()<<"]"<<std::endl;
//        std::cout<<"rHit: "<<rHit<<" ::["<<fGeantinoMapsConfig->GetRmin()<<","<<fGeantinoMapsConfig->GetRmax()<<"]"<<std::endl;
        
        if(zHit >= fGeantinoMapsConfig->GetZmin() && zHit <= fGeantinoMapsConfig->GetZmax() && rHit >= fGeantinoMapsConfig->GetRmin() && rHit <= fGeantinoMapsConfig->GetRmax()){
            MyLengthIntegratorSteppingAction::addToDetThickMap(detName_d,            thickstepRL, thickstepIL);
            MyLengthIntegratorSteppingAction::addToDetThickMap(matName,              thickstepRL, thickstepIL);
            MyLengthIntegratorSteppingAction::addToDetThickMap(detName_plus_matName, thickstepRL, thickstepIL);
            MyLengthIntegratorSteppingAction::addToDetThickMap("Total_X0",           thickstepRL, thickstepIL);
            
            const G4ElementVector* eVec = mat->GetElementVector();
            for (size_t i=0 ; i < mat->GetNumberOfElements() ; ++i)
            {
                std::string elementName = "E_" + (*eVec)[i]->GetName();
                std::string matName_plus_elementName = "ME_" + lv->GetMaterial()->GetName() + "_" + (*eVec)[i]->GetName();
                std::string detName_plus_elementName = "DE_" + detName + "_" + (*eVec)[i]->GetName();
                double el_thickstepRL = stepl * (mat->GetVecNbOfAtomsPerVolume())[i] * (*eVec)[i]->GetfRadTsai() * 100.0;
                G4double lambda0 = 35*g/cm2;
                double el_thickstepIL = stepl * amu/lambda0 * (mat->GetVecNbOfAtomsPerVolume())[i] * m_g4pow->Z23( G4int( (*eVec)[i]->GetN() + 0.5 ) );
                MyLengthIntegratorSteppingAction::addToDetThickMap(elementName,              el_thickstepRL, el_thickstepIL);
                MyLengthIntegratorSteppingAction::addToDetThickMap(matName_plus_elementName, el_thickstepRL, el_thickstepIL);
                MyLengthIntegratorSteppingAction::addToDetThickMap(detName_plus_elementName, el_thickstepRL, el_thickstepIL);
            }
            
        }
        
        //G4ThreeVector midPoint = (aStep->GetPreStepPoint()->GetPosition()+aStep->GetPostStepPoint()->GetPosition())*0.5;
        //m_rzProfRL->Fill( midPoint.z() , midPoint.perp() , thickstepRL , 1. );
        //m_rzProfIL->Fill( midPoint.z() , midPoint.perp() , thickstepIL , 1. );
        
        G4ThreeVector hitPoint = aStep->GetPreStepPoint()->GetPosition();
        G4ThreeVector endPoint = aStep->GetPostStepPoint()->GetPosition();
        
        auto analysisManager = G4AnalysisManager::Instance();
        // Protect concurrent histo filling
        {
            static std::mutex mutex_instance;
            std::lock_guard<std::mutex> lock(mutex_instance);
            //      //ROOT
            //      G4cout<<"ROOT: fill m_rzProfRL with "<<hitPoint.z()<<" "<< hitPoint.perp()<<" "<< thickstepRL <<""<< 1.<<G4endl;
            //      m_rzProfRL->Fill( hitPoint.z() , hitPoint.perp() , thickstepRL , 1. );
            //      m_rzProfIL->Fill( hitPoint.z() , hitPoint.perp() , thickstepIL , 1. );
            //      m_rzProfRL->Fill( endPoint.z() , endPoint.perp() , thickstepRL , 1. );
            //      m_rzProfIL->Fill( endPoint.z() , endPoint.perp() , thickstepIL , 1. );
            //      //~ROOT
            
            //GEANT4
            //G4cout<<"GEANT4: fill m_run->fRadName_id: "<<m_run->fRadName_id<<" with "<<hitPoint.z()<<" "<< hitPoint.perp()<<" "<< thickstepRL <<""<< 1.<<G4endl;
            
            //G4cout<<"GEANT4: fill m_run->fIntName_id: "<<m_run->fIntName_id<<" with "<<hitPoint.z()<<" "<< hitPoint.perp()<<" "<< thickstepIL <<""<< 1.<<G4endl;
            
            analysisManager->FillP2(m_run->fRadName_id, hitPoint.z() , hitPoint.perp() , thickstepRL , 1.);
            analysisManager->FillP2(m_run->fIntName_id, hitPoint.z() , hitPoint.perp() , thickstepIL , 1.);
            analysisManager->FillP2(m_run->fRadName_id, endPoint.z() , endPoint.perp() , thickstepRL , 1.);
            analysisManager->FillP2(m_run->fIntName_id, endPoint.z() , endPoint.perp() , thickstepIL , 1.);
            //~GEANT4
        }
        
        
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
      if(fGeantinoMapsConfig->GetCreateDetectorsMaps()|| fGeantinoMapsConfig->GetCreateMaterialsMaps()){
          // 1. UPDATE m_rzMapRL and m_xyMapRL per Detector and Materials
          for (auto it : L) {
              
              static std::mutex mutex_register;
              std::lock_guard<std::mutex> lock(mutex_register);
              
              plotstring = it;
              
              //G4cout<<"processing string "<<plotstring<<G4endl;
              
              //      if(!m_rzMapRL[plotstring]){
              //          G4cout<<"ROOT: m_rzMapRL 2DProfile for "<<plotstring<<" doesn't exist, I create it"<<G4endl;
              //          TString rzname = "RZRadLen_"+plotstring;
              //          std::string rznameReg = "RZRadLen_"+plotstring;
              //          TString xyname = "XYRadLen_"+plotstring;
              //          std::string xynameReg = "XYRadLen_"+plotstring;
              //          m_rzMapRL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
              //          m_xyMapRL[plotstring]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");
              //
              //      }
              //      else
              //          G4cout<<"ROOT: m_rzMapRL 2DProfile for "<<plotstring<<" EXIST!"<<G4endl;
              //        G4cout<<"ROOT: Filling m_rzMapRL histogram for plotstring: "<<plotstring<<"!"<<G4endl;
              //
              //      m_rzMapRL[plotstring]->Fill( hitPoint.z() , hitPoint.perp() , thickstepRL , 1. );
              //      m_rzMapRL[plotstring]->Fill( endPoint.z() , endPoint.perp() , thickstepRL , 1. );
              //      m_xyMapRL[plotstring]->Fill( hitPoint.x() , hitPoint.y() , thickstepRL , 1. );
              //      m_xyMapRL[plotstring]->Fill( endPoint.x() , endPoint.y() , thickstepRL , 1. );
              //
              // Geant4
              if(!m_rzMapRL_g4[plotstring]){
                  //G4cout<<"Geant4: m_rzMapRL_g4 2DProfile for "<<plotstring<<" doesn't exist, I create it"<<G4endl;
                  std::string rznameReg = "RZRadLen_"+plotstring;
                  std::string xynameReg = "XYRadLen_"+plotstring;
                  G4String rzname_g4 = "RZRadLen_"+plotstring;
                  G4String xyname_g4 = "XYRadLen_"+plotstring;
                  //          m_rzMapRL_g4[plotstring]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
                  //          m_xyMapRL_g4[plotstring]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");
                  m_rzMapRL_g4[plotstring]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,fGeantinoMapsConfig->GetZmin(),fGeantinoMapsConfig->GetZmax(), "R [mm]",1000,fGeantinoMapsConfig->GetRmin(),fGeantinoMapsConfig->GetRmax(),"%X0");
                  m_xyMapRL_g4[plotstring]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,fGeantinoMapsConfig->GetXmin(),fGeantinoMapsConfig->GetXmax(),"Y [mm]",1000,fGeantinoMapsConfig->GetYmin(),fGeantinoMapsConfig->GetYmax(),"%X0");
                  
              }
              //else G4cout<<"Geant4: m_rzMapRL_g4 2DProfile for "<<plotstring<<" EXIST!"<<G4endl;
              //  G4cout<<"Geant4: Filling m_rzMapRL_g4 histogram for plotstring: "<<plotstring<<"!"<<G4endl;
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
              
              //      if(!m_rzMapIL[plotstring]){
              //          G4cout<<"ROOT: m_rzMapIL 2DProfile for "<<plotstring<<" doesn't exist, I create it"<<G4endl;
              //          std::string rznameReg = "RZIntLen_"+plotstring;
              //          TString rzname = "RZIntLen_"+plotstring;
              //          std::string xynameReg = "XYIntLen_"+plotstring;
              //          TString xyname = "XYIntLen_"+plotstring;
              //          m_rzMapIL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
              //          m_xyMapIL[plotstring]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y[mm]",1000,-1200.,1200.,"#lambda");
              //
              //      }else G4cout<<"ROOT: m_rzMapIL 2DProfile for "<<plotstring<<" EXIST!"<<G4endl;
              //        G4cout<<"ROOT: Filling m_rzMapIL histogram for plotstring: "<<plotstring<<"!"<<G4endl;
              //
              //      m_rzMapIL[plotstring]->Fill( hitPoint.z() , hitPoint.perp() , thickstepIL , 1. );
              //      m_rzMapIL[plotstring]->Fill( endPoint.z() , endPoint.perp() , thickstepIL , 1. );
              //      m_xyMapIL[plotstring]->Fill( hitPoint.x() , hitPoint.y() , thickstepIL , 1. );
              //      m_xyMapIL[plotstring]->Fill( endPoint.x() , endPoint.y() , thickstepIL , 1. );
              
              if(!m_rzMapIL_g4[plotstring])
              {
                  //G4cout<<"Geant4: m_rzMapIL_g4 2DProfile for "<<plotstring<<" doesn't exist, I create it"<<G4endl;
                  std::string rznameReg = "RZIntLen_"+plotstring;
                  std::string xynameReg = "XYIntLen_"+plotstring;
                  G4String rzname_g4 = "RZIntLen_"+plotstring;
                  G4String xyname_g4 = "XYIntLen_"+plotstring;
                  //m_rzMapIL_g4[plotstring]= getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
                  //m_xyMapIL_g4[plotstring]= getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");
                  m_rzMapIL_g4[plotstring]= getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,fGeantinoMapsConfig->GetZmin(),fGeantinoMapsConfig->GetZmax(),"R [mm]",1000,fGeantinoMapsConfig->GetRmin(),fGeantinoMapsConfig->GetRmax(),"#lambda");
                  m_xyMapIL_g4[plotstring]= getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,fGeantinoMapsConfig->GetXmin(),fGeantinoMapsConfig->GetXmax(),"Y [mm]",1000,fGeantinoMapsConfig->GetYmin(),fGeantinoMapsConfig->GetYmax(),"#lambda");
              }
              //else G4cout<<"Geant4: m_rzMapIL_g4 2DProfile for "<<plotstring<<" EXIST!"<<G4endl;
              //   G4cout<<"Geant4: Filling m_rzMapIL_g4 histogram for plotstring: "<<plotstring<<"!"<<G4endl;
              analysisManager->FillP2(m_rzMapIL_g4[plotstring],hitPoint.z() , hitPoint.perp() , thickstepIL , 1.);
              analysisManager->FillP2(m_rzMapIL_g4[plotstring],endPoint.z() , endPoint.perp() , thickstepIL , 1.);
              analysisManager->FillP2(m_xyMapIL_g4[plotstring],hitPoint.x() , hitPoint.y()    , thickstepIL , 1.);
              analysisManager->FillP2(m_xyMapIL_g4[plotstring],endPoint.x() , endPoint.y()    , thickstepIL , 1.);
              
          }
          
      }
      if(fGeantinoMapsConfig->GetCreateElementsMaps()){
          // 3. UPDATE m_rzMapRL and m_xyMapRL per ELEMENTS
          const G4ElementVector* eVec = mat->GetElementVector();
          for (size_t i=0 ; i < mat->GetNumberOfElements() ; ++i) {
              
              static std::mutex mutex_instance;
              std::lock_guard<std::mutex> lock(mutex_instance);
              
              std::string elementName = "E_" + (*eVec)[i]->GetName();
              double el_thickstep = stepl * (mat->GetVecNbOfAtomsPerVolume())[i] * (*eVec)[i]->GetfRadTsai() * 100.0;
              
              //      if(!m_rzMapRL[elementName]){
              //          G4cout<<"ROOT: m_rzMapRL 2DProfile for "<<elementName<<" doesn't exist, I create it"<<G4endl;
              //          std::string rznameReg = "RZRadLen_"+elementName;
              //          TString rzname = "RZRadLen_"+elementName;
              //          TString xyname = "XYRadLen_"+elementName;
              //          std::string xynameReg = "XYRadLen_"+elementName;
              //
              //          m_rzMapRL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
              //          m_xyMapRL[elementName]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");
              //
              //      }
              //      else G4cout<<"ROOT: m_rzMapRL 2DProfile for "<<elementName<<" EXIST!"<<G4endl;
              //        G4cout<<"ROOT: Filling m_rzMapRL histogram for elementName: "<<elementName<<"]!"<<G4endl;
              //
              //      m_rzMapRL[elementName]->Fill( hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
              //      m_rzMapRL[elementName]->Fill( endPoint.z() , endPoint.perp() , el_thickstep , 1. );
              //      m_xyMapRL[elementName]->Fill( hitPoint.x() , hitPoint.y() , el_thickstep , 1. );
              //      m_xyMapRL[elementName]->Fill( endPoint.x() , endPoint.y() , el_thickstep , 1. );
              
              if(!m_rzMapRL_g4[elementName]){
                  //G4cout<<"Geant4: m_rzMapRL_g4 2DProfile for "<<elementName<<" doesn't exist, I create it"<<G4endl;
                  std::string rznameReg = "RZRadLen_"+elementName;
                  std::string xynameReg = "XYRadLen_"+elementName;
                  //
                  G4String rzname_g4 = "RZRadLen_"+elementName;
                  G4String xyname_g4 = "XYRadLen_"+elementName;
                  //         m_rzMapRL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"%X0");
                  //         m_xyMapRL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"%X0");
                  m_rzMapRL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,fGeantinoMapsConfig->GetZmin(),fGeantinoMapsConfig->GetZmax(),"R [mm]",1000,fGeantinoMapsConfig->GetRmin(),fGeantinoMapsConfig->GetRmax(),"%X0");
                  m_xyMapRL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,fGeantinoMapsConfig->GetXmin(),fGeantinoMapsConfig->GetXmax(),"Y [mm]",1000,fGeantinoMapsConfig->GetYmin(),fGeantinoMapsConfig->GetYmax(),"%X0");
              }
              //else G4cout<<"Geant4: m_rzMapRL_g4 2DProfile for "<<elementName<<" EXIST!"<<G4endl;
              //   G4cout<<"Geant4: Filling m_rzMapRL_g4 histogram for elementName: "<<elementName<<"!"<<G4endl;
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
              
              //        if(!m_rzMapIL[elementName]){
              //            G4cout<<"ROOT: m_rzMapIL 2DProfile for "<<elementName<<" doesn't exist, I create it"<<G4endl;
              //            TString rzname = "RZIntLen_"+elementName;
              //            std::string rznameReg = "RZIntLen_"+elementName;
              //            TString xyname = "XYIntLen_"+elementName;
              //            std::string xynameReg = "XYIntLen_"+elementName;
              //            m_rzMapIL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
              //            m_xyMapIL[elementName]=getOrCreateProfile(xynameReg, xyname, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");
              //
              //      }
              //        else G4cout<<"ROOT: m_rzMapIL 2DProfile for "<<elementName<<" EXIST!"<<G4endl;
              //        G4cout<<"ROOT: Filling m_rzMapIL histogram for elementName: "<<elementName<<"!"<<G4endl;
              //
              //      m_rzMapIL[elementName]->Fill( hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
              //      m_rzMapIL[elementName]->Fill( endPoint.z() , endPoint.perp() , el_thickstep , 1. );
              //      m_xyMapIL[elementName]->Fill( hitPoint.x() , hitPoint.y() , el_thickstep , 1. );
              //      m_xyMapIL[elementName]->Fill( endPoint.x() , endPoint.y() , el_thickstep , 1. );
              
              if(!m_rzMapIL_g4[elementName]){
                  //G4cout<<"Geant4: m_rzMapIL_g4 2DProfile for "<<elementName<<" doesn't exist, I create it"<<G4endl;
                  std::string rznameReg = "RZIntLen_"+elementName;
                  std::string xynameReg = "XYIntLen_"+elementName;
                  G4String rzname_g4 = "RZIntLen_"+elementName;
                  G4String xyname_g4 = "XYIntLen_"+elementName;
                  //         m_rzMapIL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,-3512.,3512.,"R [mm]",1000,0.,1200.,"#lambda");
                  //         m_xyMapIL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,-1200.,1200.,"Y [mm]",1000,-1200.,1200.,"#lambda");
                  m_rzMapIL_g4[elementName]=getOrCreateProfile_g4(rznameReg, rzname_g4, "Z [mm]", 1000,fGeantinoMapsConfig->GetZmin(),fGeantinoMapsConfig->GetZmax(),"R [mm]",1000,fGeantinoMapsConfig->GetRmin(),fGeantinoMapsConfig->GetRmax(),"#lambda");
                  m_xyMapIL_g4[elementName]=getOrCreateProfile_g4(xynameReg, xyname_g4, "X [mm]", 1000,fGeantinoMapsConfig->GetXmin(),fGeantinoMapsConfig->GetXmax(),"Y [mm]",1000,fGeantinoMapsConfig->GetYmin(),fGeantinoMapsConfig->GetYmax(),"#lambda");
                  
              }
              //else G4cout<<"Geant4: m_rzMapIL_g4 2DProfile for "<<elementName<<" EXIST!"<<G4endl;
              //   G4cout<<"Geant4: Filling m_rzMapIL_g4 histogram for elementName: "<<elementName<<"!"<<G4endl;
              analysisManager->FillP2(m_rzMapIL_g4[elementName],hitPoint.z() , hitPoint.perp() , el_thickstep , 1. );
              analysisManager->FillP2(m_rzMapIL_g4[elementName],endPoint.z() , endPoint.perp() , el_thickstep , 1. );
              analysisManager->FillP2(m_xyMapIL_g4[elementName],hitPoint.x() , hitPoint.y()    , el_thickstep , 1. );
              analysisManager->FillP2(m_xyMapIL_g4[elementName],endPoint.x() , endPoint.y()    , el_thickstep , 1.);
              
          }
      }

  }

  /// note that this should be called from a section protected by a mutex, since it talks to the THitSvc
//  //ROOT version
//  TProfile2D* MyLengthIntegratorSteppingAction::getOrCreateProfile(std::string regName, TString histoname, TString xtitle, int nbinsx, float xmin, float xmax, TString ytitle, int nbinsy,float ymin, float ymax,TString ztitle){
//
//    //G4cout<<"histo "<<histoname<<" not found. checking for  "<<regName<<G4endl;
//
////    if(m_hSvc->exists(regName)){
////      TH2* histo=nullptr;
////    //TO DO  if(m_hSvc->getHist(regName,histo).isSuccess())
////    return dynamic_cast<TProfile2D*>(histo);
////    } else
//      {
//      G4cout<<"...... creating  "<<regName<<G4endl;
//      TProfile2D* result= new TProfile2D(histoname,histoname,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
//      result->GetXaxis()->SetTitle(xtitle);
//      result->GetYaxis()->SetTitle(ytitle);
//      result->GetZaxis()->SetTitle(ztitle);
//
//      //if (m_hSvc && m_hSvc->regHist(regName,result).isFailure()){
//      //    G4cout<<"FATAL: Registration of histogram " << rznameReg << " failed"<<G4endl;
//    //ATH_MSG_FATAL( "Registration of histogram " << rznameReg << " failed" );
//    //throw GaudiException("Registration of histogram " + regName + " failed", "RegHistErr", StatusCode::FAILURE);
//      //}
//      return result;
//    }
//
//    // should never be here
//    G4cout<<"ERROR something went wrong in handling of THistSvc "<<regName <<" "<<histoname<<G4endl;
//    return nullptr;
//  }
    

G4int MyLengthIntegratorSteppingAction::getOrCreateProfile_g4(G4String regName,G4String histoname, G4String xtitle, int nbinsx, float xmin, float xmax,G4String ytitle, int nbinsy,float ymin, float ymax,G4String ztitle)
    {


        auto analysisManager = G4AnalysisManager::Instance();
        G4int id = analysisManager->GetP2Id(regName, false);
        //G4cout<<"***** Looking for 2dProfile with name: "<<regName<<" and result id on analysisManager: "<<analysisManager<<"  is: "<<id<<G4endl;
        //G4cout<<"***** fMasterAnalysisManager: "<<m_run->fMasterAnalysisManager<<G4endl;
        
        if(m_run->fMasterAnalysisManager->GetP2Id(regName, false)<0){
            id = m_run->fMasterAnalysisManager->CreateP2(regName,histoname,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
            //G4cout<<"!!!!!! MASTER Created 2dProfile with name: "<<regName<<" and result id is: "<<id<<G4endl;
            m_run->fMasterAnalysisManager->SetP2XAxisTitle(id,xtitle);
            m_run->fMasterAnalysisManager->SetP2YAxisTitle(id,ytitle);
            m_run->fMasterAnalysisManager->SetP2ZAxisTitle(id,ztitle);
            
        }
        if(analysisManager->GetP2Id(regName, false)<0){
            id = analysisManager->CreateP2(regName,histoname,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
            //G4cout<<"!!!!!! WORKER Created 2dProfile with name: "<<regName<<" and result id is: "<<id<<G4endl;
            analysisManager->SetP2XAxisTitle(id,xtitle);
            analysisManager->SetP2YAxisTitle(id,ytitle);
            analysisManager->SetP2ZAxisTitle(id,ztitle);

        }
        
        return id;

    }

  //---------------------------------------------------------------------------
  // Add elements and values to the map
  //---------------------------------------------------------------------------
  void MyLengthIntegratorSteppingAction::addToDetThickMap(std::string name, double thickstepRL, double thickstepIL)
  {
    auto it=m_detThickMap.find(name);
    if(it!=m_detThickMap.end()){
      (*it).second.first+=thickstepRL;
      (*it).second.second+=thickstepIL;
    } else {
      m_detThickMap.insert(std::map<std::string,std::pair<double,double>,std::less<std::string> >::value_type( name, std::pair<double,double>( thickstepRL, thickstepIL) ) );
    }
  }

  

} // namespace G4UA

#endif

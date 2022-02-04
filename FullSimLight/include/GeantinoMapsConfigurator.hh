
#ifndef GeantinoMapsConfigurator_h
#define GeantinoMapsConfigurator_h 1

#include "G4Types.hh"
#include "G4String.hh"

#include <vector>

/**
 * @file    GeantinoMapsConfigurator.hh
 * @author  Marilena Bandieramonte
 * @date    02.02.2022
 *
 */



class GeantinoMapsConfigurator {

public:

  static GeantinoMapsConfigurator* getGeantinoMapsConf();

  //GeantinoMapsConfigurator(const GeantinoMapsConfigurator&) = delete;
  //GeantinoMapsConfigurator& operator=(const GeantinoMapsConfigurator&) = delete;

  void SetRmin(G4double r){fRmin = r;}
  void SetRmax(G4double r){fRmax = r;}
  void SetZmin(G4double z){fZmin = z;}
  void SetZmax(G4double z){fZmax = z;}
  void SetXmin(G4double x){fXmin = x;}
  void SetXmax(G4double x){fXmax = x;}
  void SetYmin(G4double y){fYmin = y;}
  void SetYmax(G4double y){fYmax = y;}
  void SetEtamin(G4double eta){fEtamin = eta;}
  void SetEtamax(G4double eta){fEtamax = eta;}
  void SetCreateEtaPhiMaps(G4bool flag)   {fCreateEtaPhiMaps    =flag;}
  void SetCreateDetectorsMaps(G4bool flag){fCreateDetectorsMaps =flag;}
  void SetCreateMaterialsMaps(G4bool flag){fCreateMaterialsMaps =flag;}
  void SetCreateElementsMaps(G4bool flag) {fCreateElementsMaps  =flag;}
  void SetCreateGeantinoMaps(G4bool flag) {fCreateGeantinoMaps  =flag;}
  void SetMapsFilename(G4String filename) {fMapsFilename        =filename;}
    
  G4double GetRmin()const {return fRmin;}
  G4double GetRmax()const{return fRmax;}
  G4double GetZmin()const{return fZmin;}
  G4double GetZmax()const{return fZmax;}
  G4double GetXmin()const{return fXmin;}
  G4double GetXmax()const{return fXmax;}
  G4double GetYmin()const{return fYmin;}
  G4double GetYmax()const{return fYmax;}
  G4double GetEtamin()const{return fEtamin;}
  G4double GetEtamax()const{return fEtamax;}
  G4bool GetCreateEtaPhiMaps()  const {return fCreateEtaPhiMaps;}
  G4bool GetCreateDetectorsMaps()const{return fCreateDetectorsMaps;}
  G4bool GetCreateMaterialsMaps()const{return fCreateMaterialsMaps;}
  G4bool GetCreateElementsMaps() const{return fCreateElementsMaps;}
  G4bool GetCreateGeantinoMaps()const {return fCreateGeantinoMaps;}
  G4String GetMapsFilename()const {return fMapsFilename;}
    
private:

    static GeantinoMapsConfigurator* s_instance;
    GeantinoMapsConfigurator();
    ~GeantinoMapsConfigurator();
    
    G4String  fMapsFilename;
    G4double  fRmin;
    G4double  fRmax;
    G4double  fZmin;
    G4double  fZmax;
    G4double  fXmin;
    G4double  fXmax;
    G4double  fYmin;
    G4double  fYmax;
    G4double  fEtamin;
    G4double  fEtamax;
    G4bool    fCreateEtaPhiMaps;
    G4bool    fCreateDetectorsMaps;
    G4bool    fCreateMaterialsMaps;
    G4bool    fCreateElementsMaps;
    G4bool    fCreateGeantinoMaps=false;

}; // GeantinoMapsConfigurator

#endif // GeantinoMapsConfigurator_h 1

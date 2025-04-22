
#include "GeantinoMapsMessenger.hh"

#include "GeantinoMapsConfigurator.hh"
#include "G4UIdirectory.hh"
//#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
//#include "G4UIcmdWith3Vector.hh"


GeantinoMapsMessenger::GeantinoMapsMessenger(GeantinoMapsConfigurator *gmaps_config) : G4UImessenger(),
fGeantinoMapsConfig(gmaps_config),
fGMapsDirectory(nullptr),
fRminCmd(nullptr),
fRmaxCmd(nullptr),
fZminCmd(nullptr),
fZmaxCmd(nullptr),
fXminCmd(nullptr),
fXmaxCmd(nullptr),
fYminCmd(nullptr),
fYmaxCmd(nullptr),
fEtaminCmd(nullptr),
fEtamaxCmd(nullptr)

{
    
    fGMapsDirectory     = new G4UIdirectory("/gmaps/");
    fGMapsDirectory->SetGuidance("Geantino Maps Configuration");
    
    fRminCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/rmin",this);
    fRminCmd->SetGuidance("set geantino maps rmin");
    fRminCmd->SetParameterName("rMin",false);
    fRminCmd->SetUnitCategory("Length");
    fRminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fRmaxCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/rmax",this);
    fRmaxCmd->SetGuidance("set geantino maps rmax");
    fRmaxCmd->SetParameterName("rMax",false);
    fRmaxCmd->SetUnitCategory("Length");
    fRmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fZminCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/zmin",this);
    fZminCmd->SetGuidance("set geantino maps zmin");
    fZminCmd->SetParameterName("zMin",false);
    fZminCmd->SetUnitCategory("Length");
    fZminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fZmaxCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/zmax",this);
    fZmaxCmd->SetGuidance("set geantino maps zmax");
    fZmaxCmd->SetParameterName("zMax",false);
    fZmaxCmd->SetUnitCategory("Length");
    fZmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fXminCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/xmin",this);
    fXminCmd->SetGuidance("set geantino maps xmin");
    fXminCmd->SetParameterName("xMin",false);
    fXminCmd->SetUnitCategory("Length");
    fXminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fXmaxCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/xmax",this);
    fXmaxCmd->SetGuidance("set geantino maps xmax");
    fXmaxCmd->SetParameterName("xMax",false);
    fXmaxCmd->SetUnitCategory("Length");
    fXmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fYminCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/ymin",this);
    fYminCmd->SetGuidance("set geantino maps ymin");
    fYminCmd->SetParameterName("yMin",false);
    fYminCmd->SetUnitCategory("Length");
    fYminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fYmaxCmd    = new G4UIcmdWithADoubleAndUnit("/gmaps/ymax",this);
    fYmaxCmd->SetGuidance("set geantino maps ymax");
    fYmaxCmd->SetParameterName("yMax",false);
    fYmaxCmd->SetUnitCategory("Length");
    fYmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fEtaminCmd    = new G4UIcmdWithADouble("/gmaps/etamin",this);
    fEtaminCmd->SetGuidance("set geantino maps etamin");
    fEtaminCmd->SetParameterName("etaMin",false);
    fEtaminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fEtamaxCmd    = new G4UIcmdWithADouble("/gmaps/etamax",this);
    fEtamaxCmd->SetGuidance("set geantino maps etamax");
    fEtamaxCmd->SetParameterName("etaMax",false);
    fEtamaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    
    fVolumesListCmd = new G4UIcmdWithAString("/gmaps/volumeslist",this);
    fVolumesListCmd->SetGuidance("set list of interested volumes");
    fVolumesListCmd->SetParameterName("volumesList",false);
    fVolumesListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
}


GeantinoMapsMessenger::~GeantinoMapsMessenger() {
    delete fRminCmd;
    delete fRmaxCmd;
    delete fZminCmd;
    delete fZmaxCmd;
    delete fXminCmd;
    delete fXmaxCmd;
    delete fYminCmd;
    delete fYmaxCmd;
    delete fEtaminCmd;
    delete fEtamaxCmd;
    delete fVolumesListCmd;
}

void GeantinoMapsMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    
    if (command==fRminCmd) {
        fGeantinoMapsConfig->SetRmin(fRminCmd->GetNewDoubleValue(newValue));
        
    }
    if (command==fRmaxCmd) {
        fGeantinoMapsConfig->SetRmax(fRmaxCmd->GetNewDoubleValue(newValue));
    }
    if (command==fZminCmd) {
        fGeantinoMapsConfig->SetZmin(fZminCmd->GetNewDoubleValue(newValue));
        
    }
    if (command==fZmaxCmd) {
        fGeantinoMapsConfig->SetZmax(fRmaxCmd->GetNewDoubleValue(newValue));
    }
    if (command==fXminCmd) {
        fGeantinoMapsConfig->SetXmin(fXminCmd->GetNewDoubleValue(newValue));
        
    }
    if (command==fXmaxCmd) {
        fGeantinoMapsConfig->SetXmax(fXmaxCmd->GetNewDoubleValue(newValue));
    }
    if (command==fYminCmd) {
        fGeantinoMapsConfig->SetYmin(fYminCmd->GetNewDoubleValue(newValue));
        
    }
    if (command==fYmaxCmd) {
        fGeantinoMapsConfig->SetYmax(fRmaxCmd->GetNewDoubleValue(newValue));
    }
    if (command==fEtaminCmd) {
        fGeantinoMapsConfig->SetEtamin(fEtaminCmd->GetNewDoubleValue(newValue));
        
    }
    if (command==fEtamaxCmd) {
        fGeantinoMapsConfig->SetEtamax(fEtamaxCmd->GetNewDoubleValue(newValue));
    }
    if (command==fVolumesListCmd) {
        fGeantinoMapsConfig->SetVolumesList(newValue);
    }
    
    
    
}

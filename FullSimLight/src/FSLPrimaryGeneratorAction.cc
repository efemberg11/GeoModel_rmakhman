
#include "FSLPrimaryGeneratorAction.hh"
#include "FSLPrimaryGeneratorMessenger.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
// just for the final printout
#include "FSLDetectorConstruction.hh"

const G4int      FSLPrimaryGeneratorAction::gInfNumPrimaryPerEvt =  1;
const G4int      FSLPrimaryGeneratorAction::gSupNumPrimaryPerEvt = 10;

const G4double   FSLPrimaryGeneratorAction::gInfBeamEnergy       =   1.*GeV;
const G4double   FSLPrimaryGeneratorAction::gSupBeamEnergy       = 100.*GeV;

// these static variables stores the gun configuration just for the Print()
G4int            FSLPrimaryGeneratorAction::gNumPrimaryPerEvt(-1);
G4double         FSLPrimaryGeneratorAction::gPrimaryEnergy(-1.);
std::string      FSLPrimaryGeneratorAction::gPrimaryType("");
G4ThreeVector    FSLPrimaryGeneratorAction::gPrimaryDir(0.,0.,0.);
G4ThreeVector    FSLPrimaryGeneratorAction::gPrimaryPos(0.,0.,0.);


// These are the particle types that can be used as primary beam particle, on a event-by-event based.
const G4int FSLPrimaryGeneratorAction::gNumberCandidateParticles = 3; //only e-/e+ and gammas can be chosen randomly
const G4int FSLPrimaryGeneratorAction::gTotalNumberParticles = 5;
const G4String FSLPrimaryGeneratorAction::gNameParticlesVector[FSLPrimaryGeneratorAction::gTotalNumberParticles] = {
  "e-",
  "e+",
  "gamma",
  "geantino",
  "chargedgeantino",
};
const std::map<G4String,G4int> FSLPrimaryGeneratorAction::gPrimaryNameToIndexMap = {
  {"e-",0},
  {"e+",1},
  {"gamma",2},
  {"geantino",3},
  {"chargedgeantino", 4}
};


FSLPrimaryGeneratorAction::FSLPrimaryGeneratorAction() {

  fGeantinoMapsConfig = GeantinoMapsConfigurator::getGeantinoMapsConf();
  fIsUserNumPrimaryPerEvt     = false;
  fIsUserPrimaryType          = false;
  fIsUserPrimaryDir           = false;
  fIsUserPrimaryPos           = false;
  fIsUserPrimaryEnergy        = false;
  fNumPrimaryPerEvt           = 1;
  fPrimaryParticleName        = "e-";
  if (fGeantinoMapsConfig->GetCreateGeantinoMaps())
         fGeantinoParticleGun = new G4GeneralParticleSource;
     else
         fParticleGun         = new G4ParticleGun(1);
  fParticleTable              = G4ParticleTable::GetParticleTable();
  fPrimaryParticleEnergy      = 10.*GeV;
  fPrimaryParticlePosition    = G4ThreeVector(0.0,0.0,0.0);
  //
  fGunMessenger  = new FSLPrimaryGeneratorMessenger(this);
  
}


FSLPrimaryGeneratorAction::~FSLPrimaryGeneratorAction() {
    if (fGeantinoMapsConfig->GetCreateGeantinoMaps())
        delete fGeantinoParticleGun;
    else
        delete fParticleGun;
  delete fGunMessenger;
}


void FSLPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    
    if (fGeantinoMapsConfig->GetCreateGeantinoMaps()){
        fGeantinoParticleGun->GeneratePrimaryVertex(anEvent);
    }
    else {
        if (!fIsUserNumPrimaryPerEvt) {
            G4int idum        = static_cast<G4int>(G4UniformRand()*(gSupNumPrimaryPerEvt-gInfNumPrimaryPerEvt+1));
            fNumPrimaryPerEvt = std::max(gInfNumPrimaryPerEvt + idum,1);
        }
        for (G4int i=0; i<fNumPrimaryPerEvt; ++i) {
            // Select randomly the primary particle if it was not set by the user
            if (!fIsUserPrimaryType) {
                G4int caseBeamParticle = static_cast<G4int>(G4UniformRand()*FSLPrimaryGeneratorAction::gNumberCandidateParticles);
                fPrimaryParticleName = gNameParticlesVector[caseBeamParticle];
            }
            // Select randomly the beam energy if it was not set by the user.
            if (!fIsUserPrimaryEnergy) {
                fPrimaryParticleEnergy = gInfBeamEnergy+G4UniformRand()*(gSupBeamEnergy-gInfBeamEnergy);
            }
            // Select random direction if it was not set by the user
            if (!fIsUserPrimaryDir) {
                fPrimaryParticleDirection = G4RandomDirection();
            }
            
            //
            // Set the particle gun
            G4ParticleDefinition* pDef = fParticleTable->FindParticle(fPrimaryParticleName);
            if (!pDef) {
                G4cerr<< " *** ERROR in FSLPrimaryGeneratorAction::GeneratePrimaries() " << G4endl
                << "       UNKNOWN PRIMARY PARTICLE WITH NAME = "
                << fPrimaryParticleName << G4endl
                << G4endl;
                exit(-1);
            }
            //std::cout<<"Setting the particle gun"<<std::endl;
            
            
            fParticleGun->SetParticleDefinition       (pDef                     );
            fParticleGun->SetParticleEnergy           (fPrimaryParticleEnergy   );
            fParticleGun->SetParticlePosition         (fPrimaryParticlePosition );
            fParticleGun->SetParticleMomentumDirection(fPrimaryParticleDirection);
            //
            fParticleGun->GeneratePrimaryVertex(anEvent);
            
        }
        
    }
}

void  FSLPrimaryGeneratorAction::SetNumPrimaryPerEvt(G4int pperevt) {
  fNumPrimaryPerEvt         = pperevt;
  gNumPrimaryPerEvt         = fNumPrimaryPerEvt;
  fIsUserNumPrimaryPerEvt   = true;
}

void  FSLPrimaryGeneratorAction::SetPrimaryEnergy(G4double ekin) {
  fPrimaryParticleEnergy    = ekin;
  gPrimaryEnergy            = fPrimaryParticleEnergy;
  fIsUserPrimaryEnergy      = true;
}

void  FSLPrimaryGeneratorAction::SetPrimaryName(const G4String& pname) {
  fPrimaryParticleName      = pname;
  gPrimaryType              = fPrimaryParticleName;
  fIsUserPrimaryType        = true;
  std::cout<<"Primary name: "<<pname<<std::endl;
}

void  FSLPrimaryGeneratorAction::SetPrimaryDirection(const G4ThreeVector &pdir) {
  fPrimaryParticleDirection = pdir;
  fPrimaryParticleDirection.setMag(1.);
  gPrimaryDir = fPrimaryParticleDirection;
  fIsUserPrimaryDir         = true;
}

void  FSLPrimaryGeneratorAction::SetPrimaryPosition(const G4ThreeVector &ppos) {
  fPrimaryParticlePosition = ppos;
  gPrimaryPos = fPrimaryParticlePosition;
  fIsUserPrimaryPos         = true;
}

G4int FSLPrimaryGeneratorAction::GetPrimaryTypeIndex(const G4String& pname) {
  G4int indx = gPrimaryNameToIndexMap.find(pname)->second;
  return indx;
}

// will give proper results only at the end of the run
void  FSLPrimaryGeneratorAction::Print() {
  G4String str = "";
  G4double magFValue = FSLDetectorConstruction::GetFieldValue();
  if (magFValue>0.) {
    str += "  Magnetic field           : constant " + std::to_string(magFValue/tesla) + " [ Tesla] field \n";
  } else {
    str += "  Magnetic field           : non constant \n";
  }
  str   += "  Primaries were generated : \n";
  if (gNumPrimaryPerEvt<0) {
    str += "     Primaries per event   : random for ecah event on ["
           + std::to_string(gInfNumPrimaryPerEvt) + ", " + std::to_string(gSupNumPrimaryPerEvt) + "]\n";
  } else {
    str += "     Primaries per event   : " + std::to_string(gNumPrimaryPerEvt) + "\n";
  }
  if (gPrimaryEnergy<0.) {
    str += "     Primary energy        : random for each primary on ["
           + std::to_string(gInfBeamEnergy/GeV) + " GeV, " +std::to_string(gSupBeamEnergy/GeV) + " GeV] \n";
  } else {
    str += "     Primary energy        : " + std::to_string(gPrimaryEnergy/GeV) + " [GeV] \n";
  }
  if (gPrimaryDir.mag()==0.) {
    str += "     Primary direction     : isotropic for each primary \n";
  } else {
    G4String sdir= "[";
    sdir += std::to_string(gPrimaryDir.x()) + ", "
          + std::to_string(gPrimaryDir.y()) + ", "
          + std::to_string(gPrimaryDir.x()) + "]\n";
    str += "     Primary direction     : " + sdir;
  }
  if (gPrimaryPos.mag()==0.) {
    str += "     Primary position     : [0,0,0] \n";
  } else {
    G4String spos= "[";
    spos += std::to_string(gPrimaryPos.x()) + ", "
          + std::to_string(gPrimaryPos.y()) + ", "
          + std::to_string(gPrimaryPos.x()) + "]\n";
    str += "     Primary position     : " + spos;
  }
  if (gPrimaryType=="") {
    str += "     Primary type          : randomly selected for each primary from \n";
    for (G4int i=0; i<gNumberCandidateParticles; i++) {
      str += "       type index: " + std::to_string(i) + ",  name: " + gNameParticlesVector[i] + "\n";
    }
  } else {
    str += "     Primary type          : " + gPrimaryType + "\n";
  }
  G4cout << " \n  ======= Info On Run Conditions ======================================================== \n"
         << str
         << "  --------------------------------------------------------------------------------------- "
         << G4endl;
}

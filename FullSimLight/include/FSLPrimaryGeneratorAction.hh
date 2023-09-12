
#ifndef FSLPrimaryGeneratorAction_h
#define FSLPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "GeantinoMapsConfigurator.hh"
#include "G4GeneralParticleSource.hh"
#include <map>

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class G4ParticleTable;
class FSLPrimaryGeneratorMessenger;

class FSLPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
  FSLPrimaryGeneratorAction();
 ~FSLPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* anEvent);

  void SetNumPrimaryPerEvt(G4int pperevt);
  void SetPrimaryEnergy(G4double ekin);
  void SetPrimaryName(const G4String& pname);
  void SetPrimaryDirection(const G4ThreeVector &pdir);
  void SetPrimaryPosition(const G4ThreeVector &ppos);

  static void   Print();
  static G4int  GetNumberOfPrimaryTypes()  {  return gNumberCandidateParticles; }
  static G4int  GetPrimaryTypeIndex(const G4String& pname);
  static const G4String&  GetPrimaryName(const G4int primtypeindx) { return gNameParticlesVector[primtypeindx]; }

private:
  static const G4int       gNumberCandidateParticles;
  static const G4int       gTotalNumberParticles;
  static const G4String    gNameParticlesVector[];
  static const std::map<G4String,G4int> gPrimaryNameToIndexMap;

  static const G4int       gInfNumPrimaryPerEvt;
  static const G4int       gSupNumPrimaryPerEvt;

  static const G4double    gInfBeamEnergy;
  static const G4double    gSupBeamEnergy;

  // these static variables stores the gun configuration just for the Print()
  static G4int             gNumPrimaryPerEvt;
  static G4double          gPrimaryEnergy;
  static std::string       gPrimaryType;
  static G4ThreeVector     gPrimaryDir;
  static G4ThreeVector     gPrimaryPos;


  G4bool                   fIsUserNumPrimaryPerEvt;
  G4bool                   fIsUserPrimaryType;
  G4bool                   fIsUserPrimaryDir;
  G4bool                   fIsUserPrimaryPos;
  G4bool                   fIsUserPrimaryEnergy;

  G4int                    fNumPrimaryPerEvt;

  G4String                 fPrimaryParticleName;

  G4ParticleGun*           fParticleGun;
  G4GeneralParticleSource* fGeantinoParticleGun;
  G4ParticleTable*         fParticleTable;

  G4double                 fPrimaryParticleEnergy;

  G4ThreeVector            fPrimaryParticleDirection;
  G4ThreeVector            fPrimaryParticlePosition;

  FSLPrimaryGeneratorMessenger *fGunMessenger;
  GeantinoMapsConfigurator* fGeantinoMapsConfig;
};

#endif

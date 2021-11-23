#if USE_PYTHIA

#include "PythiaPrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryParticle.hh>

#include <err.h>
#include <unistd.h>

static const char *config = nullptr;

static const char *pythia_defaults[] = {
  "Print:quiet = on",
  "Beams:idA = 2212",
  "Beams:idB = 2212",
  "Beams:eCM = 13000.0",
  "Init:showProcesses = off",
  "Init:showMultipartonInteractions = off",
  "Init:showChangedParticleData = off",
  "Stat:showProcessLevel = off",
  "Stat:showErrors = off",
};

static const char *pythia_minbias[] = {
  "HardQCD:all = on",
  "Beams:eCM = 13000.0",
  "PhaseSpace:pTHatMin = 20.0",
};

static const char *pythia_higgs[] = {
  "HiggsSM:all = on",
  "Beams:eCM = 13000.0",
  "PhaseSpace:pTHatMin = 20.0",
};

static const char *pythia_ttbar[] = {
  "Top:gg2ttbar = on",
  "Top:qqbar2ttbar = on",
  "Beams:eCM = 13000.0",
  "PhaseSpace:pTHatMin = 20.0",
};

bool use_pythia() {
	return config != nullptr;
}

void set_pythia_config(const char *cfg) {
	config = cfg;
}

const char* get_pythia_config() { return config; }

PythiaPrimaryGeneratorAction::PythiaPrimaryGeneratorAction(int seeding) : fSeeding(seeding), fPythia("../share/Pythia8/xmldoc", false) {
  if (access(config, R_OK) == 0) {
    fPythia.readFile(config);
  } else {
    /*
    flag name="Pythia:setSeed" default="off"
    Indicates whether a user-set seed should be used every time the Pythia::init
    routine is called. If off, the random number generator is initialized with its
    default seed at the beginning of the run, and never again. If on, each new
    Pythia::init call (should several be made in the same run) results in the
    random number being re-initialized, thereby possibly starting over with the
    same sequence, if you do not watch out.

    mode name="Pythia:seed" default="-1" max="900000000"
    The seed to be used, if setSeed is on.
    A negative value gives the default seed,
    a value 0 gives a random seed based on the time, and
    a value between 1 and 900,000,000 a unique different random number sequence.
    */
    fPythia.readString("Random:setSeed = on");
    // use a reproducible seed: always the same results for benchmarks.
    fPythia.readString("Random:seed = 1234");
    //
    for (const auto str : pythia_defaults)  fPythia.readString(str);
    if (strcmp("minbias", config) == 0) {
      for (const auto str : pythia_minbias) fPythia.readString(str);
    } else if (strcmp("higgs", config) == 0) {
      for (const auto str : pythia_higgs)   fPythia.readString(str);
    } else if (strcmp("ttbar", config) == 0) {
      for (const auto str : pythia_ttbar)   fPythia.readString(str);
    } else {
      errx(1, "unknown Pythia configuration: %s", config);
    }
  }
  fPythia.init();
}

void PythiaPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
  static const G4double time = 0.0;
  static const G4ThreeVector position(0.0, 0.0, 0.0);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);
  //
  // re-seed pythia with the event ID + 1 in order to ensure that the
  // events are the same for a given event ID independently the from other
  // run conditions
  if (fSeeding == 0) {
    fPythia.rndm.init(event->GetEventID()+1);
  }
  fPythia.next();

  for (auto i = 1, n = fPythia.event.size(); i < n; ++i) {
    const auto& particle = fPythia.event[i];

    if (!particle.isFinal())
            continue;

    G4PrimaryParticle* p = new G4PrimaryParticle(particle.id());
    p->SetMass(particle.m() * GeV);
    p->SetMomentum(particle.px() * GeV, particle.py() * GeV, particle.pz() * GeV);
    vertex->SetPrimary(p);
  }

  event->AddPrimaryVertex(vertex);
}

#endif

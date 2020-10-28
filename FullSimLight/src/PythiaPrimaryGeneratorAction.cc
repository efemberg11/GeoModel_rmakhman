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

bool use_pythia()
{
	return config != nullptr;
}

void set_pythia_config(const char *cfg)
{
	config = cfg;
}

PythiaPrimaryGeneratorAction::PythiaPrimaryGeneratorAction() {
        if (access(config, R_OK) == 0) {
                pythia.readFile(config);
        } else {
                for (const auto str : pythia_defaults)
                        pythia.readString(str);

                if (strcmp("minbias", config) == 0) {
                                for (const auto str : pythia_minbias)
                                        pythia.readString(str);
                } else if (strcmp("higgs", config) == 0) {
                                for (const auto str : pythia_higgs)
                                        pythia.readString(str);
                } else if (strcmp("ttbar", config) == 0) {
                                for (const auto str : pythia_ttbar)
                                        pythia.readString(str);
                } else {
                        errx(1, "unknown Pythia configuration: %s", config);
                }
        }

        pythia.init();
}

void PythiaPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
        static const G4double time = 0.0;
        static const G4ThreeVector position(0.0, 0.0, 0.0);

        G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);

        pythia.next();

        for (auto i = 1, n = pythia.event.size(); i < n; ++i) {
                const auto& particle = pythia.event[i];

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

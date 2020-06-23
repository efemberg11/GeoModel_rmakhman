
//--------------------------------------------------------
// fillHistogramExample application: 7 May 2020
//--------------------------------------------------------

#include <getopt.h>
#include <err.h>
#include <iostream>
#include <iomanip>

#include "Histo.hh"
#include "G4Timer.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


int main() {
    
    G4cout << "========================================================" << G4endl;
    G4cout << "======          fillHistogramExample            ========" << G4endl;
    G4cout << "========================================================" << G4endl;
    
    //choose the Random engine: set to MixMax explicitely (default form 10.4)
    G4Random::setTheEngine(new CLHEP::MixMaxRng);
    // set seed and print info
    G4Random::setTheSeed(12345678);
    G4cout << G4endl
    << " ===================================================== "      << G4endl
    << "   Random engine      = " << G4Random::getTheEngine()->name() << G4endl
    << "   Initial seed       = " << G4Random::getTheSeed()           << G4endl
    << " ===================================================== "      << G4endl
    << G4endl;

    
    
    G4String mname("G4_Random");              // material
    G4String hname = "hist_" + mname;
    //G4double energy         = 10.0*MeV;   // primary energy of the e-
    //G4double gammaCutEnergy = 2.0*keV;    // gamma production cut in energy
    G4int    stat           = 100000000;   // number of samples
    G4int verbose           = 1;
    G4int nbins = 100;
    //G4int nbins1= 1200;
    //G4int nbins2= 400;
    G4double xmin = 1;
    G4double xmax = 100;
    
    // ------- Histograms name
    Histo    histo;
    histo.Add1D("1","Random numbers btw 1 - 100",nbins,xmin,xmax);
    histo.Add1D("2","Log10 of Random numbers btw 1 - 100",nbins,std::log10(xmin),std::log10(xmax));
    

    histo.SetFileName(hname);
    histo.Book();
    
    CLHEP::HepRandomEngine*     rndmEngineMod;
    rndmEngineMod = G4Random::getTheEngine();
    
    for (G4int iter=0; iter<stat; ++iter) {
        int e = rndmEngineMod->flat()* 100 + 1;
        histo.Fill(0,e,1.0);
        histo.Fill(1,log10(e),1.0);
    }
    if(verbose > 0) { G4cout << "###### Saving histograms in files " << hname <<".ascii and .root"<< G4endl; }
    histo.ScaleH1(0, 0.25/stat);
    histo.Save();
    
    G4cout << "###### End of example #####" << G4endl;
    return 0;
}

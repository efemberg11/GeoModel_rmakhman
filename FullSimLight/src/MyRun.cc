
#include "MyRun.hh"

#include "MyEventData.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>


MyRun::MyRun() : G4Run() {
  fRunData.Clear();
}


MyRun::~MyRun() {}

void MyRun::FillPerEvent(const MyEventData& data) {
  fRunData.fEdep        += data.fEdep;         fRunData.fEdep2        += data.fEdep*data.fEdep;
  fRunData.fTrackLCh    += data.fTrackLCh;     fRunData.fTrackLCh2    += data.fTrackLCh*data.fTrackLCh;
  fRunData.fTrackLNe    += data.fTrackLNe;     fRunData.fTrackLNe2    += data.fTrackLNe*data.fTrackLNe;
  fRunData.fChargedStep += data.fChargedStep;  fRunData.fChargedStep2 += data.fChargedStep*data.fChargedStep;
  fRunData.fNeutralStep += data.fNeutralStep;  fRunData.fNeutralStep2 += data.fNeutralStep*data.fNeutralStep;
  fRunData.fNGamma      += data.fNGamma;       fRunData.fNGamma2      += data.fNGamma*data.fNGamma;
  fRunData.fNElec       += data.fNElec;        fRunData.fNElec2       += data.fNElec*data.fNElec;
  fRunData.fNPosit      += data.fNPosit;       fRunData.fNPosit2      += data.fNPosit*data.fNPosit;
}


void MyRun::Merge(const G4Run* run) {
  const MyRun* localRun = static_cast<const MyRun*>(run);
  if (localRun) {
    fRunData += localRun->GetRunData();
  }
  G4Run::Merge(run);
}


void MyRun::EndOfRun() {
  const G4int  numEvents  = GetNumberOfEvent();
  //
  std::ios::fmtflags mode = G4cout.flags();
  G4int  prec = G4cout.precision(2);
  G4cout<< " \n ==================================   Run summary   ===================================== \n" << G4endl;
  G4cout<< std::setprecision(4);
  G4cout<< "    Number of events  = " << numEvents                                                           << G4endl;
  G4cout<< " \n ---------------------------------------------------------------------------------------- \n" << G4endl;
  // compute and print run statistics per event
  G4double  norm = numEvents;
  if (norm>0.) {
    norm = 1./norm;
  } else {
    return;
  }
  //compute and print statistic
  //
  const G4double meanEdep   = fRunData.fEdep*norm;
  const G4double rmsEdep    = std::sqrt(std::abs(fRunData.fEdep2*norm-meanEdep*meanEdep));
  const G4double meanLCh    = fRunData.fTrackLCh*norm;
  const G4double rmsLCh     = std::sqrt(std::abs(fRunData.fTrackLCh2*norm-meanLCh*meanLCh));
  const G4double meanLNe    = fRunData.fTrackLNe*norm;
  const G4double rmsLNe     = std::sqrt(std::abs(fRunData.fTrackLNe2*norm-meanLNe*meanLNe));
  const G4double meanStpCh  = fRunData.fChargedStep*norm;
  const G4double rmsStpCh   = std::sqrt(std::abs(fRunData.fChargedStep2*norm-meanStpCh*meanStpCh));
  const G4double meanStpNe  = fRunData.fNeutralStep*norm;
  const G4double rmsStpNe   = std::sqrt(std::abs(fRunData.fNeutralStep2*norm-meanStpNe*meanStpNe));
  const G4double meanNGam   = fRunData.fNGamma*norm;
  const G4double rmsNGam    = std::sqrt(std::abs(fRunData.fNGamma2*norm-meanNGam*meanNGam));
  const G4double meanNElec  = fRunData.fNElec*norm;
  const G4double rmsNElec   = std::sqrt(std::abs(fRunData.fNElec2*norm-meanNElec*meanNElec));
  const G4double meanNPos   = fRunData.fNPosit*norm;
  const G4double rmsNPos    = std::sqrt(std::abs(fRunData.fNPosit2*norm-meanNPos*meanNPos));

  G4cout<< "  Mean energy deposit per event = " << meanEdep/GeV <<  " +- " << rmsEdep/GeV << " [GeV]"    <<G4endl;
  G4cout<< G4endl;
  G4cout<< "  Mean track length (charged) per event = " << meanLCh/cm << " +- " << rmsLCh/cm <<  " [cm]" <<G4endl;
  G4cout<< "  Mean track length (neutral) per event = " << meanLNe/cm << " +- " << rmsLNe/cm <<  " [cm]" <<G4endl;
  G4cout<< G4endl;
  G4cout<< "  Number of steps (charged) per event = " << meanStpCh << " +- " << rmsStpCh << G4endl;
  G4cout<< "  Number of steps (neutral) per event = " << meanStpNe << " +- " << rmsStpNe << G4endl;
  G4cout<< G4endl;
  G4cout<< "  Number of secondaries per event : " << G4endl
        << "     Gammas    =  " << meanNGam      <<  " +- " << rmsNGam  << G4endl
        << "     Electrons =  " << meanNElec     <<  " +- " << rmsNElec << G4endl
        << "     Positrons =  " << meanNPos      <<  " +- " << rmsNPos  << G4endl;
  G4cout<< " ......................................................................................... \n" << G4endl;

  G4cout<< " \n ======================================================================================== \n" << G4endl;

  G4cout.setf(mode,std::ios::floatfield);
  G4cout.precision(prec);
}

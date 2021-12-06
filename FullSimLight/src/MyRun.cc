
#include "MyRun.hh"

#include "MyEventData.hh"
#include "G4SystemOfUnits.hh"
#include "G4Region.hh"

#include <iomanip>


MyRun::MyRun() : G4Run(), fScoringRegion(nullptr) {
  fRunData.Clear();
  fRunDataSpecialRegion.Clear();
}


MyRun::~MyRun() {}

void MyRun::FillPerEvent(const MyEventData& data, G4bool isspecial) {
  if (!isspecial) {
    AddData(data, fRunData);
  } else {
    AddData(data, fRunDataSpecialRegion);
  }
}


void MyRun::AddData(const MyEventData& data, MyRunData& runData) {
  runData.fEdep        += data.fEdep;         runData.fEdep2        += data.fEdep*data.fEdep;
  runData.fTrackLCh    += data.fTrackLCh;     runData.fTrackLCh2    += data.fTrackLCh*data.fTrackLCh;
  runData.fTrackLNe    += data.fTrackLNe;     runData.fTrackLNe2    += data.fTrackLNe*data.fTrackLNe;
  runData.fChargedStep += data.fChargedStep;  runData.fChargedStep2 += data.fChargedStep*data.fChargedStep;
  runData.fNeutralStep += data.fNeutralStep;  runData.fNeutralStep2 += data.fNeutralStep*data.fNeutralStep;
  runData.fNGamma      += data.fNGamma;       runData.fNGamma2      += data.fNGamma*data.fNGamma;
  runData.fNElec       += data.fNElec;        runData.fNElec2       += data.fNElec*data.fNElec;
  runData.fNPosit      += data.fNPosit;       runData.fNPosit2      += data.fNPosit*data.fNPosit;
}

void MyRun::Merge(const G4Run* run) {
  const MyRun* localRun = static_cast<const MyRun*>(run);
  if (localRun) {
    fRunData += localRun->GetRunData(false);
    fRunDataSpecialRegion += localRun->GetRunData(true);
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
  PrintEndOfRunStat(fRunData, norm);
  if (fScoringRegion != nullptr) {
    G4cout<< "\n ------------- In the Special Detector Region: " << fScoringRegion->GetName() << " \n" << G4endl;
    PrintEndOfRunStat(fRunDataSpecialRegion, norm);
  }
  G4cout<< " ......................................................................................... \n" << G4endl;

  G4cout<< " \n ======================================================================================== \n" << G4endl;

  G4cout.setf(mode,std::ios::floatfield);
  G4cout.precision(prec);
}

void MyRun::PrintEndOfRunStat(MyRunData& runData, G4double norm) {
  //compute and print statistic
  const G4double meanEdep   = runData.fEdep*norm;
  const G4double rmsEdep    = std::sqrt(std::abs(runData.fEdep2*norm-meanEdep*meanEdep));
  const G4double meanLCh    = runData.fTrackLCh*norm;
  const G4double rmsLCh     = std::sqrt(std::abs(runData.fTrackLCh2*norm-meanLCh*meanLCh));
  const G4double meanLNe    = runData.fTrackLNe*norm;
  const G4double rmsLNe     = std::sqrt(std::abs(runData.fTrackLNe2*norm-meanLNe*meanLNe));
  const G4double meanStpCh  = runData.fChargedStep*norm;
  const G4double rmsStpCh   = std::sqrt(std::abs(runData.fChargedStep2*norm-meanStpCh*meanStpCh));
  const G4double meanStpNe  = runData.fNeutralStep*norm;
  const G4double rmsStpNe   = std::sqrt(std::abs(runData.fNeutralStep2*norm-meanStpNe*meanStpNe));
  const G4double meanNGam   = runData.fNGamma*norm;
  const G4double rmsNGam    = std::sqrt(std::abs(runData.fNGamma2*norm-meanNGam*meanNGam));
  const G4double meanNElec  = runData.fNElec*norm;
  const G4double rmsNElec   = std::sqrt(std::abs(runData.fNElec2*norm-meanNElec*meanNElec));
  const G4double meanNPos   = runData.fNPosit*norm;
  const G4double rmsNPos    = std::sqrt(std::abs(runData.fNPosit2*norm-meanNPos*meanNPos));

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
}


#ifndef MyRun_h
#define MyRun_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "MyRunData.hh"

class G4Track;
class G4Region;
class MyEventData;

class MyRun : public G4Run {

public:

   MyRun();
  ~MyRun() override;

  void Merge(const G4Run*) override;

  void SetSpecialScoringRegion(G4Region* reg) { fScoringRegion = reg; }

  void FillPerEvent(const MyEventData&, G4bool);
  void EndOfRun();

  const MyRunData&  GetRunData(G4bool isspecial) const { return isspecial ? fRunDataSpecialRegion : fRunData; }


private:

  void AddData(const MyEventData& data, MyRunData& runData);
  void PrintEndOfRunStat(MyRunData& runData, G4double norm);

private:

  G4Region*  fScoringRegion;

  MyRunData  fRunData;
  MyRunData  fRunDataSpecialRegion;

};

#endif

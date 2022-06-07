
#ifndef FSLRun_h
#define FSLRun_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "FSLRunData.hh"

class G4Track;
class G4Region;
class FSLEventData;

class FSLRun : public G4Run {

public:

   FSLRun();
  ~FSLRun() override;

  void Merge(const G4Run*) override;

  void SetSpecialScoringRegion(G4Region* reg) { fScoringRegion = reg; }

  void FillPerEvent(const FSLEventData&, G4bool);
  void EndOfRun();

  const FSLRunData&  GetRunData(G4bool isspecial) const { return isspecial ? fRunDataSpecialRegion : fRunData; }


private:

  void AddData(const FSLEventData& data, FSLRunData& runData);
  void PrintEndOfRunStat(FSLRunData& runData, G4double norm);

private:

  G4Region*  fScoringRegion;

  FSLRunData  fRunData;
  FSLRunData  fRunDataSpecialRegion;

};

#endif

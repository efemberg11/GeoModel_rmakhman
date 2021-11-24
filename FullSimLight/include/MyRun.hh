
#ifndef MyRun_h
#define MyRun_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "MyRunData.hh"

class G4Track;
class MyEventData;

class MyRun : public G4Run {

public:

   MyRun();
  ~MyRun() override;

  void Merge(const G4Run*) override;

  void FillPerEvent(const MyEventData&);
  void EndOfRun();

  const MyRunData&  GetRunData() const { return fRunData; }


private:

  MyRunData  fRunData;

};

#endif

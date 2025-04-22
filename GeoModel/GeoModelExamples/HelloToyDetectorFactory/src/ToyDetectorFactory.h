// Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

#ifndef ToyDetectorFactory_h
#define ToyDetectorFactory_h 1

// local includes
#include "ToyDetectorManager.h"

// GeoModel includes
#include "GeoModelKernel/GeoVDetectorFactory.h"



class ToyDetectorFactory : public GeoVDetectorFactory  {

 public:

  // Constructor:
  ToyDetectorFactory();

  // Destructor:
  ~ToyDetectorFactory();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world);

  // Access to the results:
  virtual const ToyDetectorManager * getDetectorManager() const;

 private:

  // Illegal operations:
  const ToyDetectorFactory & operator=(const ToyDetectorFactory &right);
  ToyDetectorFactory(const ToyDetectorFactory &right);

  // The manager:
  ToyDetectorManager       *m_detectorManager;

  // StoreGateSvc             *m_detectorStore;
};

// Class ToyDetectorFactory
#endif

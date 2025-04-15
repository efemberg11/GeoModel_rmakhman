/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/RCBase.h"
#include <gtest/gtest.h>
#include <limits>

class Stub:public RCBase{
  public:
  Stub() = default;
  virtual ~Stub() = default;
};

TEST(GeoModelKernel, RCBaseStub_CanBeConstructedOnStack) {
  EXPECT_NO_THROW([[maybe_unused]] Stub stub);
  //Stub stub1; (unused here if the following are commented out)
  //none of the following compile (correctly so)
  //Stub stub2(stub1);
  //Stub stub3 = stub1;
  //Stub stub4(std::move(stub1);
  //Stub stub5 = std::move(stub3);
}

TEST(GeoModelKernel, RCBase_CanBeConstructedOnHeap) {
  RCBase * p{};
  EXPECT_NO_THROW(p = new RCBase);
  EXPECT_NE(p, nullptr);
  //deliberate leak here, RCBase d'tor is protected, so cannot delete
}

TEST(GeoModelKernel, RCBaseStub_CanBeConstructedOnHeap) {
  Stub * p{};
  EXPECT_NO_THROW(p = new Stub);
  EXPECT_NE(p, nullptr);
  //can delete the derived class directly
  EXPECT_NO_THROW(delete p);
}

TEST(GeoModelKernel, RCBaseStub_ReferenceCountOk){
  //do on heap, as is supposed to be
  Stub * pStub = new Stub;
  EXPECT_EQ(pStub->refCount(), 0);
  EXPECT_NO_THROW(pStub->ref());
  EXPECT_EQ(pStub->refCount(), 1);
  EXPECT_NO_THROW(pStub->unref());
  //now, where is the object? The pointer has not been set to zero.. so...
  //the following line would execute (not crash) and return a nonsense value, 
  //EXPECT_EQ(pStub->refCount(), 0); //returns 2043 on my machine
  //segfault : delete pStub;
  Stub * pStub2 = new Stub;
  EXPECT_NO_THROW(pStub2->unref());
  EXPECT_EQ(pStub2->refCount(), std::numeric_limits<unsigned int>::max());
  //now it's cursed, so euthanise it
  delete pStub2;
}
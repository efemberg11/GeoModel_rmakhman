/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoMaterial.h"
#include <gtest/gtest.h>

using namespace GeoModelKernelUnits;


TEST(GeoMaterial, CanBeConstructedOnHeap) {
  GeoMaterial * pG{};
  const std::string name{"Shaun"};
  const double dense{13.0*gram/centimeter3};
  EXPECT_NO_THROW(pG = new GeoMaterial(name, dense));//Shaun is dense
  pG->ref();
  pG->unref(); //should be destroyed here
}



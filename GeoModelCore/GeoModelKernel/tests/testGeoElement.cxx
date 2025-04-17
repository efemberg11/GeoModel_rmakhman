/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoElement.h"
#include <gtest/gtest.h>

class SampleElement{
  private:
    const std::string m_name{"Rhodium"};
    const std::string m_symbol{"Rh"};
    const double m_Z{45.};
    const double m_A{102.90549*GeoModelKernelUnits::gram/GeoModelKernelUnits::mole};
    GeoElement * m_ptr{};
  public:
    GeoElement * ptr() const {return m_ptr;}
    SampleElement():m_ptr(new GeoElement(m_name, m_symbol, m_Z, m_A)){
      m_ptr->ref();
    }
    ~SampleElement(){m_ptr->unref();}
};


TEST(GeoElement, CanBeConstructedOnHeap) {
  GeoElement * pG{};
  const std::string name{"Rhodium"};
  const std::string symbol{"Rh"};
  const double Z{45.};
  const double A{102.90549};
  EXPECT_NO_THROW(pG = new GeoElement(name, symbol, Z, A));
  pG->ref();
  pG->unref(); //should be destroyed here
}

TEST(GeoElement,CalculatesNCorrectly){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getN(), 102.90549);
}

TEST(GeoElement,GivesCorrectName){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getName(),"Rhodium");
}

TEST(GeoElement,GivesCorrectSymbol){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getSymbol(),"Rh");
}

TEST(GeoElement,GivesCorrectZValue){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getZ(),45);
}
TEST(GeoElement,GivesCorrectAverageAtomicMass){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getA(),102.90549*GeoModelKernelUnits::gram/GeoModelKernelUnits::mole);
}

TEST(GeoElement,GivesCorrectRadiationLength){
  SampleElement rhodium;
  const auto p = rhodium.ptr();
  EXPECT_EQ(p->getRadTsai(),1.8442455962801794e-21);
}

TEST(GeoElement,EqualityOperator){
  const std::string name{"Rhodium"};
  const std::string symbol{"Rh"};
  const double Z{45.};
  const double A{102.90549*GeoModelKernelUnits::gram/GeoModelKernelUnits::mole};
  auto pG = new GeoElement(name, symbol, Z, A);
  SampleElement rhodium;
  ASSERT_TRUE(*pG == *(rhodium.ptr()));
}

TEST(GeoElement,InequalityOperator){
  const std::string name{"Rhodium"};
  const std::string symbol{"Rh"};
  const double Z{45.};
  //alter A value _slightly_
  const double A{102.91*GeoModelKernelUnits::gram/GeoModelKernelUnits::mole};
  auto pG = new GeoElement(name, symbol, Z, A);
  SampleElement rhodium;
  ASSERT_TRUE(*pG != *(rhodium.ptr()));
  pG->ref();
  pG->unref(); //should be destroyed here
  
}


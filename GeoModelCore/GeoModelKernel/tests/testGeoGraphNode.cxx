/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoNodeAction.h"
#include <gtest/gtest.h>
#include <limits>
#include <memory>

class GeoNodeActionStub:public GeoNodeAction{
  private:
    bool m_called{};
  public:
    virtual void handleNode (const GeoGraphNode *) override{
      m_called = true;
    }
    bool called() const { return m_called; }
};


TEST(GeoGraphNode, CanBeConstructedOnHeap) {
  GeoGraphNode * pG{};
  EXPECT_NO_THROW(pG = new GeoGraphNode);
  pG->ref();
  pG->unref(); //should be destroyed here
}

TEST(GeoGraphNode, ExecExecutesHandleNodeAction){
  GeoGraphNode * pG = new GeoGraphNode;
  pG->ref();
  GeoNodeActionStub * myAction = new GeoNodeActionStub;
  EXPECT_EQ(myAction->called(), false);
  pG->exec(myAction);
  EXPECT_EQ(myAction->called(), true);
  pG->unref();
  delete myAction;
}

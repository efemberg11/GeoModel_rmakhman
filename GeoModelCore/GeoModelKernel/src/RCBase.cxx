/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/RCBase.h"
#include <exception>

RCBase::RCBase()
  : m_count(0)
{
}

RCBase::~RCBase()
{
}

void RCBase::ref () const
{
  m_count++;
}

void RCBase::unref () const
{
  if (--m_count == 0) delete this;
}

unsigned int RCBase::refCount () const
{
  return m_count.load();
}

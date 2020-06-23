/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_MyVolActionFilter_H
#define GEOMODELKERNEL_MyVolActionFilter_H

/**
 * @class MyVolActionFilter
 * 
 * @brief An example that does nothing, for people to copy and fill in
 */

#include "GeoModelKernel/GeoVolumeAction.h"

#include <string>
#include <regex>


class MyVolActionFilter : public GeoVolumeAction
{
 public:
  MyVolActionFilter(std::string re);
  ~MyVolActionFilter();

  // Callback method.
  void handleVPhysVol (const GeoVPhysVol *);

 private:
  std::regex m_re;
  std::string m_reStr;
};

#endif

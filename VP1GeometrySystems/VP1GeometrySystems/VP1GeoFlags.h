/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Header file for class VP1GeoFlags                         //
//                                                            //
//  Description: Flags for geometry system.                   //
//                                                            //
//  Author: Thomas H. Kittelmann (Thomas.Kittelmann@cern.ch)  //
//  Initial version: October 2008                             //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef VP1GEOFLAGS_H
#define VP1GEOFLAGS_H

#include <QFlags>
#include <QString>
#include <vector>
#include <string>
class VP1GeoFlags {
public:

  typedef std::string SubSystemFlag;
  typedef std::vector<std::string> SubSystemFlags;
  

  //VolumeState:
  enum VOLSTATE { CONTRACTED,//Show volume if parent is EXPANDED, never show children.
		  EXPANDED,//Dont show volume ( => show all un-zapped children or their children as appropriate).
		  ZAPPED//Dont show volume ( => show no children).
                };
  static QString toString(const VOLSTATE&);

private:
  VP1GeoFlags();
  ~VP1GeoFlags();
};

//Q_DECLARE_OPERATORS_FOR_FLAGS(VP1GeoFlags::SubSystemFlags)

#endif

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

//-----------------------------------------------------------------------------//
//                                                                             //
//  TRTDetectorFactory_Full                                                    //
//                                                                             //
//  Authors:  Joe Boudreau, Andrei and Iouris Zalite, Thomas Kittelmann        //
//                                                                             //
//  This class creates the TRT Geometry. Cf. the README file                   //
//                                                                             //
//-----------------------------------------------------------------------------//

#ifndef TRTDETECTORFACTORY_FULL_H
#define TRTDETECTORFACTORY_FULL_H

#include "GeoModelKernel/GeoDefinitions.h"
#include <string>

class GeoPhysVol;
class GeoFullPhysVol;
class GeoShape;
class TRTParameterInterface;
class GeoXmlMatManager;

class TRTDetectorFactory_Full {

 public:
  TRTDetectorFactory_Full(bool useOldActiveGasMixture
			  , bool DC2CompatibleBarrelCoordinates
			  , bool doArgon
			  , bool doKrypton);

  ~TRTDetectorFactory_Full();

  virtual void create(GeoPhysVol *world);

 private:  

  const TRTDetectorFactory_Full & operator=(const TRTDetectorFactory_Full &right);
  TRTDetectorFactory_Full(const TRTDetectorFactory_Full &right);

  // Gas mixture enumerator
  enum ActiveGasMixture { GM_XENON
			  , GM_KRYPTON
			  , GM_ARGON
  };

  ActiveGasMixture DecideGasMixture(int strawStatusHT);

  // private helper methods:
  const GeoShape* makeModule (double length
			      , GeoTrf::Vector2D corner1
			      , GeoTrf::Vector2D corner2
			      , GeoTrf::Vector2D corner3
			      , GeoTrf::Vector2D corner4
			      , GeoTrf::Transform3D & absolutePosition
			      , double shrinkDist=0 ) const;

  GeoPhysVol* makeStraw(double& activeGasZPosition
			, bool hasLargeDeadRegion=false
			, ActiveGasMixture gasMixture = GM_XENON) const;

  GeoFullPhysVol* makeStrawPlane(size_t w
				 , ActiveGasMixture gasMixture = GM_XENON);

  // private member data:
  const GeoXmlMatManager* m_materialManager;
  TRTParameterInterface*  m_data;

  bool m_useOldActiveGasMixture;
  bool m_DC2CompatibleBarrelCoordinates;
  bool m_strawsvcavailable;
  bool m_doArgon;
  bool m_doKrypton;

  GeoFullPhysVol* m_type1Planes[3] = {nullptr, nullptr, nullptr};
  GeoFullPhysVol* m_type2Planes[3] = {nullptr, nullptr, nullptr};
};

#endif // TRTDetectorFactory_Full_h

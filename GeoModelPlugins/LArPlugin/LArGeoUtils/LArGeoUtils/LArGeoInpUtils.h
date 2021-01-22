/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef LARGEOUTILS_LARGEOINPUTILS_H
#define LARGEOUTILS_LARGEOINPUTILS_H

#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include "GeoXmlInpManager/GeoInpRecordset.h"
#include "GeoXmlInpManager/GeoInpRecord.h"
#include <string>

namespace LArGeoInpUtils {

  inline const GeoInpRecord *getTransformRecord(GeoInpRecordset_ptr positionRecSet, const std::string & key) {
    for (unsigned int s=0;s<positionRecSet->size(); s++) {
      const GeoInpRecord *currentRec= &((*positionRecSet)[s]);
      if (key == currentRec->getString("NAME")) {
	return currentRec;
      }
    }
    return nullptr;
  }

  inline GeoTrf::Transform3D getTransform (const GeoInpRecord *currentRec) {
    double x             = currentRec->getDouble("TRANSX")*SYSTEM_OF_UNITS::mm;
    double y             = currentRec->getDouble("TRANSY")*SYSTEM_OF_UNITS::mm;
    double z             = currentRec->getDouble("TRANSZ")*SYSTEM_OF_UNITS::mm;
    double theta         = currentRec->getDouble("THETA")*SYSTEM_OF_UNITS::rad;
    double phi           = currentRec->getDouble("PHI")*SYSTEM_OF_UNITS::rad;
    double rotationAngle = currentRec->getDouble("ROTATIONANGLE")*SYSTEM_OF_UNITS::rad;
    
    GeoTrf::Vector3D axis(sin(theta)*cos(phi), sin(theta)*sin(phi),cos(theta));
    return GeoTrf::Translate3D(x,y,z)*GeoTrf::Rotate3D(rotationAngle,axis);
  } 

}
#endif

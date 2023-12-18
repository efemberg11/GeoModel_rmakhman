/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOABSPOSITIONINFO_H
#define GEOMODELKERNEL_GEOABSPOSITIONINFO_H

/**
 * @class GeoAbsPositionInfo
 * 
 * @brief This class represents anything whose default and aligned
 * postions are known and cached.
 */

#include <GeoModelKernel/GeoDefinitions.h>
#include <memory>

class GeoAbsPositionInfo 
{
 public:
  GeoAbsPositionInfo() = default;
  ~GeoAbsPositionInfo() = default;

  //	Returns the default absolute transform.
  const GeoTrf::Transform3D * getAbsTransform () const {
     return m_absTransform.get();
  }

  //	Returns the default absolute transform.
  const GeoTrf::Transform3D * getDefAbsTransform () const {
     return m_defAbsTransform.get();
  }

  
  //	Clears the absolute transform.
  void clearAbsTransform ();

  //	Clears the default absolute transform.
  void clearDefAbsTransform ();

  //	Sets the absolute transform.
  void setAbsTransform (const GeoTrf::Transform3D &  xform);

  //	Sets the absolute transform.
  void setDefAbsTransform (const GeoTrf::Transform3D &  xform);

 private:

  //	The absolute transform from the world coord down to this
  //	positioned object.
  std::unique_ptr<GeoTrf::Transform3D> m_absTransform{};
  
  //	The default absolute transform from the world coord down
  //	to this positioned object.
  std::unique_ptr<GeoTrf::Transform3D> m_defAbsTransform{};
};

#endif

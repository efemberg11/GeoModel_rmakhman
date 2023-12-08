/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOLOGVOL_H
#define GEOMODELKERNEL_GEOLOGVOL_H

/**
 * @class GeoLogVol
 * 
 * @brief A Logical Volume, or agglomeration of shape and material.
 * The logical volume increases the reference count of the shape and the material.
 */

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include "GeoModelKernel/GeoMaterial.h"
#include <vector>

class GeoLogVol : public RCBase
{
 public:
  GeoLogVol (const std::string &Name, const GeoShape *Shape, const GeoMaterial *Material);

  //	Returns the name of the logical volume.
  const std::string & getName () const {
     return m_name;
  }
  
  //	Returns the shape of the logical volume.
  const GeoShape* getShape () const {
      return m_shape;
  }
  
  //	Returns the material of the logical volume.
  const GeoMaterial* getMaterial () const {
     return m_material;
  }
  
 protected:
  virtual ~GeoLogVol() = default;
  
 private:
  
  //	The name of the logical volume.
  std::string m_name{};

  //	Material composition of this volume.
  GeoIntrusivePtr<const GeoMaterial> m_material{};

  //	Shape of this volume.
  GeoIntrusivePtr<const GeoShape> m_shape{};
};


#endif

/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPEINTERSECTION_H
#define GEOMODELKERNEL_GEOSHAPEINTERSECTION_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include "GeoModelKernel/PersistifierFwd.h"


class GeoShapeIntersection : public GeoShape
{
 public:
  //    Constructor taking two shape operands
  GeoShapeIntersection (const GeoShape* A, const GeoShape* B);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise
  virtual bool contains (double x, double y, double z) const;

  //    Returns the AND shape type, as a string.
  virtual const std::string & type () const {
     return  getClassType();
  }

  //    Returns the AND shape type, as a coded integer.
  virtual ShapeType typeID () const {
     return getClassTypeID();
  }

  //    Returns the first operand being ANDed
  const GeoShape* getOpA () const {
      return m_opA;
  }

  //    Returns the second operand being ANDed.
  const GeoShape* getOpB () const {
     return m_opB;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType() {
     return s_classType;
  }

  static ShapeType getClassTypeID()  {
    return s_classTypeID;
  }

 protected:
    virtual ~GeoShapeIntersection() = default;


  //    The first shape operand in the AND operation.
  GeoIntrusivePtr<const GeoShape> m_opA{};

  //    The second shape operand in the AND operation.
  GeoIntrusivePtr<const GeoShape> m_opB{};

  //    Cached volume
  mutable std::atomic<double> fVolume{-1};

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  //    For I/O only!
  GeoShapeIntersection() = default;
  friend Persistifier;
};



#endif

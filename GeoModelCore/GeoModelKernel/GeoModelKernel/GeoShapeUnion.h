/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPEUNION_H
#define GEOMODELKERNEL_GEOSHAPEUNION_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include "GeoModelKernel/PersistifierFwd.h"

class GeoShapeUnion : public GeoShape
{
 public:
  //    Constructor taking two shape operands
  GeoShapeUnion (const GeoShape* A, const GeoShape* B);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume (int npoints = 1000000) const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise
  virtual bool contains (double x, double y, double z) const;
 
  //    Returns the OR shape type, as a string.
  virtual const std::string & type() const{
     return getClassType();
  }

  //    Returns the OR shape type, as a coded integer.
  virtual ShapeType typeID() const {
     return getClassTypeID();
  }

  //    Returns true if the resulting shape is a polyhedron, false otherwise.
  virtual bool isPolyhedron () const {
    return (m_opA->isPolyhedron() && m_opB->isPolyhedron());
  }

  //    Returns total number of constituents
  virtual unsigned int getNoConstituents () const {
    return (m_opA->getNoConstituents() + m_opB->getNoConstituents());
  }

  //    Returns the first operand being ORed
  const GeoShape* getOpA() const {
      return m_opA;
  }

  //    Returns the second operand being ORed.
  const GeoShape* getOpB() const{
      return m_opB;
  }

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //    For type identification.
  static const std::string& getClassType() {
     return s_classType;
  }

  //    For type identification.
  static ShapeType getClassTypeID() {
     return s_classTypeID;
  }

 protected:
  virtual ~GeoShapeUnion() = default;

 private:
  //    The first shape operand in the OR operation
  GeoIntrusivePtr<const GeoShape> m_opA{};

  //    The second shape operand in the OR operation
  GeoIntrusivePtr<const GeoShape> m_opB{};

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  //    For I/O only!
  GeoShapeUnion() = default;
  friend Persistifier;

};

#endif

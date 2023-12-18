/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPESUBTRACTION_H
#define GEOMODELKERNEL_GEOSHAPESUBTRACTION_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"


#ifndef _GeoShapePersistification_On_
  class Persistifier;
#endif


class GeoShapeSubtraction : public GeoShape {
 public:
  //    Constructor taking two shape operands
  GeoShapeSubtraction (const GeoShape* A, const GeoShape* B);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise
  virtual bool contains (double x, double y, double z) const;

  //    Returns the NOT shape type, as a string.
  virtual const std::string & type () const{
     return getClassType();
  }

  //    Returns the NOT shape type, as a coded integer.
  virtual ShapeType typeID () const{
     return  getClassTypeID();
  }

  //    Returns the first operand in the subtraction
  const GeoShape* getOpA () const{
     return m_opA;
  }

  //    Returns the second operand in the subtraction
  const GeoShape* getOpB () const{
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
  virtual ~GeoShapeSubtraction() = default;

 private:
 
  //    The first shape operand in the Subtraction operation
  GeoIntrusivePtr<const GeoShape> m_opA{nullptr};

  //    The second shape operand in the Subtraction operation
  GeoIntrusivePtr<const GeoShape> m_opB{nullptr};

  //    Cached volume
  mutable std::atomic<double> fVolume{-1.};

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  //    For I/O only!
  GeoShapeSubtraction() = default;
  friend Persistifier;

};


#endif

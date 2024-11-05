/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPGON_H
#define GEOMODELKERNEL_GEOPGON_H

#include "GeoModelKernel/GeoShape.h"
#include <vector>

class GeoPgon : public GeoShape
{
 public:
  //    Constructor for the PGON.  Note that the constructor
  //    does not fully build this object.  The PGON is not valid
  //    until at least two polygon planes have been added.
  GeoPgon (double SPhi, double DPhi, unsigned int NSides);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume (int npoints = 0) const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the PGON shape type, as a string.
  virtual const std::string & type() const {
     return getClassType();
  }

  //    Returns the PGON shape type, as a coded integer.
  virtual ShapeType typeID() const {
     return getClassTypeID();
  }

  //    Returns true as PGON is a polyhedron.
  virtual bool isPolyhedron () const {
    return true;
  }

  //    Add another plane to the polygon. A minimum of two
  //    planes are required to create a valid polygon.
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane);

  //    Returns the number of planes that have been created for
  //    the polygon.
  unsigned int getNPlanes() const {
    return m_zPlane.size ();
  }

  //    True if the polygon has at least two planes.  False
  //    otherwise.
  bool isValid() const {
     return m_zPlane.size () >= 2;
  }

  //    Get the Z Position of the specified plane.
  double getZPlane (unsigned int i) const {
    return m_zPlane[i];
  }

  //    Get the RMin of the specified plane.
  double getRMinPlane (unsigned int i) const {
     return m_rMinPlane[i];
  }

  //    Get the Z Position of the specified plane.
  double getRMaxPlane (unsigned int i) const {
     return m_rMaxPlane[i];
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

  //    Starting angle of the segment in radians.
  double getSPhi() const {
     return m_sPhi;
  }

  //    Delta angle of the segment in radians.
  double getDPhi() const {
     return m_dPhi;
  }

  //    Number of sides in each polygonal segment.
  unsigned int getNSides () const{
     return m_nSides;
  }

 protected:
  virtual ~GeoPgon() = default;

 private:
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_sPhi{0.};
  double m_dPhi{0.};
  unsigned int m_nSides{0};

  //    Z Position of polygon planes.
  std::vector<double> m_zPlane{};

  //    Minimum radius of polygon planes.
  std::vector<double> m_rMinPlane{};

  //    Maximum radius of polygon planes.
  std::vector<double> m_rMaxPlane{};
};

#endif

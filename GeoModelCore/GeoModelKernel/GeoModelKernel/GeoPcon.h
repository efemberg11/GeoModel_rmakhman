/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPCON_H
#define GEOMODELKERNEL_GEOPCON_H

/**
 * @class GeoPcon
 *
 * @brief This shape represents a polycone.
 * Specify the starting angle and delta angle (subtended angle)
 * of the polycone first, then add at least two planes with the
 * addPlane( double zPlane, double rInner,  double rOuter) method.
 */

#include "GeoModelKernel/GeoShape.h"
#include <vector>

class GeoPcon : public GeoShape {
 public:
  //    Constructor for the PCON.  Note that the constructor
  //    does not fully build this object.  The PCON is not valid
  //    until at least two polygon planes have been added.
  GeoPcon (double SPhi, double DPhi);

  //    Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //    Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const;

  //    Returns the PCON shape type, as a string.
  virtual const std::string& type() const {
      return getClassType();
  }

  //    Returns the PCON shape type, as a coded integer.
  virtual ShapeType typeID() const {
     return getClassTypeID();
  }

  //    Add another plane to the polycone  A minimum of two
  //    planes are required to create a valid polycone.
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane);

  //    Returns the number of planes that have been created for
  //    the polycone.
  unsigned int getNPlanes() const {
      return m_zPlane.size();
  }

  //    True if the polycone has at least two planes. False
  //    otherwise.
  bool isValid() const {
     return getNPlanes() >= 2;
  }

  //    Get the Z Position of the specified plane.
  double getZPlane(unsigned int i) const {
      return m_zPlane[i];
  }

  //    Get the RMin of the specified plane.
  double getRMinPlane(unsigned int i) const {
      return m_rMinPlane[i];
  }

  //    Get the Z Position of the specified plane.
  double getRMaxPlane(unsigned int i) const {
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

 protected:
  virtual ~GeoPcon() = default;

 private:

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_sPhi{0.};
  double m_dPhi{0.};

  //    Z Position of poly-cone planes.
  std::vector<double> m_zPlane{};

  //    Minimum radius of poly-cone planes.
  std::vector<double> m_rMinPlane{};

  //    Maximum radius of poly-cone planes.
  std::vector<double> m_rMaxPlane{};
};


#endif

/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/
//=========================================================================
//
// GeoUnidentifiedShape
//
// This class is essentially a bundle of data with no fixed format.
// ASCII information can be added and retreived. It serves as a
// proxy for shapes that are not part of the kernel, i.e. extender-
// class shapes.
//
// Joe Boudreau 2020
//
//=========================================================================

#ifndef _GeoUnidentifiedShape_h_
#define _GeoUnidentifiedShape_h_
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/Query.h"
#include <string>

class GeoUnidentifiedShape: public GeoShape {

 public:
  // Constructor:
  GeoUnidentifiedShape(const std::string & name);

  // Constructor:
  GeoUnidentifiedShape(const std::string & name, const std::string & asciiData);

  // Constructor with volume:
  GeoUnidentifiedShape(const std::string & name, const std::string & asciiData, double volume);

  // Returns the user-provided name of the volume (eg "MySpecialShape");
  const std::string & name() const;

  // Returns the ascii data associated with this object (possibly empty);
  const std::string & asciiData() const;

  // Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  // Returns the bonding box of the shape
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const;

  // Returns true if the shape contains the point, false otherwise
  virtual bool contains (double x, double y, double z) const;

  // Returns the shape type, as a string.
  virtual const std::string & type () const;

  // Returns the shape type, as an coded integer.
  virtual ShapeType typeID () const;

  // Returns false as the shape is not a polyhedron.
  virtual bool isPolyhedron () const {
    return false;
  }

  // For type identification.
  static const std::string& getClassType ();

  // For type identification.,
  static ShapeType getClassTypeID ();


  // Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;



 protected:
  // Destructor:
  ~GeoUnidentifiedShape();

 private:

  const std::string         _name;
  const std::string         _asciiData;
  const Query<double>       _volume;
  static const std::string         _classType;
  static const ShapeType           _classTypeID;

};

inline const std::string& GeoUnidentifiedShape::getClassType ()
{
  return _classType;
}

inline ShapeType GeoUnidentifiedShape::getClassTypeID ()
{
  return _classTypeID;
}

#endif

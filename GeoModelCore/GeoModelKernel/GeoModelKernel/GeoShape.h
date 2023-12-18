/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPE_H
#define GEOMODELKERNEL_GEOSHAPE_H

/**
 * @class GeoShape
 *
 * @brief This class describes a Shape. Shapes will be able to do the following things:
 *          * Identify themselves.
 *          * Compute their volume.
 *          * Combine themselves using Boolean operations with other shapes.
 *      The type identification works as follows:
 *           if (myShape->typeId()==GeoBox::classTypeId()) {
 *             .....
 *           }
 *      This avoids the need for dynamic casting with these
 *      classes.
 *
 *      Also noteworthy: Shapes are allocated on the heap and
 *      deleted automatically when their reference count falls
 *      to zero.
 */

#include "GeoModelKernel/RCBase.h"
#include <GeoModelKernel/GeoDefinitions.h>
#include <string>

using ShapeType = unsigned int;
class GeoShapeIntersection;
class GeoShapeUnion;
class GeoShapeSubtraction;
class GeoShapeShift;
class GeoShapeAction;

class GeoShape : public RCBase
{
 public:
  //    Constructor for shape. Must provide the name, a string to identify this shape.
  GeoShape () = default;

  //    Returns the volume of the shape, for mass inventory.
  virtual double volume () const;

  //    Returns the bonding box of the shape.
  virtual void extent (double& xmin, double& ymin, double& zmin,
                       double& xmax, double& ymax, double& zmax) const = 0;

  //    Returns true if the shape contains the point, false otherwise.
  virtual bool contains (double x, double y, double z) const = 0;

  //    Boolean OR operation for shapes.
  const GeoShapeUnion & add (const GeoShape& shape) const;

  //    Subtraction operation for shapes.
  const GeoShapeSubtraction & subtract (const GeoShape& shape) const;

  //    Intersection of shapes.
  const GeoShapeIntersection & intersect (const GeoShape& shape) const;

  //    Shift shapes, for boolean operations.
  const GeoShapeShift & operator << (const GeoTrf::Transform3D &shift) const;

  //    Returns the shape type, as a string.
  virtual const std::string & type () const = 0;

  //    Returns the shape type, as an coded integer.
  virtual ShapeType typeID () const = 0;

  //    Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const = 0;

 protected:
  virtual ~GeoShape() = default;

  //    Returns the bounding box of the specified disk. This method is used
  //    for calculation of the extend of a tube, cone, polycone, torus.
  static void diskExtent(double rmin, double rmax, double sphi, double dphi,
                         double& xmin, double& ymin, double& xmax, double& ymax);



};

#endif

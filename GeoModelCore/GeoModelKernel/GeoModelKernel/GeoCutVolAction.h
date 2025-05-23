/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCUTVOLACTION_H
#define GEOMODELKERNEL_GEOCUTVOLACTION_H

/**
 * @class GeoCutVolAction
 *
 * @brief Geo Volume action providing a mechanism for Cutoff implementation  
 *
 * The cutoff is applied to the mother Physical Volume and then it is
 * propagated down to all of child volumes.
 *
 * GeoCutVolAction object navigates the input Physical Volume (PV) tree.
 * For each original PV in the tree it creates and a new PV with
 * the same characteristics - material, LV name, PV name, ID, transform
 * in parents coordinate frame - except for shape. The new PV hierarchy
 * is kept by the GeoCutVolAction internally and can be obtained after
 * the action is done by calling getPV() method.
 *
 * When the cut action is finished users have to use the new PV tree
 * obtained with getPV(). The original PV can be dereferenced and
 * the action object destroyed.
 *
 * In order to implement a cutoff, one needs to provide two input
 * arguments to the constructor of GeoCutVolAction
 *   1. A shape to be cut off the mother volume and all its children
 *   2. A transformation of the cutoff shape with respect to the 
 *      coordinate frame of the mother shape
 *
 * Here is an example of GeoCutVolAction usage:
 *
 * |---------------------------------------------------------------------|
 * | GeoPVLink pPhysVol;                                                 |
 * | // ... add content to pPhysVol ...                                  |
 * | GeoCutVolAction action(shape,transform);                            |
 * | pPhysVol->apply(&action);                                           |
 * | GeoPVLink cutPhysVol = action.getPV();                              |
 * | // Use cutPhysVol afterwards (atach to the upper level volumes)     |
 * |---------------------------------------------------------------------|
 */

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoVolumeAction.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <stack>

class GeoPhysVol;
class GeoFullPhysVol;

class GeoCutVolAction : public GeoVolumeAction 
{
 public:
  // Constructor parameters:
  //  * shape: the shape to be cut
  //  * xf: Shape transformation with respect to the mother Physical Volume
  GeoCutVolAction(const GeoShape& shape
		  , const GeoTrf::Transform3D& xf);

  virtual void handleVPhysVol (const GeoVPhysVol* vPV);
  
  // Get the cutoff result
  GeoIntrusivePtr<GeoPhysVol> getPV();
  GeoIntrusivePtr<GeoFullPhysVol> getFPV();

 private: 
  PVLink m_physVol{nullptr};

  const GeoShape&            m_shape;
  const GeoTrf::Transform3D& m_transform;

  std::stack<PVLink>   m_copyStack;
};

#endif

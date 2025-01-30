/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEONODEACTION_H
#define GEOMODELKERNEL_GEONODEACTION_H

/**
 * @class GeoNodeAction
 *
 * @brief This type of action sees the geometry graph in full
 *      detail, including each transformation and property
 *      node.  The function of this graph is to represent a
 *      simpler tree of positioned volumes.  To visit only those
 *      volumes while keeping track of the state (including
 *      relative positioning), use the VolumeAction.
 */

#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoSerialIdentifier.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoVSurface.h"
#include "GeoModelKernel/GeoRectSurface.h"
#include "GeoModelKernel/GeoTrapezoidSurface.h"
#include "GeoModelKernel/GeoAnnulusSurface.h"
#include "GeoModelKernel/GeoDiamondSurface.h"
#include "GeoModelKernel/GeoNodePath.h"

class GeoNodeAction 
{
 public:

  enum Depth
  { ALL_ANCESTORS = -1, SELF = 0, SELF_AND_CHILDREN = 1 };

 public:
  GeoNodeAction() = default;
  GeoNodeAction(const GeoNodeAction &right) = delete;
  GeoNodeAction & operator=(const GeoNodeAction &right) = delete; 
  virtual ~GeoNodeAction() = default;
  
  //	Handles a Node.
  virtual void handleNode (const GeoGraphNode *);
  
  //	Handles a Transform.
  virtual void handleTransform (const GeoTransform *);
  
  //	Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *);
  
  //	Handles a full physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *);
 
  //	Handles a Name Tag.
  virtual void handleNameTag (const GeoNameTag *);
  
  //	Handles a Serial Denominator.
  virtual void handleSerialDenominator (const GeoSerialDenominator *);
  
  //	Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *);
  
  //	Handles an Identifier Tag.
  virtual void handleIdentifierTag (const GeoIdentifierTag *);
  
  //	Handles a SerialIdentifier.
  virtual void handleSerialIdentifier(const GeoSerialIdentifier *);

  //    Handle GeoVSurface
  virtual void handleVSurface (const GeoVSurface *surface);

  //    Handle Rectangle Virtual Surface
  virtual void handleRectSurface (const GeoRectSurface *surface);

  //    Handle Trapezoid Virtual Surface
  virtual void handleTrapezoidSurface (const GeoTrapezoidSurface *surface);

  //    Handle Annulus Virtual Surface
  virtual void handleAnnulusSurface (const GeoAnnulusSurface *surface);

  //    Handle Diamond Virtual Surface
  virtual void handleDiamondSurface (const GeoDiamondSurface *surface);

  //	Returns a pointer to the path object.
  GeoNodePath* getPath ();
  
  Query<unsigned int>  getDepthLimit ();
  
  //	Cause the action to terminate.
  void terminate ();
  
  //	Determine whether the action should terminate.
  bool shouldTerminate () const;
  
 
  //	Sets a depth limit.
  void setDepthLimit (unsigned int limit);
  
  //	Clears a depth limit, if any.
  void clearDepthLimit ();


  
 protected:
  //	Termination flag; causes an abortion of action execution.
  bool m_terminate{false};

 private:
  
  //	A limit may be placed upon the depth to which the action
  //	descends.  0 = self.  1 = self and children.
  Query<unsigned int> m_depth{};

  GeoNodePath m_path{};  
};

#endif

/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPRINTGRAPHACTION_H
#define GEOMODELKERNEL_GEOPRINTGRAPHACTION_H

/**
 * @class GeoPrintGraphAction
 * 
 * @brief This action prints the node tree.  It can be configured
 * to print a message on the following types of nodes:
 *      * Transforms
 *      * Physical Volumes and Full Physical Volumes
 *      * SerialDenominators
 *      * SerialTransforms 
 *      * IdentifierTag
 */

#include "GeoModelKernel/GeoNodeAction.h"
#include <iostream>

class GeoPrintGraphAction : public GeoNodeAction
{
  enum Type { TRANSFORM,
	      VOLUME,
	      NAMETAG,
	      IDENTIFIERTAG};
 public:
  GeoPrintGraphAction (std::ostream &o);
  virtual ~GeoPrintGraphAction();

  //	Handles a Transform.
  virtual void handleTransform (const GeoTransform *xform) override final;
  
  //	Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *vol) override final;
  
  //	Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *vol) override final;
  
  //	Handles a Name Tag.
  virtual void handleNameTag (const GeoNameTag *nameTag) override final;
  
  //	Handles a Serial Denominator.
  virtual void handleSerialDenominator (const GeoSerialDenominator *sD) override final;
  
  //	Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *sT) override final;
  
  //	Handles an Identifier Tag.
  virtual void handleIdentifierTag (const GeoIdentifierTag *idTag) override final;
  
  //    Handle a virtual surface.
  virtual void handleVSurface(const GeoVSurface *surface) override final;

  //    Handle a rectangular surface.
  virtual void handleRectSurface( const GeoRectSurface *rect) override final; 

  //    Handle a trapezoid surface.
  virtual void handleTrapezoidSurface( const GeoTrapezoidSurface *trapezoid) override final; 

  //    Handle an annulus surface.
  virtual void handleAnnulusSurface( const GeoAnnulusSurface *annulus) override final;

  //    Handle a diamond surface.
  virtual void handleDiamondSurface( const GeoDiamondSurface *diamond) override final;

  //	Sets the notification state.  Default: everything on.
  void setNotification (Type type, bool state);
  
 private:
  GeoPrintGraphAction(const GeoPrintGraphAction &right);
  GeoPrintGraphAction & operator=(const GeoPrintGraphAction &right);
  
  //	Indent the print.
  void indent ();
  
  //	A pointer to a name tag.  If the volume is named.
  const GeoNameTag *m_nameTag;

  //	A pointer to a serial denominator.  If one exists.
  const GeoSerialDenominator *m_serialDenominator;

  //	A pointer to an identifier tag.  If the volume is
  //	identified.
  const GeoIdentifierTag *m_idTag;

  //	List of Pending Transformations.
  std::vector<const GeoTransform *>  m_pendingTransformList;

  //	On/off flag for transforms.
  bool m_transformState;

  //	On/off flag for physical volumes.
  bool m_volumeState;

  //	On/off flag for name tags.
  bool m_nametagState;

  //	On/off flag for serial denominators.
  bool m_serialDenominatorState;
  
  //	On/off flag for serial transformers.
  bool m_serialTransformerState;

  //	On/off flag for identifier tags.
  bool m_identifierState;

  std::ostream &m_o;

  //	Flag for indent (intially 0)
  mutable bool m_indented;
};


#endif

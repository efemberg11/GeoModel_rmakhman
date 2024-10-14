/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

// ---------------------------------------------------------------------//
//                                                                      //
// SoVisualizeAction.  For internal use only.                           //
// Author: Joe Boudreau                                                 //
//                                                                      //
//----------------------------------------------------------------------//

#ifndef SOVISUALIZEACTION_H
#define SOVISUALIZEACTION_H 1

#include "GeoModelKernel/GeoShapeAction.h"

class SoShape;
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>

class SoVisualizeAction : public GeoShapeAction {

public:

  SoVisualizeAction();

  virtual ~SoVisualizeAction();

  virtual void handleShape(const GeoShape *subtract);

  virtual void handleBox(const GeoBox *box);

  virtual void handleCons(const GeoCons *cons);

  virtual void handleTorus(const GeoTorus *torus);

  virtual void handlePcon(const GeoPcon *pcon);

  virtual void handleTrap(const GeoTrap *trap);
    
  virtual void handleTwistedTrap(const GeoTwistedTrap *twistedtrap);

  virtual void handleTrd(const GeoTrd *trd);

  virtual void handleTube(const GeoTube *tube);

  virtual void handleTubs(const GeoTubs *tubs);

  virtual void handleSimplePolygonBrep(const GeoSimplePolygonBrep *);

  virtual void handleTessellatedSolid (const GeoTessellatedSolid *);

  virtual void handleGenericTrap (const GeoGenericTrap *);
  
  virtual void handleRectSurface (const GeoRectSurface *);

  virtual void handleTrapezoidSurface (const GeoTrapezoidSurface *);

  virtual void handleAnnulusSurface (const GeoAnnulusSurface *);

  virtual void handleDiamondSurface (const GeoDiamondSurface *);

  SoShape * getShape() { return m_shape; }
  void reset() { m_shape = 0; }
  void reset_separator() { vs_shape = new SoSeparator; }
  SoNormal * getNormal() { return surfaceNorms; }
  SoNormalBinding * getNormalBinding() { return surfBinding; }
  SoMaterialBinding * getMaterialBinding() { return surfMaterialBinding; }
  SoCoordinate3 * getCoords() { return coords; }
  SoSeparator * getVSurfaceShape() { return vs_shape; }

private:

  SoVisualizeAction(const SoVisualizeAction &);
  const SoVisualizeAction & operator=(const SoVisualizeAction &);

  SoShape *m_shape;
  SoNormal *surfaceNorms = new SoNormal;
  SoNormalBinding* surfBinding = new SoNormalBinding;
  SoMaterialBinding* surfMaterialBinding = new SoMaterialBinding;
  SoCoordinate3 *coords;
  SoSeparator* vs_shape = new SoSeparator;

  
};

#endif

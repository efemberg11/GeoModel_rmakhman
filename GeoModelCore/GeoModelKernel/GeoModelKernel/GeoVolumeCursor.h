/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVOLUMECURSOR_H
#define GEOMODELKERNEL_GEOVOLUMECURSOR_H

#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoVSurface.h"
#include <vector>

class GeoVAlignmentStore;

class GeoVolumeCursor final : public GeoNodeAction
{
 public:
  using VSConstLink = GeoVSurface::VSConstLink;
  GeoVolumeCursor (PVConstLink parent, GeoVAlignmentStore* store=nullptr);
  virtual ~GeoVolumeCursor() = default;

  /// Advance
  void next();

  /// Check if we are at the last volume.
  bool atEnd() const;

  /// Returns the selected physical volume or nullptr if not found.
  PVConstLink getVolume () const;

  /// Returns the selected virtual surface or nullptr if not found.
  VSConstLink getSurface () const;

  /// Returns the transformation to the volume.
  GeoTrf::Transform3D getTransform () const;
  
  /// Returns the default ransformation to the volume.
  GeoTrf::Transform3D getDefTransform () const;

  /// Returns the name of the volume.  From nametag or serial denominator.
  std::string getName () const;
      
  /// Returns the id of the volume.  From the identifier.
  std::optional<int> getId () const;

  bool hasAlignableTransform() const;

 private:
  /// Handles a Transform.
  virtual void handleTransform (const GeoTransform *xform) override;

  /// Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *vol) override;

  /// Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *vol) override;
  
  /// Handles a virtual surface.
  virtual void handleVSurface (const GeoVSurface *surf) override;
  
  /// Handles a Name Tag.
  virtual void handleNameTag (const GeoNameTag *nameTag) override;

  /// Handles a Serial Denominator.
  virtual void handleSerialDenominator (const GeoSerialDenominator *sD) override;

  /// Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *sT) override;

  /// Handles an Identifier Tag.
  virtual void handleIdentifierTag (const GeoIdentifierTag *idTag) override;

  /// Handles a Serial Identifier.
  virtual void handleSerialIdentifier(const GeoSerialIdentifier *sI) override;

  /// Ressucitate (undo terminate)
  void resuscitate();

  PVConstLink                           m_parent{nullptr};
  PVConstLink                           m_volume{nullptr};
  VSConstLink                           m_surface{nullptr};
  GeoTrf::Transform3D                   m_transform{GeoTrf::Transform3D::Identity()};
  GeoTrf::Transform3D                   m_defTransform{GeoTrf::Transform3D::Identity()};
  
  unsigned int                          m_majorIndex{0};
  unsigned int                          m_minorIndex{0};
  unsigned int                          m_minorLimit{0};
  const GeoSerialTransformer*           m_serialTransformer{nullptr};
  
  const GeoNameTag*                     m_nameTag{nullptr};
  const GeoSerialDenominator*           m_serialDenominator{nullptr};
  const GeoIdentifierTag*               m_idTag{nullptr};
  std::vector<const GeoTransform *>     m_pendingTransformList{};
  unsigned int                          m_serialDenomPosition{0};
  const GeoSerialIdentifier*            m_serialIdentifier{nullptr};
  unsigned int                          m_serialIdentPosition{0};
  unsigned int                          m_volCount{0};
  bool                                  m_hasAlignTrans{false};

  GeoVAlignmentStore*                   m_alignStore{};
};

#endif

/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GeoAccessVolumeAction_h
#define GeoAccessVolumeAction_h 1

#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

class GeoVSurface;
class GeoVPhysVol;
class GeoTransform;
class GeoVAlignmentStore;

/**
 * @class GeoAccessVolumeAction
 *
 * @brief Accesses volumes, returning the ith volume and also its transformation relative to the parent.
 */

class GeoAccessVolumeAction final : public GeoNodeAction
{
 public:

  GeoAccessVolumeAction (unsigned int Index, const GeoVAlignmentStore* store);
  virtual ~GeoAccessVolumeAction()  =default; 

  /// Handles a Transform.
  virtual void handleTransform (const GeoTransform *xform) override;

  /// Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *vol) override;

  /// Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *vol) override;

  /// Handles a virtual surface, to refresh the pendingTransformlist
  virtual void handleVSurface (const GeoVSurface *surface) override;

  /// Returns the selected physical volume or nullptr if not found.
  PVConstLink getVolume () const;

  /// Returns the transformation to the volume.
  const GeoTrf::Transform3D & getTransform () const;

  /// Returns the default ransformation to the volume.
  const GeoTrf::Transform3D & getDefTransform () const;

  /// Returns the name of the volume.  From nametag or serial denominator.
  const std::string & getName () const;

  /// Handles a Name Tag.
  virtual void handleNameTag (const GeoNameTag *nameTag) override;

  /// Handles a Serial Denominator.
  virtual void handleSerialDenominator (const GeoSerialDenominator *sD) override;

  /// Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *sT) override;

  /// Handles an Identifier Tag.
  virtual void handleIdentifierTag (const GeoIdentifierTag *idTag) override;

  /// Returns the id of the volume.  From the identifier.
  Query<int> getId () const;

  /// Handles an Serial Identifier
  virtual void handleSerialIdentifier(const GeoSerialIdentifier *sI) override;

 private:
  /// Returns a pointer to the ith physical volume under this one.
  PVConstLink m_volume{};

  /// The transformation to the ith volume.
  GeoTrf::Transform3D m_transform{GeoTrf::Transform3D::Identity()};

  /// The default transformation to the ith volume.
  GeoTrf::Transform3D m_defTransform{GeoTrf::Transform3D::Identity()};

  /// The volume which we are interested in seeking.
  unsigned int m_index{0};

  /// The volume which we are interested in seeking.
  unsigned int m_counter{0};

  /// The name of the volume.  From a nametag or a serial denominator.
  mutable std::string m_name{};

  /// A pointer to a name tag.  If the volume is named.
  const GeoNameTag *m_nameTag{nullptr};

  /// A pointer to a serial denominator.  If one exists.
  const GeoSerialDenominator *m_serialDenominator{nullptr};

  /// A pointer to an identifier tag.  If the volume is identified.
  const GeoIdentifierTag *m_idTag{nullptr};

  /// List of Pending Transformations.
  std::vector<const GeoTransform *>  m_pendingTransformList{};

  /// Position of the serial denominator.  Used to assign a numeric suffix to the name, eg BaseName+99
  unsigned int m_serialDenomPosition{0};

  const GeoSerialIdentifier *m_serialIdentifier{nullptr};
  unsigned int m_serialIdentPosition{0};
  const GeoVAlignmentStore* m_alignStore{nullptr};
};

#endif

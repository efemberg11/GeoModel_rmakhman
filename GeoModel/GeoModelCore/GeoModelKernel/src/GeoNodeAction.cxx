/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoNodeAction.h"

void GeoNodeAction::handleNode (const GeoGraphNode *)
{
  // Do nothing...    
}

void GeoNodeAction::handleTransform (const GeoTransform *)
{
  // Do nothing...    
}

void GeoNodeAction::handlePhysVol (const GeoPhysVol *)
{
}

void GeoNodeAction::handleFullPhysVol (const GeoFullPhysVol *)
{
}

GeoNodePath* GeoNodeAction::getPath ()
{
  return &m_path;
}

std::optional<unsigned int>  GeoNodeAction::getDepthLimit ()
{
  return m_depth;
}

void GeoNodeAction::terminate ()
{
  m_terminate = true;
}

bool GeoNodeAction::shouldTerminate () const
{
  return m_terminate;
}

void GeoNodeAction::handleNameTag (const GeoNameTag *)
{
}

void GeoNodeAction::handleSerialDenominator (const GeoSerialDenominator *)
{
}

void GeoNodeAction::setDepthLimit (unsigned int limit)
{
  m_depth = limit;
}

void GeoNodeAction::clearDepthLimit () {
  m_depth = std::nullopt;
}

void GeoNodeAction::handleSerialTransformer (const GeoSerialTransformer  *)
{
}

void GeoNodeAction::handleIdentifierTag (const GeoIdentifierTag *)
{
}

void GeoNodeAction::handleSerialIdentifier(const GeoSerialIdentifier *)
{
}

void GeoNodeAction::handleVSurface (const GeoVSurface *surface) {
}

void GeoNodeAction::handleRectSurface (const GeoRectSurface *rect) {
}

void GeoNodeAction::handleTrapezoidSurface (const GeoTrapezoidSurface *trapezoid) {
}

void GeoNodeAction::handleAnnulusSurface (const GeoAnnulusSurface *annulus) {
}

void GeoNodeAction::handleDiamondSurface (const GeoDiamondSurface *diamond) {
}

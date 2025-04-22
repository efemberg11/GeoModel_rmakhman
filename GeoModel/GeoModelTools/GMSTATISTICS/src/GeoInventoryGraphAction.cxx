/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInventoryGraphAction.h"

GeoInventoryGraphAction::GeoInventoryGraphAction (std::ostream &o)
  : m_nameTag(nullptr)
  , m_serialDenominator(nullptr)
  , m_idTag(nullptr)
  , m_transformState(true)
  , m_volumeState(true)
  , m_nametagState(true)
  , m_serialDenominatorState(true)
  , m_serialTransformerState(true)
  , m_identifierState(true)
  , m_o(o)
  , m_indented(false)
{
}

GeoInventoryGraphAction::~GeoInventoryGraphAction()
{
}

void GeoInventoryGraphAction::handleTransform (const GeoTransform *xform)
{
  m_pendingTransformList.push_back(xform);
  if (m_transformState) {
    indent();
    m_o << "XF(" << xform << ")";
  }
}

void GeoInventoryGraphAction::handlePhysVol (const GeoPhysVol *vol)
{
  if (m_volumeState) {
    indent();
    m_o << "PV(" << vol << "),LV(" << vol->getLogVol()<< "),lvname=" << vol->getLogVol()->getName() << std::endl;
  }
  m_pendingTransformList.erase(m_pendingTransformList.begin(),m_pendingTransformList.end());
  m_indented=false;
}

void GeoInventoryGraphAction::handleFullPhysVol (const GeoFullPhysVol *vol)
{
  if (m_volumeState) {
    indent();
    m_o << "FPV(" << vol << "),LV(" << vol->getLogVol()<< "),lvname=" << vol->getLogVol()->getName() << std::endl;
  }
  m_pendingTransformList.erase(m_pendingTransformList.begin(),m_pendingTransformList.end());
  m_indented=false;
}

void GeoInventoryGraphAction::handleNameTag (const GeoNameTag *nameTag)
{
  if (m_nametagState) {
    indent();
    m_o << "NT("<<nameTag <<")" << nameTag->getName() << "+";
  }
}

void GeoInventoryGraphAction::handleSerialDenominator (const GeoSerialDenominator *sD)
{
  if (m_nametagState) {
    indent();
    m_o << "SD("<< sD <<")" << sD->getBaseName() << "+";
  }
}

void GeoInventoryGraphAction::handleSerialTransformer (const GeoSerialTransformer  *sT)
{
  if (m_volumeState) {
    indent();
    m_o << sT->getNCopies() << " PARAMETERIZED VOLUMES(" << sT->getVolume()->getLogVol()->getName() << ")" << std::endl;
  }
  m_pendingTransformList.erase(m_pendingTransformList.begin(),m_pendingTransformList.end());
  m_indented=false;
}

void GeoInventoryGraphAction::handleIdentifierTag (const GeoIdentifierTag *idTag)
{
  if (m_identifierState) {
    indent();
    m_o << "ID(" << idTag <<")" << idTag->getIdentifier() << "+";
  }
}

void GeoInventoryGraphAction::setNotification (Type type, bool state)
{
  if (type==TRANSFORM) {
    m_transformState=state;
  }
  else if (type==VOLUME) {
    m_volumeState=state; 
  }
  else if (type==NAMETAG) {
    m_nametagState=state;
  }
  else if (type==IDENTIFIERTAG) {
    m_identifierState=state;
  }
}

void GeoInventoryGraphAction::indent ()
{
  if (!m_indented) {
    m_indented=true;
    for (size_t i=0;i<getPath()->getLength(); i++) {
      m_o << "   ";
    }
  }
}



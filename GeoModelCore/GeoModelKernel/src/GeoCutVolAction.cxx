/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoCutVolAction.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"

GeoCutVolAction::GeoCutVolAction(const GeoShape& shape
				 , const GeoTrf::Transform3D& xf)
  : GeoVolumeAction (GeoVolumeAction::TOP_DOWN)
  , m_shape(shape)
  , m_transform(xf)
{
}

void GeoCutVolAction::handleVPhysVol(const GeoVPhysVol *vPV)
{
  // Determine path length: 
  // 0  - for the starting PV
  // 1  - for the first level daughters etc.
  unsigned int pathLen = this->getState()->getPath()->getLength();

  // Get Logical Volume details
  GeoIntrusivePtr<const GeoLogVol> lvOriginal = vPV->getLogVol();
  const std::string& lvNameOriginal = lvOriginal->getName();
  const GeoMaterial* lvMatOriginal = lvOriginal->getMaterial();
  const GeoShape* lvShapeOriginal = lvOriginal->getShape();

  if(pathLen==0) {
    // We are at the first PV. Create the resulting PV
    const GeoShape& cutShape = lvShapeOriginal->subtract(m_shape << m_transform);
    GeoIntrusivePtr<GeoLogVol> lvNew = make_intrusive<GeoLogVol>(lvNameOriginal,&cutShape,lvMatOriginal);

    if(dynamic_cast<const GeoFullPhysVol*>(vPV)) {
      m_physVol = make_intrusive<GeoFullPhysVol>(lvNew);
    }
    else {
      m_physVol = make_intrusive<GeoPhysVol>(lvNew);
    }
    
    // Save the new PV in the copy stack
    m_copyStack.push(m_physVol);
  }
  else {
    // determine the parent PV in the copy tree
    while(m_copyStack.size()>pathLen) m_copyStack.pop();
    GeoVPhysVol* copyParent = m_copyStack.top();

    // Calculate cut transform to the reference frame of current PV
    GeoTrf::Transform3D cutTransform = (getState()->getAbsoluteTransform()).inverse()*m_transform;

    // Construct new PV
    const GeoShape& cutShape = lvShapeOriginal->subtract(m_shape << cutTransform);
    GeoIntrusivePtr<GeoLogVol> lvNew = make_intrusive<GeoLogVol>(lvNameOriginal,&cutShape,lvMatOriginal);
    PVLink pvNew = make_intrusive<GeoPhysVol>(lvNew);

    // Get its characteristics (name, id, transform to parent)
    std::string pvName = getState()->getName();
    if(!pvName.empty()) copyParent->add(make_intrusive<GeoNameTag>(pvName));

    const Query<int> pvId = getState()->getId();
    if(pvId.isValid()) copyParent->add(make_intrusive<GeoIdentifierTag>(pvId));

    copyParent->add(make_intrusive<GeoTransform>(getState()->getTransform()));
    copyParent->add(pvNew);

    // Save new PV in the copy stack
    m_copyStack.push(pvNew);
  }
}

GeoIntrusivePtr<GeoPhysVol> GeoCutVolAction::getPV() {
  return dynamic_pointer_cast<GeoPhysVol>(m_physVol);
}

GeoIntrusivePtr<GeoFullPhysVol> GeoCutVolAction::getFPV() {
  return dynamic_pointer_cast<GeoFullPhysVol>(m_physVol);
}


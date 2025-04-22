/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoNodeAction.h"

GeoSerialTransformer::GeoSerialTransformer (const GeoVPhysVol *volume, 
                                            const GeoXF::Function *func, 
                                            unsigned int copies): 
   m_nCopies{copies},
   m_function{func->clone()},
   m_physVol{volume} {}


void GeoSerialTransformer::exec(GeoNodeAction *action) const {
    action->handleSerialTransformer(this);
}

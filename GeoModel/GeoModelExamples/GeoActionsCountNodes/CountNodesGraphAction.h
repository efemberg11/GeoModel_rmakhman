
/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

/*
 * author: Riccardo.Maria.Bianchi@cern.ch, 2022 Dec
 *
 */

#ifndef CountNodesGraphAction_H
#define CountNodesGraphAction_H

// GeoModel includes
#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoNodeAction.h"

/**
 * \class GeoCountGraphAction
 *
 * GeoCountGraphAction acts on objects of the GeoModel tree
 * and count all GeoModel nodes (GeoPhysVol, ...) and objects (GeoLogVol,
 * GeoShape, ...).
 * In this example, we build an action that count GeoPhysVol, GeoLogVol, 
 * GeoShape, and GeoMaterial nodes.
 */
class CountNodesGraphAction : public GeoNodeAction {
   
  public:
    CountNodesGraphAction(){};
    ~CountNodesGraphAction(){};
    
    // Handles a physical volume.
    virtual void handlePhysVol(const GeoPhysVol *vol);
    // Handles a Serial Transformer
    virtual void handleSerialTransformer(const GeoSerialTransformer *obj);

    unsigned long countPhysVols() { return m_physvols; }
    unsigned long countLogVols() { return m_logvols; }
    unsigned long countMaterials() { return m_materials; }
    unsigned long countShapes() { return m_shapes; }

   private:
    unsigned addPhysVol() { return ++m_physvols; };
    unsigned addLogVol() { return ++m_logvols; };
    unsigned addShape() { return ++m_shapes; };
    unsigned addMaterial() { return ++m_materials; };

    unsigned long m_physvols{0};
    unsigned long m_logvols{0};
    unsigned long m_materials{0};
    unsigned long m_shapes{0};

};  // end class

#endif

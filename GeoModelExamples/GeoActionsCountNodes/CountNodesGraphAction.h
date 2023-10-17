
/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
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
 */
class CountNodesGraphAction : public GeoNodeAction {
   public:
    CountNodesGraphAction(){};
    ~CountNodesGraphAction(){};
    //	Handles a physical volume.
    virtual void handlePhysVol(const GeoPhysVol *vol);
    //	Handles a Serial Transformer
    virtual void handleSerialTransformer(const GeoSerialTransformer *obj);

    /*
        //	Handles a full physical volume.
        virtual void handleFullPhysVol(const GeoFullPhysVol *vol);
        //	Handles a Serial Denominator.
        virtual void handleSerialDenominator(const GeoSerialDenominator *sD);
        //	Handles a  Transform.
        virtual void handleTransform(const GeoTransform *);
        //	Handles a Name Tag.
        virtual void handleNameTag(const GeoNameTag *);
        //	Handles an Identifier Tag.
        virtual void handleIdentifierTag(const GeoIdentifierTag *);
        //	Handles a SerialIdentifier.
        virtual void handleSerialIdentifier(const GeoSerialIdentifier *);
    */

    unsigned long countPhysVols() { return m_physvols; }
    unsigned long countFullPhysVols() { return m_fullphysvols; }
    unsigned long countLogVols() { return m_logvols; }

   private:
    void handleLogVol();
    void handleMaterial();

    unsigned addPhysVol() { return ++m_physvols; };
    unsigned addFullPhysVol() { return ++m_fullphysvols; };
    unsigned addLogVol() { return ++m_logvols; };
    unsigned addShape() { return ++m_shapes; };
    unsigned addMaterial() { return ++m_materials; };
    // unsigned addElement();
    // unsigned addFunction();

    unsigned long m_physvols{0};
    unsigned long m_fullphysvols{0};
    unsigned long m_logvols{0};
    unsigned long m_materials{0};
    unsigned long m_shapes{0};

};  // end class

#endif

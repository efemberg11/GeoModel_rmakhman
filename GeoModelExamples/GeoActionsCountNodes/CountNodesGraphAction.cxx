/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

/*
 * author: Riccardo.Maria.Bianchi@cern.ch, 2022 Dec
 *
 */

/*
 * This example shows how to implement counting actions to count 
 * different types of nodes in the GeoModel tree. 
 * In particular, here we want to count instances of 
 * GeoPhysVol, GeoLogVol, GeoShape, and GeoMaterial nodes. 
 */

#include "CountNodesGraphAction.h"

#include "GeoModelKernel/GeoBox.h"


//	Handles a physical volume.
void CountNodesGraphAction::handlePhysVol(const GeoPhysVol* vol) {
    
    addPhysVol(); // increment the numbers of GeoPhysVol nodes

    // LOGVOL
    const GeoLogVol* logVol = vol->getLogVol();
    addLogVol(); // increment the numbers of GeoLogVol nodes

    // MATERIAL
    const GeoMaterial* mat = vol->getLogVol()->getMaterial();
    addMaterial(); // increment the numbers of GeoMaterial nodes
    // SHAPE
    const GeoShape* shape = vol->getLogVol()->getShape();
    addShape(); // increment the numbers of GeoShape nodes 
}

//	Handles a Serial Transformer
void CountNodesGraphAction::handleSerialTransformer(
    const GeoSerialTransformer* node) {
    // get linked function and number of copies
    const GeoXF::Function* func = node->getFunction();
    unsigned int nCopies = node->getNCopies();

    // get linked VPhysVol volume
    const GeoVPhysVol* vol = &(*(node->getVolume()));
    if (dynamic_cast<const GeoPhysVol*>(vol)) {
        addPhysVol(); // increment the numbers of GeoLogVol nodes
    } else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
        // we do not count this node in this example, but you could do it 
        // by adding two methods:
        // - handleFullPhysVol(), to catch the GeoFullPhysVol instances in the GeoModel tree and handle them
        // - addFullPhysVol(), to count the numbers of instances of GeoFullLogVol nodes
    } else {
        std::cout << "ERROR!!! Unknown VPhysVol type!!" << std::endl;
    }
}


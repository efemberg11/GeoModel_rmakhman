
#include "GeoCountGraphAction.h"
namespace GeoModelIO {

//	Handles a physical volume.
void CountNodesGraphAction::handlePhysVol(const GeoPhysVol* vol) {
    // count this PhysVol
    addPhysVol();

    // LOGVOL
    const GeoLogVol* logVol = vol->getLogVol();
    unsigned long nlogvols = addLogVol();
    // const std::string logName = logVol->getName();

    // MATERIAL
    const GeoMaterial* mat = vol->getLogVol()->getMaterial();
    unsigned long nmaterials = addMaterial();
    // SHAPE
    const GeoShape* shape = vol->getLogVol()->getShape();
    unsigned long nshapes = addShape();
}

//	Handles a Serial Transformer
void CountNodesGraphAction::handleSerialTransformer(
    const GeoSerialTransformer* node) {
    // get linked function and number of copies
    const GeoXF::Function* func = node->getFunction();
    unsigned int nCopies = node->getNCopies();

    // get linked VPhysVol volume
    const GeoVPhysVol* vol = &(*(node->getVolume()));
    // const GeoPhysVol* vol = dynamic_cast<const GeoPhysVol*>(volV);
    std::string volType;
    if (dynamic_cast<const GeoPhysVol*>(vol)) {
        volType = "GeoPhysVol";
        addPhysVol();
    } else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
        volType = "GeoFullPhysVol";
        addFullPhysVol();
    } else {
        std::cout << "ERROR!!! Unknown VPhysVol type!!" << std::endl;
    }
}
/*
    //	Handles a full physical volume.
     void GeoCountGraphAction::handleFullPhysVol(const GeoFullPhysVol
   *vol);
    //	Handles a Serial Denominator.
     void GeoCountGraphAction::handleSerialDenominator(const
   GeoSerialDenominator *sD);
    //	Handles a  Transform.
     void GeoCountGraphAction::handleTransform(const GeoTransform *);
    //	Handles a Name Tag.
     void GeoCountGraphAction::handleNameTag(const GeoNameTag *);
    //	Handles an Identifier Tag.
     void GeoCountGraphAction::handleIdentifierTag(const GeoIdentifierTag
   *);
    //	Handles a SerialIdentifier.
     void GeoCountGraphAction::handleSerialIdentifier(const
   GeoSerialIdentifier *);

    void handleLogVol();
    void handleMaterial();
*/

};  // namespace GeoModelIO

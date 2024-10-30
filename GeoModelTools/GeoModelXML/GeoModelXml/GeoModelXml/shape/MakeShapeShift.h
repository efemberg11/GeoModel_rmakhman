/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_MAKE_SHAPESHIFT_H
#define GEO_MODEL_XML_MAKE_SHAPESHIFT_H
#include <xercesc/util/XercesDefs.hpp>

#include "GeoModelXml/Element2GeoItem.h"
//
//    Create and return a HepRotation3d. Caller must delete it.
//

XERCES_CPP_NAMESPACE_BEGIN
class DOMElement;
XERCES_CPP_NAMESPACE_END



class GmxUtil;

class MakeShapeShift: public Element2GeoItem {
public:
    MakeShapeShift() = default;
    virtual GeoIntrusivePtr<RCBase> make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const override;
};

#endif // MAKE_ROTATION_H

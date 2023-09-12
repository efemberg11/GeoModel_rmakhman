/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef ADDPLANE_H
#define ADDPLANE_H
#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN
class DOMElement;
XERCES_CPP_NAMESPACE_END

class GmxUtil;

class AddPlane {
public:
    AddPlane() {};
    ~AddPlane() {};
    void process(const xercesc::DOMElement *element, double &zPlane, double &rMinPlane, double &rMaxPlane);
    GmxUtil* gmxUtil=nullptr;
};


#endif //ADDPLANE_H

/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_REPLICAY_PROCESSOR_H
#define GEO_MODEL_XML_REPLICAY_PROCESSOR_H
#include <xercesc/util/XercesDefs.hpp>
//
//   Processor for replicaY tags.
//
#include <string>
#include <map>

#include "GeoModelXml/ElementProcessor.h"
class GmxUtil;

class ReplicaYProcessor: public ElementProcessor {
public:
    ReplicaYProcessor();
    void process(const xercesc::DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd);
private:
    std::map <std::string, GeoNodeList> m_map; 
};

#endif // REPLICAY_PROCESSOR_H

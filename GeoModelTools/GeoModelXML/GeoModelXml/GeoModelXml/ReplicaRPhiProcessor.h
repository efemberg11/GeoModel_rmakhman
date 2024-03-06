/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_REPLICARPHI_PROCESSOR_H
#define GEO_MODEL_XML_REPLICARPHI_PROCESSOR_H
#include <xercesc/util/XercesDefs.hpp>
//
//   Processor for replicaRPhi tags.
//
#include <string>
#include <map>

#include "GeoModelXml/ElementProcessor.h"
class GmxUtil;

class ReplicaRPhiProcessor: public ElementProcessor {
public:
    ReplicaRPhiProcessor();
    void process(const xercesc::DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd);
private:
    std::map <std::string, GeoNodeList> m_map{}; 
};

#endif // REPLICAX_PROCESSOR_H

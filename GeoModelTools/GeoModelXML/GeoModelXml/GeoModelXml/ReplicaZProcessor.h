/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_REPLICAZ_PROCESSOR_H
#define GEO_MODEL_XML_REPLICAZ_PROCESSOR_H
#include <xercesc/util/XercesDefs.hpp>
//
//   Processor for replicaZ tags.
//
#include <string>
#include <map>

#include "GeoModelXml/ElementProcessor.h"
class GmxUtil;

class ReplicaZProcessor: public ElementProcessor {
public:
    void process(const xercesc::DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd);
private:
    std::map <std::string, GeoNodeList> m_map; 
};

#endif // REPLICAX_PROCESSOR_H

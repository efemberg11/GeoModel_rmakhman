/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_MDT_PROCESSOR_H
#define GEO_MODEL_XML_MDT_PROCESSOR_H
#include <xercesc/util/XercesDefs.hpp>
//
//   Processor for mdt tags. This is a shortcut to create MDT mattresses at once, rather than going the full XML way
//
#include <string>
#include <map>

#include "GeoModelXml/ElementProcessor.h"
class GmxUtil;
class GeoNameTag;
class GeoLogVol;
class GeoPhysVol;

class MDTProcessor: public ElementProcessor {
public:
    typedef struct {
        GeoNameTag *name;
        int id;
        GeoLogVol *logVol;
        bool alignable;
    } MDTStore;
    void process(const xercesc::DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd);
private:
    std::map<std::string, MDTStore> m_map;
    GeoPhysVol* BuildExampleVolume(std::string name);
};

#endif // MDT_PROCESSOR_H

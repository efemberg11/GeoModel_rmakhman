/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_LOGVOL_PROCESSOR_H
#define GEO_MODEL_XML_LOGVOL_PROCESSOR_H
#include <xercesc/util/XercesDefs.hpp>
//
//   Processor for logvol tags.
//
#include <string>
#include <map>

#include "GeoModelXml/ElementProcessor.h"
#include "GeoModelKernel/GeoNameTag.h"
class GmxUtil;

class LogvolProcessor: public ElementProcessor {
public:
     struct LogVolStore {
        GeoIntrusivePtr<GeoNameTag> name{};
        int id{0};
        GeoLogVolPtr logVol{};
        bool alignable{false};
    } ; 
    void process(const xercesc::DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd);
    void zeroId(const xercesc::DOMElement *element);
private:
    std::map<std::string, LogVolStore> m_map; 
};

#endif // LOGVOL_PROCESSOR_H

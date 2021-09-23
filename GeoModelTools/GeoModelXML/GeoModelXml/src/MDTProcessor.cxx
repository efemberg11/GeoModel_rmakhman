/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//   Processor for MDT elements
//
//   Add name to list.
//   Create a physvol using my logvol; add it to list.
//
//
#include "GeoModelXml/MDTProcessor.h"
#include "OutputDirector.h"

#include <map>

#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelXml/GmxUtil.h"
#include "GeoModelXml/GeoNodeList.h"
#include "xercesc/util/XMLString.hpp"

using namespace std;
using namespace xercesc;

void MDTProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
GeoLogVol *lv;
GeoPhysVol* pv;
GeoNameTag *physVolName;

    gmxUtil.positionIndex.incrementLevel();

    XMLCh * name_tmp = XMLString::transcode("name");
    char *name2release = XMLString::transcode(element->getAttribute(name_tmp));
    string name(name2release);
    XMLString::release(&name2release);
    XMLString::release(&name_tmp);
//
//    Look for the logvol in the map; if not yet there, add it
//
    map<string, MDTStore>::iterator entry;
    if ((entry = m_map.find(name)) == m_map.end()) { // Not in registry; make a new item
//
//    Name
//
        m_map[name] = MDTStore();
        MDTStore *store = &m_map[name];
        physVolName = new GeoNameTag(name);
        store->name = physVolName;
        store->id = 0;
    }
//
//    Make the LogVol and add it to the map ready for next time
//
//  lv = new GeoLogVol(name, s, m);
//  store->logVol = lv;

else { // Already in the registry; use it.
  physVolName = entry->second.name;
  lv = entry->second.logVol;
}

      pv=BuildExampleVolume(name);

      toAdd.push_back(pv);

    gmxUtil.positionIndex.decrementLevel();
    return;
}

#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

GeoPhysVol* MDTProcessor::BuildExampleVolume(std::string name)
{

  // Define the units
 #define gr   SYSTEM_OF_UNITS::gram
 #define mole SYSTEM_OF_UNITS::mole
 #define cm3  SYSTEM_OF_UNITS::cm3

 // Define the chemical elements

 static GeoElement*  el_Iron     = new GeoElement ("Iron"     ,"Fe" , 26.0 ,  55.847  *gr/mole);
 static GeoElement*  el_Carbon   = new GeoElement ("Carbon"   ,"C"  ,  6.0 ,  12.0107 *gr/mole);

 // Define the materials

 // Steel: Iron + Carbon
 GeoMaterial* mat_Steel  = new GeoMaterial("Steel", 7.9 *gr/cm3);
 mat_Steel->add(el_Iron  , 0.98);
 mat_Steel->add(el_Carbon, 0.02);
 mat_Steel->lock();

 // A cone
GeoCons* cons = new GeoCons(10.*SYSTEM_OF_UNITS::cm, 20.*SYSTEM_OF_UNITS::cm, 30.*SYSTEM_OF_UNITS::cm,
  40.*SYSTEM_OF_UNITS::cm, 25.*SYSTEM_OF_UNITS::cm, 0.*SYSTEM_OF_UNITS::degree, 270.*SYSTEM_OF_UNITS::degree);

  GeoLogVol* consLog = new GeoLogVol(name, cons, mat_Steel);
  GeoPhysVol* consPhys = new GeoPhysVol(consLog);

  return consPhys;

}

/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

//
//   Processor for logvol elements
//
//   Add name to list.
//   Create a physvol using my logvol; add it to list.
//   Process children and get list of things to be added to the physvol.
//   Add them to the physvol.
//
//
// Updates:
// - 2022/02, Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
//            * Removed the automatic cration of a GeoNameTag for each volume, to save memory
//            * Added support for the GeoNameTag node
//


#include "GeoModelXml/LogvolProcessor.h"
#include "OutputDirector.h"

#include <map>

#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoVolumeTagCatalog.h"
#include "GeoModelXml/GmxUtil.h"
#include "GeoModelXml/GeoNodeList.h"
#include "xercesc/util/XMLString.hpp"

// using namespace CLHEP;

using namespace std;
using namespace xercesc;

//class GeoSerialDenominator;
//class GeoSerialTransformer;
//class GeoSerialIdentifier;

std::string getNodeType(const GeoGraphNode* node) {
    if ( dynamic_cast<const GeoPhysVol*>(node) )
        return "GeoPhysVol";
    if ( dynamic_cast<const GeoFullPhysVol*>(node) )
        return "GeoFullPhysVol";
    if ( dynamic_cast<const GeoIdentifierTag*>(node) )
        return "GeoIdentifierTag";
    if ( dynamic_cast<const GeoNameTag*>(node) )
        return "GeoNameTag";
    if ( dynamic_cast<const GeoLogVol*>(node) )
        return "GeoLogVol";
    if ( dynamic_cast<const GeoShape*>(node) )
        return "GeoShape";
    if ( dynamic_cast<const GeoMaterial*>(node) )
        return "GeoMaterial";
    //if ( dynamic_cast<const GeoSerialDenominator*>(node) )
        //return "GeoSerialDenominator";
    //if ( dynamic_cast<const GeoSerialIdentifier*>(node) )
        //return "GeoSerialIdentifier";
    //if ( dynamic_cast<const GeoSerialTransformer*>(node) )
        //return "GeoSerialTransformer";
    if ( dynamic_cast<const GeoTransform*>(node) )
        return "GeoTransform";
    return "UnidentifiedNode";
}

void LogvolProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
  GeoLogVol *lv;
  GeoNameTag *nameTag_physVolName;

  gmxUtil.positionIndex.incrementLevel();

  // get the name of the LogVol
  XMLCh * name_tmp = XMLString::transcode("name");
  char *name2release = XMLString::transcode(element->getAttribute(name_tmp));
  string name(name2release);
  gmxUtil.positionIndex.addToLevelMap(name,gmxUtil.positionIndex.level());
  XMLString::release(&name2release);
  XMLString::release(&name_tmp);

  // get the value for the "named" option;
  // if "true", add a GeoNameTag to the GeoModel tree
  XMLCh * named_tmp = XMLString::transcode("named");
  char *toRelease2 = XMLString::transcode(element->getAttribute(named_tmp));
  string named(toRelease2);
  XMLString::release(&toRelease2);
  XMLString::release(&named_tmp);
  bool isNamed = bool(named.compare(string("true")) == 0);
  
  // get the value for the "identifier" option;
  // if "true", add a GeoIdentifierTag to the GeoModel tree
  XMLCh * id_tmp = XMLString::transcode("identifier");
  char *toRelease3 = XMLString::transcode(element->getAttribute(id_tmp));
  string idStr(toRelease3);
  XMLString::release(&toRelease3);
  XMLString::release(&id_tmp);
  bool hasIdentifier = bool(idStr.compare(string("true")) == 0);

  //

  XMLCh * envelope_tmp = XMLString::transcode("envelope");
  char *env = XMLString::transcode(element->getAttribute(envelope_tmp));
  string envelope(env);
  bool is_envelope=(envelope.compare(string("true"))==0);
  XMLString::release(&env);

//
//    Look for the logvol in the map; if not yet there, add it
//
  map<string, LogVolStore>::iterator entry;
  if ((entry = m_map.find(name)) == m_map.end()) { // Not in registry; make a new item
//
//    Name
//
    m_map[name] = LogVolStore();
    LogVolStore *store = &m_map[name];
    if(isNamed) {
        nameTag_physVolName = new GeoNameTag(name);
        store->name = nameTag_physVolName;
    }
    store->id = 0;
//
//    Get the shape.
//
    DOMDocument *doc = element->getOwnerDocument();
    XMLCh * shape_tmp = XMLString::transcode("shape");
    const XMLCh *shape = element->getAttribute(shape_tmp);
    DOMElement *refShape = doc->getElementById(shape);
    // Check it is a shape... its parent should be a <shapes>. DTD cannot do this for us.
    DOMNode *parent = refShape->getParentNode();
    if (XMLString::compareIString(parent->getNodeName(), XMLString::transcode("shapes")) != 0) {
        char* shape_s = XMLString::transcode (shape);
        msglog << MSG::FATAL << "Processing logvol " << name <<
            ". Error in gmx file. An IDREF for a logvol shape did not refer to a shape.\n" <<
            "Shape ref was " << shape_s << "; exiting" << endmsg;
        XMLString::release (&shape_s);
        std::abort();
    }
//
//    What sort of shape?
//
    name2release = XMLString::transcode(refShape->getNodeName());
    string shapeType(name2release);
    XMLString::release(&name2release);
    XMLString::release(&shape_tmp);

    const GeoShape *shGeo = static_cast<const GeoShape *>(gmxUtil.geoItemRegistry.find(shapeType)->process(refShape, gmxUtil));
//
//    Get the material
//
    XMLCh * material_tmp = XMLString::transcode("material");
    const XMLCh *material = element->getAttribute(material_tmp);
    DOMElement *refMaterial = doc->getElementById(material);
    // Check it is a material... its parent should be a <materials>. DTD cannot do this for us.
    parent = refMaterial->getParentNode();
    XMLCh * materials_tmp = XMLString::transcode("materials");
    if (XMLString::compareIString(parent->getNodeName(), materials_tmp) != 0) {
        char* material_s = XMLString::transcode (material);
        msglog << MSG::FATAL << "Processing logvol " << name <<
            ". Error in gmx file. An IDREF for a logvol material did not refer to a material.\n" <<
            "Material ref was " << material_s << "; exiting" << endmsg;
        XMLString::release (&material_s);
        std::abort();
    }
    std::string nam_mat=XMLString::transcode(material);

    const GeoMaterial* matGeo = nullptr;

    if (gmxUtil.matManager)
    {
        if (!gmxUtil.matManager->isMaterialDefined(nam_mat))
        {
            GeoMaterial* tempMat=static_cast<GeoMaterial *>(gmxUtil.tagHandler.material.process(refMaterial, gmxUtil));
            // we let GMX create the material and store it in the MM

            gmxUtil.matManager->addMaterial(tempMat);
        }
        matGeo = gmxUtil.matManager->getMaterial(nam_mat);
    }
    else {
        matGeo = static_cast<const GeoMaterial *>(gmxUtil.tagHandler.material.process(refMaterial, gmxUtil));
    }
//
//    Make the LogVol and add it to the map ready for next time
//
    lv = new GeoLogVol(name, shGeo, matGeo);
    store->logVol = lv;

    XMLString::release(&material_tmp);
    XMLString::release(&materials_tmp);
  }
  else { // Already in the registry; use it.
    //msglog << MSG::DEBUG << "LogVol w/ name '" << name << "' already present, picking it from cache..." << endmsg;
    if(isNamed) {
        nameTag_physVolName = entry->second.name;
    }
    lv = entry->second.logVol;
  }


//
//    Process the logvol children (side effect: sets formulae for indexes before calculating them)
//
// RMB: Note -- here the code looks for "children of the LogVol" but this is not true: they are PhysVol children. In fact, they are later added to the newly created PhysVol... needs to be clearified/updated
  GeoNodeList childrenAdd;
  for (DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling()) {
    if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
      DOMElement *el = dynamic_cast<DOMElement *> (child);
      name2release = XMLString::transcode(el->getNodeName());
      string name(name2release);
      XMLString::release(&name2release);
      //msglog << MSG::DEBUG << "Processing child: '" << name << "'..." << endmsg;
      gmxUtil.processorRegistry.find(name)->process(el, gmxUtil, childrenAdd);
    }
  }
//
//   Make a list of things to be added
//
  if(isNamed) {
      toAdd.push_back(nameTag_physVolName);
  }

  XMLCh * sensitive_tmp = XMLString::transcode("sensitive");
  bool sensitive = element->hasAttribute(sensitive_tmp);
  int sensId = 0;
  //std::vector<int> extraSensIds;//extra Identfiers to be used in case we split this volume into multiple DetectorElements
  map<string, int> index;
  //map<string, int> updatedIndex;//extra indices to be used in case we split this volume
  if (sensitive) {
    gmxUtil.positionIndex.setCopyNo(m_map[name].id++);
    gmxUtil.positionIndex.indices(index, gmxUtil.eval);
    sensId = gmxUtil.gmxInterface()->sensorId(index);
    if(hasIdentifier) { //TODO: check if all "sensitive" volumes must have an identifier. If that's the case, then we can remove this "if" here
        //        toAdd.push_back(new GeoIdentifierTag(m_map[name].id)); // Normal copy number
        toAdd.push_back(new GeoIdentifierTag(sensId));
    }
  }
  else {
      if(hasIdentifier) {
          toAdd.push_back(new GeoIdentifierTag(m_map[name].id)); // Normal copy number
          gmxUtil.positionIndex.setCopyNo(m_map[name].id++);
      }
  }
  XMLString::release(&sensitive_tmp);
  //
  //    Make a new PhysVol/FullPhysVol and add everything to it, then add it to the list of things for my caller to add
  //
  
  // get the value for the "alignable" option
  XMLCh * alignable_tmp = XMLString::transcode("alignable");
  char *toRelease = XMLString::transcode(element->getAttribute(alignable_tmp));
  string alignable(toRelease);
  XMLString::release(&toRelease);
  XMLString::release(&alignable_tmp);
  

  if (sensitive || (alignable.compare(string("true")) == 0)) {
    //msglog << MSG::DEBUG << "Handling a FullPhysVol (i.e., an 'alignable' or 'sensitive' volume) ..." << endmsg;
    GeoFullPhysVol *pv = new GeoFullPhysVol(lv);
    if (is_envelope) GeoVolumeTagCatalog::VolumeTagCatalog()->addTaggedVolume("Envelope",name,pv);
    for (GeoNodeList::iterator node = childrenAdd.begin(); node != childrenAdd.end(); ++node) {
	     pv->add(*node);
    }
    toAdd.push_back(pv); // NB: the *PV is third item added, so reference as toAdd[2].
    //
    //    Add sensitive volumes to detector manager via GmxInterface
    //
    if (sensitive) {
      XMLCh * sensitive_tmp = XMLString::transcode("sensitive");
      name2release = XMLString::transcode(element->getAttribute(sensitive_tmp));
      string sensitiveName(name2release);
      XMLString::release(&name2release);
      XMLString::release(&sensitive_tmp);
	    //splitting sensors where we would like multiple DetectorElements per GeoVFullPhysVol (e.g.ITk Strips)
      XMLCh * splitLevel_tmp = XMLString::transcode("splitLevel");
      bool split = element->hasAttribute(splitLevel_tmp);
      char* splitString;
	    int splitLevel = 1;
	    if (split) {
        splitString = XMLString::transcode(element->getAttribute(splitLevel_tmp));
        splitLevel = gmxUtil.evaluate(splitString);
        XMLString::release(&splitString);
        XMLString::release(&splitLevel_tmp);
        for(int i=0;i<splitLevel;i++){
          std::string field = "eta_module";//eventually specify in Xml the field to split in?
          std::pair<std::string,int> extraIndex(field,i);
          gmxUtil.gmxInterface()->addSplitSensor(sensitiveName, index,extraIndex, sensId, dynamic_cast<GeoVFullPhysVol *> (pv));
        }
	    }
	    else gmxUtil.gmxInterface()->addSensor(sensitiveName, index, sensId, dynamic_cast<GeoVFullPhysVol *> (pv));
    }
  }
  else {
    //msglog << MSG::DEBUG << "Handling a standard PhysVol..." << endmsg;
    GeoPhysVol *pv = new GeoPhysVol(lv);
    if (is_envelope) GeoVolumeTagCatalog::VolumeTagCatalog()->addTaggedVolume("Envelope",name,pv);
    //msglog << MSG::DEBUG << "Now, looping over all the children of the LogVol (in the GMX meaning)..." << endmsg; 
    for (GeoNodeList::iterator node = childrenAdd.begin(); node != childrenAdd.end(); ++node) {
      pv->add(*node);
      //msglog << MSG::DEBUG << "LVProc, PV child: " << *node << " -- " << getNodeType(*node) << endmsg;
    }
    //msglog << MSG::DEBUG << "End of loop over children." << endmsg;
    toAdd.push_back(pv);
  }

  gmxUtil.positionIndex.decrementLevel();
  if(gmxUtil.positionIndex.level()==-1){ //should mean that we are at the end of processing the geometry
    gmxUtil.positionIndex.printLevelMap();
  }
  return;
}

void LogvolProcessor::zeroId(const xercesc::DOMElement *element) {

  XMLCh * name_tmp = XMLString::transcode("name");
  char *name2release = XMLString::transcode(element->getAttribute(name_tmp));
  string name(name2release);
  XMLString::release(&name2release);
  XMLString::release(&name_tmp);
  //
  //    Look for the logvol in the map; if not yet there, add it
  //
  map<string, LogVolStore>::iterator entry;
  if ((entry = m_map.find(name)) != m_map.end()) {
    entry->second.id = 0;
  }
  /* else: Not an error: it is usually just about to be made with id = 0; no action needed. */
}



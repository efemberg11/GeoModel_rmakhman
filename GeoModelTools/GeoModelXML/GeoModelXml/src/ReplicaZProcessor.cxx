/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//   replicaZ element processor.
//

#include "GeoModelXml/ReplicaZProcessor.h"

#include "OutputDirector.h"
#include <sstream>
#include <string>
#include <cstdlib>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GeoNodeList.h"
#include "GeoModelXml/ProcessorRegistry.h"
#include "GeoModelXml/GmxUtil.h"


using namespace xercesc;
using namespace std;

void ReplicaZProcessor::tokenize(string &str, char delim, vector<string> &out) const
{
        size_t start;
        size_t end = 0;

        while ((start = str.find_first_not_of(delim, end)) != string::npos)
        {
                end = str.find(delim, start);
                out.push_back(str.substr(start, end - start));
        }
}


void ReplicaZProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
char *toRelease;
XMLCh *ref = XMLString::transcode("ref");
XMLCh * alignable_tmp = XMLString::transcode("alignable");
const XMLCh *idref;
DOMDocument *doc = element->getOwnerDocument();

    bool alignable = element->hasAttribute(alignable_tmp);
//
//    How many copies?
//
    int nCopies;
    XMLCh * n_tmp = XMLString::transcode("n");
    toRelease = XMLString::transcode(element->getAttribute(n_tmp));
    nCopies = gmxUtil.evaluate(toRelease);
    XMLString::release(&toRelease);
    XMLString::release(&n_tmp);
//
//   offset along Z
//
    double offsetZ=0;
    XMLCh * offset_tmp = XMLString::transcode("offset");
    toRelease = XMLString::transcode(element->getAttribute(offset_tmp));
    offsetZ = gmxUtil.evaluate(toRelease);
    XMLString::release(&toRelease);
    XMLString::release(&offset_tmp);
//
//   step along Z
//
    double stepZ=0;
    XMLCh * step_tmp = XMLString::transcode("step");
    toRelease = XMLString::transcode(element->getAttribute(step_tmp));
    stepZ = gmxUtil.evaluate(toRelease);
    XMLString::release(&toRelease);
    XMLString::release(&step_tmp);

//  
//  skip
//
    XMLCh * skip_tmp = XMLString::transcode("skip");
    std::vector<int> elementsToSkip;
    if (element->hasAttribute(skip_tmp))
    {
        toRelease = XMLString::transcode(element->getAttribute(skip_tmp));
        std::string skip_str(toRelease);
        //std::cout << "skip string "<<skip_str<<std::endl;
        std::vector<std::string> parsed;
        tokenize(skip_str,' ',parsed);
        for (auto k : parsed)
        {
                std::vector<std::string> tmp_parsed;
                //std::cout<<" parsed "<<k<<std::endl;
                tokenize(k,'-',tmp_parsed);
                if (tmp_parsed.size()==1) elementsToSkip.push_back(std::stoi(tmp_parsed[0]));
                else if (tmp_parsed.size()==2)
                {
                        int i1=std::stoi(tmp_parsed[0]);
                        int i2=std::stoi(tmp_parsed[1]);
                        //std::cout<<" indices "<<i1<<" "<<i2<<std::endl;
                        assert(i1<i2);
                        for (int l=i1;l<i2+1;l++) elementsToSkip.push_back(l);
                }

        }

    }


//
//    See if it is in the map; if so, xfList is already done. If not, fill xfList.
//
    XMLCh * name_tmp = XMLString::transcode("name");
    toRelease = XMLString::transcode(element->getAttribute(name_tmp));
    string name(toRelease);
    XMLString::release(&toRelease);
    XMLString::release(&name_tmp);
    map<string, GeoNodeList>::iterator entry;
    GeoNodeList *xfList;
    if ((entry = m_map.find(name)) == m_map.end()) { // Not in registry; make a new item
//
//    Add empty node list to the map
//
        m_map[name] = GeoNodeList();
        xfList = &m_map[name];


//
//    Produce all the transformations
//
        GeoAlignableTransform *geoAXf;
        GeoTransform *geoXf;

//
//    If varname not given, we get the CLHEP xForm and raise it to the power i, so NOT applied to first object.
//    No transform (i.e. identity) for the first; so one less transform than objects
//
	  GeoTrf::Transform3D hepXf0=GeoTrf::TranslateZ3D(offsetZ);
            if (alignable) {
                geoAXf = new GeoAlignableTransform (hepXf0) ;
                hepXf0 = geoAXf->getTransform();
            }
            else {
                geoXf = new GeoTransform (hepXf0);
                hepXf0 = geoXf->getTransform();
            }
            GeoTrf::Transform3D hepXf=hepXf0; 
            for (int i = 0; i < nCopies; ++i) {
                xfList->push_back((GeoGraphNode *) new GeoTransform(hepXf));
                hepXf = hepXf * GeoTrf::TranslateZ3D(stepZ) ;
            }
    }
    else {
        xfList = &entry->second;
    }
//
//    Get object to be copied
//
    DOMElement *object = element->getLastElementChild();
    toRelease = XMLString::transcode(object->getNodeName());
    string nodeName(toRelease);
    XMLString::release(&toRelease);
    ElementProcessor *objectProcessor = gmxUtil.processorRegistry.find(nodeName);
//
//    Zero its copy number. Only needed if an item is used in 2 or more multicopies;
//    harmless in case of only one use.
//
    if (nodeName == "logvolref") {
        idref = object->getAttribute(ref);
        DOMElement *elem = doc->getElementById(idref);
        gmxUtil.tagHandler.logvol.zeroId(elem);
    }
    else if (nodeName == "assemblyref") {
        idref = object->getAttribute(ref);
        DOMElement *elem = doc->getElementById(idref);
        gmxUtil.tagHandler.assembly.zeroId(elem);
    }
    else if (nodeName == "transform") { // Object is either an assemlyref or logvolref, and there is only one of these
      XMLCh * logvolref_tmp = XMLString::transcode("logvolref");
      DOMNodeList *lvList = element->getElementsByTagName(logvolref_tmp);
      if (lvList->getLength() > 0) {
	const XMLCh *idref =  dynamic_cast<DOMElement *> (lvList->item(0))->getAttribute(ref);
	DOMElement *lv = doc->getElementById(idref);
	gmxUtil.tagHandler.logvol.zeroId(lv);
	XMLString::release(&logvolref_tmp);
      }
      else {
	XMLCh * assemblyref_tmp = XMLString::transcode("assemblyref") ;
	DOMNodeList *asList = element->getElementsByTagName(assemblyref_tmp);
	if (asList->getLength() > 0) {
	  const XMLCh *idref =  dynamic_cast<DOMElement *> (asList->item(0))->getAttribute(ref);
	  DOMDocument *doc = element->getOwnerDocument();
	  DOMElement *as = doc->getElementById(idref);
	  gmxUtil.tagHandler.assembly.zeroId(as);
	  XMLString::release(&assemblyref_tmp);
	}
	else {
	  msglog << MSG::FATAL << 
	    "ReplicaYProcessor: error in " << name << ". <transform> object was neither assemblyref nor logvolref\n"
                    << "Exiting " << endmsg;
	  exit(999); // Should do better
	}
      }
    }
//
//    If alignable, add transformation to DetectorManager via GmxInterface.
//    (NB. Alignable is messy because it involves both a transformation and an object as well as
//    the multicopy or transform element to tell us when to insert it and what level of alignment 
//    is wanted; and passing info from one routine to another when you try to keep a uniform interface is 
//    difficult; and we only have partial GeoModel trees so we cannot use GeoModel tree traversal at this 
//    moment (comes back to no way of knowing if we have a GeoFullPV or GeoPV)).
//
//    Also: no one is using it at the time of writing, so it is ***TOTALLY*** untested.
//    It is done now to (i) make sure there ought to be a solution (ii) implement (imperfectly) 
//    a way now while things are fresh in my mind.
//
    int level;
    if (alignable) {
        istringstream(XMLString::transcode(element->getAttribute(alignable_tmp))) >> level;
    }
//
//    Add transforms and physvols etc. to list to be added
//
    map<string, int> index;
    for (int copy = 0; copy < nCopies; ++copy) {
        if (elementsToSkip.size()>0 && std::find(elementsToSkip.begin(),elementsToSkip.end(),copy)!=elementsToSkip.end()) continue;
        toAdd.push_back((*xfList)[copy]);
        int lastTransform = toAdd.size() - 1;
        objectProcessor->process(object, gmxUtil, toAdd);
        if (alignable) {

            cout << "copy = " << copy << "; level = " << level << endl;
            cout << "\nAdd Alignable named " << endl;
            cout << ((GeoNameTag *) (toAdd[lastTransform + 1]))->getName() << endl;
            cout << " with id " << endl;
            cout << ((GeoIdentifierTag *) (toAdd[lastTransform + 2]))->getIdentifier() << endl;

            gmxUtil.positionIndex.incrementLevel(); // Logvol has unfortunately already decremented this; temp. restore it
            gmxUtil.positionIndex.indices(index, gmxUtil.eval);
            gmxUtil.gmxInterface()->addAlignable(level, index, (GeoVFullPhysVol *) toAdd[lastTransform + 3],
                                                 (GeoAlignableTransform *) toAdd[lastTransform]);
            gmxUtil.positionIndex.decrementLevel(); // Put it back where it was
            index.clear();
        }
    }

    XMLString::release(&ref);
    XMLString::release(&alignable_tmp);

}

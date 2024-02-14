/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

//
//   Processor for transform elements
//
//   Decide if I need alignable transforms from my second child
//   Add transform from first child to list
//   Create physvol using my second child; add it to list.
//
//   Updates:
//   - 2022 Apr, Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
//               Commented call to addAlignable
//
#include "GeoModelXml/TransformProcessor.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelXml/GmxUtil.h"
#include "xercesc/util/XMLString.hpp"


//  using namespace CLHEP;
using namespace std;
using namespace xercesc;

void TransformProcessor::process(const DOMElement *element, GmxUtil &gmxUtil, GeoNodeList &toAdd) {
char *tagName;
 XMLCh * alignable_tmp;

 alignable_tmp = XMLString::transcode("alignable");
 bool alignable = element->hasAttribute(alignable_tmp);
 //
 //    Do second element first, to find what sort of transform is needed (shape or logvol etc.?)
 //
 GeoNodeList objectsToAdd;
 DOMElement *object = element->getLastElementChild();
 tagName = XMLString::transcode(object->getTagName());
 string objectName(tagName);
 gmxUtil.processorRegistry.find(objectName)->process(object, gmxUtil, objectsToAdd);
 XMLString::release(&tagName);
 XMLString::release(&alignable_tmp);
 //
 //    Get the transformation
 //
 DOMElement *transformation = element->getFirstElementChild();
 tagName = XMLString::transcode(transformation->getTagName()); // transformation or transformationref
 
 // TODO:  ******* Should check here that an alignable transform is given an alignable transformation and object; to be done
 
 toAdd.push_back((GeoGraphNode *)(gmxUtil.geoItemRegistry.find(string(tagName))->process(transformation, gmxUtil)));
 XMLString::release(&tagName);
 //
 //    Add transformation to DetectorManager via GmxInterface, if it is alignable
//
    if (alignable) { 
        int level;
        alignable_tmp = XMLString::transcode("alignable");
        istringstream(XMLString::transcode(element->getAttribute(alignable_tmp))) >> level;
        map<string, int> index;
        gmxUtil.positionIndex.incrementLevel(); // Logvol has unfortunately already decremented this; temp. restore it
        gmxUtil.positionIndex.indices(index, gmxUtil.eval);

        //Checking all objects to find right one not so efficient - Define const int in LogvolProcessor?
        //sanity check... see if we find a FullPhysVol somewhere in the object list...
        GeoVFullPhysVol * fpv;
        for(GeoGraphNode * iFpv:objectsToAdd){ //maybe this needs to be in reverse???
        fpv = dynamic_cast<GeoVFullPhysVol*> (iFpv);
        if (fpv) break; //if we find it, use it...
        }

        //now check that the AlignableTransform is actually valid!
        GeoAlignableTransform * gat = dynamic_cast<GeoAlignableTransform *>(toAdd.back());

        if(fpv && gat) {
            //splitting sensors where we would like multiple DetectorElements per GeoVFullPhysVol (e.g.ITk Strips)
            XMLCh * splitLevel_tmp = XMLString::transcode("splitLevel");
            bool split = element->hasAttribute(splitLevel_tmp);
            char* splitString;
	        int splitLevel = 1;
	        if (split) {
                splitString = XMLString::transcode(element->getAttribute(splitLevel_tmp));
                splitLevel = gmxUtil.evaluate(splitString);
                XMLString::release(&splitString);
                for(int i=0;i<splitLevel;i++){
                    std::string field = "eta_module";//eventually specify in Xml the field to split in?
                    std::pair<std::string,int> extraIndex(field,i);
                    gmxUtil.gmxInterface()->addSplitAlignable(level, index, extraIndex,fpv,gat);
                }
            }
            else gmxUtil.gmxInterface()->addAlignable(level, index,fpv,gat);
            XMLString::release(&splitLevel_tmp);
        }    
        else{
             std::cout<<"WARNING:";
             if(!gat) std::cout<<" No valid AlignableTransform";
             if(!fpv) std::cout<<" No valid FullPhysicalVolume";
             std::cout<<" found for ";
            for (map<string, int>::iterator i = index.begin(); i != index.end(); ++i) {
            std::cout << i->second << "   ";
            }
            std::cout<<std::endl;
        }

        gmxUtil.positionIndex.decrementLevel(); 
	XMLString::release(&alignable_tmp);
    }
//
//    And add the name and physvol etc. after the transformation
//
    toAdd.insert(toAdd.end(), objectsToAdd.begin(), objectsToAdd.end());

    return;
}

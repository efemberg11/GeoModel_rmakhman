/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MakeMaterial.h"
#include "OutputDirector.h"
#include <string>
#include <xercesc/dom/DOM.hpp>
#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "xercesc/util/XMLString.hpp"
#include "GeoModelXml/GmxUtil.h"
#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/throwExcept.h"

using namespace xercesc;
using namespace std;
using namespace GeoModelKernelUnits;

MakeMaterial::MakeMaterial() {}

GeoIntrusivePtr<RCBase>MakeMaterial::make(const xercesc::DOMElement *element, GmxUtil &gmxUtil) const {
    char *name;
    char *density;
    char *densitySF;
    double rho{0.};
    char *fracString;
    double fraction{0.};
    char *qString;
    XMLCh *ref = XMLString::transcode("ref");
    XMLCh *materials_tmp = XMLString::transcode("materials");
    XMLCh *density_tmp = XMLString::transcode("density");
    XMLCh *densitysf_tmp = XMLString::transcode("densitysf");
    XMLCh *name_tmp = XMLString::transcode("name");
    XMLCh* elementref_tmp = XMLString::transcode("elementref");
    XMLCh *fraction_tmp = XMLString::transcode("fraction");
    XMLCh *chemicalref_tmp = XMLString::transcode("chemicalref");
    XMLCh *elemcontent_tmp = XMLString::transcode("elemcontent");
    XMLCh *quantity_tmp = XMLString::transcode("quantity");
    XMLCh *materialref_tmp = XMLString::transcode("materialref");
    const XMLCh *idref;
    DOMDocument *doc = element->getOwnerDocument();
    char *toRelease;

    //
    //    Get material density scale-factor for the block of materials this one is in
    //
    DOMNode *parent = element->getParentNode();
    if (XMLString::compareIString(parent->getNodeName(), materials_tmp) != 0) {
       THROW_EXCEPTION("Asked to make a material for non-material element. Parent element was " <<
                             XMLString::transcode(parent->getNodeName()) << "; error in gmx file; exiting");
    }
    double scaleFactor(1.0);
    DOMElement *el = dynamic_cast<DOMElement *> (parent);
    densitySF = XMLString::transcode(el->getAttribute(densitysf_tmp));
    scaleFactor = gmxUtil.evaluate(densitySF);
    //
    //    Get my name
    //
    name = XMLString::transcode(element->getAttribute(name_tmp));
    //
    //   Get my density
    //
    density = XMLString::transcode(element->getAttribute(density_tmp));
    rho = gmxUtil.evaluate(density) * scaleFactor;
    XMLString::release(&density);
    //
    //    Create it
    //
    //std::cout<<" MakeMaterial: creating material "<<name<<" "<<rho<<std::endl;
    GeoIntrusivePtr<GeoMaterial> material=  make_intrusive<GeoMaterial>(name, rho * g/cm3);

    XMLString::release(&name);
    //
    //   Add my element contents
    //
    DOMNodeList *elRefs = element->getElementsByTagName(elementref_tmp);
    int nElRefs = elRefs->getLength();
    for (int i = 0; i < nElRefs; ++i) {
        DOMElement *elRef = dynamic_cast<DOMElement *>(elRefs->item(i));
        idref = elRef->getAttribute(ref);
        DOMElement *elem = doc->getElementById(idref);

        toRelease = XMLString::transcode(elem->getNodeName());
        string nodeName(toRelease);
        XMLString::release(&toRelease);
        if (nodeName != "element") {
            THROW_EXCEPTION("Error in xml/gmx file: An elementref referenced a " << nodeName << " instead of an element.");
        }

        GeoIntrusivePtr<const GeoElement> geoElem{};
        if (gmxUtil.matManager) {
            name=XMLString::transcode(idref);
            if (!gmxUtil.matManager->isElementDefined(name)) {
                gmxUtil.tagHandler.element.process(elem, gmxUtil);
            }
            geoElem=gmxUtil.matManager->getElement(name);
            XMLString::release(&name);
        } else {
            geoElem= dynamic_pointer_cast<GeoElement>( gmxUtil.tagHandler.element.process(elem, gmxUtil));
            if (!geoElem) THROW_EXCEPTION("could not retrieve element!!!!!");
        }


        fracString = XMLString::transcode(elRef->getAttribute(fraction_tmp));
        fraction = gmxUtil.evaluate(fracString);
        XMLString::release(&fracString);
        material->add(geoElem, fraction);  
    }
    //
    //   Add my chemical contents
    //
    DOMNodeList *chemRefs = element->getElementsByTagName(chemicalref_tmp);
    int nChemRefs = chemRefs->getLength();
    
    for (int i = 0; i < nChemRefs; ++i) {
        DOMElement *chemRef = dynamic_cast<DOMElement *>(chemRefs->item(i));
        idref = chemRef->getAttribute(ref);
        DOMElement *chem = doc->getElementById(idref);

        toRelease = XMLString::transcode(chem->getNodeName());
        string nodeName(toRelease);
        XMLString::release(&toRelease);
        if (nodeName != string("chemical")) {
            THROW_EXCEPTION("Error in xml/gmx file: A chemref referenced a " << nodeName << " instead of a chemical.");
        }

        fracString = XMLString::transcode(chemRef->getAttribute(fraction_tmp));
        fraction = gmxUtil.evaluate(fracString);
        XMLString::release(&fracString);
        // Loop over chemical contents, adding each element to this material
        DOMNodeList *chemEls = chem->getElementsByTagName(elemcontent_tmp);
        int nChemEls = chemEls->getLength();
        vector<GeoIntrusivePtr<const GeoElement>> geoElem{};
        vector<double> atomicWeight{}, formula{};
        double molWeight = 0.0;
        for (int i = 0; i < nChemEls; ++i) {
            DOMElement *chemEl = dynamic_cast<DOMElement *>(chemEls->item(i));
            idref = chemEl->getAttribute(ref);
            DOMElement *elem = doc->getElementById(idref);

            toRelease = XMLString::transcode(elem->getNodeName());
            string nodeName(toRelease);
            XMLString::release(&toRelease);
            if (nodeName != "element") {
                THROW_EXCEPTION("Error in xml/gmx file: An elementref referenced a " << nodeName 
                                << " instead of an element.");
            }

            if (gmxUtil.matManager) {
                name=XMLString::transcode(idref);
                geoElem.push_back(gmxUtil.matManager->getElement(name));
                XMLString::release(&name);
            } else {
                geoElem.push_back(dynamic_pointer_cast< GeoElement>(gmxUtil.tagHandler.element.process(elem, gmxUtil)));
            }

            atomicWeight.push_back(geoElem.back()->getA());

            qString = XMLString::transcode(chemEl->getAttribute(quantity_tmp));
            formula.push_back(gmxUtil.evaluate(qString));
            XMLString::release(&qString);

            molWeight += atomicWeight.back() * formula.back();
        }
        for (int i = 0; i < nChemEls; ++i) {
            material->add(geoElem[i], fraction * formula[i] * atomicWeight[i] / molWeight);
        }
    }

    //
    //   Add my material contents
    //
    elRefs = element->getElementsByTagName(materialref_tmp);
    nElRefs = elRefs->getLength();
    
    
    for (int i = 0; i < nElRefs; ++i) {
        DOMElement *elRef = dynamic_cast<DOMElement *>(elRefs->item(i));
        idref = elRef->getAttribute(ref);
        DOMElement *elem = doc->getElementById(idref);

        toRelease = XMLString::transcode(elem->getNodeName());
        string nodeName(toRelease);
    
    
        XMLString::release(&toRelease);
        if (nodeName != "material") {
            THROW_EXCEPTION("Error in xml/gmx file: A materialref referenced a " << nodeName << " instead of a material.");
        }

        GeoIntrusivePtr<const GeoMaterial>geoMaterial{};
        if (gmxUtil.matManager) {
            name=XMLString::transcode(idref);
            geoMaterial=gmxUtil.matManager->getMaterial(name);
            XMLString::release(&name);
        } else {        
            geoMaterial= dynamic_pointer_cast<GeoMaterial>( gmxUtil.tagHandler.material.process(elem, gmxUtil));
            if (!geoMaterial) THROW_EXCEPTION("something is wrong!!!!");
        }
        fracString = XMLString::transcode(elRef->getAttribute(fraction_tmp));
        fraction = gmxUtil.evaluate(fracString);
        XMLString::release(&fracString);
        material->add(geoMaterial, fraction);
    }

    material->lock(); // Calculate my params and prevent modification

    XMLString::release(&ref);
    XMLString::release(&materials_tmp);
    XMLString::release(&density_tmp);
    XMLString::release(&densitysf_tmp);
    XMLString::release(&name_tmp);
    XMLString::release(&elementref_tmp);
    XMLString::release(&fraction_tmp);
    XMLString::release(&chemicalref_tmp);
    XMLString::release(&elemcontent_tmp);
    XMLString::release(&quantity_tmp);
    XMLString::release(&materialref_tmp);
    return material;
}

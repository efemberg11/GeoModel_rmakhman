
#ifndef physvolHandler_H
#define physvolHandler_H

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"

class physvolHandler:public GDMLHandler {
public:

	physvolHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		thePhysicalVolume=0;
		
		name=getAttributeAsString("name");
		//std::cout<<" this is physvolHandler: name "<<name<<std::endl;

		StopLoop(true);
		xercesc::DOMNode *child;
		
		//GeoTrf::TransformRT pVTransform=GeoTrf::Transform3D::Identity();
		GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
		GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);

		for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
		{
			if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
				XercesParser::elementLoop(child);
				XMLHandler *h=theController->XMLStore()->GetHandler(child);
                if(h){
                    std::string nH=h->GetName();
                    //std::cout<<" handler name "<<nH<<std::endl;
                    if (nH=="volumeref") {
                        volumerefHandler* vH=dynamic_cast<volumerefHandler*>(h);
                        if (!vH) std::cout<<" something is wrong! can not retrieve volumerefHandler!!!"<<std::endl;
                        else thePhysicalVolume=(vH->getVolume()).second;
                    }
                    else if (nH=="positionref" || nH=="position") {
                        positionHandler* pH=dynamic_cast<positionHandler*>(h);
                        if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                        else {
                            pVTranslation=pH->getPosition().v3d;
                        }
					
                    }
                    else if (nH=="rotationref" || nH=="rotation" ) {
                        rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                        if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                        else {
                            rotation r=rH->getRotation();
                            pVRotation=GeoTrf::RotateX3D(-r.rotx)*GeoTrf::RotateY3D(-r.roty)*GeoTrf::RotateZ3D(-r.rotz);
                        }
                    }
                    else std::cout<<" Name of the handler not defined "<<nH<<std::endl;
                }
                else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
                
            }
			
		}
		trf=new GeoTransform(GeoTrf::Translate3D(pVTranslation.x(),pVTranslation.y(),pVTranslation.z())*pVRotation);

	}
	GeoPhysVol* getPhysicalVolume() {return thePhysicalVolume;}
	GeoTransform* getTransform() {return trf;}
	void postLoopHandling()
	{
	}
private:
	std::string name;
	GeoPhysVol* thePhysicalVolume=0;
	GeoTransform* trf=0;
};


#endif /* end of include guard:  */

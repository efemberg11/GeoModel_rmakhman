#include "GDMLInterface/physvolHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"
#include "GDMLInterface/scaleHandler.h"

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"

physvolHandler::physvolHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void physvolHandler::ElementHandle() {
        
    //NB the attribute 'name' is not mandatory
    //better to use the getAttributeAsString with the boolean option
    	bool isPresent;
	name=getAttributeAsString("name", isPresent);
	std::cout<<" this is physvolHandler: name "<<name<<std::endl;

	StopLoop(true);
	xercesc::DOMNode *child;
        
//	GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
//	GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);
    
	GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
	GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);
	GeoTrf::Transform3D pVScale=GeoTrf::Scale3D::Identity();
	
	GeoLogVol* theLogicalVolume;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) 
		{
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
            		if(h)
			{
                		std::string nH=h->GetName();
                		std::cout<<" handler name "<<nH<<std::endl;
                		if (nH=="volumeref") 
				{
                    			volumerefHandler* vH=dynamic_cast<volumerefHandler*>(h);
                    			if (!vH) std::cout<<" something is wrong! can not retrieve volumerefHandler!!!"<<std::endl;
                     			else 
		     			{
		     				theLogicalVolume=(vH->getVolume()).first;
						std::cout<<" blablabla "<<theLogicalVolume->getName()<<std::endl;
						thePhysicalVolume=(vH->getVolume()).second;
		     			}
		     			std::cout<<" physvol "<<thePhysicalVolume<<std::endl;
                		}
                		else if (nH=="positionref" || nH=="position")
                		{
                    			positionHandler* pH=dynamic_cast<positionHandler*>(h);
                    			if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                    			else
                    			{
                        			pVTranslation=pH->getPosition().v3d;
                    			}
                		}
                		else if (nH=="rotationref" || nH=="rotation" )
				{
                    			rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                    			if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                    			else
					{
                        				rotation r=rH->getRotation();
                        				pVRotation=GeoTrf::RotateX3D(-r.rotx)*GeoTrf::RotateY3D(-r.roty)*GeoTrf::RotateZ3D(-r.rotz);
                    			}
                		}
                		else if (nH=="scale") 
				{
                    			scaleHandler* rH=dynamic_cast<scaleHandler*>(h);
                    			if (!rH) std::cout<<" something is wrong! can not retrieve scaleHandler!!!"<<std::endl;
                    			else 
					{
                        			scale r=rH->getScale();
                        			pVScale=GeoTrf::Scale3D(r.scalex, r.scaley, r.scalez);
                    			}
                		}
                		else std::cout<<" Name of the handler not defined "<<nH<<std::endl;
            		}
            		else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
        	}
	}
	
    	trf=new GeoTransform(GeoTrf::Translate3D(pVTranslation.x(),pVTranslation.y(),pVTranslation.z())*pVRotation*pVScale);
	theMotherVolume->add(thePhysicalVolume);
    	theMotherVolume->add(trf);
}
GeoPhysVol* physvolHandler::getPhysicalVolume() 
{
	return thePhysicalVolume;
}
GeoTransform* physvolHandler::getTransform() 
{
	return trf;
}

#include "GDMLInterface/subtractionHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLController.h"

#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/booleanHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"

#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeShift.h"


subtractionHandler::subtractionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
void subtractionHandler::ElementHandle() 
{
		
	std::string name=getAttributeAsString("name");
	//std::cout<<" this is subtractionHandler: name "<<name<<std::endl;

	StopLoop(true);
	xercesc::DOMNode *child;
		
	bool firstTrf=false;
	GeoTrf::Transform3D firstRotation=GeoTrf::Transform3D::Identity();
	GeoTrf::Vector3D firstTranslation=GeoTrf::Vector3D(0,0,0);
	bool secondTrf=false;
	GeoTrf::Transform3D secondRotation=GeoTrf::Transform3D::Identity();
	GeoTrf::Vector3D secondTranslation=GeoTrf::Vector3D(0,0,0);
	GeoShape* firstSolid;
	GeoShape* secondSolid;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
                	if(h)
			{
                    		std::string nH=h->GetName();
                    		//std::cout<<" handler name "<<nH<<std::endl;
                    		if (nH=="first") 
				{
                        		booleanHandler* fH=dynamic_cast<booleanHandler*>(h);
                        		if (!fH) std::cout<<" something is wrong! can not retrieve firstHandler!!!"<<std::endl;
                        		else firstSolid=(fH->getSolid());
                        		//if (firstSolid) std::cout<<" retrieved firstSolid! "<<std::endl;
                    		}
                    		else if (nH=="second") 
				{
                    	    		booleanHandler* sH=dynamic_cast<booleanHandler*>(h);
                        		if (!sH) std::cout<<" something is wrong! can not retrieve secondHandler!!!"<<std::endl;
                        		else secondSolid=(sH->getSolid());
                        		//if (secondSolid) std::cout<<" retrieved secondSolid! "<<std::endl;
                    		}
                    		else if (nH=="positionref" || nH=="position") 
				{
                        		positionHandler* pH=dynamic_cast<positionHandler*>(h);
                        		if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                        		else 
					{
                            			//std::cout<<"-----> "<<pH->getPosition().y<<std::endl;
                            			secondTranslation=pH->getPosition().v3d;
                        		}
                        		secondTrf=true;
					
                    		}
                    		else if (nH=="rotationref" || nH=="rotation" ) 
				{
                        		rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                        		if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                        		else 
					{
                            			rotation r=rH->getRotation();
                            			secondRotation=GeoTrf::RotateX3D(r.rotx)*GeoTrf::RotateY3D(r.roty)*GeoTrf::RotateZ3D(r.rotz);
                        		}
                        		secondTrf=true;
                    		}
                    		else if (nH=="firstpositionref" || nH=="firstposition") 
				{
                        		positionHandler* pH=dynamic_cast<positionHandler*>(h);
                        		if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                        		else 
					{
                            			firstTranslation=pH->getPosition().v3d;
                        		}
                        		firstTrf=true;
                    		}
                    		else if (nH=="firstrotationref" || nH=="firstrotation" ) 
				{
                        		rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                        		if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                        		else 
					{
                            			rotation r=rH->getRotation();
                            			firstRotation=GeoTrf::RotateX3D(r.rotx)*GeoTrf::RotateY3D(r.roty)*GeoTrf::RotateZ3D(r.rotz);
                        		}
                        		firstTrf=true;
                    		}
                    		else std::cout<<" Name of the handler not defined "<<nH<<std::endl;
                	}
                	else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
                
		}
			
	}
		
		
	if (firstTrf) 
	{
		GeoTrf::Transform3D firstTrf=GeoTrf::Translate3D(firstTranslation.x(),firstTranslation.y(),firstTranslation.z())*firstRotation;
		firstSolid= new GeoShapeShift(firstSolid,firstTrf);
	}
	if (secondTrf) 
	{
		GeoTrf::Transform3D secondTrf=GeoTrf::Translate3D(secondTranslation.x(),secondTranslation.y(),secondTranslation.z())*secondRotation;
		secondSolid=new GeoShapeShift(secondSolid,secondTrf);
	}
	GeoShapeSubtraction* subtractionSolid=new GeoShapeSubtraction(firstSolid,secondSolid);
	theController->saveSolid(name,subtractionSolid);

}

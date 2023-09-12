#include "GDMLInterface/intersectionHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"
#include "GDMLInterface/booleanHandler.h"
#include "GDMLInterface/GDMLController.h"

#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"

intersectionHandler::intersectionHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}
	
void intersectionHandler::ElementHandle()
{
	std::string name=getAttributeAsString("name");
	//std::cout<<" this is physvolHandler: name "<<name<<std::endl;

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

	for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
			GeoModelTools::XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
                	if(h){
                    		std::string nH=h->GetName();
                    		//std::cout<<" handler name "<<nH<<std::endl;
                    		if (nH=="first") {
                        	booleanHandler* fH=dynamic_cast<booleanHandler*>(h);
                        	if (!fH) std::cout<<" something is wrong! can not retrieve firstHandler!!!"<<std::endl;
                        	else firstSolid=(fH->getSolid());
                    		}
                    		else if (nH=="second") {
                        		booleanHandler* sH=dynamic_cast<booleanHandler*>(h);
                        		if (!sH) std::cout<<" something is wrong! can not retrieve secondHandler!!!"<<std::endl;
                        		else secondSolid=(sH->getSolid());
                    		}
                    		else if (nH=="positionref" || nH=="position") {
                        		positionHandler* pH=dynamic_cast<positionHandler*>(h);
                        		if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                        		else {
                            			secondTranslation=pH->getPosition().v3d;
                        		}
                        		secondTrf=true;
					
                    		}
                    		else if (nH=="rotationref" || nH=="rotation" ) {
                        		rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                        		if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                        		else {
                            			rotation r=rH->getRotation();
                            			secondRotation=GeoTrf::RotateX3D(r.rotx)*GeoTrf::RotateY3D(r.roty)*GeoTrf::RotateZ3D(r.rotz);
                            
                        		}
                        		secondTrf=true;
                        
                    		}
                    		else if (nH=="firstpositionref" || nH=="firstposition") {
                        		positionHandler* pH=dynamic_cast<positionHandler*>(h);
                        		if (!pH) std::cout<<" something is wrong! can not retrieve positionHandler!!!"<<std::endl;
                        		else {
                            			firstTranslation=pH->getPosition().v3d;
                        		}
                        		firstTrf=true;
                    		}
                    		else if (nH=="firstrotationref" || nH=="firstrotation" ) {
                        		rotationHandler* rH=dynamic_cast<rotationHandler*>(h);
                        		if (!rH) std::cout<<" something is wrong! can not retrieve rotationHandler!!!"<<std::endl;
                        		else {
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
		
	GeoShapeIntersection* intersectionSolid=new GeoShapeIntersection(firstSolid,secondSolid);
	theController->saveSolid(name,intersectionSolid);

}

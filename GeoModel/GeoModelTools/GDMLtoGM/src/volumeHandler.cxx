#include "GDMLInterface/volumeHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/replicaHandler.h"

#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoSerialTransformer.h"


volumeHandler::volumeHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
	new replicaHandler("replicavol",c);
}

void volumeHandler::ElementHandle() 
{
	material=nullptr;
	shape=nullptr;
	name=getAttributeAsString("name");
	//name=stripPointer(getAttributeAsString("name"));
	//std::cout<<" this is volumeHandler: name "<<name<<std::endl;

	StopLoop(true);
	xercesc::DOMNode *child;
	GeoPhysVol* tempPhys=0;
	GeoGraphNode* tempTransform=0;
	
	GeoLogVol* tempLV=0;

	for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
			// XercesParser::elementLoop(child);
			GeoModelTools::XMLHandler *h=theController->XMLStore()->GetHandler(child);
            		if (h){
	              		std::string nH=h->GetName();
                		//std::cout<<" handler name "<<nH<<std::endl;
				
                		if (nH=="materialref") {
					GeoModelTools::XercesParser::elementLoop(child);
                    			materialrefHandler* mH=dynamic_cast<materialrefHandler*>(h);
                    			if (!mH) std::cout<<" something is wrong! can not retrieve materialrefHandler!!!"<<std::endl;
                    			else material=mH->getMaterial();
                		}
                		else if (nH=="solidref") {
					GeoModelTools::XercesParser::elementLoop(child);
                    			solidrefHandler* sH=dynamic_cast<solidrefHandler*>(h);
                    			if (!sH) std::cout<<" something is wrong! can not retrieve solidrefHandler!!!"<<std::endl;
                    			else shape=sH->getSolid();
                		}
				else if (nH=="physvol") {
                    			physvolHandler* pH=dynamic_cast<physvolHandler*>(h);
                    			if (!pH) std::cout<<" something is wrong! can not retrieve solidrefHandler!!!"<<std::endl;
                    			else
                    			{
						GeoPhysVol* tempPV;
						if (!tempLV) 
						{
							//std::cout<<" volumeHandler new volume "<<name<<std::endl;
							// std::cout<<" replicaHandler: tempLV is 0!!!!!!"<<std::endl;
							tempLV=new GeoLogVol(name,shape,material);
							tempPV=new GeoPhysVol(tempLV);
							
							theVolume=std::make_pair(tempLV,tempPV);
							theController->saveLogicalVolume(name,theVolume);
						}
						pH->setMotherVolume(tempPV);
                    			}
					GeoModelTools::XercesParser::elementLoop(child);
                		}
				else if (nH=="replicavol") {
					replicaHandler* rH=dynamic_cast<replicaHandler*>(h);
					if (!rH) std::cout<<" something is wrong! can not retrieve replicaHandler!!!"<<std::endl;
					else
					{
						GeoPhysVol* tempPV;
						if (!tempLV) 
						{
							tempLV=new GeoLogVol(name,shape,material);
							tempPV=new GeoPhysVol(tempLV);
							theVolume=std::make_pair(tempLV,tempPV);
							theController->saveLogicalVolume(name,theVolume);
						}
						rH->setMotherVolume(tempPV);
					}
					GeoModelTools::XercesParser::elementLoop(child);
				}
            		}
            
        	} //else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
	}
	if (!tempLV) 
	{
		//std::cout<< " creating new GeoLogVol "<<name<<" ";
		tempLV=new GeoLogVol(name,shape,material);
		
		GeoPhysVol* tempPV=new GeoPhysVol(tempLV);
		
		theVolume=std::make_pair(tempLV,tempPV);
		theController->saveLogicalVolume(name,theVolume);
	}

}
void volumeHandler::postLoopHandling()
{
}

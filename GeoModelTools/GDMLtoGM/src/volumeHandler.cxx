#include "GDMLInterface/volumeHandler.h"

volumeHandler::volumeHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
}

void volumeHandler::ElementHandle() 
{
	theVolume=std::make_pair(nullptr,nullptr);
	material=nullptr;
	shape=nullptr;
	name=getAttributeAsString("name");
	//name=stripPointer(getAttributeAsString("name"));
	//std::cout<<" this is volumeHandler: name "<<name<<std::endl;

	StopLoop(true);
	xercesc::DOMNode *child;
	std::vector<GeoPhysVol*> tempPhys;
	std::vector<GeoTransform*> tempTransform;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) {
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
            if (h){
                std::string nH=h->GetName();
                //std::cout<<" handler name "<<nH<<std::endl;
                if (nH=="materialref") {
                    materialrefHandler* mH=dynamic_cast<materialrefHandler*>(h);
                    if (!mH) std::cout<<" something is wrong! can not retrieve materialrefHandler!!!"<<std::endl;
                    else material=mH->getMaterial();
                }
                else if (nH=="solidref") {
                    solidrefHandler* sH=dynamic_cast<solidrefHandler*>(h);
                    if (!sH) std::cout<<" something is wrong! can not retrieve solidrefHandler!!!"<<std::endl;
                    else shape=sH->getSolid();
                }
                else if (nH=="physvol") {
                    physvolHandler* pH=dynamic_cast<physvolHandler*>(h);
                    if (!pH) std::cout<<" something is wrong! can not retrieve solidrefHandler!!!"<<std::endl;
                    else
                    {
                        tempPhys.push_back(pH->getPhysicalVolume());
                        tempTransform.push_back(pH->getTransform());
                    }
                }
                else std::cout<<" handler name not defined "<<nH<<std::endl;
            }
            
        } //else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;
	}
	//std::cout << "Creating logical volume "<<name<<" shape "<<shape<<" mat "<<material<<std::endl;
	GeoLogVol* tempLV=new GeoLogVol(name,shape,material);
	GeoPhysVol* tempPV=new GeoPhysVol(tempLV);
	theVolume=std::make_pair(tempLV,tempPV);
	if (tempPhys.size()!=0)
	{
		for (int i=0;i<tempPhys.size();i++)
		{
			GeoPhysVol* pV=tempPhys[i];
			if (tempTransform[i]) 
			{	
				theVolume.second->add(tempTransform[i]);
			}
			theVolume.second->add(pV);
		}
	}
	theController->saveLogicalVolume(name,theVolume);
}
void volumeHandler::postLoopHandling()
{
}

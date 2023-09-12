#include "GDMLInterface/replicate_axisHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/directionHandler.h"
#include "GDMLInterface/widthHandler.h"
#include "GDMLInterface/offsetHandler.h"

replicate_axisHandler::replicate_axisHandler(std::string n, GDMLController* c): GDMLHandler(n,c) 
{
	new widthHandler("width",c);
	new offsetHandler("offset",c);
	new directionHandler("direction",c);
}
void replicate_axisHandler::ElementHandle() {
	//std::cout<<" this is replicate_axisHandler"<<std::endl;

	StopLoop(true);
	xercesc::DOMNode *child;

	for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE) 
		{
			GeoModelTools::XercesParser::elementLoop(child);
			GeoModelTools::XMLHandler *h=theController->XMLStore()->GetHandler(child);
            if(h){
                std::string nH=h->GetName();
                //std::cout<<" handler name "<<nH<<std::endl;
                if (nH=="direction") {
                    directionHandler* dH=dynamic_cast<directionHandler*>(h);
                    if (!dH) std::cout<<" something is wrong! can not retrieve directionHandler!!!"<<std::endl;
                    else parameters.a=dH->getAxis();
                }
                else if (nH=="width")
                {
                    widthHandler* wH=dynamic_cast<widthHandler*>(h);
                    if (!wH) std::cout<<" something is wrong! can not retrieve widthHandler!!!"<<std::endl;
                    else
                    {
                        parameters.w=wH->getWidth();
                    }
			    }
                else if (nH=="offset")
                {
                    offsetHandler* oH=dynamic_cast<offsetHandler*>(h);
                    if (!oH) std::cout<<" something is wrong! can not retrieve offsetHandler!!!"<<std::endl;
                    else
                    {
                        parameters.o=oH->getOffset();
                    }
                }
                else std::cout<<" Name of the handler not defined "<<nH<<std::endl;
            }
            else std::cout<<"WARNING: handler not defined.. continuing"<<std::endl;

        }
    }
    //  std::cout << " this is the handle : "<<parameters.w<<" "<<parameters.o<< std::endl;
}


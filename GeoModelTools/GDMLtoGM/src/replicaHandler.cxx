#include "GDMLInterface/replicaHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLController.h"

#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoModelKernel/GeoXF.h"

#include "GDMLInterface/replicate_axisHandler.h"
#include "GDMLInterface/Axis.h"

replicaHandler::replicaHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
	new replicate_axisHandler("replicate_along_axis",c);
} 
void replicaHandler::ElementHandle()
{

    nCopies=getAttributeAsInt("number");
    //std::cout<<"this is replicaHandler::ElementHandle(): nCopies= "<<nCopies<<std::endl;

    StopLoop(true);
    xercesc::DOMNode *child;

    GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
    GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);

    axis_params p;
    
    GeoLogVol* theLogicalVolume;
    

    for (child=GeoModelTools::XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
    {	
    	if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE)
	{
		
		GeoModelTools::XercesParser::elementLoop(child);
		XMLHandler *h=theController->XMLStore()->GetHandler(child);
		//std::cout<<" namenamename "<<h->GetName()<<std::endl;
            	if(h){
                	std::string nH=h->GetName();
                	//std::cout<<" handler name "<<nH<<std::endl;
                	if (nH=="replicate_along_axis") 
			{
                    		replicate_axisHandler* rH=dynamic_cast<replicate_axisHandler*>(h);
                    		if (!rH) std::cout<<" something is wrong! can not retrieve replicate_axisHandler!!!"<<std::endl;
                     		else p=rH->getAxisParameters();
                	}
                	else if (nH=="volumeref") 
			{
                    		volumerefHandler* vH=dynamic_cast<volumerefHandler*>(h);
                    		if (!vH) std::cout<<" something is wrong! can not retrieve volumerefHandler!!!"<<std::endl;
                    		else
				{
		     			theLogicalVolume=(vH->getVolume()).first;
					//std::cout<<" replicaHandler: log vol name  "<<theLogicalVolume->getName()<<std::endl;
					thePhysicalVolume=new GeoPhysVol(theLogicalVolume);
					//thePhysicalVolume=(vH->getVolume()).second;

		     		}
                	}
            	}
        }
    }

    //std::cout<<" start serial transform "<<std::endl;

    double pStartXYZ,pStartPhi;
    pStartXYZ=-0.5*nCopies*p.w+p.o;
    pStartPhi=p.o;
    
    const unsigned int NREPLICAS=nCopies;
    GeoGenfun::Variable i;
    GeoSerialTransformer* s1=0;
    

//    for (int i=0;i<nCopies;i++)
//    {
        if (p.a == PHI_axis)
        {
                GeoGenfun::GENFUNCTION pCurrent=pStartPhi + i*p.w + p.w/2.;
		GeoXF::TRANSFUNCTION tr=GeoXF::Pow(GeoTrf::RotateZ3D(1.0),pCurrent);
		s1 = new GeoSerialTransformer(thePhysicalVolume,&tr, NREPLICAS);
	}
	else if (p.a == X_axis)
        {
                GeoGenfun::GENFUNCTION pCurrent=pStartXYZ+p.w/2.+i*p.w ;
                GeoXF::TRANSFUNCTION tr=GeoXF::Pow(GeoTrf::TranslateX3D(1.0),pCurrent);
		s1 = new GeoSerialTransformer(thePhysicalVolume,&tr, NREPLICAS);
	}
	else if (p.a == Y_axis)
        {
                GeoGenfun::GENFUNCTION pCurrent=pStartXYZ+p.w/2.+i*p.w ;
                GeoXF::TRANSFUNCTION tr=GeoXF::Pow(GeoTrf::TranslateY3D(1.0),pCurrent);
		s1 = new GeoSerialTransformer(thePhysicalVolume,&tr, NREPLICAS);
        }
        else if (p.a == Z_axis)
	{
                GeoGenfun::GENFUNCTION pCurrent=pStartXYZ+p.w/2.+i*p.w ;
                GeoXF::TRANSFUNCTION tr=GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),pCurrent);
		s1 = new GeoSerialTransformer(thePhysicalVolume,&tr, NREPLICAS);
        }
        else
                std::cout<< " WARNING - GDMLtoGM::replicaHandle: unknown axis!"<<std::endl;

	theMotherVolume->add(s1);

}
GeoPhysVol* replicaHandler::getPhysicalVolume()
{
	return thePhysicalVolume;
}
GeoSerialTransformer* replicaHandler::getTransform() 
{
	if (!transform) std::cout<<" replicaHandler: transform is 0!!!!!"<<std::endl;
	return transform;
}
void replicaHandler::postLoopHandling()
{
}
int replicaHandler::getNCopies()
{
	return nCopies;
}

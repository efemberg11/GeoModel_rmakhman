#include "GDMLInterface/replicaHandler.h"

#include "GeoModelXMLParser/XercesParser.h"

#include "GDMLInterface/GDMLController.h"

#include "GDMLInterface/volumerefHandler.h"
#include "GDMLInterface/positionHandler.h"
#include "GDMLInterface/rotationHandler.h"

#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"

#include "GDMLInterface/replicate_axisHandler.h"
#include "GDMLInterface/Axis.h"

replicaHandler::replicaHandler(std::string n, GDMLController* c): GDMLHandler(n,c)
{
	new replicate_axisHandler("replicate_along_axis",c);
} 
void replicaHandler::ElementHandle()
{
    nCopies=getAttributeAsInt("number");
    std::cout<<"this is replicaHandler::ElementHandle(): nCopies= "<<nCopies<<std::endl;

	StopLoop(true);
    xercesc::DOMNode *child;

    GeoTrf::Transform3D pVRotation=GeoTrf::Transform3D::Identity();
    GeoTrf::Vector3D pVTranslation=GeoTrf::Vector3D(0,0,0);

    axis_params p;

    GeoPhysVol* DaughterPV=0;

	for (child=XercesParser::GetCurrentElement()->getFirstChild();child!=0;child=child->getNextSibling())
	{
		if (child->getNodeType()==xercesc::DOMNode::ELEMENT_NODE)
		{
			XercesParser::elementLoop(child);
			XMLHandler *h=theController->XMLStore()->GetHandler(child);
            if(h){
                std::string nH=h->GetName();
                //std::cout<<" handler name "<<nH<<std::endl;
                if (nH=="replicate_along_axis") {
                    replicate_axisHandler* rH=dynamic_cast<replicate_axisHandler*>(h);
                    if (!rH) std::cout<<" something is wrong! can not retrieve replicate_axisHandler!!!"<<std::endl;
                     else p=rH->getAxisParameters();
                }
                else if (nH=="volumeref") {
                    volumerefHandler* vH=dynamic_cast<volumerefHandler*>(h);
                    if (!vH) std::cout<<" something is wrong! can not retrieve volumerefHandler!!!"<<std::endl;
                    else DaughterPV=(vH->getVolume()).second;
                    thePhysicalVolume=DaughterPV;
                }
            }
        }
    }

    double pStartXYZ,pStartPhi, pCurrent;
    GeoTransform *tr;
    pStartXYZ=-0.5*nCopies*p.w+p.o;
    pStartPhi=p.o;

    for (int i=0;i<nCopies;i++)
    {
        switch (p.a)
        {
            case PHI_axis:
                pCurrent=pStartPhi + i*p.w + p.w/2.;
                tr=new GeoTransform(GeoTrf::RotateZ3D(pCurrent));
                break;
            case X_axis:
                pCurrent=pStartXYZ+i*p.w + p.w/2.;
                tr=new GeoTransform(GeoTrf::TranslateX3D(pCurrent));
                break;
            case Y_axis:
                pCurrent=pStartXYZ+i*p.w + p.w/2.;
                tr=new GeoTransform(GeoTrf::TranslateY3D(pCurrent));
                break;
            case Z_axis:
                pCurrent=pStartXYZ+i*p.w + p.w/2.;
                tr=new GeoTransform(GeoTrf::TranslateZ3D(pCurrent));
                break;
            default:
                std::cout<< " WARNING - GDMLtoGM::replicaHandle: unknown axis!"<<std::endl;
        }
        transforms.push_back(tr);
    }
}
GeoPhysVol* replicaHandler::getPhysicalVolume()
{
	return thePhysicalVolume;
}
GeoTransform* replicaHandler::getTransform(int i) 
{
	return transforms[i];
}
void replicaHandler::postLoopHandling()
{
}
int replicaHandler::getNCopies()
{
	return nCopies;
}

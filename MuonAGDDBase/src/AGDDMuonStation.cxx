/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "MuonAGDDBase/AGDDMuonStation.h"

#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoMaterial.h"

void AGDDMuonStation::CreateSolid() 
{
	std::cout<<"this is AGDDMuonStation::CreateSolid()"<<std::endl;
	void *p=GetSolid();
	if (!p)
	{
		std::cout<<" creating solid with dimensions "<<
		m_small_x<<" "<<m_large_x<<" "<<m_y<<" "<<m_z<<std::endl;
		GeoShape* solid=new GeoTrd(m_small_x/2.,m_large_x/2.,m_y/2.,m_y/2.,m_z/2.);
		SetSolid(solid);
	}
}

void AGDDMuonStation::CreateVolume() 
{
    std::cout<<"this is AGDDMuonStation::CreateVolume()"<<std::endl;
	static int ifirst=1;
	static const GeoMaterial* air=0;
	if (ifirst)
	{
		ifirst=0;
        air = GetMMMaterial("std::Air");
		if (!air) std::cout<<" Air not found!"<<std::endl;
	}

	CreateSolid();

	if (!GetVolume())
	{
	    std::cout<<"trying to create logical volume"<<std::endl;
		const GeoShape* shape=(GeoShape *)GetSolid();
		if (!shape) std::cout<<" something wrong, shape is 0"<<std::endl;
		GeoLogVol *a=new GeoLogVol(GetName(),shape,air);
		GeoPhysVol *a_phys=new GeoPhysVol(a);
		SetVolume(a_phys);
		std::cout<<"done creating volume "<<std::endl;
	}
}

const GeoMaterial* AGDDMuonStation::GetMMMaterial(std::string name)
{
	static int iEntry=1;
	static GeoMaterial *dummy=0,*ether=0;
	if (iEntry)
	{
		GeoElement * aluminium=new GeoElement("Aluminum"  , "Al"   ,13,26.9815); 
		dummy = new GeoMaterial("Dummy",1.);
		ether = new GeoMaterial("Ether",1.);
		dummy->add(aluminium,1.0);
		dummy->lock();
		ether->add(aluminium,1.0);
		ether->lock();
		iEntry=0;
	}
	//std::cout<<" returning a null material pointer !!!! "<<std::endl;
	std::cout<<" material name "<<name<<std::endl;
	if (name=="special::Ether") return ether;	
	return dummy;
}

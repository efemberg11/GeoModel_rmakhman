#pragma once

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include <string>

class MaterialManager {
public:
	static GeoMaterial* getMaterial(std::string name)
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
		if (name=="special::Ether") return ether;	
		return dummy;
	}

};

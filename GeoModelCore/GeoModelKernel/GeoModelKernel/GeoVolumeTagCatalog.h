#pragma once

#include <string>
#include <map>
#include "GeoModelKernel/GeoVPhysVol.h"

#define volumeTags std::map<std::string, GeoVPhysVol* >
#define tagCatalog std::map<std::string, volumeTags >

class GeoVolumeTagCatalog {
public:
	void addTaggedVolume(std::string category, std::string tag, GeoVPhysVol* v)
	{
		std::cout << "adding volume "<< v->getLogVol()->getName()<<" to category "<<category<<" with tag "<<tag<<std::endl;
		theTags[category][tag]=v;	
	} 
	GeoVPhysVol* getTaggedVolume(std::string category, std::string tag)
	{
		std::cout << "getting volume "<< " with tag "<<tag<<" from category "<<category<<std::endl;
		std::cout<<" getting volume "<<tag<<" pointer "<<theTags[category][tag]<<std::endl;
		return theTags[category][tag];
	}

	GeoVolumeTagCatalog() {}
	static GeoVolumeTagCatalog* VolumeTagCatalog() {
		static GeoVolumeTagCatalog* theCatalog=new GeoVolumeTagCatalog;
		return theCatalog;
	}
	tagCatalog theTags;
};

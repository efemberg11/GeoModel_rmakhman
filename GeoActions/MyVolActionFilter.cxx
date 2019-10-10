/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

  author: Riccardo Maria BIANCHI @ CERN
  10 Oct 2019
*/

#include "MyVolActionFilter.h"

#include <iostream>


MyVolActionFilter::MyVolActionFilter(std::string re)
 : GeoVolumeAction (GeoVolumeAction::TOP_DOWN)
{
	// creating a RegEx object with the string passed by the user
	m_re = std::regex(re, std::regex_constants::ECMAScript | std::regex_constants::icase);

	// store the original string used to filter volumes
	m_reStr = re;
}

MyVolActionFilter::~MyVolActionFilter()
{
}

void MyVolActionFilter::handleVPhysVol (const GeoVPhysVol* vol)
{
	std::string volLogName = vol->getLogVol()->getName();

	std::cout << "current volume's path: " << this->getState()->getPath()->getLength() << std::endl;
	std::cout << "current volume's logvol's name: " << volLogName << std::endl;

	if (std::regex_search(volLogName, m_re)) {
			std::cout << "Volume's LogVol's name contains the phrase '" << m_reStr << "'" << std::endl;
		}
}

/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

  author: Riccardo Maria BIANCHI @ CERN
  10 Oct 2019
*/

#include "MyVolAction.h"

#include <iostream>


MyVolAction::MyVolAction()
 : GeoVolumeAction (GeoVolumeAction::TOP_DOWN)
{
}

MyVolAction::~MyVolAction()
{
}

void MyVolAction::handleVPhysVol (const GeoVPhysVol* vol)
{
	std::cout << "current volume's path: " << this->getState()->getPath()->getLength() << std::endl;
	std::cout << "current volume's logvol's name: " << vol->getLogVol()->getName() << std::endl;
}

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "AGDDHandlers/posRPhiZHandler.h"
#include "AGDDKernel/AGDDPositioner.h"

#include <iostream>
#include <vector>

posRPhiZHandler::posRPhiZHandler(std::string s):XMLHandler(s),p(0)
{
}

void posRPhiZHandler::ElementHandle()
{
	bool res;
	std::string volume=getAttributeAsString("volume",res);
	std::vector<double> vvv=getAttributeAsVector("R_Phi_Z",res);
	
	double radius,phi,zpos;

	radius=vvv[0];
	phi=vvv[1];
	zpos=vvv[2];

	const double degrad=M_PI/180.;
	
	GeoTrf::Transform3D crot = GeoTrf::Transform3D::Identity();

        vvv=getAttributeAsVector("rot",res);
        if (res) 
        {
            crot = crot*GeoTrf::RotateZ3D(vvv[2]*degrad)*GeoTrf::RotateY3D(vvv[1]*degrad)*GeoTrf::RotateX3D(vvv[0]*degrad);
        }

	crot = GeoTrf::RotateZ3D(phi*degrad)*crot;
	double x=radius*cos(phi*degrad);
	double y=radius*sin(phi*degrad);
	GeoTrf::Vector3D cvec=GeoTrf::Vector3D(x,y,zpos);

	p=new AGDDPositioner(volume,crot,cvec);
}

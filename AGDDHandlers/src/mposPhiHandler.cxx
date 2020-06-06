/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "AGDDHandlers/mposPhiHandler.h"
#include "AGDDKernel/AGDDPositioner.h"
#include <iostream>
#include <vector>

mposPhiHandler::mposPhiHandler(std::string s):XMLHandler(s),p(0)
{
}

void mposPhiHandler::ElementHandle()
{

	bool res;
	std::string volume=getAttributeAsString("volume");
	int icopy=getAttributeAsInt("ncopy",0);
	double phi0=getAttributeAsDouble("Phi0",res);
	if (!res) phi0=0;
	double dphi=getAttributeAsDouble("dPhi",res);
	
	if (!res) // dphi must be calculated 
	{
		if (icopy>0) 
			dphi=360./icopy;
		else
		{
			std::cout<<"mposPhiHandler: both icopy and dphi are null!! volume "<<volume<<" cannot continue!"<<std::endl;
			return;
		}
	}
	bool impliedRot;
	std::string s;
	std::string iR=getAttributeAsString("impliedRot",impliedRot);
	if (impliedRot) s=iR;
	double lateral_displacement=getAttributeAsDouble("S",res);
	if (!res) lateral_displacement=0;

	std::vector<double> vvv;
	vvv = getAttributeAsVector("R_Z",res);
	double rad=0,zpos=0;
	if (res)
	{
		rad=vvv[0];
		zpos=vvv[1];
	}
	
	GeoTrf::Transform3D crot = GeoTrf::Transform3D::Identity();
	
	vvv=getAttributeAsVector("rot",res);
	const double deg=M_PI/180.;
	if (res) 
	{
		crot = crot*GeoTrf::RotateZ3D(vvv[2]*deg)*GeoTrf::RotateY3D(vvv[1]*deg)*GeoTrf::RotateX3D(vvv[0]*deg);
	}
	
	for (int i=0;i<icopy;i++)
	{
		GeoTrf::Vector2D position(rad,lateral_displacement);
		double phi=phi0+dphi*i;
		position = Eigen::Rotation2Dd(phi*deg)*position; // @TODO: is this the correct to replace Vector2D.rotate ?

		double x=position.x();
		double y=position.y();
		
		GeoTrf::Vector3D cvec=GeoTrf::Vector3D(x,y,zpos);
		if (s!="false"&&i>0) crot = GeoTrf::RotateZ3D(dphi*deg)*crot;
		else if (s!="false"&&i==0) crot = GeoTrf::RotateZ3D(phi0*deg)*crot;

		p=new AGDDPositioner(volume,crot,cvec);
	}
}

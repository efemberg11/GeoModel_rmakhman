/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "AGDDHandlers/posXYZHandler.h"
#include "AGDDKernel/AGDDPositioner.h"
#include "AGDDHandlers/globals.h"

#include <iostream>

posXYZHandler::posXYZHandler(std::string s):XMLHandler(s)
{
//	std::cout<<"Creating handler for posXYZ"<<std::endl;
}

void posXYZHandler::ElementHandle()
{
//	std::cout<<"handling for posXYZ";
	bool res,posRet,rotRet;
	std::string volume=getAttributeAsString("volume",res);
	std::string  sym=getAttributeAsString("sym",res);
	if (res) std::cout<<" symmetry implemented for "<<volume<<std::endl;
	std::string sRot="";
	GeoTrf::Vector3D cvec = GeoTrf::Vector3D::Identity();
	GeoTrf::Transform3D crot = GeoTrf::Transform3D::Identity();
	std::vector<double> X_Y_Z=getAttributeAsVector("X_Y_Z",posRet);
	if (posRet) 
	{
		cvec =GeoTrf::Vector3D(X_Y_Z[0],X_Y_Z[1],X_Y_Z[2]);
	}
	std::vector<double> rot=getAttributeAsVector("rot",rotRet);
	const double deg=M_PI/180.;
	if (rotRet) 
	{
		crot = crot*GeoTrf::RotateZ3D(rot[2]*deg)*GeoTrf::RotateY3D(rot[1]*deg)*GeoTrf::RotateX3D(rot[0]*deg);
	}
	if (s_printFlag) {
		std::cout<<"   posXYV "<<volume;
		if (posRet) std::cout<<" pos= ("<<X_Y_Z[0]<<";"<<X_Y_Z[1]<<";"<<X_Y_Z[2]<<")";
		if (rotRet) std::cout<<" rot= ("<<rot[0]<<";"<<rot[1]<<";"<<rot[2]<<")";
		std::cout<<std::endl;
	}
	AGDDPositioner *p=new AGDDPositioner(volume,crot,cvec);
	globals::currentPositioner=p;
}

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "AGDDHandlers/mposWedgeHandler.h"
#include "AGDDKernel/AGDDPositioner.h"

#include <iostream>
#include <vector>

mposWedgeHandler::mposWedgeHandler(std::string s):XMLHandler(s),p(0)
{
}

void mposWedgeHandler::ElementHandle()
{
	bool res;
	std::string volume=getAttributeAsString("volume",res);
	int iWedge=getAttributeAsInt("wedge_number",8);
	std::vector<double> iSectors=getAttributeAsVector("sectors",res);
		
	double dWedge=360./iWedge;

	const double degrad=M_PI/180.;
	
	double radius=0;
	for (int i=0;i<iWedge;i++)
	{
		double Wedge=dWedge*i;
		if ((int)iSectors[i]==0) continue;
	    GeoTrf::Transform3D crot = GeoTrf::RotateZ3D(Wedge*degrad);
		double x=radius*cos(Wedge*degrad);
		double y=radius*sin(Wedge*degrad);
		double zpos=0;
		GeoTrf::Vector3D cvec=GeoTrf::Vector3D(x,y,zpos);

		p=new AGDDPositioner(volume,crot,cvec);
	}
}

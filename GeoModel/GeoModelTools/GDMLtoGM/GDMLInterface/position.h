#ifndef position_H
#define position_H

#include "GeoModelKernel/GeoDefinitions.h"

struct position {
	std::string name="";
	double x=0;
	double y=0;
	double z=0;
	
	GeoTrf::Vector3D v3d;
};

#endif

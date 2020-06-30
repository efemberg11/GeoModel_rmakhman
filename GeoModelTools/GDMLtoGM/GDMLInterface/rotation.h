#ifndef rotation_H
#define rotation_H

#include "GeoModelKernel/GeoDefinitions.h"

struct rotation {
	rotation():rotx(0),roty(0),rotz(0) {;} 
	std::string name="";
	double rotx=0;
	double roty=0;
	double rotz=0;
};

#endif

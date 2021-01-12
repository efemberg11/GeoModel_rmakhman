#ifndef scale_H
#define scale_H

#include "GeoModelKernel/GeoDefinitions.h"

struct scale {
    scale():scalex(0),scaley(0),scalez(0) {;}
	std::string name="";
	double scalex=0;
	double scaley=0;
	double scalez=0;
        double unit;
};

#endif

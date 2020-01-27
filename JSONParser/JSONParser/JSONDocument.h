#pragma once

#include "GeoModelKernel/GeoDefinitions.h"
#include <map>
#include <vector>
#include <string>

struct point {
	std::string key;
	std::string unit;
	double x;
	double y;
	double z;
};

struct facet {
	std::string type;
	int nVertices;
	std::vector<std::string> vertices;
};

struct tessellated {
	std::string name;
	std::vector<facet> facets;
	std::map<std::string, point, std::less<std::string> > points;
};

class JSONDocument {
public:
	tessellated solidRep;
//	std::map<std::string, tessellated, std::less<std::string> > tessellatedSolids;
};

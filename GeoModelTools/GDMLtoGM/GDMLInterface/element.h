#ifndef element_H
#define element_H

#include <vector>

#include "GDMLInterface/isotope.h"
#include "GDMLInterface/fraction.h"

struct element {
	std::string name;
	std::string symbol;
	int N;
	int Z;
	double w;
	std::vector<fraction> isotopeList;

	void addIsotope(fraction i) {isotopeList.push_back(i);}
	void clear() {isotopeList.clear(); N=Z=w=0; name="";}
};

#endif

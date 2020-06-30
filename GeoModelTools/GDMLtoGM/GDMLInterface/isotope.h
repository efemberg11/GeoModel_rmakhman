#ifndef isotope_h
#define isotope_h

#include <string>

// placeholder to deal with the likes of G4Isotope which do not exist in GeoModel

extern std::string stripPointer(const std::string);

struct isotope {
	int N;
	int Z;
	std::string symbol;
	std::string name;
	std::string ref;
	double atomic_weight;
	void setWeight(double w) {atomic_weight=w;}
	void clear() {N=Z=0;name="";atomic_weight=0.;}
};

#endif

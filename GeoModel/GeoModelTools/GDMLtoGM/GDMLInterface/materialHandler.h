#ifndef materialHandler_H
#define materialHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include "GDMLInterface/fraction.h"

#include <string>
#include <vector>

class materialHandler:public GDMLHandler {
public:

	materialHandler(std::string n, GDMLController* c);
	void ElementHandle();
	void postLoopHandling();
	void addFraction(fraction f);
	void setDensity(double d);
private:
	std::string name;
	double density;
	bool explicitMaterial=false;
	int m_Z=0;
	double m_A=0;
  	std::vector<fraction> fractionList;
};


#endif /* end of include guard:  */

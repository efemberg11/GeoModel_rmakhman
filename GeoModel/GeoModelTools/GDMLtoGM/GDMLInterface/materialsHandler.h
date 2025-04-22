
#ifndef materialsHandler_H
#define materialsHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class materialsHandler:public GDMLHandler {
public:
	materialsHandler(std::string n, GDMLController*c): GDMLHandler(n,c) {;}
	void ElementHandle()
	{
		//std::cout<<"materialsHandler: element/material definition starts here"<<std::endl;
	}
	void postLoopHandling()
	{
		//std::cout<<"materialsHandler: elements/material definition done!"<<std::endl;
	}

};


#endif /* end of include guard:  */

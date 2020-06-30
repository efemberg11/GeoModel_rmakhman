
#ifndef setupHandler_H
#define setupHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class setupHandler:public GDMLHandler {
public:
	setupHandler(std::string n, GDMLController*c): GDMLHandler(n,c) {;}
	void ElementHandle()
	{
		//std::cout<<"setupHandler: World definition starts here"<<std::endl;
	}
	void postLoopHandling()
	{
		//std::cout<<"setupHandler: World definition done!"<<std::endl;
	}

};


#endif /* end of include guard:  */

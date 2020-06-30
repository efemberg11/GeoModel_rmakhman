
#ifndef topTreeHandler_H
#define topTreeHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class topTreeHandler:public GDMLHandler {
public:
	topTreeHandler(std::string n, GDMLController* c):GDMLHandler(n,c) {;}
	void ElementHandle()
	{
	//	std::cout<<" this is the top-of-the-tree element "<<GetName()<<std::endl;
	}
	void postLoopHandling() {;}

};


#endif /* end of include guard:  */


#ifndef structureHandler_H
#define structureHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class structureHandler:public GDMLHandler {
public:
	structureHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {;}
  void postLoopHandling() {;}
};


#endif /* end of include guard:  */

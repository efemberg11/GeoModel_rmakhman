
#ifndef widthHandler_H
#define widthHandler_H

// 

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class widthHandler:public GDMLHandler {
public:
	widthHandler(std::string n, GDMLController* c);
	void ElementHandle();
	double getWidth();
private:
	double unit;
	double value;
	double width;
};


#endif /* end of include guard:  */

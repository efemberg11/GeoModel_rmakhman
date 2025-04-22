
#ifndef eltubeHandler_H
#define eltubeHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>

class eltubeHandler:public GDMLHandler {
public:
	eltubeHandler(std::string, GDMLController*);
	void ElementHandle();
};


#endif /* end of include guard:  */

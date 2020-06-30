
#ifndef GDMLHandler_H
#define GDMLHandler_H

#include "GeoModelXMLParser/XMLHandler.h"
#include "GDMLInterface/GDMLController.h"
#include <string>
#include <iostream>

class GDMLHandler:public XMLHandler {
public:
  GDMLHandler(std::string n, GDMLController* c):XMLHandler(n),theController(c)
  {;}
  virtual void postLoopHandling() {;}
protected:
  GDMLController* theController;
};


#endif /* end of include guard:  */

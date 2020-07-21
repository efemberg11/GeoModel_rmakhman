
#ifndef twoDimVertexHandler_H
#define twoDimVertexHandler_H

#include "GDMLInterface/GDMLHandler.h"
#include "GDMLInterface/GDMLController.h"

struct twoDVtx {
	double xv;
	double yv;
};

class twoDimVertexHandler:public GDMLHandler {
public:
	twoDimVertexHandler(std::string n, GDMLController* c): GDMLHandler(n,c) {}
	void ElementHandle() {
		v.xv=getAttributeAsDouble("x");
		v.yv=getAttributeAsDouble("y");
	}
	twoDVtx& getVtx() {return v;}
private:
	twoDVtx v;
};


#endif /* end of include guard:  */

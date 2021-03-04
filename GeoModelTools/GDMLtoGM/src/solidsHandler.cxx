#include "GDMLInterface/solidsHandler.h"
#include "GDMLInterface/GDMLController.h"

// handlers being created here
#include "GDMLInterface/boxHandler.h"
#include "GDMLInterface/tubeHandler.h"
#include "GDMLInterface/coneHandler.h"
#include "GDMLInterface/eltubeHandler.h"
#include "GDMLInterface/paraHandler.h"
#include "GDMLInterface/torusHandler.h"
#include "GDMLInterface/trdHandler.h"
#include "GDMLInterface/polyconeHandler.h"
#include "GDMLInterface/polyhedraHandler.h"
#include "GDMLInterface/zplaneHandler.h"
#include "GDMLInterface/unionHandler.h"
#include "GDMLInterface/subtractionHandler.h"
#include "GDMLInterface/intersectionHandler.h"
#include "GDMLInterface/booleanHandler.h"
#include "GDMLInterface/trapHandler.h"
#include "GDMLInterface/twistedTrapHandler.h"
#include "GDMLInterface/tessellatedHandler.h"
#include "GDMLInterface/triangularHandler.h"
#include "GDMLInterface/quadrangularHandler.h"
#include "GDMLInterface/xtruHandler.h"
#include "GDMLInterface/arb8Handler.h"
#include "GDMLInterface/sectionHandler.h"
#include "GDMLInterface/twoDimVertexHandler.h"
#include "GDMLInterface/unidentifiedShapeHandler.h"

solidsHandler::solidsHandler(std::string n, GDMLController* c):GDMLHandler(n,c) 
{
	new boxHandler("box",c);
	new tubeHandler("tube",c);
	new coneHandler("cone",c);
	new eltubeHandler("eltube",c);
	new paraHandler("para",c);
	new torusHandler("torus",c);
	new trdHandler("trd",c);
	new polyconeHandler("polycone",c);
	new polyhedraHandler("polyhedra",c);
	new zplaneHandler("zplane",c);
	new booleanHandler("first",c);
	new booleanHandler("second",c);
	new unionHandler("union",c);
	new subtractionHandler("subtraction",c);
	new intersectionHandler("intersection",c);
	new trapHandler("trap",c);
        new twistedTrapHandler("twistedtrap",c);
	new tessellatedHandler("tessellated",c);
	new triangularHandler("triangular",c);
	new quadrangularHandler("quadrangular",c);
	new xtruHandler("xtru",c);
	new arb8Handler("arb8",c);
	new twoDimVertexHandler("twoDimVertex",c);
	new sectionHandler("section",c);
        new unidentifiedShapeHandler("cutTube",c);
}
void solidsHandler::ElementHandle() 
{
}
void solidsHandler::postLoopHandling() 
{
}

#include "JSONParser/tessellatedHandler.h"
#include "JSONParser/pointsHandler.h"
#include "JSONParser/facetsHandler.h"

JSONHandler* jh1=handlerStore::createHandler<tessellatedHandler>("1_tessellated");
JSONHandler* jh2=handlerStore::createHandler<pointsHandler>("2_Points");
JSONHandler* jh3=handlerStore::createHandler<facetsHandler>("3_Facets");


#pragma once

#include <string>
#include "nlohmann/json.hpp"
#include "JSONParser/JSONHandler.h"
#include "JSONParser/handlerStore.h"
#include "JSONParser/JSONDocument.h"

extern JSONDocument jDocument;

class pointsHandler: public JSONHandler {

friend class handlerStore;

public:
        virtual void ElementHandle(const json& j)
        {
                std::cout<<"--------> Handling for points!!! new version"<<std::endl;
     
		for (auto it=j.begin();it!=j.end();it++)
        	{
			point p;
                	const json& jj=it.value();
                	auto ir =jj.begin();
                	p.key=ir.key();
                	p.unit = (ir.value())["unit"];
                	p.x=(ir.value())["x"];
                	p.y=(ir.value())["y"];
                	p.z=(ir.value())["z"];
			jDocument.solidRep.points[p.key]=p;
        	}
	}

protected:
        pointsHandler(std::string n):JSONHandler(n) 
        {       
        }

};


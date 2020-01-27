#pragma once

#include <string>
#include "JSONParser/JSONDocument.h"
#include "nlohmann/json.hpp"
#include "JSONParser/JSONHandler.h"
#include "JSONParser/handlerStore.h"

extern JSONDocument jDocument;

class tessellatedHandler: public JSONHandler {

friend class handlerStore;

public:
        virtual void ElementHandle(const json& j)
        {
                std::cout<<"--------> Handling for tessellated!!! "<<j<<std::endl;
		jDocument.solidRep.name=j["name"];
	}

protected:
        tessellatedHandler(std::string n):JSONHandler(n) 
        {       
        }

};


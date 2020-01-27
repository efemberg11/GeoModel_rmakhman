#pragma once

#include <string>
#include "nlohmann/json.hpp"
#include "JSONParser/JSONHandler.h"
#include "JSONParser/handlerStore.h"
#include "JSONParser/JSONDocument.h"

extern JSONDocument jDocument;

class facetsHandler: public JSONHandler {

friend class handlerStore;

public:
        virtual void ElementHandle(const json& j)
        {
                std::cout<<"--------> Handling for facets!!!"<<std::endl;
		
		for (auto it=j.begin(); it!=j.end() ; it++)
		{
			facet f;
			f.nVertices=0;
			
			const json& jj=it.value();
			for (auto ir=jj.begin();ir!=jj.end();ir++)
			{
				const json& jk=ir.value();
				if (jk.is_string())
					f.type=jk;
				else if (jk.is_array()) 
					for (auto iz=jk.begin();iz!=jk.end();iz++)
					{
						f.nVertices++;
						f.vertices.push_back(*iz);
					}
			}
			jDocument.solidRep.facets.push_back(f);
			
		}
		
	}

protected:
        facetsHandler(std::string n):JSONHandler(n) 
        {       
        }

};


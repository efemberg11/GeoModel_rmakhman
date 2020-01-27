
#include "JSONParser/JSONDocument.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

JSONDocument jDocument;

void to_json(json& j, const point& p)
{
	j={
		{p.key,{
			{"unit",p.unit},{"x",p.x},{"y",p.y},{"z",p.z}
		}}
	};
}

void from_json(const json& j, point& p) 
{
        for (auto it=j.begin();it!=j.end();it++)
        { 
        	const json& jj=it.value();
        	auto ir =jj.begin();
        	p.key=ir.key();
        	p.unit = (ir.value())["unit"];
        	p.x=(ir.value())["x"];
        	p.y=(ir.value())["y"];
        	p.z=(ir.value())["z"];
        }
}

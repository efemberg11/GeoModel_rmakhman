#ifndef JSONParser_H
#define JSONParser_H

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "JSONParser/handlerStore.h"
#include "JSONParser/JSONHandler.h"

using json = nlohmann::json;

class JSONParser {
public:
	JSONParser(const char* files) 
	{
		std::istringstream iss(files);
    		std::string token;
    		while (std::getline(iss, token, ':'))
		{
			std::cout <<" file added "<<token<<std::endl;
			addFile(token);
    		}
	}
	void addFile(std::string n) {fileNames.push_back(n);}
	void parse() 
	{
		for (auto it: fileNames)
		{
			std::ifstream i(it);
        		json j = json::parse(i);
			json &jj=j;
			IterateModel(jj);
		}
	}
	inline void IterateModel(json&);
private:
	std::vector<std::string> fileNames;
};

void JSONParser::IterateModel(json& jj)
{
	for (auto it = jj.begin(); it != jj.end(); ++it)
        {
		std::string key=it.key();
		JSONHandler *h=handlerStore::getHandlerStore()->getHandler(key);
		h->ElementHandle(it.value());
        }
}

#endif

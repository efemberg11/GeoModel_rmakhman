#ifndef JSONHandler_H
#define JSONHandler_H

#include <map>
#include <string>

#include "nlohmann/json.hpp"

using nlohmann::json;

#include "handlerStore.h"

class JSONHandler {

friend class handlerStore;

public:
	virtual void ElementHandle(const json& j) = 0;
	std::string name() {return _name;}
protected:
	JSONHandler(std::string n):_name(n) {handlerStore::getHandlerStore()->registerHandler(this);}
	virtual ~JSONHandler() {;}
private:
	std::string _name;
	std::map<std::string, JSONHandler*,std::less<std::string> > registeredHandlers;
};

#endif

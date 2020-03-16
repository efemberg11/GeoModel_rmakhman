#ifndef HandlerStore_H
#define HandlerStore_H

#include <iostream>
#include <string>
#include <map>

class JSONHandler;

class handlerStore {
public:
	static handlerStore* getHandlerStore() ;
	void registerHandler(JSONHandler* h) ;
	JSONHandler* getHandler(std::string name) ;
	void listHandlers();
	template <typename T> static T* createHandler(std::string n)
	{
		return new T(n);
	}
	
private:
	handlerStore() 
	{
		std::cout<<" this is the handlerStore being created"<<std::endl;
	}
	std::map< std::string, JSONHandler*, std::less<std::string> > store;
};

#endif

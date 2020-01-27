#include "JSONParser/handlerStore.h"
#include "JSONParser/JSONHandler.h"

handlerStore* handlerStore::getHandlerStore() 
{
	static handlerStore *theStore=new handlerStore();
	return theStore;
}

void handlerStore::registerHandler(JSONHandler* h) 
{
	std::cout<<"registering JSONHandler "<<h->name()<<std::endl;
	store[h->name()]=h;
}
JSONHandler* handlerStore::getHandler(std::string name)
{
	if (store.find(name)!=store.end()) return store[name];
	else
	{
		std::cout<<" JSONHandler "<<name<<" not found in store!!"<<std::endl;
		return 0;
	}
		
}
void handlerStore::listHandlers()
{
	std::cout<<" HandlerStore: list of available level 0th handlers"<<std::endl;
	for (auto it: store)
	{
			std::cout<<"\t\t---> Handler "<<(it.second)->name()<<std::endl;
	}
}

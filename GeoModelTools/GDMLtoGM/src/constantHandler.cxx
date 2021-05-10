#include "GDMLInterface/constantHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>


constantHandler::constantHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"constructing constantHandler!"<<std::endl;
}

void constantHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  double value=getAttributeAsDouble("value");

  double lunit=1.;
  if (this->GetName()=="quantity")
  {
  	bool unitTest=true;
  	lunit=getAttributeAsDouble("unit",unitTest);
	if (!unitTest)
		std::cout<<" Warning, <quantity /> tag requires an unit attribute to be defined!!!!"<<std::endl;
	std::string type=getAttributeAsString("type","density");
  }
  ExpressionEvaluator::GetEvaluator()->RegisterConstant(name,value*lunit);
}

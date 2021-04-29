#include "GDMLInterface/variableHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>



variableHandler::variableHandler(std::string s,GDMLController* g):GDMLHandler(s,g)
{
  // std::cout<<"variableHandler!"<<std::endl;
}

void variableHandler::ElementHandle()
{

  std::string name=getAttributeAsString("name");

  //std::cout << "handling for variable: name "<< name <<'\n';

  double value=getAttributeAsDouble("value");

  Evaluator()->RegisterVariable(name,value);
}

#include "GDMLInterface/defineHandler.h"
#include "GDMLInterface/GDMLHandler.h"
#include <iostream>

defineHandler::defineHandler(std::string s, GDMLController* g):GDMLHandler(s,g)
{
  //std::cout<<"constructing defineHandler!"<<std::endl;
}

void defineHandler::ElementHandle()
{
  // std::cout << "handling define clause" << '\n';
}

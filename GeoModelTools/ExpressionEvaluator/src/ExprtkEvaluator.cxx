
#include "ExpressionEvaluator/ExprtkEvaluator.h"
#include "ExpressionEvaluator/IEvaluator.h"

#include <iostream>

using namespace GeoModelTools;

ExprtkEvaluator::ExprtkEvaluator()
{
  //  std::cout<<" ########### new ExprtkEvaluator being created "<<std::endl;
  symbol_table.add_constants();
  expression.register_symbol_table(symbol_table);
}

ExprtkEvaluator::~ExprtkEvaluator()
{
}

double ExprtkEvaluator::evaluate(const char * expression_string)
{
  
  if (!parser.compile(expression_string,expression))
  {
      std::cout<<"Compilation error: "<<expression_string<<std::endl;
      for (std::size_t i = 0; i < parser.error_count(); ++i)
      {
         typedef exprtk::parser_error::type error_t;

         error_t error = parser.get_error(i);

         std::cout<<"\t Error "<<i<<" Position: "<<error.token.position<<" Type: "<<
	 	exprtk::parser_error::to_str(error.mode)<<" Msg: "<<error.diagnostic<<std::endl;
	 _status=ERROR;
      }

      _status=OK;
      return 0;
  }
  
  T result = expression.value();
  return result;
}

int ExprtkEvaluator::status() const
{
  return _status;
}

int ExprtkEvaluator::error_position() const
{
  return 0;
}

void ExprtkEvaluator::print_error() const
{

}

std::string ExprtkEvaluator::error_name() const
{
  return "error not set";
}

void ExprtkEvaluator::setConstant(const char * name, double value)
{
  //std::cout<<"Calling ExprtkEvaluator setConstant "<<name<<" "<<value<<std::endl;

    symbol_table.add_constant(name,value);
}

void ExprtkEvaluator::setConstant(const char * name, const char * expr)
{
  //std::cout<<"Calling ExprtkEvaluator setConstant "<<name<<" "<<expression<<" ";
  T e=evaluate(expr);

    symbol_table.add_constant(name,e);
  //std::cout<<e<<std::endl;
}

void ExprtkEvaluator::setVariable(const char * name, double value)
{
  //std::cout<<"Calling ExprtkEvaluator setVariable "<<name<<" "<<value<<std::endl;
    if (varMap.find(name)==varMap.end())
    {
      varMap[name]=value;
      symbol_table.add_variable(name,varMap[name]);
    }
    else varMap[name]=value;
}

void ExprtkEvaluator::setVariable(const char * name, const char * expr)
{
  //std::cout<<"Calling ExprtkEvaluator setVariable "<<name<<" "<<expression<<" ";
  T e=evaluate(expr);

  if (varMap.find(name)==varMap.end())
  {
    varMap[name]=e;
    symbol_table.add_variable(name,varMap[name]);
  }
  else varMap[name]=e;
}

void ExprtkEvaluator::setArray(const char * name, std::vector<T> array)
{
  if (vecMap.find(name) == vecMap.end())
  {
    for (auto it: array) vecMap[name].push_back(it);
    symbol_table.add_vector(name,vecMap[name]);
  }
  else std::cout<<"Warning: vector "<<name<<" already defined!!!"<<std::endl;
  //std::cout<<" array "<<" name "<<array[0]<<" "<<array[1]<<" "<<array[2]<<" "<<array[3]<<std::endl;
}

bool ExprtkEvaluator::findVariable(const char * name) const
{
  return (varMap.find(name)!=varMap.end());
}

bool ExprtkEvaluator::findFunction(const char * /*name*/, int /*npar*/) const
{
  return true;
}

void ExprtkEvaluator::removeVariable(const char * /*name*/)
{

}

void ExprtkEvaluator::removeFunction(const char * /*name*/, int /*npar*/)
{

}

void ExprtkEvaluator::clear()
{
	varMap.clear();
	vecMap.clear();
}

void ExprtkEvaluator::setStdMath()
{
  // std::cout<< "calling ExprtkEvaluator::setStdMath() "<<std::endl;
  setConstant("pi",     3.14159265358979323846);
  setConstant("e",      2.7182818284590452354);
  setConstant("gamma",  0.577215664901532861);
  setConstant("radian", 1.0);
  setConstant("rad",    1.0);
  setConstant("degree", 3.14159265358979323846/180.);
  setConstant("deg",    3.14159265358979323846/180.);
}

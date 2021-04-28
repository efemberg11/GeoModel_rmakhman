/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

//
#include "ExpressionEvaluator/ExprtkEvaluator.h"
#include "ExpressionEvaluator/ExpressionEvaluator.h"
#include "GeoModelKernel/Units.h"
#include <string>
#include <vector>
#include <iostream>

ExpressionEvaluator::ExpressionEvaluator()
{
  m_calc.clear();
  m_calc.setStdMath();                 // set standard constants and functions
  m_calc.setSystemOfUnits(1.e+3, 1./1.60217733e-25, 1.e+9, 1./1.60217733e-10,1.0, 1.0, 1.0);
}

ExpressionEvaluator::~ExpressionEvaluator()
{
}

bool ExpressionEvaluator::RegisterConstant( std::string& c, double v )
{
  double value = v;
  
  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error registering constant " << c << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }
  
  RegisterVariable( c, value );
  return true;
}
bool ExpressionEvaluator::RegisterArray( std::string& c, std::vector<double> v)
{

  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error registering array " << c << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }

   m_calc.setArray(c.c_str(),v);
   return true;
}

bool ExpressionEvaluator::RegisterVariable( const std::string& var_name, double value)
{

  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error registering variable " << var_name << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }  

  m_calc.setVariable( var_name.c_str(), value );
  return true;
}

bool ExpressionEvaluator::RegisterPhysConstant( std::string& c, std::string value, std::string unit  )
{

  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error registering physical constant " << c << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }

  std::string expr = value;
  expr += "*(";
  expr += unit;
  expr += ")";

  //std::cout << "Expression evaluator:: evaluating string: " << expr << std::endl;

  double dvalue      = Eval( expr );
//  double unit_value = EvaluateString( unit );

  RegisterVariable( c, dvalue );
  return true;
}

bool ExpressionEvaluator::RegisterExpression( std::string& name, std::string text )
{
  std::string expr = "(";
  expr += text;
  expr += ")";
  double value = Eval( expr );

  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error registering expression " << name << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }

  RegisterVariable( name, value );
  return true;
}

double ExpressionEvaluator::Eval( const std::string& expr )
{
  return Eval( expr.c_str() );
}

double ExpressionEvaluator::Eval( const char* expr_mod )
{
  if( m_calc.status() != ExprtkEvaluator::OK )
  {
    std::cerr << "Expression evaluator:: Error evaluating expression " << expr_mod << std::endl;
    m_calc.print_error();
    std::cout << std::endl;
    return false;
  }
  
  // hack to go aroud a problem introduced in AGDD by the use of the ** operator
  
  std::string expression=expr_mod;
  replace_substring(expression,"**","^");

  double res = m_calc.evaluate( expression.c_str() );
  return res;
}

void ExpressionEvaluator::replace_substring(std::string& s, const std::string toReplace, const std::string replacement)
{
	size_t index = 0;
	while (true) {
     		/* Locate the substring to replace. */
     		index = s.find(toReplace, index);
     		if (index == std::string::npos) break;

     		/* Make the replacement. */
     		s.replace(index, toReplace.length(), replacement);

     		/* Advance index forward so the next iteration doesn't pick it up as well. */
     		index += toReplace.length();
	}
}


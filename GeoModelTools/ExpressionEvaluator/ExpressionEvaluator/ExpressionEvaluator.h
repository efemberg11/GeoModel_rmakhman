/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

// totally reworked implementation of the CLHEP ExpressionEvaluator

#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H 1

#include "ExpressionEvaluator/ExprtkEvaluator.h"

#include <string>
#include <vector>
#include <map>

class ExpressionEvaluator
{
public:
  ~ExpressionEvaluator();

	static ExpressionEvaluator* GetEvaluator()
  {
    static ExpressionEvaluator* eval=new ExpressionEvaluator();
    return eval;
  }
  bool RegisterConstant( std::string& c, double v );
  bool RegisterPhysConstant( std::string&, std::string, std::string  );
  bool RegisterExpression( std::string& c, std::string v  );
  bool RegisterArray( std::string& c, std::vector<double> v );
  bool RegisterVariable( const std::string& var_name, double value);

  double Eval( const std::string& expr );
  double Eval( const char* expr );

private:
  ExpressionEvaluator();
  ExprtkEvaluator     m_calc;
  
  void replace_substring(std::string& , const std::string , const std::string );
};

#endif // GDML_EXPRESSION_EVALUATOR_H

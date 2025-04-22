// -*- C++ -*-
// $Id: Evaluator.h,v 1.2 2010/07/20 17:00:49 garren Exp $
// ---------------------------------------------------------------------------

#ifndef EXPRTK_EVALUATOR_H
#define EXPRTK_EVALUATOR_H

#include <string>
#include <vector>
#include <map>

#include "ExpressionEvaluator/IEvaluator.h"
#include "ExpressionEvaluator/exprtk.hpp"

typedef double T; // numeric type (float, double, mpfr etc...)

typedef exprtk::symbol_table<T> symbol_table_t;
typedef exprtk::expression<T>     expression_t;
typedef exprtk::parser<T>             parser_t;

typedef std::map<std::string,std::vector<double> > vectorMap;
typedef std::map<std::string,double> variableMap;


namespace GeoModelTools { 

/*
 Evaluator based on exprtk (see https://www.partow.net/programming/exprtk/index.html)
*/
class ExprtkEvaluator: public IEvaluator {
public:

  enum {
    OK,                         /**< Everything OK */
    ERROR
  };

  ExprtkEvaluator();

  /**
   * Destructor.
   */
  virtual ~ExprtkEvaluator();

  double evaluate(const char * expression);

int status() const;

  int error_position() const;

  void print_error() const;

  std::string error_name() const;

  void setConstant(const char * name, double value);

  void setConstant(const char * name, const char * expression);

  void setVariable(const char * name, double value);

  void setVariable(const char * name, const char * expression);

  void setFunction(const char * /*name*/, double (*/*fun*/)()) {;}

  void setArray(const char * name, std::vector<double> array);

  void setFunction(const char * /*name*/, double (*/*fun*/)(double)) {;}

  void setFunction(const char * /*name*/, double (*/*fun*/)(double,double)) {;}

  void setFunction(const char * /*name*/, double (*/*fun*/)(double,double,double)) {;}

  void setFunction(const char * /*name*/,
		   double (*/*fun*/)(double,double,double,double)) {;}

  void setFunction(const char * /*name*/,
                   double (*/*fun*/)(double,double,double,double,double)) {;}

  bool findVariable(const char * name) const;

  bool findFunction(const char * name, int npar) const;

  void removeVariable(const char * name);

  void removeFunction(const char * name, int npar);

  void clear();

  void setStdMath();

private:
  ExprtkEvaluator(const ExprtkEvaluator &) ;             // copy constructor is not allowed
  ExprtkEvaluator & operator=(const ExprtkEvaluator &); // assignment is not allowed
  symbol_table_t symbol_table;
  expression_t expression;
  parser_t parser;

  variableMap varMap;
  vectorMap vecMap;
  int _status=OK;
  
  void replace_substring(std::string& s, const std::string toReplace, const std::string replacement);

};

}

#endif

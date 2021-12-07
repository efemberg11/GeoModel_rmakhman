// -*- C++ -*-
// $Id: Evaluator.h,v 1.2 2010/07/20 17:00:49 garren Exp $
// ---------------------------------------------------------------------------

#ifndef IEVALUATOR_H
#define IEVALUATOR_H

#include <string>

namespace GeoModelTools {

/**
 * IEvaluator: base class for Evaluator
 */
class IEvaluator {
 public: 

  enum {
    OK,                         /**< Everything OK */
    ERROR
  };

  IEvaluator() {;}

  /**
   * Destructor.
   */
  ~IEvaluator() {;} 

  virtual double evaluate(const char * expression) = 0;

  virtual int status() const {return 0;}

  virtual int error_position() const {return 0;} 

  virtual void print_error() const {;}

  virtual std::string error_name() const {return "";}

  virtual void setVariable(const char * name, double value) = 0;

  virtual void setVariable(const char * name, const char * expression) = 0;

  virtual void setFunction(const char * /*name*/, double (*/*fun*/)()) {;}

  virtual void setFunction(const char * /*name*/, double (*/*fun*/)(double)) {;}

  virtual void setFunction(const char * /*name*/, double (*/*fun*/)(double,double)) {;}

  virtual void setFunction(const char * /*name*/, double (*/*fun*/)(double,double,double)) {;}

  virtual void setFunction(const char * /*name*/,
                           double (*/*fun*/)(double,double,double,double)) {;}

  virtual void setFunction(const char * /*name*/,
                           double (*/*fun*/)(double,double,double,double,double)) {;}

  virtual bool findVariable(const char * name) const =0;

  virtual bool findFunction(const char * name, int npar) const =0;

  virtual void removeVariable(const char * /*name*/) {;}

  virtual void removeFunction(const char * /*name*/, int /*npar*/) {;}
  
  virtual void clear() {;}

  virtual void setStdMath() {;}


  void setSystemOfUnits(double meter    = 1.0,
                        double kilogram = 1.0,
                        double second   = 1.0,
                        double ampere   = 1.0,
                        double kelvin   = 1.0,
                        double mole     = 1.0,
                        double candela  = 1.0) ;

private:  
  // IEvaluator(const IEvaluator &);             // copy constructor is not allowed
  // IEvaluator & operator=(const IEvaluator &); // assignment is not allowed
};

}

#endif /* HEP_EVALUATOR_H */

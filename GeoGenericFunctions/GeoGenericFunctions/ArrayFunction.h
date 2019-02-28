//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#ifndef _ArrayFunction_h_
#define _ArrayFunction_h_
//-------------------------------------------------------//
// This one dimensional function takes its values from   //
// an array..which it copies in.                         //
//-------------------------------------------------------//

#include "GeoGenericFunctions/AbsFunction.h"

#include <vector>
#include <initializer_list>

namespace GeoGenfun {
class ArrayFunction : public AbsFunction  {
  
  FUNCTION_OBJECT_DEF(ArrayFunction)
    
    public:
  
  // Constructor
  ArrayFunction(const double *begin, const double *end);

  // Initializer list constructor:
  ArrayFunction(std::initializer_list<double> values);
  
  // Destructor
  virtual ~ArrayFunction();
  
  // Copy constructor
  ArrayFunction(const ArrayFunction &right);
  
  // Retreive function value
  virtual double operator ()(double argument) const;
  virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

  // Derivative.  
  Derivative partial (unsigned int) const;
  
  // Does this function have an analytic derivative?
  virtual bool hasAnalyticDerivative() const {return true;}
    
 private:
  
  // It is illegal to assign a ArrayFunction
  const ArrayFunction & operator=(const ArrayFunction &right);

  std::vector<double> _values;
};
}
#endif

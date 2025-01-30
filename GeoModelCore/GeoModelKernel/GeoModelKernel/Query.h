/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_QUERY_H
#define GEOMODELKERNEL_QUERY_H

/**
 * @class Query
 *
 * @brief  This class can be used to return the result of a query      
 * that can fail.  Objects of this class can convert themselves to the       
 * result type T.  Client code can check for a valid result; if it doesn't,  
 * and the result is valid, no problem.   But if the result is invalid you   
 * will print an error message and terminate.                                
 *
 * Joe Boudreau
 * based on Barton & Nackman's "Scientific and Engineering C++"              
 */

#include <GeoModelKernel/throwExcept.h>
#include <optional>
template < class T > class Query: public std::optional<T>
{

public:
  /// Use all constructors from the std::optional
  using std::optional<T>::optional;

  // Convert to "T"  
  inline operator T () const;

  // Test Validity 
  inline bool isValid () const;

};

template < class T > inline Query < T >::operator T () const {
  if (!this->isValid()){
    THROW_EXCEPTION("Nothing has been saved in query of type "<<typeid(T).name());
  }
  return this->value_or(T{});
}
template < class T > inline bool Query < T >::isValid () const {
  return this->has_value();
}


#endif

#ifndef _GENPERSISTIFIER_H_
#define _GENPERSISTIFIER_H_
#include <map>
#include <string>
#include <typeinfo>
#include <sstream>
#include <deque>
//
// Forward declaration of GeoGenfun::AbsFunction
//
namespace GeoGenfun {
  class AbsFunction;
}
//
// Forward definition of GenFunctionRecorder;
//
class GenFunctionRecorder;



class GenFunctionPersistifier {

 public:

  // Constructor
  GenFunctionPersistifier();

  // Destructor
  ~GenFunctionPersistifier();

  // Registers a recorder. Assumes ownership and will delete.
  void add(const std::type_info & tInfo, const GenFunctionRecorder * recorder);

  // Persistifies the function.  Result goes into the coded string.
  void persistify(const GeoGenfun::AbsFunction & f) const;

  // Retrieves the coded string after the persistify operation:
  const std::string & getCodedString() const;

  // Retrieves the stream used to build the expression:
  std::ostringstream & getStream() const;

  // Retreive floating point data
  std::deque<double> & getFloatingPointData() const;

private:

  // Store the actual recorders, map them into the type name
  std::map<std::string, const GenFunctionRecorder *> recorderMap;

  // Here is the result:
  mutable std::string         codedString;
  mutable std::deque<double>  floatingPointData;

  
  // An ostringstream is used to build the coded string:
  mutable std::ostringstream *stream;

  // Deleted methods:
  GenFunctionPersistifier(const GenFunctionPersistifier &)               = delete;
  GenFunctionPersistifier & operator = (const GenFunctionPersistifier &) = delete;

};


#endif

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPRECORD_H
#define GEOXMLINPMANAGER_GEOINPRECORD_H

#include <variant>
#include <string>
#include <map>
#include <memory>

enum GeoInpType
{
 GEOINP_INT
// , GEOINP_LONG
// , GEOINP_FLOAT
 , GEOINP_DOUBLE
 , GEOINP_STRING
};

typedef std::variant<int
//		     , long
//		     , float
		     , double
		     , std::string> GeoInp;

typedef std::map<std::string,GeoInpType> GeoInpDef;
typedef std::shared_ptr<GeoInpDef> GeoInpDef_ptr;

class GeoInpRecord {
  friend class GeoInpRowHandler;

public:
  GeoInpRecord(GeoInpDef_ptr def);

  bool isFieldNull(const std::string& field) const;
  
  int         getInt    (const std::string& field) const;
//  long        getLong   (const std::string& field) const;
//  float       getFloat  (const std::string& field) const;
  double      getDouble (const std::string& field) const;
  std::string getString (const std::string& field) const;

  void dump() const;
protected:
  void addValue(const std::string& field
		, GeoInp value);
  GeoInpDef_ptr getDef() const;
  
private:
  GeoInpDef_ptr                 m_def;
  std::map<std::string,GeoInp>  m_record;

  std::map<std::string,GeoInp>::const_iterator checkField(const std::string& field
							  , GeoInpType fieldType) const;
};

inline std::map<std::string,GeoInp>::const_iterator GeoInpRecord::checkField(const std::string& field
									     , GeoInpType fieldType) const
{
  auto defIt = m_def->find(field);
  if(defIt==m_def->end()) throw std::runtime_error( "Wrong name for the field "+ field);
  if(defIt->second!=fieldType) throw std::runtime_error( "Wrong type requested for the field "+ field);

  auto recIt = m_record.find(field);
  if(recIt==m_record.end()) throw std::runtime_error( "Null value for the field "+ field);
  return recIt;
}

inline GeoInpDef_ptr GeoInpRecord::getDef() const
{
  return m_def;
}

#endif

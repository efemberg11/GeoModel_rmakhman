/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef XMLHandler_H
#define XMLHandler_H

#include <string>
#include <iostream>
#include <xercesc/dom/DOM.hpp>

#include "ExpressionEvaluator/ExpressionEvaluator.h"

namespace GeoModelTools {

class XMLHandlerStore;

class XMLHandler {
public:
     XMLHandler(const std::string& n);
     virtual ~XMLHandler() {}
     const std::string& GetName() const {return m_name;}
     virtual void ElementHandle()=0;
     virtual void Handle(xercesc::DOMNode *t)
     {
          SetCurrentElement(t);
          ElementHandle();
     }
     void StopLoop(bool);
     bool IsLoopToBeStopped() const;
     virtual void postLoopHandling() {;}
protected:
     std::string m_name;
     bool m_stopLoop{false};

     static xercesc::DOMNode *s_currentElement;
     static void SetCurrentElement(xercesc::DOMNode *t) {s_currentElement=t;}
     static xercesc::DOMNode *GetCurrentElement() {return s_currentElement;}

     bool isAttribute(const std::string& attributeName) const;

     std::string getAttribute(const std::string& attributeName, bool& attrIsPresent) const;
     std::string getAttributeAsString(const std::string& attributeName) const;
     double getAttributeAsDouble(const std::string& attributeName) const;
     float getAttributeAsFloat(const std::string& attributeName) const;
     int getAttributeAsInt(const std::string& attributeName) const;
     long getAttributeAsLong(const std::string& attributeName) const;
     std::vector<double> getAttributeAsVector(const std::string& attributeName) const;
     std::vector<int> getAttributeAsIntVector(const std::string& attributeName) const;
     std::string getAttributeAsString(const std::string& attributeName, bool& attrIsPresent) const;
     double getAttributeAsDouble(const std::string& attributeName, bool& attrIsPresent) const;
     float getAttributeAsFloat(const std::string& attributeName, bool& attrIsPresent) const;
     int getAttributeAsInt(const std::string& attributeName, bool& attrIsPresent) const;
     long getAttributeAsLong(const std::string& attributeName, bool& attrIsPresent) const;
     std::vector<double> getAttributeAsVector(const std::string& attributeName, bool& attrIsPresent) const;
     std::vector<int> getAttributeAsIntVector(const std::string& attributeName, bool& attrIsPresent) const;
     
     
     std::string getAttributeAsString(const std::string& attributeName, const std::string& fallBackVal) const;
     double getAttributeAsDouble(const std::string& attributeName, const double fallBackVal) const;
    
     float getAttributeAsFloat(const std::string& attributeName, const float fallBackVal) const;
     
     int getAttributeAsInt(const std::string& attributeName, const int fallBackVal) const;
    
     long getAttributeAsLong(const std::string& attributeName, const long fallBackVal) const;

    
     std::vector<double> getAttributeAsVector(const std::string& attributeName, 
                                             const std::vector<double>& fallBackVal) const;
   
     std::vector<int> getAttributeAsIntVector(const std::string& attributeName, 
                                              const std::vector<int>& fallBackVal) const;

     static bool s_printFlag;

     ExpressionEvaluator* Evaluator() const {
          static ExpressionEvaluator* eval=ExpressionEvaluator::GetEvaluator();
          return eval;
     }

};

}  // end namespace

#endif

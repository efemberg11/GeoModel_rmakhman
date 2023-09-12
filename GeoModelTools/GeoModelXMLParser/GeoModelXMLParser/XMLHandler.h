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
	XMLHandler(std::string n);
	virtual ~XMLHandler() {}
	std::string GetName() {return m_name;}
	virtual void ElementHandle()=0;
	virtual void Handle(xercesc::DOMNode *t)
	{
		SetCurrentElement(t);
		ElementHandle();
	}
	void StopLoop(bool);
	bool IsLoopToBeStopped();
	virtual void postLoopHandling() {;}
protected:
	std::string m_name;
	bool m_stopLoop;

	static xercesc::DOMNode *s_currentElement;
	static void SetCurrentElement(xercesc::DOMNode *t) {s_currentElement=t;}
	static xercesc::DOMNode *GetCurrentElement() {return s_currentElement;}

	bool isAttribute(const std::string) const;

	std::string getAttribute(const std::string, bool&) const;
	std::string getAttributeAsString(const std::string) const;
	double getAttributeAsDouble(const std::string) const;
	float getAttributeAsFloat(const std::string) const;
	int getAttributeAsInt(const std::string) const;
	long getAttributeAsLong(const std::string) const;
	std::vector<double> getAttributeAsVector(const std::string) const;
	std::vector<int> getAttributeAsIntVector(const std::string) const;
	std::string getAttributeAsString(const std::string, bool&) const;
	double getAttributeAsDouble(const std::string, bool&) const;
  	float getAttributeAsFloat(const std::string, bool&) const;
	int getAttributeAsInt(const std::string, bool&) const;
  	long getAttributeAsLong(const std::string, bool&) const;
	std::vector<double> getAttributeAsVector(const std::string, bool&) const;
	std::vector<int> getAttributeAsIntVector(const std::string, bool&) const;
	std::string getAttributeAsString(const std::string, const std::string) const;
	double getAttributeAsDouble(const std::string, const double) const;
  	float getAttributeAsFloat(const std::string, const float) const;
	int getAttributeAsInt(const std::string, const int) const;
  	long getAttributeAsLong(const std::string, const long) const;
	std::vector<double> getAttributeAsVector(const std::string, const std::vector<double>) const;
	std::vector<int> getAttributeAsIntVector(const std::string, const std::vector<int>) const;
	static bool s_printFlag;

	ExpressionEvaluator* Evaluator() const
	{
		static ExpressionEvaluator* eval=ExpressionEvaluator::GetEvaluator();
		return eval;
	}
	static std::vector<std::string>& tokenize(const std::string&,const std::string&);
private:
	void RegisterToStore();
};

}  // end namespace

#endif

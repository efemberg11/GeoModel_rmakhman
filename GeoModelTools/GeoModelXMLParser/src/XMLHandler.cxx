/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXMLParser/XMLHandler.h"
#include "GeoModelXMLParser/XMLHandlerStore.h"
#include "GeoModelHelpers/StringUtils.h"
#include "GeoModelHelpers/throwExcept.h"
#include "ExpressionEvaluator/ExpressionEvaluator.h"

#include <vector>

using namespace xercesc;
using namespace GeoModelTools;

DOMNode* XMLHandler::s_currentElement=0;
bool XMLHandler::s_printFlag=false;

XMLHandler::XMLHandler(std::string n):m_name(n)  {
//    std::cout<< " creating new handler "<<n<<std::endl;
    m_stopLoop=false;
    RegisterToStore();
}

void XMLHandler::RegisterToStore() 
{
    XMLHandlerStore::GetHandlerStore()->RegisterHandler(this);
}

void XMLHandler::StopLoop(bool v)
{
    m_stopLoop=v;
}
bool XMLHandler::IsLoopToBeStopped()
{
    return m_stopLoop;
}
bool XMLHandler::isAttribute(const std::string name) const
{
    bool res;
    std::string temp=getAttribute(name,res);
    return res;
}
std::string XMLHandler::getAttribute(const std::string name, bool& isPresent) const
{
    std::string retValue="";
    isPresent=false;
    if (s_currentElement->hasAttributes()) {
        DOMNamedNodeMap *pAttributes = s_currentElement->getAttributes();
        DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->getNamedItem(XMLString::transcode(name.c_str()));
        if (pAttributeNode) {

            char* val=XMLString::transcode(pAttributeNode->getValue());
          if (val) {
            
            isPresent=true;
            retValue=val;
            XMLString::release(&val);
            return retValue;
          }
        }
        else return retValue;
            
    }
    return retValue;
}
std::string XMLHandler::getAttributeAsString(const std::string name) const
{
    bool isPresent{false};
    std::string temp=getAttribute(name,isPresent);
    if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
    return temp;
}
double XMLHandler::getAttributeAsDouble(const std::string name) const
{
	double res=0.;
	bool isPresent{false};
	std::string temp=getAttribute(name,isPresent);
	if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
	return res;
}
float XMLHandler::getAttributeAsFloat(const std::string name) const
{
	float res=0.;
	bool isPresent{false};
	std::string temp=getAttribute(name,isPresent);
    if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
	return res;
}
int XMLHandler::getAttributeAsInt(const std::string name) const
{
	int res=0;
	bool isPresent{false};
	std::string temp=getAttribute(name,isPresent);
    if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
	return res;
}
long XMLHandler::getAttributeAsLong(const std::string name) const
{
	long res=0;
	bool isPresent{false};
	std::string temp=getAttribute(name,isPresent);
    if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
	return res;
}
std::vector<double> XMLHandler::getAttributeAsVector(const std::string name) const
{
	bool isPresent{false};
	std::vector<double> vect;
	std::string temp=getAttribute(name,isPresent);
	if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	std::vector<std::string> v= GeoStrUtils::tokenize(temp, ";");
	for (unsigned int i=0;i<v.size();i++) {
		vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
	}
	return vect;
}

std::vector<int> XMLHandler::getAttributeAsIntVector(const std::string name) const {
	bool isPresent{false};
	std::vector<int> vect;
	std::string temp=getAttribute(name,isPresent);
	if (!isPresent) {
        THROW_EXCEPTION("--> Attribute '" << name << "' is not present! Check your data!");
    }
	std::vector<std::string> v= GeoStrUtils::tokenize(temp, ";");
	for (unsigned int i=0;i<v.size();i++) {
		vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
	}
	return vect;
}

std::string XMLHandler::getAttributeAsString(const std::string name, bool& isPresent) const {
        std::string temp=getAttribute(name,isPresent);
        return temp;
} 
double XMLHandler::getAttributeAsDouble(const std::string name, bool& isPresent) const {
    double res=0.;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
    }
    return res;
}
float XMLHandler::getAttributeAsFloat(const std::string name, bool& isPresent) const {
    float res=0.;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
		res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
    }
    return res;
}
int XMLHandler::getAttributeAsInt(const std::string name, bool& isPresent) const {
    int res=0;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
    }
    return res;
}
long XMLHandler::getAttributeAsLong(const std::string name, bool& isPresent) const {
    long res=0;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
    }
    return res;
}
std::vector<double> XMLHandler::getAttributeAsVector(const std::string name, bool& isPresent) const {
    std::vector<double> vect;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) 
    {
        std::vector<std::string> v= GeoStrUtils::tokenize(temp, ";");
        for (unsigned int i=0;i<v.size();i++) {
            vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
        }
    }
    return vect;
}

std::vector<int> XMLHandler::getAttributeAsIntVector(const std::string name, bool& isPresent) const {
        std::vector<int> vect;
        std::string temp=getAttribute(name,isPresent);
        if (isPresent) {
			std::vector<std::string> v= GeoStrUtils::tokenize(temp, ";");
			for (unsigned int i=0;i<v.size();i++) {
				vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
			}
        }
        return vect;
}

std::string XMLHandler::getAttributeAsString(const std::string name, const std::string def) const {
    bool isPresent{false};
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) return temp;
    else return def;
}
double XMLHandler::getAttributeAsDouble(const std::string name, const double def) const {
    bool isPresent{false};
    double res=0.;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
        return res;
    }
    return def;
}
float XMLHandler::getAttributeAsFloat(const std::string name, const float def) const {
    bool isPresent{false};
    float res=0.;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
        return res;
    }
    return def;
}
int XMLHandler::getAttributeAsInt(const std::string name, const int def) const {
    bool isPresent{false};
    int res=0;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
        return res;
    }
    return def;
}
long XMLHandler::getAttributeAsLong(const std::string name, const long def) const {
    bool isPresent{false};
    long res=0;
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        res=ExpressionEvaluator::GetEvaluator()->Eval(temp.c_str());
        return res;
    }
    return def;
}
std::vector<double> XMLHandler::getAttributeAsVector(const std::string name, const std::vector<double> def) const {
    bool isPresent{false};
    std::vector<double> vect{};
    std::string temp=getAttribute(name,isPresent);
    if (isPresent) {
        std::vector<std::string> v= GeoStrUtils::tokenize(temp,";");
        for (unsigned int i=0;i<v.size();i++){
            vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
        }
        return vect;
    }
    return def;
}

std::vector<int> XMLHandler::getAttributeAsIntVector(const std::string name, const std::vector<int> def) const {
	bool isPresent{false};
	std::vector<int> vect;
	std::string temp=getAttribute(name,isPresent);
	if (isPresent) {
		std::vector<std::string> v= GeoStrUtils::tokenize(temp, ";");
		for (unsigned int i=0;i<v.size();i++) {
			vect.push_back(ExpressionEvaluator::GetEvaluator()->Eval(v[i].c_str()));
		}
		return vect;
	}
	return def;
}

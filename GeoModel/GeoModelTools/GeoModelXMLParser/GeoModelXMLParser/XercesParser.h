/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef XercesParser_H
#define XercesParser_H

#include <string>
#include "GeoModelXMLParser/IAGDDParser.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

//using namespace xercesc;

namespace GeoModelTools {

class XercesParser: public IAGDDParser {
public:
	XercesParser();
	XercesParser(std::string);
	~XercesParser();
	bool ParseFile(std::string);
	bool ParseFileAndNavigate(std::string);
	bool ParseString(std::string);
	bool ParseStringAndNavigate(std::string);
	bool WriteToFile(std::string);
	void navigateTree();
	static void elementLoop();
	static void elementLoop(xercesc::DOMNode*);
	static xercesc::DOMNode* GetCurrentElement() {return s_currentElement;}
	bool Initialize();
	bool Finalize();
private:
        xercesc::DOMDocument *m_doc;
	
	xercesc::XercesDOMParser *m_parser;
	
	bool m_initialized;
protected:
	static xercesc::DOMNode *s_currentElement;
};

}  // end namespace

#endif

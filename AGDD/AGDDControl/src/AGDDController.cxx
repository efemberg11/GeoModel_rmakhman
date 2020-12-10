/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#include "AGDDControl/AGDDController.h"
#include "AGDDKernel/AGDDBuilder.h"
#include "GeoModelXMLParser/IAGDDParser.h"
#include "GeoModelXMLParser/XercesParser.h"
#include "AGDDControl/AGDD2GeoModelBuilder.h"
#include "AGDDKernel/AGDDVolumeStore.h"
#include "AGDDKernel/AGDDSectionStore.h"
#include "AGDDKernel/AGDDSection.h"
#include "AGDDKernel/AGDDVolume.h"
#include "AGDDKernel/AGDDPositioner.h"
#include "AGDDKernel/AliasStore.h"

#include "GeoModelKernel/GeoVDetectorManager.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoPVConstLink.h"

#include "AGDDHandlers/HandlerList.h"

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

std::vector<const GeoLogVol*> volumeMap;

void navigateVolumeContents(const GeoVPhysVol *pv, unsigned int ilev)
{
	const GeoLogVol *cvl = pv->getLogVol();
	std::string vname = cvl->getName();
	if (std::find(volumeMap.begin(),volumeMap.end(),cvl)!=volumeMap.end())
	{
	}
	else
	{
		volumeMap.push_back(cvl);
	}
// 	for (unsigned int i=0;i<ilev;i++)
// 			std::cout<<"\t";
// 	std::cout<<"----- volume "<<vname<<std::endl;
	
	unsigned int ivol=pv->getNChildVols();
	for (unsigned int i=0;i<ivol;i++)
	{
		const GeoVPhysVol *child=&(*(pv->getChildVol(i)));
		navigateVolumeContents(child,ilev+1);
	}
}

AGDDController::~AGDDController()
{
	if (m_theParser) delete m_theParser;
	if (m_theBuilder) delete m_theBuilder;
}

AGDDController::AGDDController():m_theBuilder(0),m_locked(false),m_disableSections(false),
								m_printLevel(0)
{
//	m_theParser=new AMDBParser;
	std::cout<<" is it going through here?"<<std::endl;
	localInitialization();
	m_theParser=new XercesParser;
	m_theBuilder=new AGDD2GeoModelBuilder;
}

void AGDDController::SetBuilder(AGDDBuilder *b) 
{
	m_theBuilder=b;
}
void AGDDController::SetParser(IAGDDParser *b) 
{
	m_theParser=b;
}
AGDDBuilder* AGDDController::GetBuilder() 
{
	return m_theBuilder;
}
IAGDDParser* AGDDController::GetParser() 
{
	return m_theParser;
}
void AGDDController::AddFile(std::string fName) 
{
	m_filesToParse.push_back(fName);
}
void AGDDController::AddSection(std::string section) 
{
	m_sectionsToBuild.push_back(section);
}
void AGDDController::AddVolume(std::string section) 
{
	m_volumesToBuild.push_back(section);
}


void AGDDController::ParseFiles()
{
	if (!m_theParser) m_theParser=new XercesParser;
	for (unsigned int i=0;i<m_filesToParse.size();i++)
		m_theParser->ParseFileAndNavigate(m_filesToParse[i]);
}

void AGDDController::BuildSections()
{
	for (unsigned int i=0;i<m_sectionsToBuild.size();i++)
	{
		//if (m_printLevel) 
		std::cout<< " -----> now building section "<<m_sectionsToBuild[i]<<std::endl;
		m_theBuilder->BuildFromSection(m_sectionsToBuild[i]);
	} 
}

void AGDDController::BuildAllSections()
{
	AGDDSectionStore* ss=AGDDSectionStore::GetSectionStore();
	for (sectionIterator it=ss->SectionBegin();it!=ss->SectionEnd();it++)
	{
		std::string fake=(it->second)->TopVolume();
		if (fake!="useless") m_theBuilder->BuildFromSection(it->first);
	}
}

void AGDDController::BuildVolumes()
{
	for (unsigned int i=0;i<m_volumesToBuild.size();i++)
		m_theBuilder->BuildFromVolume(m_volumesToBuild[i]);
}

void AGDDController::BuildAll()
{
		BuildVolumes();
		BuildSections();
}

void AGDDController::PrintSections() const
{
	AGDDSectionStore *ss=AGDDSectionStore::GetSectionStore();
  	ss->PrintAllSections();
}

void AGDDController::ParseString(std::string s)
{
	if (!m_theParser) m_theParser=new XercesParser;
	m_theParser->ParseStringAndNavigate(s);
}

bool AGDDController::WriteAGDDtoDBFile(std::string s)
{
	if (!m_theParser)
	{
		std::cout<< "  -----> asking for a dump without defined parser makes no sense - crashing!" <<std::endl;
		return false;
	}
	else {
		m_theParser->WriteToFile(s);
		return true;
	}
}

AGDDController* AGDDController::GetController()
{
	std::cout<<" this is AGDDController::GetController()"<<std::endl;
	static AGDDController* theController=new AGDDController;
	return theController;
}

void AGDDController::UseGeoModelDetector(std::string name)
{

}

void AGDDController::PrintVolumeHierarchy(std::string name, int ilevel)
{
	AGDDVolumeStore *vs=AGDDVolumeStore::GetVolumeStore();
	AGDDVolume *vol=vs->GetVolume(name);
	int currentLevel=ilevel+1;
	for (int i=0;i<ilevel;i++) std::cout<<"    ";
	std::cout<<"|  "<<name<<std::endl;
	if (!vol->NrOfDaughter()) return;
	std::vector<std::string> nameVec;
	std::vector<std::string>::iterator it;
	for (int i=0;i<vol->NrOfDaughter();i++)
	{
		std::string nameV=vol->GetDaughter(i)->GetVolume()->GetName();
		it=find(nameVec.begin(),nameVec.end(),nameV);
		if (it!=nameVec.end()) continue;
		nameVec.push_back(nameV);
		PrintVolumeHierarchy(nameV,currentLevel);
	}
}

void AGDDController::Clean()
{
	delete m_theParser;
	m_theParser=0;
	
	m_filesToParse.clear();
	m_sectionsToBuild.clear();
	m_volumesToBuild.clear();
	m_structuresToBuild.clear();
	
	AGDDSectionStore::GetSectionStore()->Clean();
	AGDDVolumeStore::GetVolumeStore()->Clean();
}

void AGDDController::localInitialization()
{
	std::cout<<" this is localInitialization() "<<std::endl;
	addHandler(new AGDDHandler("AGDD"));
	addHandler(new BoltHandler("Bolt"));
	addHandler(new IbeamHandler("Ibeam"));
	addHandler(new UbeamHandler("Ubeam"));
	addHandler(new addmaterialHandler("addmaterial"));
	addHandler(new aliasHandler("alias"));
	addHandler(new arrayHandler("array"));
	addHandler(new boxHandler("box"));
	addHandler(new colorHandler("color"));
	
	addHandler(new compositeHandler("composite"));
	addHandler(new compositionHandler("composition"));
	addHandler(new consHandler("cons"));
	addHandler(new defaultsHandler("defaults"));
	addHandler(new elcylHandler("elcyl"));
	addHandler(new elementHandler("element"));
	addHandler(new foreachHandler("foreach"));
	addHandler(new fractionmassHandler("fractionmass"));
	addHandler(new gvxyHandler("gvxy"));
	
	addHandler(new gvxysxHandler("gvxysx"));
	addHandler(new gvxy_pointHandler("gvxy_point"));
	addHandler(new gvxysxyHandler("gvxysxy"));
	addHandler(new intersectionHandler("intersection"));
	addHandler(new materialHandler("material"));
	addHandler(new materialsHandler("materials"));
	addHandler(new mposPhiHandler("mposPhi"));
	addHandler(new mposWedgeHandler("mposWedge"));
	
	addHandler(new msgHandler("msg"));
	addHandler(new natomsHandler("natoms"));
	addHandler(new pconHandler("pcon"));
	addHandler(new pgonHandler("pgon"));
	addHandler(new polyplaneHandler("polyplane"));
	addHandler(new posRPhiZHandler("posRPhiZ"));
	addHandler(new posXYZHandler("posXYZ"));
	addHandler(new ringHandler("ring"));
	
	
	addHandler(new sectionHandler("section"));
	addHandler(new sectorHandler("sector"));
	addHandler(new snakeHandler("snake"));
	addHandler(new snake_pointHandler("snake_point"));
	addHandler(new subtractionHandler("subtraction"));
	addHandler(new trdHandler("trd"));
	addHandler(new tubsHandler("tubs"));
	addHandler(new unionHandler("union"));
	addHandler(new varHandler("var"));
	addHandler(new versionHandler("version"));
	addHandler(new importHandler("import"));
	
	addHandler(new chamberPositionerHandler("chamberPosition"));

	AliasStore::GetAliasList()->AddAlias("Aluminium1","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium2","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium3","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium4","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium5","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium6","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium7","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium8","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Aluminium9","std::Aluminium");
	
	AliasStore::GetAliasList()->AddAlias("Iron1","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron2","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron3","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron4","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron5","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron6","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron7","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron8","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Iron9","std::Iron");
	
	AliasStore::GetAliasList()->AddAlias("Iron","std::Iron");
	AliasStore::GetAliasList()->AddAlias("Copper","std::Copper");
	AliasStore::GetAliasList()->AddAlias("Lead","std::Lead");
	AliasStore::GetAliasList()->AddAlias("Aluminium","std::Aluminium");
	AliasStore::GetAliasList()->AddAlias("Carbon","std::Carbon");
	
	AliasStore::GetAliasList()->AddAlias("Brass","sct::Brass");
	AliasStore::GetAliasList()->AddAlias("PolyBoron5percent","std::Polyethylene");	
	AliasStore::GetAliasList()->AddAlias("Polylithium","shield::Polylithium");
	AliasStore::GetAliasList()->AddAlias("ShieldBrass","shield::ShieldBrass");
	AliasStore::GetAliasList()->AddAlias("ShieldIron","shield::ShieldIron");
	AliasStore::GetAliasList()->AddAlias("ShieldSteel","shield::ShieldSteel");
	AliasStore::GetAliasList()->AddAlias("PolyBoronB4C","shield::PolyboronB4C");
	AliasStore::GetAliasList()->AddAlias("PolyBoronB2O3","shield::PolyboronB2O3");
	AliasStore::GetAliasList()->AddAlias("PolyBoronH3B03","shield::PolyboronH3B03");
	AliasStore::GetAliasList()->AddAlias("PolyBoron207HD5","shield::Polyboron207HD5");
}



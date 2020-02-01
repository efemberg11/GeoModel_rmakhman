/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoElementHandler.h"
#include "GeoDummyHandler.h"
#include "GeoMaterialHandler.h"
#include "GeoMatComponentHandler.h"
#include "GeoNamespaceHandler.h"

#include "XMLParser/XercesParser.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/Units.h"

#include <iomanip>

GeoXmlMatManager* GeoXmlMatManager::s_instance{nullptr};

class Impl {
public:
  Impl(GeoXmlMatManager* man)
    : m_elementHandler("Element",man)
    , m_elementsHandler("GeoElements")
    , m_materialHandler("Material",man)
    , m_matComponentHandler("Component",man)
    , m_namespaceHandler("Namespace",man)
    , m_materialsHandler("GeoMaterials")
  {}
  ~Impl(){}
  
  //______________________________________________________
  // Handlers for the XML parser
  GeoElementHandler      m_elementHandler;
  GeoDummyHandler        m_elementsHandler;
  GeoMaterialHandler     m_materialHandler;
  GeoMatComponentHandler m_matComponentHandler;
  GeoNamespaceHandler    m_namespaceHandler;
  GeoDummyHandler        m_materialsHandler;

  // Map of elements indexed by Name
  typedef std::map<std::string, GeoElement*> ElementMap;
  ElementMap m_elements;

  // Map of materials indexed by fully qualified name Namespace::MaterialName
  typedef std::map<std::string, GeoMaterial*> MaterialMap;
  MaterialMap m_materials;

  //______________________________________________________ 
  // Auxiliary variables for building new material
  std::string m_currentNamespace{""};
  GeoMaterial* m_currentMaterial{nullptr};
  bool m_firstComponent{true};
  bool m_hasSubMaterial{false};
  bool m_calculateFraction{false};
  double m_totalFraction{0.};
  std::vector<GeoElement*> m_elementComponents;
  std::vector<double>      m_elementFractions;
  
};


GeoXmlMatManager::GeoXmlMatManager()
  : m_pImpl(new Impl(this))
{
  XercesParser xercesParser;
  xercesParser.ParseFileAndNavigate("elements.xml");
  xercesParser.ParseFileAndNavigate("materials.xml");
  // The last material needs to be locked
  lockMaterial();
  // Build special materials
  buildSpecialMaterials();
}

GeoXmlMatManager::~GeoXmlMatManager()
{
  delete m_pImpl;
}

const GeoXmlMatManager* GeoXmlMatManager::getManager()
{
  return (s_instance ? s_instance : new GeoXmlMatManager());
}

GeoMaterial* GeoXmlMatManager::getMaterial(const std::string& name)
{
  auto materialIt = m_pImpl->m_materials.find(name);
  return materialIt==m_pImpl->m_materials.end() ? nullptr : materialIt->second;
}

const GeoMaterial* GeoXmlMatManager::getMaterial(const std::string & name) const
{
  auto materialIt = m_pImpl->m_materials.find(name);
  return materialIt==m_pImpl->m_materials.end() ? nullptr : materialIt->second;  
}

GeoElement* GeoXmlMatManager::getElement(const std::string & name)
{
  auto elementIt = m_pImpl->m_elements.find(name);
  return elementIt==m_pImpl->m_elements.end() ? nullptr : elementIt->second;
}

const GeoElement* GeoXmlMatManager::getElement(const std::string & name) const
{
  auto elementIt = m_pImpl->m_elements.find(name);
  return elementIt==m_pImpl->m_elements.end() ? nullptr : elementIt->second;
}

GeoElement* GeoXmlMatManager::getElement(unsigned int atomicNumber)
{
  for(auto element : m_pImpl->m_elements) {
    if((unsigned)(element.second->getZ())==atomicNumber) {
      return element.second;
    }
  }
  return nullptr;
}

const GeoElement* GeoXmlMatManager::getElement(unsigned int atomicNumber) const
{
  for(auto element : m_pImpl->m_elements) {
    if((unsigned)(element.second->getZ())==atomicNumber) {
      return element.second;
    }
  }
  return nullptr;
}

std::ostream& GeoXmlMatManager::printAll(std::ostream& o) const
{
  o << "============Material Manager Element List========================" << std::endl;

  for (auto element : m_pImpl->m_elements) {
    GeoElement* el = element.second;
    o << el->getSymbol() << '\t' 
      << el->getZ() << '\t' 
      << el->getA() * (GeoModelKernelUnits::mole / GeoModelKernelUnits::gram) << '\t' 
      << el->getName() << std::endl;    
  }

  for (auto material : m_pImpl->m_materials) {
    GeoMaterial* mat = material.second;
    o << "Material: " << material.first 
      <<  " Density " << mat->getDensity() * (GeoModelKernelUnits::cm3 / GeoModelKernelUnits::gram)  
      << std::endl;
    for (size_t i = 0; i< mat->getNumElements(); ++i) {
      o << " ***** ***** " 
	<< int (mat->getFraction(i)*100) << "% \t"  
	<< mat->getElement(i)->getName() << std::endl;
    }
  }  	  	
  return o;
}

void GeoXmlMatManager::addElement(const std::string &name
				  , const std::string &symbol
				  , double z
				  , double a)
{
  GeoElement* newElement = new GeoElement(name,symbol,z,a*(GeoModelKernelUnits::gram/GeoModelKernelUnits::mole));
  newElement->ref();
  auto result = m_pImpl->m_elements.insert(std::pair<std::string,GeoElement*>(name,newElement));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine element" + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void GeoXmlMatManager::addNamespace(const std::string &name)
{
  lockMaterial();
  m_pImpl->m_currentNamespace = name;
}

void GeoXmlMatManager::addMaterial(const std::string &name
				   , double density)
{
  if(m_pImpl->m_currentNamespace.empty()) {
    std::string errorMessage("Material " + name + " has not beed defined within a namespace");
    throw std::runtime_error(errorMessage.c_str());
  }
  lockMaterial();
  m_pImpl->m_currentMaterial = new GeoMaterial(name, density * (GeoModelKernelUnits::gram / GeoModelKernelUnits::cm3));
  auto result = m_pImpl->m_materials.insert(std::pair<std::string,GeoMaterial*>(m_pImpl->m_currentNamespace+"::"+name, m_pImpl->m_currentMaterial));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine material " + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void GeoXmlMatManager::addMatComponent(const std::string &name
				       , double fraction)
{
  if(m_pImpl->m_firstComponent) {
    m_pImpl->m_firstComponent = false;
    if(fraction>=1.) {
      m_pImpl->m_calculateFraction = true;
    }
  }

  size_t separatorPos = name.find("::");
  if(separatorPos==std::string::npos) {
    // New components is an element
    auto elementIt = m_pImpl->m_elements.find(name);
    if(elementIt==m_pImpl->m_elements.end()) {
      std::string errorMessage("Unknown element " + name + " used in the definition of material " + m_pImpl->m_currentMaterial->getName());
      throw std::runtime_error(errorMessage.c_str());
    }
    GeoElement* element = elementIt->second;

    if(m_pImpl->m_calculateFraction) {
      m_pImpl->m_totalFraction += fraction*element->getA();
      m_pImpl->m_elementComponents.push_back(element);
      m_pImpl->m_elementFractions.push_back(fraction);
    }
    else {
      m_pImpl->m_currentMaterial->add(element,fraction);
    }
  }
  else {
    // New component is a material
    auto materialIt = m_pImpl->m_materials.find(name);
    if(materialIt==m_pImpl->m_materials.end()) {
      std::string errorMessage("Unknown material " + name + " used in the components name " + name);
      throw std::runtime_error(errorMessage.c_str());
    }
    GeoMaterial* material = materialIt->second;
    m_pImpl->m_hasSubMaterial = true;
    m_pImpl->m_currentMaterial->add(material, fraction);
  }		
}

void GeoXmlMatManager::lockMaterial()
{
  if(!m_pImpl->m_currentMaterial) return;
  
  if(m_pImpl->m_calculateFraction && m_pImpl->m_hasSubMaterial && m_pImpl->m_elementComponents.size()>0) {
    std::string errorMessage("Wrong definition of the material " + m_pImpl->m_currentMaterial->getName() + ". The exact fraction for elements must be indicated");
    throw std::runtime_error(errorMessage.c_str());
  }

  if(m_pImpl->m_calculateFraction && !m_pImpl->m_elementComponents.empty()) {
    const double inv_totalFraction = 1. / m_pImpl->m_totalFraction;
    for(unsigned i=0; i<m_pImpl->m_elementComponents.size(); i++) {
      m_pImpl->m_currentMaterial->add(m_pImpl->m_elementComponents[i],m_pImpl->m_elementFractions[i]*m_pImpl->m_elementComponents[i]->getA() * inv_totalFraction);
    }
  }
  m_pImpl->m_currentMaterial->lock();
  m_pImpl->m_currentMaterial->ref();

  // Get ready for building next material
  m_pImpl->m_currentMaterial = nullptr;
  m_pImpl->m_firstComponent = true;
  m_pImpl->m_hasSubMaterial = false;
  m_pImpl->m_calculateFraction = false;
  m_pImpl->m_totalFraction = 0.;
  m_pImpl->m_elementComponents.clear();
  m_pImpl->m_elementFractions.clear();
}

void GeoXmlMatManager::dump()
{
  const char separator(' ');
  std::cout << "*******************************************************" << std::endl;
  std::cout << "***               GeoXmlMaterialManager             ***" << std::endl;
  std::cout << "*******************************************************" << std::endl;
  std::cout << "*** Elements:" << std::endl;

  for(const auto& item : m_pImpl->m_elements) {
    GeoElement* element = item.second;
    std::cout << "* "<<  std::left << std::setw(15) << std::setfill(separator) << element->getName()
	      << " " << std::left << std::setw(5) << std::setfill(separator) << element->getSymbol()
	      << " " <<  std::left << std::setw(15) << std::setfill(separator) << element->getZ()
	      << " " <<  std::left << std::setw(5) << std::setfill(separator) << element->getA()
	      << std::endl;
  }
  std::cout << "*******************************************************" << std::endl;

}

void GeoXmlMatManager::buildSpecialMaterials()
{
  // Ether
  GeoElement* ethElement = new GeoElement("Ether","ET",500.0,0.0);
  ethElement->ref();
  m_pImpl->m_elements.insert(std::pair<std::string,GeoElement*>("Ether",ethElement));
  GeoMaterial* ether = new GeoMaterial("Ether",0.0);
  ether->add(ethElement,1.);
  ether->lock();
  m_pImpl->m_materials.insert(std::pair<std::string,GeoMaterial*>("special::Ether", ether));
  // HyperUranium
  GeoMaterial* hu = new GeoMaterial("HyperUranium",0.0);
  hu->add(ethElement,1.);
  hu->lock();
  m_pImpl->m_materials.insert(std::pair<std::string,GeoMaterial*>("special::HyperUranium", hu));
}

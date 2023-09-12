/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoMaterialManager.h"

#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/Units.h"

#include <cstdlib>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <vector>

GeoMaterialManager* GeoMaterialManager::s_instance{nullptr};

class Impl {
public:
  Impl(GeoMaterialManager* /*man*/)
  {}
  ~Impl(){}

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


GeoMaterialManager::GeoMaterialManager()
  : m_pImpl(new Impl(this))
{
 
}

GeoMaterialManager::~GeoMaterialManager()
{
  delete m_pImpl;
}

const GeoMaterialManager* GeoMaterialManager::getManager()
{
  if(!s_instance) {
    try {
      s_instance = new GeoMaterialManager();
    }
    catch(std::runtime_error& ex) {
      std::cerr << ex.what() << std::endl;
      return nullptr;
    }
  }
  return s_instance;
}

const GeoMaterial* GeoMaterialManager::getMaterial(const std::string & name) const
{
  auto materialIt = m_pImpl->m_materials.find(name);
  return materialIt==m_pImpl->m_materials.end() ? nullptr : materialIt->second;  
}

const GeoElement* GeoMaterialManager::getElement(const std::string & name) const
{
  auto elementIt = m_pImpl->m_elements.find(name);
  return elementIt==m_pImpl->m_elements.end() ? nullptr : elementIt->second;
}

const GeoElement* GeoMaterialManager::getElement(unsigned int atomicNumber) const
{
  for(auto element : m_pImpl->m_elements) {
    if((unsigned)(element.second->getZ())==atomicNumber) {
      return element.second;
    }
  }
  return nullptr;
}

std::ostream& GeoMaterialManager::printAll(std::ostream& o) const
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

void GeoMaterialManager::addElement(const std::string &name
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

void GeoMaterialManager::addNamespace(const std::string &name)
{
  lockMaterial();
  m_pImpl->m_currentNamespace = name;
}

void GeoMaterialManager::addMaterial(const std::string &name
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

void GeoMaterialManager::addMatComponent(const std::string &name
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

void GeoMaterialManager::lockMaterial()
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

void GeoMaterialManager::dump()
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


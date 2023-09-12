/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelXml/MaterialManager.h"

#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/Units.h"

#include <cstdlib>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <vector>

MaterialManager* MaterialManager::s_instance{nullptr};

class Impl {
public:
  Impl(MaterialManager* man)
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


MaterialManager::MaterialManager()
  : m_pImpl(new Impl(this))
{
	// std::cout<<" this is the GMX MaterialManager being instantiated!"<<std::endl;
	// std::cout<<"     building special materials "<<std::endl;
	buildSpecialMaterials();
}

MaterialManager::~MaterialManager()
{
  delete m_pImpl;
}

MaterialManager* MaterialManager::getManager()
{
  if(!s_instance) {
    try {
      s_instance = new MaterialManager();
    }
    catch(std::runtime_error& ex) {
      std::cerr << ex.what() << std::endl;
      return nullptr;
    }
  }
  return s_instance;
}

const GeoMaterial* MaterialManager::getMaterial(const std::string & name) const
{
  //std::cout << "Material Manager: attempt at retrieving material : "<<name<<std::endl;
  // step 1 - check for simple name
  auto materialIt = m_pImpl->m_materials.find(name);
  if (materialIt!=m_pImpl->m_materials.end())
  {
    //std::cout<<" material "<<name<<" found!"<<std::endl;
    return materialIt->second;
  }
  // step 2 - check for material in namespace
  std::string nameNS=m_pImpl->m_currentNamespace+"::"+name;
  auto materialNSIt = m_pImpl->m_materials.find(nameNS);
  if (materialNSIt!=m_pImpl->m_materials.end())
  {
    //std::cout<<" material "<<nameNS<<" found!"<<std::endl;
    return materialNSIt->second;
  }

  // well, something is wrong...
  //std::cout<<"MaterialManager::getMaterial() Warning! could not find material "<<name<<std::endl;
  return nullptr;
}

const GeoElement* MaterialManager::getElement(const std::string & name) const
{
  //std::cout << "Material Manager: attempt at retrieving element : "<<name<<std::endl;
  auto elementIt = m_pImpl->m_elements.find(name);
  return elementIt==m_pImpl->m_elements.end() ? nullptr : elementIt->second;
}

const GeoElement* MaterialManager::getElement(unsigned int atomicNumber) const
{
  //std::cout << "Material Manager: attempt at retrieving material : "<<atomicNumber<<std::endl;
  for(auto element : m_pImpl->m_elements) {
    if((unsigned)(element.second->getZ())==atomicNumber) {
      return element.second;
    }
  }
  return nullptr;
}

void MaterialManager::printAll() const
{
  std::cout << "============Material Manager Element List========================" << std::endl;

  for (auto element : m_pImpl->m_elements) {
    GeoElement* el = element.second;
    std::cout << el->getSymbol() << '\t'
      << el->getZ() << '\t'
      << el->getA() * (GeoModelKernelUnits::mole / GeoModelKernelUnits::gram) << '\t'
      << el->getName() << std::endl;
  }

  for (auto material : m_pImpl->m_materials) {
    GeoMaterial* mat = material.second;
    std::cout << "Material: " << material.first
      <<  " Density " << mat->getDensity() * (GeoModelKernelUnits::cm3 / GeoModelKernelUnits::gram)
      << std::endl;
    for (size_t i = 0; i< mat->getNumElements(); ++i) {
      std::cout << " ***** ***** "
	     << int (mat->getFraction(i)*100) << "% \t"
	     << mat->getElement(i)->getName() << std::endl;
    }
  }
}

void MaterialManager::addElement(const std::string &name
				  , const std::string &symbol
				  , double z
				  , double a)
{
  //std::cout << "Material Manager: adding element : "<<name<<" "<<symbol<<" "<<z<<" "<<a<<std::endl;
  GeoElement* newElement = new GeoElement(name,symbol,z,a*(GeoModelKernelUnits::gram/GeoModelKernelUnits::mole));
  newElement->ref();
  auto result = m_pImpl->m_elements.insert(std::pair<std::string,GeoElement*>(name,newElement));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine element" + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void MaterialManager::addElement(GeoElement* el)
{
  std::string name=el->getName();
  //std::cout << "Material Manager: adding element : "<<name<<std::endl;
  GeoElement* newElement = el;
  newElement->ref();
  auto result = m_pImpl->m_elements.insert(std::pair<std::string,GeoElement*>(name,newElement));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine element" + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void MaterialManager::addNamespace(const std::string &name)
{
  //std::cout << "Material Manager: adding namespace : "<<name<<std::endl;
  lockMaterial();
  m_pImpl->m_currentNamespace = name;
}

void MaterialManager::addMaterial(const std::string &name
				   , double density)
{
  //std::cout << "Material Manager: adding material : "<<name<<" "<<density<<std::endl;
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

void MaterialManager::addMaterial(GeoMaterial* mat)
{
  const std::string name=mat->getName();
  //std::cout << "Material Manager: adding material : "<<name<<std::endl;
  if(m_pImpl->m_currentNamespace.empty()) {
    std::string errorMessage("Material " + name + " has not beed defined within a namespace");
    throw std::runtime_error(errorMessage.c_str());
  }
  lockMaterial();
  m_pImpl->m_currentMaterial = mat;
  auto result = m_pImpl->m_materials.insert(std::pair<std::string,GeoMaterial*>(m_pImpl->m_currentNamespace+"::"+name, m_pImpl->m_currentMaterial));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine material " + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

bool MaterialManager::isMaterialDefined(const std::string s) const
{
  if(m_pImpl->m_currentNamespace.empty()) {
    std::string errorMessage("Material " + s + " has not beed defined within a namespace");
    throw std::runtime_error(errorMessage.c_str());
  }
  return (m_pImpl->m_materials.find(s)!=m_pImpl->m_materials.end());
}

bool MaterialManager::isElementDefined(const std::string s) const
{
  return (m_pImpl->m_elements.find(s)!=m_pImpl->m_elements.end());
}

void MaterialManager::addMatComponent(const std::string &name
				       , double fraction)
{
  //std::cout << "Material Manager: adding material component : "<<name<<" "<<fraction<<std::endl;
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

void MaterialManager::lockMaterial()
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

void MaterialManager::dump()
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

void MaterialManager::buildSpecialMaterials()
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

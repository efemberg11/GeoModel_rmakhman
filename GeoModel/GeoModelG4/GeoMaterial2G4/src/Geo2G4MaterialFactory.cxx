/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoMaterial2G4/Geo2G4MaterialFactory.h"
#include "Geo2G4ElementFactory.h"
#include "GeoMaterial2G4/Geo2G4MatPropTableFactory.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoMaterial2G4/GeoExtendedMaterial.h"
#include "GeoMaterial2G4/GeoMaterialPropertiesTable.h"

#include "G4Material.hh"


// Geo2G4MaterialFactory::Geo2G4MaterialFactory(): m_msg("Geo2G4MaterialFactory")
Geo2G4MaterialFactory::Geo2G4MaterialFactory()
{
}

G4Material* Geo2G4MaterialFactory::Build(const GeoMaterial* theMat)
{
  static Geo2G4ElementFactory eFactory;
  Geo2G4MatPropTableFactory* tFactory = Geo2G4MatPropTableFactory::instance();

  //
  // Check if this material has already been defined.
  //
  std::string nam = theMat->getName();
  
  if(m_definedMaterials.find(nam) != m_definedMaterials.end())
    return m_definedMaterials[nam];
  int nelements = theMat->getNumElements();
    
  // Different actions depending whether we are dealing with
  // standard or extended materials

  const GeoExtendedMaterial* extMat = dynamic_cast<const GeoExtendedMaterial*>(theMat);
  G4Material* newmaterial = 0;
  
  if(extMat) {
    G4State state = kStateUndefined;

    switch(extMat->getState())
      {
      case stateUndefined:
        state = kStateUndefined;
        break;
      case stateSolid:
        state = kStateSolid;
        break;
      case stateLiquid:
        state = kStateLiquid;
        break;
      case stateGas:
        state = kStateGas;
        break;
      default:
        break;
      }

    double temperature = extMat->getTemperature();
    double pressure = extMat->getPressure();

    newmaterial= new G4Material(nam,
                                extMat->getDensity(),
                                nelements,
                                state,
                                temperature,
                                pressure);

    // Build G4MaterialPropertiesTable if needed
    GeoMaterialPropertiesTable* geoPropTable = extMat->GetMaterialPropertiesTable();

    if(geoPropTable) {
      G4MaterialPropertiesTable* g4PropTable = tFactory->Build(geoPropTable);
      if(g4PropTable)
        newmaterial->SetMaterialPropertiesTable(g4PropTable);
    }
  }
  else
    newmaterial= new G4Material(nam,
                                theMat->getDensity(),
                                nelements);
      
  
  for (int ii = 0; ii< nelements; ii++)  {
    G4Element* theG4Ele = eFactory.Build(theMat->getElement(ii));
    newmaterial->AddElement(theG4Ele, theMat->getFraction(ii));
  }

  m_definedMaterials[theMat->getName()]=newmaterial;
    
//  std::cout <<" ...... DEBUG of m_definedMaterials --- START ---" <<std::endl;
//  for (auto& t : m_definedMaterials)
//     std::cout <<"GeoModelMaterialName: "<< t.first << " ---  G4MaterialName: "
//        << t.second->GetName() << std::endl;
//  std::cout <<" ...... DEBUG of m_definedMaterials --- END ---" <<std::endl;

  // Check if we have the situation when on GeoModel side two different
  // materials share the same name.
  // Print an INFO message if so.
  if(m_definedMatNames.find(nam)==m_definedMatNames.end())
    m_definedMatNames[nam] = theMat->getName();
  else if(m_definedMatNames[nam] != theMat->getName())
    std::cout << "!!! On GeoModel side two different materials share the name: " << std::endl;
  return newmaterial;
}

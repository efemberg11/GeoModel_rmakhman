/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModel2G4/ExtParameterisedVolumeBuilder.h"
#include "GeoModel2G4/Geo2G4AssemblyFactory.h"
#include "GeoModel2G4/Geo2G4AssemblyVolume.h"
#include "GeoModel2G4/Geo2G4LVFactory.h"
#include "GeoModel2G4/Geo2G4STParameterisation.h"
#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4ReflectionFactory.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVParameterised.hh"
#include "globals.hh"

#include "GeoModelKernel/GeoAccessVolAndSTAction.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include <iostream>

#include "GeoModel2G4/CLHEPtoEigenConverter.h"

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

ExtParameterisedVolumeBuilder::ExtParameterisedVolumeBuilder(std::string n):
  VolumeBuilder(n),
  m_getMatEther(true),
  m_matEther(0),m_matHypUr(0)
{
}

G4LogicalVolume* ExtParameterisedVolumeBuilder::Build(const PVConstLink theGeoPhysVolume) const
{
  PVConstLink theGeoPhysChild;
  const GeoSerialTransformer* serialTransformerChild=0;
  G4LogicalVolume* theG4LogChild;
  unsigned int numChildNodes;                      // number of child nodes (PV and ST)
  bool descend;                                    // flag to continue geo tree navigation
  bool serialExists = false;                       // flag for existence of ST among childs
  std::string nameChild;

  if(m_getMatEther) getMatEther();

  static Geo2G4LVFactory LVFactory;
  //std::cout<<"    ----->ExtParameterisedVolumeBuilder::Build()"<<std::endl;
  G4LogicalVolume* theG4LogVolume = LVFactory.Build(theGeoPhysVolume,descend);
  //std::cout<<"    ----->LVFactory built"<<std::endl;
  if(!descend) return theG4LogVolume;

  numChildNodes = theGeoPhysVolume->getNChildVolAndST();
  //std::cout<<"    ----->numChildNodes"<<numChildNodes<<std::endl;

  // *****************************************************************
  // **
  // ** If m_ST2Param flag is set:
  // ** Check if there's any serial transformer among child volumes
  // **
  // *****************************************************************

  if(m_paramOn)
    for(size_t counter1=0; counter1<numChildNodes; counter1++)
      {
        GeoAccessVolAndSTAction actionVolAndST(counter1);
        theGeoPhysVolume->exec(&actionVolAndST);

        if((serialTransformerChild=actionVolAndST.getSerialTransformer()))
          {
            nameChild = actionVolAndST.getName();
            serialExists = true;
            break;
          }
      }
  //if(G4VERBOSE>1) std::cout<<"    -----> Next steps:"<<std::endl;
  // ***************************************************************************
  // **                Next steps:
  // **
  // ** 1. If ST exists and numChildNodes==1, translate ST to G4 ST
  // **
  // ** 2. If ST exists and numChildNodes !=1, print information message and
  // **    translate ST to single placements as well as all other child volumes
  // **
  // ** 3. There's no ST - ok, nothing special ...
  // **
  // ***************************************************************************

  if(serialExists && (numChildNodes==1))
    {
      theGeoPhysChild = serialTransformerChild->getVolume();

      // Build the child
      if(!(theG4LogChild = Build(theGeoPhysChild))) return 0;

      if (nameChild == "ANON") nameChild=theG4LogChild->GetName();
      nameChild += "_Param";

      Geo2G4STParameterisation* stParameterisation = new Geo2G4STParameterisation(serialTransformerChild->getFunction(),
                                                                                  serialTransformerChild->getNCopies());

      G4VPhysicalVolume* pvParametrised __attribute__ ((unused)) = new G4PVParameterised(nameChild,
                                                                                         theG4LogChild,
                                                                                         theG4LogVolume,
                                                                                         kUndefined,
                                                                                         serialTransformerChild->getNCopies(),
                                                                                         stParameterisation);
    }
  else
    {
      if(serialExists)
        {
          std::string volName = theGeoPhysVolume->getLogVol()->getName();
          PrintSTInfo(volName);
        }

      GeoVolumeCursor av(theGeoPhysVolume);
      while (!av.atEnd())
        {
          int id = 16969;

          // Get child phys volume
          theGeoPhysChild = av.getVolume();
          // Get its transform
	  G4Transform3D theG4Position(Amg::EigenTransformToCLHEP(av.getTransform()));

	  std::optional<int> Qint =  av.getId();
          if(Qint) id = *Qint;
          if(m_matEther->getName()  == theGeoPhysChild->getLogVol()->getMaterial()->getName() || hasEnding(theGeoPhysChild->getLogVol()->getMaterial()->getName(), "Ether"))
            {
              Geo2G4AssemblyVolume* assembly = BuildAssembly(theGeoPhysChild);

              if(Qint)
                assembly->MakeImprint(theG4LogVolume,theG4Position,id);
              else
                assembly->MakeImprint(theG4LogVolume,theG4Position);
            }
          else if(m_matHypUr->getName()  == theGeoPhysChild->getLogVol()->getMaterial()->getName() )
            {
              Geo2G4AssemblyVolume* assembly = BuildAssembly(theGeoPhysChild);

              if(Qint)
                assembly->MakeImprint(theG4LogVolume,theG4Position,id,true);
              else
                assembly->MakeImprint(theG4LogVolume,theG4Position,0,true);
            }
          else
            {
              nameChild = av.getName();

              // Build the child
              if(!(theG4LogChild = Build(theGeoPhysChild))) return 0;

              if (nameChild == "ANON") nameChild=theG4LogChild->GetName();

              //G4PhysicalVolumesPair pvPair =
              G4ReflectionFactory::Instance()->Place(theG4Position,nameChild,theG4LogChild,theG4LogVolume,false,id);

              //TODO Optical volumes
              // if GeoModel volume is optical store it in the map
//              if(optical_volumes!=0)
//                {
//                  const GeoOpticalPhysVol* opticalGeoPhysChild =
//                    dynamic_cast < const GeoOpticalPhysVol* >(theGeoPhysChild.operator->());
//                  if(opticalGeoPhysChild)
//                    (*optical_volumes)[opticalGeoPhysChild] = pvPair.first;
//                }
            }

          av.next();
        }
    }

  return theG4LogVolume;
}



Geo2G4AssemblyVolume* ExtParameterisedVolumeBuilder::BuildAssembly(PVConstLink pv) const
{
  PVConstLink theGeoPhysChild;
  G4LogicalVolume* theG4LogChild = 0;
  Geo2G4AssemblyVolume* theG4AssemblyChild = 0;
  bool descend;                                    // flag to continue geo tree navigation

  if(m_getMatEther) getMatEther();

  static Geo2G4AssemblyFactory AssemblyFactory;

  Geo2G4AssemblyVolume* assemblyVolume = AssemblyFactory.Build(pv,descend);

  if(!descend) return assemblyVolume;

  // Loop over child volumes and add them to the Geo2G4AssemblyVolume
  GeoVolumeCursor av(pv);
  while (!av.atEnd())
    {
      theGeoPhysChild = av.getVolume();
      std::string nameChild = av.getName();

      std::string strVolume = std::string("Volume ") + nameChild + " ("
        + theGeoPhysChild->getLogVol()->getName() + ")";

      // Check if it is an assembly
      if(m_matEther->getName() == theGeoPhysChild->getLogVol()->getMaterial()->getName() ||
         m_matHypUr->getName() == theGeoPhysChild->getLogVol()->getMaterial()->getName() ||
         hasEnding(theGeoPhysChild->getLogVol()->getMaterial()->getName(), "Ether")||
         hasEnding(theGeoPhysChild->getLogVol()->getMaterial()->getName(), "HyperUranium"))
        {
          // Build the child assembly
          if(!(theG4AssemblyChild = BuildAssembly(theGeoPhysChild))) return 0;

          // Get its transform
            G4Transform3D theG4Position(Amg::EigenTransformToCLHEP(av.getTransform()));

          assemblyVolume->AddPlacedAssembly(theG4AssemblyChild,theG4Position);
        }
      else
        {
	  std::optional<int> Qint =  av.getId();

          // Build the child
          if(!(theG4LogChild = Build(theGeoPhysChild))) return 0;

          // Get its transform
            G4Transform3D theG4Position(Amg::EigenTransformToCLHEP(av.getTransform()));

          int placedID = 0;
          if(Qint) placedID = *Qint;

          std::string placedName = nameChild=="ANON" ? "" : nameChild;


          assemblyVolume->AddPlacedVolume(theG4LogChild,theG4Position,placedID,placedName);
        }

      av.next();
    }

  return assemblyVolume;
}

void ExtParameterisedVolumeBuilder::PrintSTInfo(std::string volume) const
{
    std::cout<< "**********************************************" << std::endl;
    std::cout<< "**  " << std::endl;
    std::cout<< "**  The Volume " << volume  << std::endl;
    std::cout<< "**  Has children of two different types" << std::endl;
    std::cout<< "**  PeoPhysVolume and GeoSerialTransformer" << std::endl;
    std::cout<< "**  In this case GeoSerialTransformer will be " << std::endl;
    std::cout<< "**  translated into G4 placement but not in " << std::endl;
    std::cout<< "**  G4Parameterisation" << std::endl;
    std::cout<< "**  " << std::endl;
    std::cout<< "********************************************** " << std::endl;
}

void ExtParameterisedVolumeBuilder::getMatEther() const
{
    GeoElement* ethElement = new GeoElement("EtherEl","ET",500.0,0.0);
    ethElement->ref();
    GeoMaterial* ether = new GeoMaterial("special::Ether",0.0);
    ether->add(ethElement,1.);
    // "Alternative" assembly material
    GeoMaterial* hyperUranium = new GeoMaterial("special::HyperUranium",0.0);
    hyperUranium->add(ethElement,1.);
    m_matEther = ether;
    m_matHypUr = hyperUranium;
    m_getMatEther = false;
}

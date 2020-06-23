/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "G4MagFieldSvcBase.hh"

// Geant4 includes
#include "G4MagneticField.hh"

//-----------------------------------------------------------------------------
// Implementation file for class : G4MagFieldSvcBase
//
// 2015-11-17: Andrea Dell'Acqua
//-----------------------------------------------------------------------------


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
G4MagFieldSvcBase::G4MagFieldSvcBase(const std::string& name/*,
                                     ISvcLocator* pSvcLocator*/)
  //: base_class(name, pSvcLocator)
{
    std::cout<<"G4MagFieldSvcBase:"<<name<<std::endl;
    m_fieldHolder= nullptr;
}

//=============================================================================
// Retrieve a G4 mag field object
//=============================================================================
G4MagneticField* G4MagFieldSvcBase::getField()
{
  // Check if we already have a field setup for current thread
  auto field = m_fieldHolder;
  // If not, we create one
  if(!field) {
    field = makeField();
    m_fieldHolder = field;
  }
//    std::cout.precision(8);
//    double point[3];
//    double bfield[3];
//    for(int i=0; i<10; i++){
//        point[0]=i;
//        point[1]=i+10;
//        point[2]=i+100;
//        field->GetFieldValue(point,bfield);
//        
//        std::cout<<"field->GetField(" <<point[0]<<" "<<point[1]<<" "<<point[2]<<"): "<<bfield[0]<<" "<<bfield[1]<<" "<<bfield[2]<<std::endl;
//    }

  return field;
}


/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "StandardFieldSvc.h"
#include "MagFieldServices/AtlasFieldSvc.h"

//-----------------------------------------------------------------------------
// Constructor for the AtlasField G4 field object
//-----------------------------------------------------------------------------
AtlasField::AtlasField(MagField::IMagFieldSvc* mfield)
  : m_magFieldSvc_AtlasField(mfield)
{
    std::cout<<"New instance of AtlasField, setting m_magFieldSvc_AtlasField: "<<m_magFieldSvc_AtlasField<<" to "<<mfield<<std::endl;

}

//-----------------------------------------------------------------------------
// Constructor for the StandardFieldSvc
//-----------------------------------------------------------------------------
StandardFieldSvc::StandardFieldSvc(const std::string& name/*,
                                   ISvcLocator* pSvcLocator*/)
  : G4MagFieldSvcBase(name)
  // m_magFieldSvc("MagField::AtlasFieldSvc/AtlasFieldSvc", name)
{
    MagField::AtlasFieldSvc * atlasFieldSvs = new MagField::AtlasFieldSvc();
    atlasFieldSvs->handle();
    m_magFieldSvc = atlasFieldSvs;
    //declareProperty("MagneticFieldSvc", m_magFieldSvc);
}

//-----------------------------------------------------------------------------
// Initialize the service
//-----------------------------------------------------------------------------
//StatusCode StandardFieldSvc::initialize()
//{
//  ATH_MSG_INFO( "Initializing " << name() );
//  ATH_CHECK( m_magFieldSvc.retrieve() );
//  return StatusCode::SUCCESS;
//}

//-----------------------------------------------------------------------------
// Create and retrieve the magnetic field
//-----------------------------------------------------------------------------
G4MagneticField* StandardFieldSvc::makeField()
{
  std::cout<<"StandardFieldSvc::makeField with m_magFieldSvc "<<m_magFieldSvc<<std::endl;
  return new AtlasField( &*m_magFieldSvc );
}


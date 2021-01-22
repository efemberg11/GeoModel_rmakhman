/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#include "LArGeoEndcap/EndcapPresamplerConstruction.h"
#include "GeoModelKernel/GeoMaterial.h"  
#include "GeoModelKernel/GeoFullPhysVol.h"  
#include "GeoModelKernel/GeoPhysVol.h"  
#include "GeoModelKernel/GeoVPhysVol.h"  
#include "GeoModelKernel/GeoLogVol.h"  
#include "GeoModelKernel/GeoTubs.h"  
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"

#include <string>
#include <cmath>


// EndcapPresamplerConstruction
// Insert the LAr Endcap Presampler into a pre-defined mother volume 
// i.e. into the front cold wall of endcap cryostat (cyl.#14)
// 14-Jan-2002 Mikhail Leltchouk



EndcapPresamplerConstruction::EndcapPresamplerConstruction( bool imb ):
  m_psPhysical(0)
{
  m_isModule = imb;
}

EndcapPresamplerConstruction::~EndcapPresamplerConstruction() {;}


GeoFullPhysVol* EndcapPresamplerConstruction::Envelope()
{
  if (m_psPhysical) return m_psPhysical->clone();

  GeoXmlInpManager*       inpman          = GeoXmlInpManager::getManager();
  const GeoXmlMatManager* materialManager = GeoXmlMatManager::getManager();

  const GeoMaterial *LAr  = materialManager->getMaterial("std::LiquidArgon");
  if (!LAr) {
    throw std::runtime_error("Error in EndcapCryostatConstruction, std::LiquidArgon is not found.");
  }

  GeoInpRecordset_ptr presamplerPosition = inpman->getRecordsetPtr("PresamplerPosition");

  ///////////////////////////////////////////////////////////////////
  // LAr Endcap Presampler GEOMETRY
  ///////////////////////////////////////////////////////////////////
  double Rmin = (*presamplerPosition)[0].getDouble("RMIN")*SYSTEM_OF_UNITS::cm;
  double Rmax = (*presamplerPosition)[0].getDouble("RMAX")*SYSTEM_OF_UNITS::cm;
  double HalfZ = ((*presamplerPosition)[0].getDouble("TCK")/2.)*SYSTEM_OF_UNITS::cm;  

  std::string name = "LAr::Endcap::Presampler::LiquidArgon";

  double phi_size = 360.*SYSTEM_OF_UNITS::deg;
  double start_phi = 0.;
  
  if( m_isModule ){
    phi_size = M_PI / 4;
    start_phi = M_PI / 2 - M_PI / 8;
  }
  
  GeoTubs* endcapPresamplerSolid  = new GeoTubs(Rmin, Rmax, HalfZ, start_phi, phi_size);

  const GeoLogVol* endcapPresamplerLog = new GeoLogVol(name,endcapPresamplerSolid, LAr);
  m_psPhysical = new GeoFullPhysVol(endcapPresamplerLog);

  return m_psPhysical;
}

/*  G3 code for comparison: cryogeo.age

 Fill  CREE                  ! e. cold wall front flange
       Section   = 14        ! endcap element number
       Material  = 2         ! material type
       Rmin      = 22.1      ! minimal radius            23.2 > 22.1
       Dr        = 194.4     ! radial thickness 193.8 > 195.4 > 194.4cold
       Zmin      = 356.1     ! location in z (minimum)  357.0 > 356.1
       Dz        = 6.5       ! lenght in z                5.0 >   6.5

 FILL  PSAM    ! CEPS parameters - ABSOLUTE nominal positions
       zpos = 362.4    ! nominal positions
       Tck  = 0.4      ! thinkness
       rmin = 123.174  ! minimal radius
       rmax = 170.198  ! maximum radius
* ---------------------------------------------------------------------
Block     CEPS  is the EC presampler
     Material   Liquid_Argon
     Medium     sensitive    isvol=1
     Attribute  CEPS         seen=1 colo=3
     Shape      TUBE         Rmin=psam_rmin  Rmax=psam_rmax  dz=psam_Tck/2

* eta limits are calculated based on geometry, taken into account Zshift:

     eta1=asinh((psam_zpos+CRYM_Zshift)/psam_rmax)
     eta2=asinh((psam_zpos+CRYM_Zshift)/psam_rmin)
     nbin=nint((eta2-eta1)/chit_Detaps)
     deta=(eta2-eta1)/nbin
     HITS CEPS  comp=1:1:   eta:deta:(eta1,eta2),
                            phi:chit_Dphi:(0,2*M_PI),
                            Eloss:0:(0,chit_Emax)
endblock
* ---------------------------------------------------------------------
*/

/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "LArGeoUtils/LArGeoMaterialManager.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"

LArGeoMaterialManager::LArGeoMaterialManager()
{
}

LArGeoMaterialManager::~LArGeoMaterialManager()
{
  for(auto material : m_materials) material.second->unref();
}

void LArGeoMaterialManager::buildMaterials()
{
  const GeoXmlMatManager* matman = GeoXmlMatManager::getManager();
  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();

  GeoInpRecordset_ptr barrelGeometryRec        = inpman->getRecordsetPtr("BarrelGeometry");
  GeoInpRecordset_ptr barrelAccordionCablesRec = inpman->getRecordsetPtr("BarrelAccordionCables");
  GeoInpRecordset_ptr barrelMotherboardsRec    = inpman->getRecordsetPtr("BarrelMotherboards");
  GeoInpRecordset_ptr coldContractionRec       = inpman->getRecordsetPtr("ColdContraction");

  const GeoMaterial *Copper  = matman->getMaterial("std::Copper");
  if (!Copper) throw std::runtime_error("Error in LArGeoMaterialManager, std::Copper is not found.");

  const GeoMaterial *Iron  = matman->getMaterial("std::Iron");
  if (!Iron) throw std::runtime_error("Error in LArGeoMaterialManager, std::Iron is not found.");

  const GeoMaterial *Lead  = matman->getMaterial("std::Lead");
  if (!Lead) throw std::runtime_error("Error in LArGeoMaterialManager, std::Lead is not found.");

  const GeoMaterial *LAr  = matman->getMaterial("std::LiquidArgon");
  if (!LAr) throw std::runtime_error("Error in LArGeoMaterialManager, std::LiquidArgon is not found.");

  const GeoMaterial *Air  = matman->getMaterial("std::Air");
  if (!Air) throw std::runtime_error("Error in LArGeoMaterialManager, std::Air is not found.");

  const GeoMaterial *Kapton  = matman->getMaterial("std::Kapton");
  if (!Kapton) throw std::runtime_error("Error in LArGeoMaterialManager, std::Kapton is not found.");

  const GeoMaterial *Glue  = matman->getMaterial("LAr::Glue");
  if (!Glue) throw std::runtime_error("Error in LArGeoMaterialManager, LAr::Glue is not found.");

  const GeoMaterial *G10  = matman->getMaterial("LAr::G10");
  if (!G10) throw std::runtime_error("Error in LArGeoMaterialManager, LAr::G10 is not found.");

  //----------------------------------------------------------------------------------------//
  // LAR...                                                                                 //
  //----------------------------------------------------------------------------------------//
  {

    //**** GU: be careful
    // In GeoMaterial::add, the fractions are per MASS
    //
    double Tggl, Tgfe, Tgpb, Thgl, Thfe, Thpb, Thcu, Thfg, ThMBcu, ThMBG10;

    // thin and thick lead absorbers
    double Totalthick,Totalmass,Fracpb,Fracfe,Fracgl;
    double Totalthicke,Totalmasse,FracCu,FracKap;
    // contraction from warm to cold

    double contract=(*coldContractionRec)[0].getDouble("ABSORBERCONTRACTION");

    // first thin absorbers
    Tggl=(*barrelGeometryRec)[0].getDouble("TGGL")*SYSTEM_OF_UNITS::cm*contract;
    Tgfe=(*barrelGeometryRec)[0].getDouble("TGFE")*SYSTEM_OF_UNITS::cm*contract;
    Tgpb=(*barrelGeometryRec)[0].getDouble("TGPB")*SYSTEM_OF_UNITS::cm*contract;
    Totalthick = Tggl+Tgfe+Tgpb;
    Totalmass = (Tgpb*Lead->getDensity()+Tgfe*Iron->getDensity()+Tggl*Glue->getDensity());
    //***GU below are the fraction per mass
    Fracpb = (Tgpb*Lead->getDensity())/Totalmass;
    Fracfe = (Tgfe*Iron->getDensity())/Totalmass;
    Fracgl = (Tggl*Glue->getDensity())/Totalmass;
    double density = Totalmass/Totalthick/(contract*contract*contract);

    GeoMaterial* Thin_abs = new GeoMaterial("Thinabs",density);
    Thin_abs->add(Lead,Fracpb);
    Thin_abs->add(Iron,Fracfe);
    Thin_abs->add(Glue,Fracgl);
    Thin_abs->lock();
    Thin_abs->ref();
    m_materials["Thinabs"]=Thin_abs;

    // then thick absorbers
    Thgl=(*barrelGeometryRec)[0].getDouble("THGL")*SYSTEM_OF_UNITS::cm*contract; // LArEMBThickAbsGlue
    Thfe=(*barrelGeometryRec)[0].getDouble("THFE")*SYSTEM_OF_UNITS::cm*contract; // LArEMBThickAbsIron
    Thpb=(*barrelGeometryRec)[0].getDouble("THPB")*SYSTEM_OF_UNITS::cm*contract; // LArEMBThickAbsLead

    Totalthick = Thgl+Thfe+Thpb;
    Totalmass = (Thpb*Lead->getDensity()+Thfe*Iron->getDensity()+Thgl*Glue->getDensity());
    //**GU below are the fractions per mass
    Fracpb = (Thpb*Lead->getDensity())/Totalmass;
    Fracfe = (Thfe*Iron->getDensity())/Totalmass;
    Fracgl = (Thgl*Glue->getDensity())/Totalmass;
    density = Totalmass/Totalthick/(contract*contract*contract);

    GeoMaterial* Thick_abs = new GeoMaterial("Thickabs",density);
    Thick_abs->add(Lead,Fracpb);
    Thick_abs->add(Iron,Fracfe);
    Thick_abs->add(Glue,Fracgl);
    Thick_abs->lock();
    Thick_abs->ref();
    m_materials["Thickabs"] = Thick_abs;

    // electrode =mixture Kapton+Cu
    Thcu=(*barrelGeometryRec)[0].getDouble("THCU")*SYSTEM_OF_UNITS::cm; // LArEMBThickElecCopper
    Thfg=(*barrelGeometryRec)[0].getDouble("THFG")*SYSTEM_OF_UNITS::cm; // LArEMBThickElecKapton
    Totalthicke = Thcu+Thfg;
    Totalmasse = (Thcu*Copper->getDensity()+Thfg*Kapton->getDensity());
    //**GU below are the fractions per mass
    FracCu = (Thcu*Copper->getDensity())/Totalmasse;
    FracKap = (Thfg*Kapton->getDensity())/Totalmasse;
    //    density = Totalmasse/Totalthicke;

    double contract_e=1./1.003625; // From Fares (J.T.)
    density = (Totalmasse/Totalthicke)/(contract_e*contract_e*contract_e);

    GeoMaterial* Kapton_Cu = new GeoMaterial("KaptonC",density);
    Kapton_Cu->add(Copper,FracCu);
    Kapton_Cu->add(Kapton,FracKap);
    Kapton_Cu->lock();
    Kapton_Cu->ref();
    m_materials["KaptonC"] = Kapton_Cu;

    //  material for Cables/electronics (mixture of Kapton and copper)
    //  density = 2.440*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3;
    //**GU get fractions per mass
    double frmassCu = (*barrelAccordionCablesRec)[0].getDouble("PERCU");  // LArEMBmasspercentCu
    double frmassKap= (*barrelAccordionCablesRec)[0].getDouble("PERKAP"); // LArEMBmasspercentKap
    //GU 28 July 2005 recompute correctly density
    const double frmassKapOverCu = frmassKap / frmassCu;
    density = Copper->getDensity()*(1.+frmassKapOverCu)
             /(1.+frmassKapOverCu*Copper->getDensity()/Kapton->getDensity());
    GeoMaterial* Cable_elect = new GeoMaterial("Cables",density);
    double fractionmass;
    Cable_elect->add(Copper, fractionmass=frmassCu*SYSTEM_OF_UNITS::perCent);
    Cable_elect->add(Kapton, fractionmass=frmassKap*SYSTEM_OF_UNITS::perCent);
    Cable_elect->lock();
    Cable_elect->ref();
    m_materials["Cables"]=Cable_elect;

    // material for motherboard
    // Mother_board is defined as a mixture of epox_G10 (C8 H14 O4) and Copper
    ThMBcu  = (*barrelMotherboardsRec)[0].getDouble("THICU")*SYSTEM_OF_UNITS::cm;  // LArEMBCuThickness
    ThMBG10 = (*barrelMotherboardsRec)[0].getDouble("THIG10")*SYSTEM_OF_UNITS::cm; // LArEMBG10Thickness
    double TotalthickMBe = ThMBcu+ThMBG10;
    double TotalmassMBe = (ThMBcu*Copper->getDensity()+ThMBG10*G10->getDensity());
    double FracMBCu = (ThMBcu*Copper->getDensity())/TotalmassMBe;
    double FracMBG10 = (ThMBG10*G10->getDensity())/TotalmassMBe;
    density = TotalmassMBe/TotalthickMBe;
    GeoMaterial*  Moth_elect = new GeoMaterial("MBoards",density);
    // ****GU:   use fraction per masses of G10 and Cu
    Moth_elect->add(G10,FracMBG10);
    Moth_elect->add(Copper,FracMBCu);
    Moth_elect->lock();
    Moth_elect->ref();
    m_materials["MBoards"] = Moth_elect;


    //==GU  28 July 2005
    //  implement latest definitions from hard coded geometry
    const GeoElement* Si = matman->getElement("Silicon");
    const GeoElement *O = matman->getElement("Oxygen");

    density = (*barrelMotherboardsRec)[0].getDouble("DG10")*(SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3);   //LArEMBEpoxyVolumicMass
    GeoMaterial* SiO2 = new GeoMaterial("SiO2",density);
    double fractionSi=28.09/(28.09+2*16.0);
    SiO2->add(Si,fractionSi);
    double fractionO=2.*16.0/(28.09+2.*16.0);
    SiO2->add(O,fractionO);
    SiO2->lock();
// Gten for the bars of the calorimeter= mixture of regular G10 and SiO2
    density=1.72*SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3;    // should be replaced by number from database
    GeoMaterial* Gten_bar = new GeoMaterial("G10_bar",density);
    Gten_bar->add(G10,0.38);    // should be replaced by number from database
    Gten_bar->add(SiO2,0.62);   // should be replaced by number from database
    Gten_bar->lock();
    Gten_bar->ref();
    m_materials["G10_bar"] = Gten_bar;

// material for the effective M_PIn+summing board effect
    double ThSBCu = 0.28*SYSTEM_OF_UNITS::mm;      // should be replaced by number from database
    double ThSBAr = 9.72*SYSTEM_OF_UNITS::mm;      // should be replaced by number from database
    double TotalThickSB = ThSBCu+ThSBAr;
    double dcu = Copper->getDensity();
    double dar = LAr->getDensity();
    const double TotalMassSB = ThSBCu*dcu + ThSBAr*dar;
    const double inv_TotalMassSB = 1. /TotalMassSB;
    double fracSBCu = ThSBCu*dcu*inv_TotalMassSB;
    double fracSBAr = ThSBAr*dar*inv_TotalMassSB;
    density = TotalMassSB/TotalThickSB;
    GeoMaterial* Summing_board = new GeoMaterial("SBoard",density);
    Summing_board->add(LAr,fracSBAr);
    Summing_board->add(Copper,fracSBCu);
    Summing_board->lock();
    Summing_board->ref();
    m_materials["SBoard"] = Summing_board;

//==end GU

  // Make sure these get deleted, if there's nothing referencing them.
  SiO2->ref();
  SiO2->unref();
  }

  //----------------------------------------------------------------------------------------//
  // LAR Endcap only                                                                        //
  //----------------------------------------------------------------------------------------//
  {

    // ----------------------
    // Vacuum for TB cryostat
    // ----------------------

    const GeoElement *H = matman->getElement( "Hydrogen" );

    GeoMaterial *Vacuum = new GeoMaterial( "Vacuum", SYSTEM_OF_UNITS::universe_mean_density );
    Vacuum->add( H, 1. );
    //m_storedManager->addMaterial("LAr", Vacuum );

    // ----------------------------
    // Materials for EMEC absorbers
    // ----------------------------

    double Tggl, Tgfe, Tgpb, Thgl, Thfe, Thpb;
    double Totalthick,Totalmass,Fracpb,Fracfe,Fracgl;

    // contraction from warm to cold
    double contract=(*coldContractionRec)[0].getDouble("ABSORBERCONTRACTION");

    //
    // EMEC thin absorbers
    //

/*  Tggl = 0.30 * SYSTEM_OF_UNITS::mm;
    Tgfe = 0.40 * SYSTEM_OF_UNITS::mm;
    Tgpb = 1.70 * SYSTEM_OF_UNITS::mm; */

    Tggl = 0.20 * SYSTEM_OF_UNITS::mm;
    Tgfe = 0.40 * SYSTEM_OF_UNITS::mm;
    Tgpb = 1.69 * SYSTEM_OF_UNITS::mm;

    Totalthick = Tggl+Tgfe+Tgpb;
    Totalmass = (Tgpb*Lead->getDensity()+Tgfe*Iron->getDensity()+Tggl*Glue->getDensity());
    // Fractions per mass
    Fracpb = (Tgpb*Lead->getDensity())/Totalmass;
    Fracfe = (Tgfe*Iron->getDensity())/Totalmass;
    Fracgl = (Tggl*Glue->getDensity())/Totalmass;
    double density = Totalmass/Totalthick/(contract*contract*contract);

    GeoMaterial* Thin_abs = new GeoMaterial("EMEC_Thinabs",density);
    Thin_abs->add(Lead,Fracpb);
    Thin_abs->add(Iron,Fracfe);
    Thin_abs->add(Glue,Fracgl);
    //m_storedManager->addMaterial("LAr", Thin_abs);

    //
    // EMEC thick absorbers
    //

/*    Thgl = 0.30 * SYSTEM_OF_UNITS::mm;
    Thfe = 0.40 * SYSTEM_OF_UNITS::mm;
    Thpb = 2.20 * SYSTEM_OF_UNITS::mm; */

    Thgl = 0.20 * SYSTEM_OF_UNITS::mm;
    Thfe = 0.40 * SYSTEM_OF_UNITS::mm;
    Thpb = 2.20 * SYSTEM_OF_UNITS::mm;

    Totalthick = Thgl+Thfe+Thpb;
    Totalmass = (Thpb*Lead->getDensity()+Thfe*Iron->getDensity()+Thgl*Glue->getDensity());
    // Fractions per mass
    Fracpb = (Thpb*Lead->getDensity())/Totalmass;
    Fracfe = (Thfe*Iron->getDensity())/Totalmass;
    Fracgl = (Thgl*Glue->getDensity())/Totalmass;
    density = Totalmass/Totalthick/(contract*contract*contract);

    GeoMaterial* Thick_abs = new GeoMaterial("EMEC_Thickabs",density);
    Thick_abs->add(Lead,Fracpb);
    Thick_abs->add(Iron,Fracfe);
    Thick_abs->add(Glue,Fracgl);
    //m_storedManager->addMaterial("LAr",Thick_abs);

	//
	// EMEC shell = iron + glue, identical for inner and outer absorbers
	//
    Thgl = 0.20 * SYSTEM_OF_UNITS::mm;
    Thfe = 0.40 * SYSTEM_OF_UNITS::mm;

    Totalthick = Thgl+Thfe;
    Totalmass = (Thfe*Iron->getDensity()+Thgl*Glue->getDensity());
    // Fractions per mass
    Fracfe = (Thfe*Iron->getDensity())/Totalmass;
    Fracgl = (Thgl*Glue->getDensity())/Totalmass;
    density = Totalmass/Totalthick/(contract*contract*contract);


    GeoMaterial* EMEC_shell = new GeoMaterial("EMEC_shell",density);
    EMEC_shell->add(Iron,Fracfe);
    EMEC_shell->add(Glue,Fracgl);
    //m_storedManager->addMaterial("LAr",EMEC_shell);


    // ----------------------------
    // Materials for EMEC barrettes
    // ----------------------------

    double TgG10, ThG10;
    double FracG10;

    //
    // EMEC Outer Wheel barrette
    //

    //!! Check whether G10 or G10_bar is to be used!!!!

/*    Tggl = 0.30 * SYSTEM_OF_UNITS::mm;
    Tgfe = 0.40 * SYSTEM_OF_UNITS::mm;
    TgG10 =1.70 * SYSTEM_OF_UNITS::mm;*/

    Tggl = 0.20 * SYSTEM_OF_UNITS::mm;
    Tgfe = 0.40 * SYSTEM_OF_UNITS::mm;
    TgG10 =1.69 * SYSTEM_OF_UNITS::mm;

    Totalthick = Tggl+Tgfe+TgG10;
    Totalmass = (TgG10*G10->getDensity()+Tgfe*Iron->getDensity()+Tggl*Glue->getDensity());
    // Fractions per mass
    FracG10 = (TgG10*G10->getDensity())/Totalmass;
    Fracfe = (Tgfe*Iron->getDensity())/Totalmass;
    Fracgl = (Tggl*Glue->getDensity())/Totalmass;
    //    density = Totalmass/Totalthick;
    density = (Totalmass/Totalthick)/(contract*contract*contract);

    GeoMaterial* G10FeOuter = new GeoMaterial("G10FeOuter",density);
    G10FeOuter->add(G10,FracG10);
    G10FeOuter->add(Iron,Fracfe);
    G10FeOuter->add(Glue,Fracgl);
    G10FeOuter->lock();
    G10FeOuter->ref();
    m_materials["G10FeOuter"] = G10FeOuter;


    //
    // EMEC Inner Wheel barrette
    //

/*    Thgl = 0.30 * SYSTEM_OF_UNITS::mm;
    Thfe = 0.40 * SYSTEM_OF_UNITS::mm;
    ThG10 =2.20 * SYSTEM_OF_UNITS::mm;*/

    Thgl = 0.20 * SYSTEM_OF_UNITS::mm;
    Thfe = 0.40 * SYSTEM_OF_UNITS::mm;
    ThG10 =2.20 * SYSTEM_OF_UNITS::mm;

    Totalthick = Thgl+Thfe+ThG10;
    Totalmass = (ThG10*G10->getDensity()+Thfe*Iron->getDensity()+Thgl*Glue->getDensity());
    // Fractions per mass
    FracG10 = (ThG10*G10->getDensity())/Totalmass;
    Fracfe = (Thfe*Iron->getDensity())/Totalmass;
    Fracgl = (Thgl*Glue->getDensity())/Totalmass;
    //    density = Totalmass/Totalthick;
    density = (Totalmass/Totalthick)/(contract*contract*contract);

    GeoMaterial* G10FeInner = new GeoMaterial("G10FeInner",density);
    G10FeInner->add(G10,FracG10);
    G10FeInner->add(Iron,Fracfe);
    G10FeInner->add(Glue,Fracgl);
    G10FeInner->lock();
    G10FeInner->ref();
    m_materials["G10FeInner"] = G10FeInner;

  }
}

const GeoMaterial* LArGeoMaterialManager::getMaterial(const std::string& name) const
{
  auto it = m_materials.find(name);
  return it==m_materials.end() ? nullptr : it->second;
}

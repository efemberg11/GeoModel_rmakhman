/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/


#include "GeoModelKernel/GeoVGeometryPlugin.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoIdentifierTag.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoSerialIdentifier.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoXF.h"

// volumes used:  Pcon, Tubs, Cons, Box, Trap 
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTrap.h"
// For functions:
#include "GeoGenericFunctions/Abs.h"
#include "GeoGenericFunctions/Sin.h"
#include "GeoGenericFunctions/Cos.h"
#include "GeoGenericFunctions/Sqrt.h"
#include "GeoGenericFunctions/ATan.h"
#include "GeoGenericFunctions/Rectangular.h"
#include "GeoGenericFunctions/Mod.h"
#include "GeoGenericFunctions/Variable.h"
#include "GeoGenericFunctions/FixedConstant.h"


// Some additional objects to store for future reference:

#include <string>
#include <cmath>
#include <cfloat>

#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

using namespace GeoGenfun;
using namespace GeoXF;

#define BUILD_ACCORDION_PLATES 1
#define BUILD_FRONT_STEEL      1

class AccordionPlugin : public GeoVGeometryPlugin  {

 public:

  // Constructor:
  AccordionPlugin();

  // Destructor:
  ~AccordionPlugin();

  // Creation of geometry:
  virtual void create(GeoPhysVol *world);

 private:

  // Illegal operations:
  const AccordionPlugin & operator=(const AccordionPlugin &right)=delete;
  AccordionPlugin(const AccordionPlugin &right) = delete;

  // Private operations:
  GeoGenfun::FunctionNoop Fx(double r, GeoGenfun::GENFUNCTION G, const double Cenx[], const double Ceny[] ) const;
  GeoGenfun::FunctionNoop Fy(double r, GeoGenfun::GENFUNCTION G, const double Cenx[], const double Ceny[] ) const;
  GeoGenfun::FunctionNoop Del1(GeoGenfun::GENFUNCTION G) const;
  GeoGenfun::FunctionNoop Del2(GeoGenfun::GENFUNCTION G) const;
  GeoGenfun::FunctionNoop ATan2(GeoGenfun::GENFUNCTION y, GeoGenfun::GENFUNCTION x) const;
  
};





AccordionPlugin::AccordionPlugin()
{
}


AccordionPlugin::~AccordionPlugin()
{
}


//## Other Operations (implementation)
void AccordionPlugin::create(GeoPhysVol *world)
{
  // Get the materials that we shall use.
  // -------------------------------------//


  double degrees=M_PI/180.0;
  double mm=1.0;
  double cm=10.0;
  
  bool doDetailedAbsorberStraight = true;
  bool doDetailedAbsorberFold     = true;



  GeoGenfun::Cos  Cos;
  GeoGenfun::Sin  Sin;
  GeoGenfun::Sqrt Sqrt;
  GeoGenfun::ATan ATan;

  double twopi64 = M_PI/32.;
  double twopi32 = 2.*twopi64;  



  double contract = 0.997;// m_parameters->GetValue("LArEMBAbsorberContraction");

  const GeoElement *Pb = new GeoElement("Lead","Pb",1,1);
  GeoMaterial* myLead = new GeoMaterial("myLead",1.0);
  myLead->add(Pb,1.0);
  myLead->lock();

  //
  // All the materials in this example are bogus. 
  //
  const GeoMaterial *myIron       = myLead;
  const GeoMaterial *Iron         = myLead;
  const GeoMaterial *LAr          = myLead;
  const GeoMaterial *Lead         = myLead;
  const GeoMaterial *G10_bar      = myLead;
  const GeoMaterial *Moth_elect   = myLead;
  const GeoMaterial *Cable_elect  = myLead;
  const GeoMaterial *Thin_abs     = myLead;
  const GeoMaterial *Thick_abs    = myLead;
  const GeoMaterial *Kapton_Cu    = myLead;
  const GeoMaterial *Sumb         = myLead;
  const GeoMaterial *Glue         = myLead;

  
  std::string baseName = "LAr::EMB::";
  
  
  // -------------------------------------------------------------
  // start Detector geometry
  //--------------------------------------------------------------
  
  double Moth_Z_min = 0.3*cm;    //m_parameters->GetValue("LArEMBMotherZmin");
  double Moth_Z_max = 316.5*cm; //m_parameters->GetValue("LArEMBMotherZmax");

  // radius and phi ranges of EM barrel

  double Moth_inner_radius = 144.7*cm;// m_parameters->GetValue("LArEMBMotherRmin");
  double Moth_outer_radius = 200.35*cm; // m_parameters->GetValue("LArEMBMotherRmax");

  double Moth_Phi_Min = 0.;
  double Moth_Phi_Max = 2*M_PI;

  // number of zigs for accordion
  int    Nbrt = 14; //(int) ( m_parameters->GetValue("LArEMBnoOFAccZigs") );  // =14

  // Z coordinates for half barrel in 
  double Bar_Z_min = Moth_Z_min;
  double Bar_Z_max = Moth_Z_max;

  // Radius of the first fold
//GU -> to be consistent with hard coded geometry
  double Rhocen1 = 150.002*cm;// m_parameters->GetValue("LArEMBRadiusAtCurvature",0);

  // R and Z coordinates of ETA_cuts.
  double Bar_Eta_cut = 1.475;//(double) (m_parameters->GetValue("LArEMBMaxEtaAcceptance"));

  double Bar_Z_cut,  Bar_Rcmx ;
// z max at low radius
  Bar_Z_cut = 0.3*cm + 309.4*cm; //(double) (m_parameters->GetValue("LArEMBMotherZmin"))
  //            + (double)  (m_parameters->GetValue("LArEMBG10FrontDeltaZ"));
// minimal radius at Bar_z_max
  Bar_Rcmx  = 	154.8*cm;// (double) (m_parameters->GetValue("LArEMBRminHighZ"));

  double Zp0 = 0.;

  // Half-length of this Half barrel
  double Zhalf = (Bar_Z_max-Bar_Z_min)/2.;
  // Half-length of the half barrel up to Zcut
  double Zhalfc = (Bar_Z_cut-Bar_Z_min)/2.;


  // Accordion shape parameters:
  // rho phi of curvature centers and delta's
  // IN 2D_transverse LOCAL framework of a layer and symetry axis as X_axis
  // delta's are GEOMETRICAL angles of straight parts w.r. the Y_axis )

  double Rhocen[15]={150.002*cm,
		     152.1*cm,
		     155.966*cm,
		     159.72*cm,
		     163.457*cm,
		     167.102*cm,
		     170.743*cm,
		     174.307*cm,
		     177.876*cm,
		     181.375*cm,
		     184.887*cm,
		     188.336*cm,
		     191.802*cm,
		     195.21*cm,
		     197.048*cm};
  double Phicen[15]={-.106187*degrees,
		     -.569751*degrees,
		     .573092*degrees,
		     -.576518*degrees,
		     .579943*degrees,
		     -.582296*degrees,
		     .585638*degrees,
		     -.588207*degrees,
		     .590596*degrees,
		     -.59285*degrees,
		     .595587*degrees,
		     -.59744*degrees,
		     .599714*degrees,
		     -.601911*degrees,
		     -.0811661*degrees};
  double Delta[15]={46.2025*degrees,
		    45.0574*degrees,
		    43.3446*degrees,
		    42.4478*degrees,
		    40.9436*degrees,
		    40.2251*degrees,
		    38.8752*degrees,
		    38.2915*degrees,
		    37.0608*degrees,
		    36.5831*degrees,
		    35.4475*degrees,
		    35.0556*degrees,
		    33.9977*degrees,
		    33.6767*degrees,
		    90*degrees};
  double deltay[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double deltax[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double eTx=2.0*atan(exp(-0.8));
  double TetaTrans[15]={eTx, eTx, eTx, eTx, eTx, eTx, eTx, eTx,
			eTx, eTx, eTx, eTx, eTx, eTx, eTx};
// parity of accordion waves
  int checkParity=0;   // for case where first absorber wave goes up
  if (Phicen[0]<0.) checkParity=1;  // case where first absorber wave goes down


  int Ncell      = 64;// (int) (m_parameters->GetValue("LArEMBnoOFPhysPhiCell"));
  int Nabsorber  = (Ncell==64)  ? Ncell + 1 : Ncell;
  int Nelectrode = Ncell;

  // If the user has specifically limited the number of cells (e.g. for visualization)
  // then only create this number....
 


// z of end of G10 ring
  double z1   = 309.4*cm+Moth_Z_min; // m_parameters->GetValue("LArEMBG10FrontDeltaZ")+ Moth_Z_min;
// radius of end of G10 ring
  const double r1    = Moth_inner_radius + 2.3*cm + 2.0*cm; 
  //    + m_parameters->GetValue("LArEMBInnerElectronics")
  //    + m_parameters->GetValue("LArEMBG10SupportBarsIn");
// minimum radius at end of volume
  const double r2    =  154.8*cm; //m_parameters->GetValue("LArEMBRminHighZ");
  const double inv_r2_r1 = 1. / (r2-r1);
  
// max z of Stac (@ r = Rhocen[0] )
  double zmax1_Stac = z1 + (Rhocen[0]-r1)*(Moth_Z_max-z1)*inv_r2_r1;


  //
  // Front and Back ELECTRONICS in ECAM
  //

    // add 1.3 Gaudi::Units::mm in z to allow cleareance for absorber with non                    //
    // 0 thickness, at eta=1.475, low r part of the barrel                          //
    // this affects STAC and TELB volumes                                           //
    double clearance = 1.3*mm;     


  GeoPhysVol *stacPhysical=NULL;
  //---------------------------------ACCORDION VOLUME---------------------------//
  //  STAC = Pcon: LAr volume in which the accordion structure is located       //
  //     (the front/back small straight parts are in ECAM not in STAC)           //
  //  in test beam case need to have STAC between -1.055 and size 23.555 degrees
  //  (i.e. a little bit wider than one calorimeter module)
  {                                                                             //

    double Moth_Phi_Min2 = (Ncell == 64) ? -1.055*degrees : 0.;                     //
    double Moth_Phi_Max2 = (Ncell == 64) ? 24.61*degrees  : 2*M_PI;                 //

    double Zplan1[] = {Bar_Z_min,zmax1_Stac+clearance,Bar_Z_max};                //
    double Riacc1[] = {Rhocen[0],Rhocen[0], Bar_Rcmx-clearance};                //
    double Roacc1[] = {Rhocen[Nbrt],Rhocen[Nbrt],Rhocen[Nbrt]};                 //
    //                                                                          //
    std::string name = baseName + "STAC";                                       //
    GeoPcon* pCone = new GeoPcon(Moth_Phi_Min2,Moth_Phi_Max2);                  //
    for (int i=0; i<3; i++) pCone->addPlane(Zplan1[i],Riacc1[i],Roacc1[i]);     //
    const GeoLogVol* logVol = new GeoLogVol(name,pCone,LAr);                    //
    stacPhysical = new GeoPhysVol(logVol);                                      //
    world->add(new GeoTransform(GeoTrf::TranslateZ3D(-Zp0)));            //
    GeoNameTag *stacNameTag=new GeoNameTag("Piece Of Accordion");
    world->add(stacNameTag);
    world->add(stacPhysical);                                       //
  }                                                                             //
  //----------------------------------------------------------------------------//

#ifdef BUILD_ACCORDION_PLATES
  {
    // Construct the straight and the corner parts of the Accordion plates
    
    double Xtip_pb    = 0.2*cm;//m_parameters->GetValue("LArEMBLeadTipThickFront");
    double Xtip_pc    = 1.3*cm;// m_parameters->GetValue("LArEMBLeadTipThickEnd");
    double Xtip_gt    = 0.0*cm;//m_parameters->GetValue("LArEMBG10TipThickFront");
    double Xtip_gs    = 1.3*cm;//m_parameters->GetValue("LArEMBG10TipThickEnd");
    double Thgl       = .0278*cm;//m_parameters->GetValue("LArEMBThickAbsGlue");
    double Thfe       = .04*cm;//m_parameters->GetValue("LArEMBThickAbsIron");
    double Thpb       = .1532*cm;//m_parameters->GetValue("LArEMBThickAbsLead");
    double Thpb_thin  = .1131*cm;//m_parameters->GetValue("LArEMBThinAbsLead");
    double Thcu       = .0105*cm;//m_parameters->GetValue("LArEMBThickElecCopper");
    double Thfg       = .017*cm;//m_parameters->GetValue("LArEMBThickElecKapton");
    double Psi        = .3515625*degrees;//m_parameters->GetValue("LArEMBPhiGapAperture");   // 360.*Gaudi::Units::deg/1024
    double Contract   = contract;//m_parameters->GetValue("LArEMBAbsorberContraction");

    double Thce = (Thpb+Thgl+Thfe)*Contract;
    double Thel = Thcu+Thfg;
   
    double Alfa = Psi;                        // size of one phi cell
   
    double Zmin = Bar_Z_min;
    double Zmax = Bar_Z_max;
// Zcp1 = z max for eta=0.8 at the radius of the middle of the fold
// Zcp2 = z max for eta=1.475 at the radius of the middle of the fold
// Along = lenght of the straight sections
    double Zcp1[15],Zcp2[15],Along[14];

// Rhol = radius at the beginning of the straight section
// Rhoh = radius at the end of the straight section
// Zcp1l = z max for eta=0.8 at the beginning of the straight section
// Zcp1h = z max for eta=0.8 at the end of the straight section
// Zcp2l = z max for eta~=1.475 at the beginning of the straight section
// Zcp2h = z max for eta~=1.475 at the end of the straight section
    double Zcp1l[14],Zcp1h[14],Zcp2l[14],Zcp2h[14];
    double Rhol[14],Rhoh[14];

    double safety_along = 0.007*mm;
 
   
    // Compute centers of curvature coordinates in a local frame.
   
  double Cenx[15]={0}, Ceny[15]={0} ;
    for (int jf=0; jf<(Nbrt+1); jf++)
      {
	Cenx[jf] = Rhocen[jf]*cos(Phicen[jf]); // Phicen[] already in radians
	Ceny[jf] = Rhocen[jf]*sin(Phicen[jf]);
        Zcp1[jf] = Rhocen[jf]/tan(TetaTrans[jf]);
        if (Rhocen[jf] < r2)
           Zcp2[jf] = z1 + (Rhocen[jf]-r1)*inv_r2_r1*(Moth_Z_max-z1);
         else
           Zcp2[jf]=Moth_Z_max;

      }

// Compute staight lengths of folds
    double Rint = 0.278*cm; //m_parameters->GetValue("LArEMBNeutFiberRadius");
    for (int jl=0; jl<Nbrt; jl++)
    {
      double Adisc2 = (Cenx[jl+1]-Cenx[jl])*(Cenx[jl+1]-Cenx[jl])+
                      (Ceny[jl+1]-Ceny[jl])*(Ceny[jl+1]-Ceny[jl]);
      double Along2 = Adisc2 - 4.*Rint*Rint; 
      Along[jl] = sqrt(Along2);
      double ddelta = M_PI/2.-Delta[jl];
// different parity depending on direction of first wave
      if (checkParity==0) {
        if (jl%2==1) ddelta=-1.*ddelta;
      } else {
        if (jl%2==0) ddelta=-1.*ddelta;
      }
      double xlong=Along[jl]-2.*safety_along;
      double x2=0.5*(Cenx[jl+1]+Cenx[jl])+0.5*xlong*cos(ddelta);
      double y2=0.5*(Ceny[jl+1]+Ceny[jl])+0.5*xlong*sin(ddelta);
      double x1=0.5*(Cenx[jl+1]+Cenx[jl])-0.5*xlong*cos(ddelta);
      double y1=0.5*(Ceny[jl+1]+Ceny[jl])-0.5*xlong*sin(ddelta);
      Rhol[jl] = sqrt(x1*x1+y1*y1);
      Rhoh[jl] = sqrt(x2*x2+y2*y2);
      Zcp1l[jl] = Rhol[jl]/tan(TetaTrans[jl]);
      Zcp1h[jl] = Rhoh[jl]/tan(TetaTrans[jl+1]);
      if (Rhol[jl] < r2) 
         Zcp2l[jl] = z1 + (Rhol[jl]-r1)*inv_r2_r1*(Moth_Z_max-z1);
       else
         Zcp2l[jl]=Moth_Z_max;
      if (Rhoh[jl] < r2) 
         Zcp2h[jl] = z1 + (Rhoh[jl]-r1)*inv_r2_r1*(Moth_Z_max-z1);
       else    
         Zcp2h[jl]=Moth_Z_max;
  }

   

    double Gama0      = -.003068;// (NOT IN DEGREES) m_parameters->GetValue("LArEMBAbsPhiFirst");
   
    GeoGenfun::Variable icopy;
    GeoGenfun::GENFUNCTION Game = Gama0 + Psi/2 + Alfa*icopy;
    GeoGenfun::GENFUNCTION Gama = Gama0 + Alfa*icopy;
   
    // Creation of the straight absorber parts. Front (TIPB) & Back (TIPC)
    // Creation of the straight electrode parts. Front (TIPK) & Back (TIPL)

    enum FB {FRONT, BACK, TERM};
   
    for (int fb=FRONT; fb!=TERM; fb++) 
      {
       
	int irl = fb==FRONT ? 0 : Nbrt;
       
	double Xtips = Xtip_pb + Xtip_gt;
	double Xtipt = Xtip_pc + Xtip_gs;

	// Absorber (thick, thin)
	{
	  double radius =  fb==FRONT ? Cenx[0] - Xtip_pb/2    : Cenx[Nbrt] + Xtipt/2;
	  double Xhalfb  = fb==FRONT ? Xtip_pb/2 -0.002*mm    : Xtipt/2 - .004*mm;
	  double Zhalfb  = fb==FRONT ? (Bar_Z_cut-Zmin)/2. : (Zmax-Zmin)/2.;
	  double dz01 = (std::min(Zcp1[irl],Zmax)-Zmin)/2.;  // half lenght for thick lead

	  std::string name         = baseName + "FrontBack::Absorber";
	  GeoBox *box              = new GeoBox(Xhalfb,Thce/2,Zhalfb);
	  GeoBox *box2             = new GeoBox(Xhalfb,Thce/2,dz01);
	  const GeoLogVol *logVol  = new GeoLogVol(name,box,Thin_abs);
	  const GeoLogVol *logVol2 = new GeoLogVol(name,box2,Thick_abs);
	  GeoPhysVol* physVol      = new GeoPhysVol(logVol);
	  GeoPhysVol* physVol2     = new GeoPhysVol(logVol2);
	  physVol->add(new GeoTransform(GeoTrf::Translate3D(0.,0.,dz01-Zhalfb)));
	  physVol->add(physVol2);
	  GeoGenfun::GENFUNCTION Xcd = radius*Cos(Gama);
	  GeoGenfun::GENFUNCTION Ycd = radius*Sin(Gama);
	  GeoGenfun::GENFUNCTION Zcd = GeoGenfun::FixedConstant(Zmin+Zhalfb);
	 
	 
	  GeoXF::TRANSFUNCTION TX = 
	    GeoXF::Pow(GeoTrf::TranslateX3D(1.0),Xcd) *
	    GeoXF::Pow(GeoTrf::TranslateY3D(1.0),Ycd) *
	    GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),Zcd) * 
	    GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Gama);
	  GeoSerialTransformer *st = new GeoSerialTransformer(physVol, &TX, Nabsorber);     
	  stacPhysical->add(st);
	}
	// G10 (only for front part)
	if (fb==FRONT)
	{
	  double Xhalfbg = Xtip_gt/2-0.002*mm;
	  double radiusg = Cenx[0]-Xtip_pb/2. - Xtips/2   ;   
	  double Zhalfbg = (Bar_Z_cut-Zmin)/2.    ;
	  std::string name        = baseName + "FrontBack::G10";
	  GeoBox *box = new GeoBox(Xhalfbg,Thce/2,Zhalfbg);
	  const GeoLogVol *logVol  = new GeoLogVol(name,box,G10_bar);
	  GeoPhysVol      *physVol = new GeoPhysVol(logVol);

#ifdef BUILD_FRONT_STEEL
// GU 28-07-2005
// add small iron pieces on each side of the G10
          name  = baseName + "FrontBack::Steel";
          double Tgfe = 0.4*cm; // m_parameters->GetValue("LArEMBThinAbsIron");
          GeoBox *box2 = new GeoBox(Xhalfbg,Tgfe/4.,Zhalfbg);
          const GeoLogVol *logVol2 = new GeoLogVol(name,box2,Iron);
          GeoPhysVol *physVol2 = new GeoPhysVol(logVol2);
          physVol->add(new GeoTransform(GeoTrf::Translate3D(0.,0.5*(-Thce+Tgfe/2.),0.))); 
          physVol->add(physVol2);
          physVol->add(new GeoTransform(GeoTrf::Translate3D(0.,0.5*(+Thce-Tgfe/2.),0.)));
          physVol->add(physVol2);
#endif   // build_front_steel

// position of G10+Steel inside LAr bath
	  GeoGenfun::GENFUNCTION Xcd = radiusg*Cos(Gama);
	  GeoGenfun::GENFUNCTION Ycd = radiusg*Sin(Gama);
	  GeoGenfun::GENFUNCTION Zcd = GeoGenfun::FixedConstant(Zhalfbg+Zmin);
	  GeoXF::TRANSFUNCTION TX = 
	    GeoXF::Pow(GeoTrf::TranslateX3D(1.0),Xcd) *
	    GeoXF::Pow(GeoTrf::TranslateY3D(1.0),Ycd) *
	    GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),Zcd) * 
	    GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Gama);
	  GeoSerialTransformer *st = new GeoSerialTransformer(physVol, &TX, Nabsorber);     
	  stacPhysical->add(st);
	}
	// Electrode
	{
	  double Xhalfbe = fb==FRONT ? Xtips/2 -0.002*mm      : Xtipt/2 - .004*mm;
	  double Zhalfbe = fb==FRONT ? (Bar_Z_cut-Zmin)/2.    : (Zmax - Zmin)/2;
	  double radiuse = fb==FRONT ? Cenx[0] - Xtips/2   : Cenx[Nbrt] + Xtipt/2;
	  std::string name        = baseName + "FrontBack::Electrode";
	  GeoBox *box             = new GeoBox(Xhalfbe,Thel/2,Zhalfbe);
	  const GeoLogVol *logVol = new GeoLogVol(name,box,Kapton_Cu);
	  GeoPhysVol* physVol     = new GeoPhysVol(logVol);
	  GeoGenfun::GENFUNCTION Xcd  = radiuse*Cos(Game);
	  GeoGenfun::GENFUNCTION Ycd  = radiuse*Sin(Game);
	  GeoGenfun::GENFUNCTION Zcd  = GeoGenfun::FixedConstant(Zmin+Zhalfbe);
	 
	  GeoXF::TRANSFUNCTION TX = 
	    GeoXF::Pow(GeoTrf::TranslateX3D(1.0),Xcd) *
	    GeoXF::Pow(GeoTrf::TranslateY3D(1.0),Ycd) *
	    GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),Zcd) * 
	    GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Game);
	 
	  GeoSerialTransformer *st = new GeoSerialTransformer(physVol, &TX, Nelectrode);     
	  stacPhysical->add(st);

	}	 
      }


    //
    // Loop through the straight and corner(Fold) parts of the Accordion plates
    // Repeat each part around Phi, then move to the next, towards outer radii
    //

// GU 09/06/2004 add some safety in z size
    double safety_zlen=0.050*mm;
   
    for(int irl=0; irl<Nbrt; irl++)   // loop over zig-zag in radius
      {
        int iparit;
// different parity depending on direction of first wave
        if (checkParity==0) {       
	 iparit= (1-2*(irl%2));  // +1 for irl=0,2,4,..   -1 for irl=1,3,5,..
        } else {
         iparit=(2*(irl%2)-1);  // -1 for irl=0,2,3...  +1 for irl=1,3,5
        }
       
// length of tubs for folds
        double Zcon1 = Zcp2[irl];
        double Zcon2 = Zcp2[irl+1];
// if the radius at the lower edge of this fold is smaller than
// Bar_rcmx, take the lenght at this radius instead of the radius
// of the fold to avoid going outside the mother volume
        if (irl>0 && Rhoh[irl-1] < Bar_Rcmx) Zcon1=Zcp2h[irl-1];
        if (Rhoh[irl] < Bar_Rcmx) Zcon2=Zcp2h[irl];

        double dz0 = (std::min(Zcon1,Zmax)-Zmin)/2.;     // half lenght in z (fold)
        double dza = (std::min(Zcon2,Zmax)-Zmin)/2.;     // (for last fold)
        double dz01 = (std::min(Zcp1[irl],Zmax)-Zmin)/2.;  // half lenght for thick lead
        double dza1 = (std::min(Zcp1[irl+1],Zmax)-Zmin)/2.;


// special case if Rhocen[il] < Bar_Rcmx < Rhocen[il+1]
// we had to divide the fold in two different pieces to deal with the shape
        int ndivi=1;
        if (Rhocen[irl] < Bar_Rcmx && Rhocen[irl+1] > Bar_Rcmx) {
            ndivi=2;
        } 

        for (int idivi=0;idivi<ndivi;idivi++) {

            // lenght in z at beginning and end of straigth part
            double bl1,tl1;
            double frac;
            if (ndivi==1) {
              bl1 = (std::min(Zcp2l[irl],Zmax)-Zmin)/2.;
              tl1 = (std::min(Zcp2h[irl],Zmax)-Zmin)/2.;
              frac=1.;
            }
            else {
              if (idivi==0) {
                bl1 = (std::min(Zcp2l[irl],Zmax)-Zmin)/2.;
                tl1 = (Zmax-Zmin)/2.;
                frac=(Bar_Rcmx-Rhol[irl])/(Rhoh[irl]-Rhol[irl]);
              }
              else {
                bl1 = (Zmax-Zmin)/2.;
                tl1 = (Zmax-Zmin)/2.;
                frac=(Rhoh[irl]-Bar_Rcmx)/(Rhoh[irl]-Rhol[irl]);
              }
            }
//GU 09/06/2004 add small safety in size tl1 and bl1
            tl1=tl1-safety_zlen;
            bl1=bl1-safety_zlen;

//    =================================== Absorbers
	    {
	   
	      // thickness of absorber
	      double Dz = Thce/2.;
	   
	      // For absorbers
	      GeoGenfun::GENFUNCTION x1a = Fx(irl+0., Gama, Cenx, Ceny)
                                           +deltay[irl]*Del1(Gama)
                                           +deltax[irl]*Del2(Gama);
	      GeoGenfun::GENFUNCTION x2a = Fx(irl+1., Gama, Cenx, Ceny)
                                           +deltay[irl+1]*Del1(Gama)
                                           +deltax[irl+1]*Del2(Gama);
	      GeoGenfun::GENFUNCTION y1a = Fy(irl+0., Gama, Cenx, Ceny)
                                           -deltay[irl]*Del2(Gama)
                                           +deltax[irl]*Del1(Gama);
	      GeoGenfun::GENFUNCTION y2a = Fy(irl+1., Gama, Cenx, Ceny)
                                           -deltay[irl+1]*Del2(Gama)
                                           +deltax[irl+1]*Del1(Gama);
	      GeoGenfun::GENFUNCTION dx = x2a - x1a;
	      GeoGenfun::GENFUNCTION dy = y2a - y1a;
	   
	      // Da the two fold centers distance, da straight part length
	   
	      GeoGenfun::GENFUNCTION Da = Sqrt ( dx*dx + dy*dy );
	      GeoGenfun::GENFUNCTION da = Sqrt ( (Da - 2.*Rint)*(Da + 2.*Rint) );
	   
	      // newalpha (slant angle) value of the rotation angle around Z_axis
	      GeoGenfun::GENFUNCTION cosalfa = (da*dx -iparit*2.*Rint*dy)/Da/Da;
	      GeoGenfun::GENFUNCTION sinalfa = (da*dy +iparit*2.*Rint*dx)/Da/Da;
	      GeoGenfun::GENFUNCTION newalpha = ATan2(sinalfa,cosalfa);       
	   
	      GeoGenfun::GENFUNCTION h1 = da/2. * frac  - .007*mm;
	   
	      double Zx0 = (tl1+bl1)/2.;
// thick absorber pieces
// more correct computation with z lenght computed exactly at the
// radius of the end and the beginning of the straight sections
              double Xb1,Xt1;
              if (ndivi==1) {
                Xb1 = (Zcp1l[irl]-Zmin)/2.;
                Xt1 = (Zcp1h[irl]-Zmin)/2.;
              } else {
                double xfrac=(Bar_Rcmx-Rhol[irl])/(Rhoh[irl]-Rhol[irl]);
                if (idivi==0) {
                  Xb1 = (Zcp1l[irl]-Zmin)/2.;
                  Xt1 = (xfrac*Zcp1h[irl]+(1.-xfrac)*Zcp1l[irl]-Zmin)/2.;
                }
                else {
                  Xb1 = (xfrac*Zcp1h[irl]+(1.-xfrac)*Zcp1l[irl]-Zmin)/2.;
                  Xt1 = (Zcp1h[irl]-Zmin)/2.;
                } 
              }
	    // translation in x to include thick absorber into thin absorber
	      double Xtrans = (Xb1+Xt1)/2.-Zx0 + .007*mm;    

            // lengths that remain to be covered with the thin absorber
              double Xt2 = tl1-Xt1;
              double Xb2 = bl1-Xb1;

             // trabslation that would be needed to include think absorber only into overall thin+thick volume
              double Xtrans2 =  Zx0 - (Xb2+Xt2)/2.;
              Xt2 = Xt2 -0.007*mm;
              Xb2 = Xb2 -0.007*mm;
           
	   
	      GeoGenfun::GENFUNCTION alpha = ATan(0.5*(bl1-tl1)/h1);
	      GeoGenfun::GENFUNCTION alpha_t = ATan(0.5*(Xb1-Xt1)/h1);     

            // angle that would be needed for trap do describe only thin absorber
            //     ------------------|---------X---------|
            //     <------------------------------------->  2*tl1
            //                       <--------->   Xt2
            //
            //     <--------------------------------->  2*bl1
            //                    <-------->  Xb2 
            //     ---------------|--------X---------|
            // alpha = (-) angle between X's
            //   tan(alpha) = delta X size / width,   deltaX size = 2*tl1-Xt2-(2*bl1-Xb2),  width = 2.*h1  
	      GeoGenfun::GENFUNCTION alpha_2 = ATan((2.*bl1-Xb2-(2.*tl1-Xt2))/(2.*h1));      
	   
	   
	    // .newalpha is already computed angle wrt z axis
	    // P/2 rotation is to get absorber aligned along local x axis
	    // instead of y, then rotate with angle newalpha
	      GeoGenfun::GENFUNCTION alfrot =  -M_PI/2. - newalpha;

	      GeoGenfun::GENFUNCTION Xcd    = (x1a + x2a)/2. + (2.*idivi-1.)*(1.-frac)*da/2.*cosalfa;
	      GeoGenfun::GENFUNCTION Ycd    = (y1a + y2a)/2. + (2.*idivi-1.)*(1.-frac)*da/2.*sinalfa;
	      GeoGenfun::GENFUNCTION Zcd    = GeoGenfun::FixedConstant(Zmin+(tl1+bl1)/2.+safety_zlen);
	   
	      GeoXF::TRANSFUNCTION TX = 
	        GeoXF::Pow(GeoTrf::TranslateX3D(1.0),Xcd) *
	        GeoXF::Pow(GeoTrf::TranslateY3D(1.0),Ycd) *
	        GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),Zcd) * 
	        GeoXF::Pow(GeoTrf::RotateZ3D(1.0),-alfrot)*
	        GeoTrf::RotateY3D(-90*degrees);                    

	    // 

	      for (int instance = 0; instance < Nabsorber; instance++) 
	      {
		
		// Define some commonly-used volume names.
		std::string thinName   = baseName + "ThinAbs::Straight";
		std::string thickName  = baseName + "ThickAbs::Straight";

	        GeoPhysVol* thinPhys = nullptr;
                GeoTrap* thinTrap = new GeoTrap(Dz,0.,0.,h1(instance),tl1,bl1,alpha(instance),
                                                         h1(instance),tl1,bl1,alpha(instance));
                if (!doDetailedAbsorberStraight) {

                  const GeoLogVol* thinLog = new GeoLogVol(thinName,thinTrap,Thin_abs);
                  thinPhys = new GeoPhysVol(thinLog);

                  GeoTrap* thickTrap = new GeoTrap(Dz,0.,0.,h1(instance),Xt1,Xb1,alpha_t(instance),
                                                            h1(instance),Xt1,Xb1,alpha_t(instance));
		  const GeoLogVol* thickLog =  new GeoLogVol(thickName,thickTrap,Thick_abs);
                  GeoPhysVol* thickPhys = new GeoPhysVol(thickLog);
		  // put thick absorber in straight_phys
		  thinPhys->add(new GeoTransform(GeoTrf::TranslateX3D(Xtrans)));
		  thinPhys->add(thickPhys);

                }

                if (doDetailedAbsorberStraight) {

                   // overall volume => steel
                   const GeoLogVol* thinLog = new GeoLogVol(thinName,thinTrap,myIron);
                   thinPhys = new GeoPhysVol(thinLog);

                   // inside steel put glue, Dz size = Dz size of steel - total thickness of steel/2. covers full eta range

                   double dz_glue = Dz - 0.5*Thfe*Contract;
                   GeoTrap* thickTrapGlue = new GeoTrap(dz_glue,0.,0.,h1(instance),tl1,bl1,alpha(instance),
                                                                      h1(instance),tl1,bl1,alpha(instance));
                   std::string thickGlueName = baseName + "ThickAbsGlue::Straight";
                   const GeoLogVol* thickTrapGlueLog = new GeoLogVol(thickGlueName,thickTrapGlue, Glue);
                   GeoPhysVol * thickTrapGluePhys = new GeoPhysVol(thickTrapGlueLog);
                   thinPhys->add(new GeoTransform(GeoTrf::Translate3D(0.,0.,0.)));
                   thinPhys->add(thickTrapGluePhys);

                   // inside glue put lead, Dz size = 0.5*Thpb*Contract, 2 separate volumes for eta<0.8 and eta>0.8
                   double dz_lead_thick=0.5*Thpb*Contract;
                   GeoTrap* thickTrapLead = new GeoTrap(dz_lead_thick,0.,0.,h1(instance),Xt1,Xb1,alpha_t(instance),
                                                                            h1(instance),Xt1,Xb1,alpha_t(instance));
                   std::string thickLeadName= baseName+"ThickAbsLead::Straight";
                   const GeoLogVol* thickTrapLeadLog = new GeoLogVol(thickLeadName,thickTrapLead, myLead);
                   GeoPhysVol * thickTrapLeadPhys = new GeoPhysVol(thickTrapLeadLog);
                   thickTrapGluePhys->add(new GeoTransform(GeoTrf::TranslateX3D(Xtrans)));
                   thickTrapGluePhys->add(thickTrapLeadPhys);

                   double dz_lead_thin = 0.5*Thpb_thin*Contract;
                   GeoTrap* thinTrapLead = new GeoTrap(dz_lead_thin,0.,0.,h1(instance),Xt2,Xb2,alpha_2(instance),
                                                                          h1(instance),Xt2,Xb2,alpha_2(instance));
                   std::string thinLeadName = baseName+"ThinAbsLead::Straight";
                   const GeoLogVol* thinTrapLeadLog = new GeoLogVol(thinLeadName,thinTrapLead, myLead);
                   GeoPhysVol * thinTrapLeadPhys = new GeoPhysVol(thinTrapLeadLog);
                   thickTrapGluePhys->add(new GeoTransform(GeoTrf::TranslateX3D(Xtrans2)));
                   thickTrapGluePhys->add(thinTrapLeadPhys);

                    
                }
		
		{
		  GeoSerialTransformer *st = new GeoSerialTransformer(thinPhys, &TX, Nabsorber); 
		  stacPhysical->add(new GeoSerialIdentifier(irl*10000+idivi*1000000));
		  stacPhysical->add(st);
		  break;
		}
	      }    // loop over instances


              if (idivi==0) {
// Folds 
                double phi0_safety=0.;
                if (irl==0) phi0_safety=0.003;
  
// for most section, one fold = fold at the beginning of this section
// for last section, should also to last fold, at the end of the section

                int irl1=irl;
                int irl2 = (irl==Nbrt-1) ? irl+1 : irl;

                for (int jrl=irl1; jrl<=irl2; jrl++) {

// get slant angle for the previous zig-zag
                 int iirl=jrl-1;
                 if (iirl<0) iirl=1;
                 GeoGenfun::GENFUNCTION x0a = Fx(iirl, Gama, Cenx, Ceny)
                                           +deltay[iirl]*Del1(Gama)
                                           +deltax[iirl]*Del2(Gama);
                 GeoGenfun::GENFUNCTION y0a = Fy(iirl, Gama, Cenx, Ceny)
                                           -deltay[iirl]*Del2(Gama)
                                           +deltax[iirl]*Del1(Gama);
                 GeoGenfun::GENFUNCTION dx0 = x1a - x0a;
                 GeoGenfun::GENFUNCTION dy0 = y1a - y0a;
              // Da the two fold centers distance, da straight part length
                 GeoGenfun::GENFUNCTION Da0 = Sqrt ( dx0*dx0 + dy0*dy0 );
                 GeoGenfun::GENFUNCTION da0 = Sqrt ( (Da0 - 2.*Rint)*(Da0 + 2.*Rint) );
              // newalpha (slant angle) value of the rotation angle around Z_axis
                 GeoGenfun::GENFUNCTION cosalfa0 = (da0*dx0 +iparit*2.*Rint*dy0)/Da0/Da0;
                 GeoGenfun::GENFUNCTION sinalfa0 = (da0*dy0 -iparit*2.*Rint*dx0)/Da0/Da0;
                 GeoGenfun::GENFUNCTION alpha_prev = ATan2(sinalfa0,cosalfa0);

                GeoGenfun::Mod Mod2Pi(2*M_PI);
// down folds  (add +M_PI and then rotate by -M_PI to follow same logic as old code)
                 GeoGenfun::GENFUNCTION phi0_dfold_0 = 
                  GeoGenfun::FixedConstant(M_PI/2.+phi0_safety);
                 GeoGenfun::GENFUNCTION dphi_dfold_0 = Mod2Pi(newalpha-phi0_safety - Gama);
                 GeoGenfun::GENFUNCTION phi0_dfold_1 = Mod2Pi(M_PI/2.+ alpha_prev - Gama);
                 GeoGenfun::GENFUNCTION dphi_dfold_1  = Mod2Pi(newalpha-alpha_prev);
                 GeoGenfun::GENFUNCTION phi0_dfold_2 =  Mod2Pi(M_PI/2.+ newalpha - Gama);
                 GeoGenfun::GENFUNCTION dphi_dfold_2 = Mod2Pi(- newalpha + Gama);
// up folds
                 GeoGenfun::GENFUNCTION phi0_ufold_0 =
                  Mod2Pi(M_PI/2.+newalpha-Gama);
                 GeoGenfun::GENFUNCTION dphi_ufold_0 = Mod2Pi(-newalpha+Gama-phi0_safety);
                 GeoGenfun::GENFUNCTION phi0_ufold_1 = Mod2Pi(M_PI/2. + newalpha - Gama);
                 GeoGenfun::GENFUNCTION dphi_ufold_1 =  Mod2Pi(alpha_prev - newalpha);
                 GeoGenfun::GENFUNCTION phi0_ufold_2 = GeoGenfun::FixedConstant(M_PI/2.);
                 GeoGenfun::GENFUNCTION dphi_ufold_2 = Mod2Pi(newalpha-Gama);

                 const GeoGenfun::AbsFunction* phi0_fold=0;
                 const GeoGenfun::AbsFunction* dphi_fold=0;
                 const GeoXF::Function* TXfold=0;

                 std::string thinName;
                 std::string thickName;
                 if (jrl%2==checkParity) {
                    thinName     = baseName + "ThinAbs::CornerDownFold";
                    thickName    = baseName + "ThickAbs::CornerDownFold";
                 } else {
                    thinName     = baseName + "ThinAbs::CornerUpFold";
                    thickName    = baseName + "ThickAbs::CornerUpFold";
                 }

// radius of tubs for folds
                 double Rcmin=Rint-Thce/2.;
                 double Rcmax=Rint+Thce/2.;

// GU 09/06/2004 add some safety in z size
                 double ddz0  = dz0-safety_zlen;
                 double ddz01 = dz01-safety_zlen;
                 if (jrl==Nbrt){
                    ddz0=dza-safety_zlen;
                    ddz01=dza1-safety_zlen;
                 } 


                 GeoGenfun::GENFUNCTION zpos     = GeoGenfun::FixedConstant(Zmin+dz0);
                 double phirot=0;
                 if (jrl%2==checkParity) phirot = -M_PI;
                 GeoXF::TRANSFUNCTION TXfold1=
                   GeoXF::Pow(GeoTrf::TranslateX3D(1.0),x1a) *
                   GeoXF::Pow(GeoTrf::TranslateY3D(1.0),y1a) *
                   GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),zpos) *
                   GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Gama+phirot);
                 GeoXF::TRANSFUNCTION TXfold2 =
                   GeoXF::Pow(GeoTrf::TranslateX3D(1.0),x2a) *
                   GeoXF::Pow(GeoTrf::TranslateY3D(1.0),y2a) *
                   GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),zpos) *
                   GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Gama+phirot);

// first fown fold
                 if (jrl==0 && checkParity==0) {
                    phi0_fold = &(phi0_dfold_0);
                    dphi_fold = &(dphi_dfold_0);
                    TXfold = &(TXfold1);
                 }
// normal fold
                 if (jrl%2==checkParity && jrl >0 && jrl<Nbrt) {
                    phi0_fold = &(phi0_dfold_1);
                    dphi_fold = &(dphi_dfold_1);
                    TXfold = &(TXfold1);
                 }
// last fold if down
                 if (jrl%2 ==checkParity && jrl == Nbrt) {
                    phi0_fold = &(phi0_dfold_2);
                    dphi_fold = &(dphi_dfold_2);
                    TXfold = &(TXfold2);
                 }
// first up fold
                 if (jrl==0 && checkParity==1) {
                    phi0_fold = &(phi0_ufold_0);
                    dphi_fold = &(dphi_ufold_0);
                    TXfold = &(TXfold1);
                 }
// up fold
                 if (jrl%2 ==(1-checkParity) && jrl>0 && jrl < Nbrt) {
                    phi0_fold = &(phi0_ufold_1);
                    dphi_fold = &(dphi_ufold_1);
                    TXfold = &(TXfold1);
                 }
// last fold if up
                 if (jrl%2==(1-checkParity) && jrl==Nbrt) {
                    phi0_fold = &(phi0_ufold_2);
                    dphi_fold = &(dphi_ufold_2);
                    TXfold = &(TXfold2);
                 }

                 for (int instance = 0; instance < Nabsorber; instance++)
                 {

                  GeoPhysVol* thinPhys = nullptr;

                  if (!doDetailedAbsorberFold) {
                     GeoTubs* thinTubs        = new GeoTubs(Rcmin,Rcmax,ddz0,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     GeoTubs* thickTubs       = new GeoTubs(Rcmin,Rcmax,ddz01,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     const GeoLogVol* thinLog  = new GeoLogVol(thinName,thinTubs,Thin_abs);
                     const GeoLogVol* thickLog = new GeoLogVol(thickName,thickTubs,Thick_abs);

                     thinPhys   = new GeoPhysVol(thinLog);
                     GeoPhysVol* thickPhys  = new GeoPhysVol(thickLog);

                     thinPhys->add(new GeoTransform(GeoTrf::TranslateZ3D(ddz01-ddz0)));
                     thinPhys->add(thickPhys);
                  }

                  if (doDetailedAbsorberFold) {

                     // bigger volume in steel to cover outer edges of absorber
                     GeoTubs* thinTubs        = new GeoTubs(Rcmin,Rcmax,ddz0,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     const GeoLogVol* thinLog  = new GeoLogVol(thinName,thinTubs,myIron);
                     thinPhys   = new GeoPhysVol(thinLog);

                     // put inside volume with glue
                     GeoTubs* glueTubs = new GeoTubs(Rcmin+0.5*Thfe*Contract,Rcmax-0.5*Thfe*Contract,ddz0,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     std::string foldGlueName = baseName+"Glue::Fold";
                     const GeoLogVol* glueTubsLog = new GeoLogVol(foldGlueName,glueTubs,Glue);
                     GeoPhysVol* glueTubsPhys = new GeoPhysVol(glueTubsLog);
                     thinPhys->add(new GeoTransform(GeoTrf::TranslateZ3D(0.)));
                     thinPhys->add(glueTubsPhys); 

                     // put inside glue, volumes for lead, 2 thickness => 2 volumes

                     // thick lead
                     GeoTubs* thickLeadTubs = new GeoTubs(Rint-0.5*Thpb*Contract,Rint+0.5*Thpb*Contract,ddz01,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     std::string foldThickLeadName = baseName+"ThickLead::Fold";
                     const GeoLogVol* thickLeadLog = new GeoLogVol(foldThickLeadName,thickLeadTubs,myLead);
                     GeoPhysVol* thickLeadPhys = new GeoPhysVol(thickLeadLog);
                     glueTubsPhys->add(new GeoTransform(GeoTrf::TranslateZ3D(ddz01-ddz0)));
                     glueTubsPhys->add(thickLeadPhys);

                     // thin lead
                     GeoTubs* thinLeadTubs = new GeoTubs(Rint-0.5*Thpb_thin*Contract,Rint+0.5*Thpb_thin*Contract,ddz0-ddz01,
                                           (*phi0_fold)(instance),(*dphi_fold)(instance));
                     std::string foldThinLeadName = baseName+"ThinLead::Fold";
                     const GeoLogVol* thinLeadLog = new GeoLogVol(foldThinLeadName,thinLeadTubs,myLead);
                     GeoPhysVol* thinLeadPhys = new GeoPhysVol(thinLeadLog);
                     glueTubsPhys->add(new GeoTransform(GeoTrf::TranslateZ3D(ddz01))); 
                     glueTubsPhys->add(thinLeadPhys);

                  }


                  {
                     GeoSerialTransformer *st = new GeoSerialTransformer(thinPhys, TXfold, Nabsorber);
                     stacPhysical->add(new GeoSerialIdentifier(jrl*10000));
                     stacPhysical->add(st);
                     break;
                  }

                 }    // loop over absorber folds instance
                }     // loop over jrl

              } // idivi==0
	    }   // end absorbers

// ========================  Electrodes:
	    {
	   
	      // thickness of electrode
	      double Dze = Thel/2.;
	   
	      // For electrodes
	      GeoGenfun::GENFUNCTION x1e = Fx(irl+0., Game, Cenx, Ceny)
                                           +deltay[irl]*Del1(Game)
                                           +deltax[irl]*Del2(Game);
	      GeoGenfun::GENFUNCTION x2e = Fx(irl+1., Game, Cenx, Ceny)
                                           +deltay[irl+1]*Del1(Game)
                                           +deltax[irl+1]*Del2(Game);
	      GeoGenfun::GENFUNCTION y1e = Fy(irl+0., Game, Cenx, Ceny) 
                                           -deltay[irl]*Del2(Game)
                                           +deltax[irl]*Del1(Game);
	      GeoGenfun::GENFUNCTION y2e = Fy(irl+1., Game, Cenx, Ceny)
                                           -deltay[irl+1]*Del2(Game)
                                           +deltax[irl+1]*Del1(Game);
	      GeoGenfun::GENFUNCTION dxe = x2e - x1e;
	      GeoGenfun::GENFUNCTION dye = y2e - y1e;
	    // De the two fold centers distance, de straight part length
	      GeoGenfun::GENFUNCTION De = Sqrt ( dxe*dxe + dye*dye );
	      GeoGenfun::GENFUNCTION de = Sqrt ( (De - 2.*Rint)*(De + 2.*Rint) );
	   
	    //newalphe (slant angle) value of the rotation angle around Z_axis
	      GeoGenfun::GENFUNCTION cosalfae = (de*dxe -iparit*2.*Rint*dye)/De/De;
	      GeoGenfun::GENFUNCTION sinalfae = (de*dye +iparit*2.*Rint*dxe)/De/De;
	      GeoGenfun::GENFUNCTION newalphe = ATan2(sinalfae,cosalfae);
	   
	   
	    // newalphae is already computed angle wrt z axis
	    // P/2 rotation is to get absorber aligned along local x axis
	    // instead of y, then rotate with angle newalpha
	      GeoGenfun::GENFUNCTION alfrote = -M_PI/2. - newalphe;
	   
	      GeoGenfun::GENFUNCTION Xcde    = (x1e + x2e)/2.+ (2.*idivi-1.)*(1.-frac)*de/2.*cosalfae;
	      GeoGenfun::GENFUNCTION Ycde    = (y1e + y2e)/2.+ (2.*idivi-1.)*(1.-frac)*de/2.*sinalfae;
	      GeoGenfun::GENFUNCTION Zcde       = GeoGenfun::FixedConstant(Zmin+(tl1+bl1)/2.+safety_zlen);
	   
	   
	      GeoGenfun::GENFUNCTION h1e      = de/2.*frac - .007*mm;
	      GeoGenfun::GENFUNCTION alpha_e  = ATan(0.5*(bl1-tl1)/h1e); 
	   
	   
	      GeoXF::TRANSFUNCTION TXE = 
	        GeoXF::Pow(GeoTrf::TranslateX3D(1.0),Xcde) *
	        GeoXF::Pow(GeoTrf::TranslateY3D(1.0),Ycde) *
	        GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),Zcde) * 
	        GeoXF::Pow(GeoTrf::RotateZ3D(1.0),-alfrote)*
	        GeoTrf::RotateY3D(-90*degrees);                    
	   
	   
	      for (int instance = 0; instance < Nelectrode; instance++) 
	      {
		std::string name = baseName + "Electrode::Straight";
		GeoTrap* trap = new GeoTrap(Dze,0.,0.,h1e(instance),tl1,bl1,alpha_e(instance),
                                                      h1e(instance),tl1,bl1,alpha_e(instance));
		const GeoLogVol* logVol = new GeoLogVol(name,trap,Kapton_Cu);
		GeoPhysVol* physVol = new GeoPhysVol(logVol);
		{
		  GeoSerialTransformer *st = new GeoSerialTransformer(physVol, &TXE, Nelectrode); 
		  stacPhysical->add(new GeoSerialIdentifier(irl*10000+idivi*1000000));
		  stacPhysical->add(st);
		  break;
		}
	      }   // loop over instances



              if (idivi==0) {
// Folds 
                double phi0_safety=0.;
                if (irl==0) phi0_safety=0.003;
  
// for most section, one fold = fold at the beginning of this section
// for last section, should also to last fold, at the end of the section

                int irl1=irl;
                int irl2 = (irl==Nbrt-1) ? irl+1 : irl;

                for (int jrl=irl1; jrl<=irl2; jrl++) {

// get slant angle for the previous zig-zag
                 int iirl=jrl-1;
                 if (iirl<0) iirl=1;
                 GeoGenfun::GENFUNCTION x0e = Fx(iirl, Game, Cenx, Ceny)
                                           +deltay[iirl]*Del1(Game)
                                           +deltax[iirl]*Del2(Game);
                 GeoGenfun::GENFUNCTION y0e = Fy(iirl, Game, Cenx, Ceny)
                                           -deltay[iirl]*Del2(Game)
                                           +deltax[iirl]*Del1(Game);
                 GeoGenfun::GENFUNCTION dx0 = x1e - x0e;
                 GeoGenfun::GENFUNCTION dy0 = y1e - y0e;
              // Da the two fold centers distance, da straight part length
                 GeoGenfun::GENFUNCTION Da0 = Sqrt ( dx0*dx0 + dy0*dy0 );
                 GeoGenfun::GENFUNCTION da0 = Sqrt ( (Da0 - 2.*Rint)*(Da0 + 2.*Rint) );
              // newalpha (slant angle) value of the rotation angle around Z_axis
                 GeoGenfun::GENFUNCTION cosalfa0 = (da0*dx0 +iparit*2.*Rint*dy0)/Da0/Da0;
                 GeoGenfun::GENFUNCTION sinalfa0 = (da0*dy0 -iparit*2.*Rint*dx0)/Da0/Da0;
                 GeoGenfun::GENFUNCTION alphe_prev = ATan2(sinalfa0,cosalfa0);

// down folds (add +M_PI and then rotate by -M_PI to follow same logic as old code)
                 GeoGenfun::Mod Mod2Pi(2*M_PI);
                 GeoGenfun::GENFUNCTION phi0_dfold_0 = 
                                       GeoGenfun::FixedConstant(M_PI/2.+phi0_safety);
                 GeoGenfun::GENFUNCTION dphi_dfold_0 = Mod2Pi(newalphe-phi0_safety-Game);
                 GeoGenfun::GENFUNCTION phi0_dfold_1 = Mod2Pi(M_PI/2.+ alphe_prev - Game);
                 GeoGenfun::GENFUNCTION dphi_dfold_1  = Mod2Pi(newalphe-alphe_prev);
                 GeoGenfun::GENFUNCTION phi0_dfold_2 =  Mod2Pi(M_PI/2.+ newalphe - Game);
                 GeoGenfun::GENFUNCTION dphi_dfold_2 = Mod2Pi(- newalphe + Game);  
// up folds
                 GeoGenfun::GENFUNCTION phi0_ufold_0 =
                  Mod2Pi(M_PI/2.+newalphe-Game);
                 GeoGenfun::GENFUNCTION dphi_ufold_0 = Mod2Pi(-newalphe+Game-phi0_safety);
                 GeoGenfun::GENFUNCTION phi0_ufold_1 = Mod2Pi(M_PI/2. + newalphe - Game);
                 GeoGenfun::GENFUNCTION dphi_ufold_1 =  Mod2Pi(alphe_prev - newalphe);
                 GeoGenfun::GENFUNCTION phi0_ufold_2 = GeoGenfun::FixedConstant(M_PI/2.);
                 GeoGenfun::GENFUNCTION dphi_ufold_2 = Mod2Pi(newalphe - Game);

                 const GeoGenfun::AbsFunction* phi0_fold=0;
                 const GeoGenfun::AbsFunction* dphi_fold=0;
                 const GeoXF::Function* TXfold=0;

                 std::string eName;
                 if (jrl%2==checkParity) {
                    eName     = baseName + "Electrode::CornerDownFold";
                 } else {
                    eName     = baseName + "Electrode::CornerUpFold";
                 }

// radius of tubs for folds
                 double Rcmine=Rint-Thel/2.;
                 double Rcmaxe=Rint+Thel/2.;

// GU 09/06/2004 add some safety in z size
                 double ddz0  = dz0-safety_zlen;
                 if (jrl==Nbrt) {
                   ddz0 = dza - safety_zlen;
                 }

                 GeoGenfun::GENFUNCTION zpos     = GeoGenfun::FixedConstant(Zmin+dz0);
                 double phirot = 0;
                 if (jrl%2==checkParity)  phirot = -M_PI;
                 GeoXF::TRANSFUNCTION TXfold1=
                   GeoXF::Pow(GeoTrf::TranslateX3D(1.0),x1e) *
                   GeoXF::Pow(GeoTrf::TranslateY3D(1.0),y1e) *
                   GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),zpos) *
                   GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Game+phirot);
                 GeoXF::TRANSFUNCTION TXfold2 =
                   GeoXF::Pow(GeoTrf::TranslateX3D(1.0),x2e) *
                   GeoXF::Pow(GeoTrf::TranslateY3D(1.0),y2e) *
                   GeoXF::Pow(GeoTrf::TranslateZ3D(1.0),zpos) *
                   GeoXF::Pow(GeoTrf::RotateZ3D(1.0),Game+phirot);

// first fown fold
                 if (jrl==0 && checkParity==0) {
                    phi0_fold = &(phi0_dfold_0);
                    dphi_fold = &(dphi_dfold_0);
                    TXfold = &(TXfold1);
                 }
// normal fold
                 if (jrl%2==checkParity && jrl >0 && jrl<Nbrt) {
                    phi0_fold = &(phi0_dfold_1);
                    dphi_fold = &(dphi_dfold_1);
                    TXfold = &(TXfold1);
                 }
// last fold if down
                 if (jrl%2 ==checkParity && jrl == Nbrt) {
                    phi0_fold = &(phi0_dfold_2);
                    dphi_fold = &(dphi_dfold_2);
                    TXfold = &(TXfold2);
                 }
// first up fold
                 if (jrl==0 && checkParity==1 ) {
                    phi0_fold = &(phi0_ufold_0);
                    dphi_fold = &(dphi_ufold_0);
                    TXfold = &(TXfold1);

                 }
// up fold
                 if (jrl%2 ==(1-checkParity) && jrl>0 && jrl < Nbrt) {
                    phi0_fold = &(phi0_ufold_1);
                    dphi_fold = &(dphi_ufold_1);
                    TXfold = &(TXfold1);
                 }
// last fold if up
                 if (jrl%2==(1-checkParity) && jrl==Nbrt) {
                    phi0_fold = &(phi0_ufold_2);
                    dphi_fold = &(dphi_ufold_2);
                    TXfold = &(TXfold2);
                 }

                 for (int instance = 0; instance < Nelectrode; instance++)
                 {

                  GeoTubs* foldeTubs       = new GeoTubs(Rcmine,Rcmaxe,ddz0,
                                         (*phi0_fold)(instance),(*dphi_fold)(instance));
                  const GeoLogVol* foldeLog  = new GeoLogVol(eName,foldeTubs,Kapton_Cu);

                  GeoPhysVol* foldePhys   = new GeoPhysVol(foldeLog);

                  {
                     GeoSerialTransformer *st = new GeoSerialTransformer(foldePhys, TXfold, Nelectrode);
                     stacPhysical->add(new GeoSerialIdentifier(jrl*10000));
                     stacPhysical->add(st);
                     break;
                  }

                 }    // loop over electrode folds instance
                }     // loop over jrl

              } // idivi==0
	    }    // end electrodes
        }        // end loop over ndivi
      }         //  end loop over irl (zig-zag)
  }
  std::cout << "HELLO from point1" << std::endl;

#endif  //  BUILD_ACCORDION_PLATES
  
}





GeoGenfun::FunctionNoop AccordionPlugin::Fx(double r, GeoGenfun::GENFUNCTION G, const double Cenx[], const double Ceny[] ) const
{
  GeoGenfun::Cos Cos;
  GeoGenfun::Sin Sin;
  int i = (int)rint(r-.1), j = (int)rint(r+.1) ;
  GeoGenfun::GENFUNCTION result =  (Cos(G)*(Cenx[i]+Cenx[j])/2-Sin(G)*(Ceny[i]+Ceny[j])/2) ;
  return GeoGenfun::FunctionNoop(&result);
}

GeoGenfun::FunctionNoop AccordionPlugin::Fy(double r, GeoGenfun::GENFUNCTION G, const double Cenx[], const double Ceny[] ) const
{
  GeoGenfun::Cos Cos;
  GeoGenfun::Sin Sin;
  int i = (int)rint(r-.1), j = (int)rint(r+.1) ;
  GeoGenfun::GENFUNCTION result = (Sin(G)*(Cenx[i]+Cenx[j])/2+Cos(G)*(Ceny[i]+Ceny[j])/2) ;
  return GeoGenfun::FunctionNoop(&result);
}

GeoGenfun::FunctionNoop AccordionPlugin::Del1(GeoGenfun::GENFUNCTION G) const
{
  GeoGenfun::Cos Cos;
  GeoGenfun::Sin Sin;
  GeoGenfun::GENFUNCTION result = (Cos(  G ) * Sin( G ) );
  return GeoGenfun::FunctionNoop(&result);
}

GeoGenfun::FunctionNoop AccordionPlugin::Del2(GeoGenfun::GENFUNCTION G) const
{
  GeoGenfun::Cos Cos;
  GeoGenfun::GENFUNCTION result = (Cos(  G ) * Cos( G ) );
  return GeoGenfun::FunctionNoop(&result);
}


GeoGenfun::FunctionNoop AccordionPlugin::ATan2(GeoGenfun::GENFUNCTION y, GeoGenfun::GENFUNCTION x) const {

  // Manufacture a Theta Function:
  GeoGenfun::Rectangular Theta;
  Theta.x0().setValue(0.0);
  Theta.x1().setValue(DBL_MAX);
  Theta.baseline().setValue(0.0);
  Theta.height().setValue(1.0);

  // Manufacture an ATan function:
  GeoGenfun::ATan ATan;
  

  // Manufacture a Mod function, putting this on the range (0-2PI)
  GeoGenfun::Mod Mod2Pi(2*M_PI);

  // Now take ATan if x is positive 
  
  GeoGenfun::GENFUNCTION result = Theta(x)*ATan(y/x) + Theta(-x)*(Mod2Pi(ATan(y/x)+M_PI)); 
  return GeoGenfun::FunctionNoop(&result);

}




extern "C" AccordionPlugin *createAccordionPlugin() {
  return new AccordionPlugin;
}

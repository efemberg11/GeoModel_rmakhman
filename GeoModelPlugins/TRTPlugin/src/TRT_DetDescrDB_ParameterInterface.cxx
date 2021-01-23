/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "TRT_DetDescrDB_ParameterInterface.h"
#include "TopLevelPlacements.h"

#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits

#include "GeoXmlInpManager/GeoInpRecordset.h"
#include "GeoXmlInpManager/GeoInpRecord.h"

//_________________________________________________________________________________________
TRT_DetDescrDB_ParameterInterface::TRT_DetDescrDB_ParameterInterface()
  : TRTParameterInterface()
  , m_placements(nullptr)
{
  SetValues();
}

//_________________________________________________________________________________________
TRT_DetDescrDB_ParameterInterface::~TRT_DetDescrDB_ParameterInterface() {

  delete[] barrelNumberOfStrawLayersInModule;
  delete[] barrelXOfFirstGlobalAlignmentStraw;
  delete[] barrelYOfFirstGlobalAlignmentStraw;
  delete[] barrelXOfSecondGlobalAlignmentStraw;
  delete[] barrelYOfSecondGlobalAlignmentStraw;
  delete[] barrelNumberOfStrawsInModule;
  delete[] barrelIndexOfSecondGlobalAlignmentStraw;

  for (unsigned int iRing = 0; iRing < nBarrelRings; iRing++) {
    delete[] barrelNumberOfStrawsInStrawLayer[iRing];
    delete[] barrelXOfCoolingTube[iRing];
    delete[] barrelYOfCoolingTube[iRing];
    delete[] shellCornerXPosition[iRing];
    delete[] shellCornerYPosition[iRing];
    
    // The values get copied to ArrayFunction objects so there are safe to delete.
    delete[] strawXPosition[iRing];
    delete[] strawYPosition[iRing];
  };

  delete[] endCapLayerZPositionA;
  delete[] endCapLayerZPositionB;
  delete[] endCapLayerZPositionC;


  delete m_placements;
  

}

//_________________________________________________________________________________________
void TRT_DetDescrDB_ParameterInterface::SetValues() {

  GeoXmlInpManager* inpman = GeoXmlInpManager::getManager();
 
  GeoInpRecordset_ptr switchSet = inpman->getRecordsetPtr("TRTSwitches");
  const GeoInpRecord& switches  = (*switchSet)[0];

  oldConfiguration = true;
  if (!switches.isFieldNull("VERSIONNAME")) {
    oldConfiguration = false;
    versionName = switches.getString("VERSIONNAME");
    layout      = switches.getString("LAYOUT");
    versionDescription =  switches.getString("DESCRIPTION");
  }

  // Flags isCosmicRun and initialLayout don't have any effect if TRTTopLevel table is present.
  // The latter controls which parts are present.
  isCosmicRun = false;
  if (!switches.isFieldNull("COSMICLAYOUT")) {
    isCosmicRun = switches.getInt("COSMICLAYOUT");
  }
  //NB: DONT DO: bool useoldactivegasmixture ( switches.getInt("GASVERSION") == 0 );
  //Since user might have overriden it from joboptions!!

  initialLayout = switches.getInt("INITIALLAYOUT");

  GeoInpRecordset_ptr RDB_DigVersion = inpman->getRecordsetPtr("TRTDigVersion");
  const GeoInpRecord& RDBVars_DigVersion  = (*RDB_DigVersion)[0];
  digversion = RDBVars_DigVersion.getInt("DIGVERSION");
  digversionname = RDBVars_DigVersion.getString("DIGVERSIONNAME");

  /////////////////////////////////////////////////////////////////////////////////////////
  //                                   Top Level Placements                              //
  /////////////////////////////////////////////////////////////////////////////////////////
  if (!oldConfiguration) {
    GeoInpRecordset_ptr topLevelTable = inpman->getRecordsetPtr("TRTTopLevel");
    m_placements = new TopLevelPlacements(topLevelTable);
  }
  else {
    m_placements = new TopLevelPlacements(GeoInpRecordset_ptr());
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  //                                   Extra Scaled Material                             //
  /////////////////////////////////////////////////////////////////////////////////////////

  // NB. This is at Inner Detector node level
  m_scalingTable = inpman->getRecordsetPtr("TrtMatScaling");

  


  /////////////////////////////////////////////////////////////////////////////////////////
  //                     Variables common between Barrel and End Cap                     //
  /////////////////////////////////////////////////////////////////////////////////////////

  GeoInpRecordset_ptr RDB_CommonPars  = inpman->getRecordsetPtr("TRTCommonPars");
  const GeoInpRecord& RDBVars_CommonPars = (*RDB_CommonPars)[0];

  lengthOfDeadRegion = RDBVars_CommonPars.getFloat("LENGTHOFDEADREGION")*SYSTEM_OF_UNITS::mm;
  outerRadiusOfWire  = RDBVars_CommonPars.getFloat("OUTERRADIUSOFWIRE")*SYSTEM_OF_UNITS::mm;
  innerRadiusOfStraw = RDBVars_CommonPars.getFloat("INNERRADIUSOFSTRAW")*SYSTEM_OF_UNITS::mm;
  outerRadiusOfStraw = RDBVars_CommonPars.getFloat("OUTERRADIUSOFSTRAW")*SYSTEM_OF_UNITS::mm;

  /////////////////////////////////////////////////////////////////////////////////////////
  //                        Variables related to the Barrel part                         //
  /////////////////////////////////////////////////////////////////////////////////////////

  GeoInpRecordset_ptr RDB_BarrelOverallPars  = inpman->getRecordsetPtr("TRTBarrelOverallPars");
  const GeoInpRecord& RDBVars_BarrelOverallPars = (*RDB_BarrelOverallPars)[0];
  GeoInpRecordset_ptr RDB_BarrelStrawCoord  = inpman->getRecordsetPtr("TRTBarrelStrawCoord");
  GeoInpRecordset_ptr RDB_BarrelRingDepPars = inpman->getRecordsetPtr("TRTBarrelRingDepPars");
  GeoInpRecordset_ptr RDB_BarrelNStrawInLay = inpman->getRecordsetPtr("TRTBarrelNStrawInLay");
  GeoInpRecordset_ptr RDB_BarrelServices = inpman->getRecordsetPtr("TRTBarrelServices");

  includeBarServiceAndFlange = (RDB_BarrelServices->size()!=0);

  if (includeBarServiceAndFlange) {

    const GeoInpRecord& RDBVars_BarrelServices = (*RDB_BarrelServices)[0];
    barFlangeZMin = RDBVars_BarrelServices.getDouble("BARFLANGEZMIN")*SYSTEM_OF_UNITS::mm;
    barFlangeZMax = RDBVars_BarrelServices.getDouble("BARFLANGEZMAX")*SYSTEM_OF_UNITS::mm;
    barFlangeRMin = RDBVars_BarrelServices.getDouble("BARFLANGERMIN")*SYSTEM_OF_UNITS::mm;
    barFlangeRMax = RDBVars_BarrelServices.getDouble("BARFLANGERMAX")*SYSTEM_OF_UNITS::mm;

    barServicesZMin = RDBVars_BarrelServices.getDouble("BARSERVICESZMIN")*SYSTEM_OF_UNITS::mm;
    barServicesZMax = RDBVars_BarrelServices.getDouble("BARSERVICESZMAX")*SYSTEM_OF_UNITS::mm;
    barServicesRMin = RDBVars_BarrelServices.getDouble("BARSERVICESRMIN")*SYSTEM_OF_UNITS::mm;
    barServicesRMax = RDBVars_BarrelServices.getDouble("BARSERVICESRMAX")*SYSTEM_OF_UNITS::mm;

  } else if (!includeBarServiceAndFlange) {

    barFlangeZMin = -1.;
    barFlangeZMax = -1.;
    barFlangeRMin = -1.;
    barFlangeRMax = -1.;

    barServicesZMin = -1.;
    barServicesZMax = -1.;
    barServicesRMin = -1.;
    barServicesRMax = -1.;
  }

  if (!RDBVars_BarrelOverallPars.isFieldNull("BRLVIRTVOLZCLEARANCE")) {
    barrelVirtualVolumeZClearance    =  RDBVars_BarrelOverallPars.getFloat("BRLVIRTVOLZCLEARANCE")*SYSTEM_OF_UNITS::mm;
    barrelVirtualVolumeRMinClearance =  RDBVars_BarrelOverallPars.getFloat("BRLVIRTVOLRMINCLEAR")*SYSTEM_OF_UNITS::mm;
    barrelVirtualVolumeRMaxClearance =  RDBVars_BarrelOverallPars.getFloat("BRLVIRTVOLRMAXCLEAR")*SYSTEM_OF_UNITS::mm;
  } else {
    barrelVirtualVolumeZClearance    = 0.001*SYSTEM_OF_UNITS::micrometer;//fixme: use epsilon(?).
    barrelVirtualVolumeRMinClearance = 0.001*SYSTEM_OF_UNITS::micrometer;
    barrelVirtualVolumeRMaxClearance = 0.001*SYSTEM_OF_UNITS::micrometer;
  }

  innerRadiusOfBarrelVolume = RDBVars_BarrelOverallPars.getFloat("INNRADIUSOFBARRELVOL")*SYSTEM_OF_UNITS::mm;
  outerRadiusOfBarrelVolume = RDBVars_BarrelOverallPars.getFloat("OUTRADIUSOFBARRELVOL")*SYSTEM_OF_UNITS::mm;
  lengthOfBarrelVolume = RDBVars_BarrelOverallPars.getFloat("LENGTHOFBARRELVOLUME")*SYSTEM_OF_UNITS::mm;
  barrelOuterRadiusOfCoolingTube = RDBVars_BarrelOverallPars.getFloat("OUTRADIUSOFCOOLTUBE")*SYSTEM_OF_UNITS::mm;
  barrelInnerRadiusOfCoolingTube = RDBVars_BarrelOverallPars.getFloat("INNRADIUSOFCOOLTUBE")*SYSTEM_OF_UNITS::mm;
  barrelThicknessOfModuleWalls = RDBVars_BarrelOverallPars.getFloat("THICKOFMODULEWALLS")*SYSTEM_OF_UNITS::mm;
  barrelNumberOfLayersWithLargeDeadRegion = RDBVars_BarrelOverallPars.getInt("NLAYWTHLRGDEADREGION");
  thicknessOfBarrelInnerSupport = RDBVars_BarrelOverallPars.getFloat("THICKOFBRLINNSUPPORT")*SYSTEM_OF_UNITS::mm;
  thicknessOfBarrelOuterSupport = RDBVars_BarrelOverallPars.getFloat("THICKOFBRLOUTSUPPORT")*SYSTEM_OF_UNITS::mm;
  barrelOuterRadiusOfStrawHole = RDBVars_BarrelOverallPars.getFloat("OUTRADIUSOFSTRAWHOLE")*SYSTEM_OF_UNITS::mm;

  virtualBarrelVolumeLength = std::max(std::max(barFlangeZMax,barServicesZMax),lengthOfBarrelVolume/2)
    + barrelVirtualVolumeZClearance;
  virtualBarrelOuterRadius  = std::max(std::max(barFlangeRMax,barServicesRMax),outerRadiusOfBarrelVolume)
    + barrelVirtualVolumeRMaxClearance;

  virtualBarrelInnerRadius  = innerRadiusOfBarrelVolume;
  if (barServicesRMin>0.0 &&virtualBarrelInnerRadius>barServicesRMin) virtualBarrelInnerRadius = barServicesRMin;
  if (barFlangeRMin>0.0 && virtualBarrelInnerRadius>barFlangeRMin) virtualBarrelInnerRadius = barFlangeRMin;
  virtualBarrelInnerRadius -= barrelVirtualVolumeRMinClearance;

  nBarrelModules = RDBVars_BarrelOverallPars.getInt("NBARRELMODULES");
  nBarrelModulesUsed = nBarrelModules; //Hardwired to be all modules. Silly variable anyway.
  nBarrelPhi = RDBVars_BarrelOverallPars.getInt("NBARRELPHI");

  barrelLengthOfStraw  = RDBVars_BarrelOverallPars.getFloat("LENGTHOFSTRAW")*SYSTEM_OF_UNITS::mm;
  barrelLengthOfTwister         = RDBVars_BarrelOverallPars.getFloat("LENGTHOFTWISTER")*SYSTEM_OF_UNITS::mm;
  barrelLengthOfLargeDeadRegion = RDBVars_BarrelOverallPars.getFloat("LENLARGEDEADREGION")*SYSTEM_OF_UNITS::mm;

  barrelNumberOfStrawLayersInModule = new unsigned int[nBarrelRings];
  for (unsigned int i=0; i<nBarrelRings; i++) {
    const GeoInpRecord& ringVars = (*RDB_BarrelRingDepPars)[i];
    barrelNumberOfStrawLayersInModule[i] = ringVars.getInt("NSTRAWLAYERSINMODULE");
  };


  for (unsigned int i = 0; i < nBarrelRings; i++)
    barrelNumberOfStrawsInStrawLayer[i] = new unsigned int[ barrelNumberOfStrawLayersInModule[i] ];

  unsigned int cumulLayer = 0;
  for (unsigned int i=0; i<nBarrelRings; i++)
    for (unsigned int j=0; j<barrelNumberOfStrawLayersInModule[i]; j++) {
      const GeoInpRecord& layerVars = (*RDB_BarrelNStrawInLay)[cumulLayer++];
      barrelNumberOfStrawsInStrawLayer[i][j] = layerVars.getInt("NSTRAWSINSTRAWLAYER");
    }

  barrelXOfFirstGlobalAlignmentStraw  = new double[nBarrelRings];
  barrelYOfFirstGlobalAlignmentStraw  = new double[nBarrelRings];
  barrelXOfSecondGlobalAlignmentStraw = new double[nBarrelRings];
  barrelYOfSecondGlobalAlignmentStraw = new double[nBarrelRings];
  barrelNumberOfStrawsInModule            = new unsigned int[nBarrelRings];
  barrelIndexOfSecondGlobalAlignmentStraw = new unsigned int[nBarrelRings];

  nCoolingTubes = RDBVars_BarrelOverallPars.getInt("NCOOLINGTUBES");
  if ( nCoolingTubes != 2 ) {
    std::cout << "DD Database indicates nCoolingTubes != 2, but the code" << " can't cope with that. Putting nCoolingTubes=2" << std::endl;
    nCoolingTubes = 2;
  };

  nShellCorners = RDBVars_BarrelOverallPars.getInt("NSHELLCORNERS");
  if ( nShellCorners != 4 ) {
    std::cout << "DD Database indicates nShellCorners != 4, but the code" << " can't cope with that. Putting nShellCorners=4" << std::endl;
    nShellCorners = 4;
  };

  for (unsigned int iRing = 0; iRing < nBarrelRings; iRing++) {
    const GeoInpRecord& row = (*RDB_BarrelRingDepPars)[iRing];
    barrelXOfFirstGlobalAlignmentStraw[iRing]  = row.getFloat("XOFGLOBALALIGNSTRW1")*SYSTEM_OF_UNITS::mm;
    barrelYOfFirstGlobalAlignmentStraw[iRing]  = row.getFloat("YOFGLOBALALIGNSTRW1")*SYSTEM_OF_UNITS::mm;
    barrelXOfSecondGlobalAlignmentStraw[iRing] = row.getFloat("XOFGLOBALALIGNSTRW2")*SYSTEM_OF_UNITS::mm;
    barrelYOfSecondGlobalAlignmentStraw[iRing] = row.getFloat("YOFGLOBALALIGNSTRW2")*SYSTEM_OF_UNITS::mm;
    barrelIndexOfSecondGlobalAlignmentStraw[iRing] = row.getInt("INDEX2NDGLBALGNSTRW");
    barrelNumberOfStrawsInModule[iRing] = row.getInt("NSTRAWSINMODULE");

    barrelXOfCoolingTube[iRing] = new double[nCoolingTubes];
    barrelXOfCoolingTube[iRing][0] = row.getFloat("XOFCOOLINGTUBE1")*SYSTEM_OF_UNITS::mm;
    barrelXOfCoolingTube[iRing][1] = row.getFloat("XOFCOOLINGTUBE2")*SYSTEM_OF_UNITS::mm;

    barrelYOfCoolingTube[iRing] = new double[nCoolingTubes];
    barrelYOfCoolingTube[iRing][0] = row.getFloat("YOFCOOLINGTUBE1")*SYSTEM_OF_UNITS::mm;
    barrelYOfCoolingTube[iRing][1] = row.getFloat("YOFCOOLINGTUBE2")*SYSTEM_OF_UNITS::mm;

    shellCornerXPosition[iRing] = new double[nShellCorners];
    shellCornerXPosition[iRing][0] = row.getFloat("SHELLCORNERXPOS1")*SYSTEM_OF_UNITS::mm;
    shellCornerXPosition[iRing][1] = row.getFloat("SHELLCORNERXPOS2")*SYSTEM_OF_UNITS::mm;
    shellCornerXPosition[iRing][2] = row.getFloat("SHELLCORNERXPOS3")*SYSTEM_OF_UNITS::mm;
    shellCornerXPosition[iRing][3] = row.getFloat("SHELLCORNERXPOS4")*SYSTEM_OF_UNITS::mm;

    shellCornerYPosition[iRing] = new double[nShellCorners];
    shellCornerYPosition[iRing][0] = row.getFloat("SHELLCORNERYPOS1")*SYSTEM_OF_UNITS::mm;
    shellCornerYPosition[iRing][1] = row.getFloat("SHELLCORNERYPOS2")*SYSTEM_OF_UNITS::mm;
    shellCornerYPosition[iRing][2] = row.getFloat("SHELLCORNERYPOS3")*SYSTEM_OF_UNITS::mm;
    shellCornerYPosition[iRing][3] = row.getFloat("SHELLCORNERYPOS4")*SYSTEM_OF_UNITS::mm;

  };

  unsigned int cumulativeStrawNumber = 0;
  for (unsigned int i = 0; i < nBarrelRings; i++) {
    strawXPosition[i] = new double[ barrelNumberOfStrawsInModule[i] ];
    strawYPosition[i] = new double[ barrelNumberOfStrawsInModule[i] ];
    for (unsigned int j = 0; j < barrelNumberOfStrawsInModule[i]; j++) {
      const GeoInpRecord& row = (*RDB_BarrelStrawCoord)[cumulativeStrawNumber++];
      strawXPosition[i][j]=row.getFloat("STRAWXPOSITION")*SYSTEM_OF_UNITS::mm;
      strawYPosition[i][j]=row.getFloat("STRAWYPOSITION")*SYSTEM_OF_UNITS::mm;
    };
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //                        Variables related to the End Cap part                        //
  /////////////////////////////////////////////////////////////////////////////////////////

  GeoInpRecordset_ptr RDB_EndCapOverallPars = inpman->getRecordsetPtr("TRTEndCapOverallPars");
  const GeoInpRecord& RDBVars_EndCapOverallPars = (*RDB_EndCapOverallPars)[0];
  GeoInpRecordset_ptr RDB_ECWheelTypDepPars = inpman->getRecordsetPtr("TRTECWheelTypDepPars");
  const GeoInpRecord& RDBVars_A_Wheel = (*RDB_ECWheelTypDepPars)[0];
  const GeoInpRecord& RDBVars_B_Wheel = (*RDB_ECWheelTypDepPars)[1];
  const GeoInpRecord& RDBVars_C_Wheel = (*RDB_ECWheelTypDepPars)[2];
  GeoInpRecordset_ptr RDB_EndCapStrwLayZPos = inpman->getRecordsetPtr("TRTEndCapStrwLayZPos");
  GeoInpRecordset_ptr RDB_ECMembranePars = inpman->getRecordsetPtr("TRTECMembranePars");
  GeoInpRecordset_ptr RDB_ECHeatExchAndFoil = inpman->getRecordsetPtr("TRTECHeatExchAndFoil");
  GeoInpRecordset_ptr RDB_ECWheelDepPars = inpman->getRecordsetPtr("TRTECWheelDepPars");
  GeoInpRecordset_ptr RDB_ECEndCapDepPars = inpman->getRecordsetPtr("TRTECEndCapDepPars");

  innerRadiusOfEndCapVolumeAB = RDBVars_EndCapOverallPars.getFloat("INNERRADIUSOFECVOLAB")*SYSTEM_OF_UNITS::mm;
  outerRadiusOfEndCapVolumeAB = RDBVars_EndCapOverallPars.getFloat("OUTERRADIUSOFECVOLAB")*SYSTEM_OF_UNITS::mm;
  innerRadiusOfEndCapVolumeC  = RDBVars_EndCapOverallPars.getFloat("INNERRADIUSOFECVOLC")*SYSTEM_OF_UNITS::mm;
  outerRadiusOfEndCapVolumeC  = RDBVars_EndCapOverallPars.getFloat("OUTERRADIUSOFECVOLC")*SYSTEM_OF_UNITS::mm;
  lengthOfEndCapVolumeAB    = RDBVars_EndCapOverallPars.getFloat("LENGTHOFENDCAPVOLAB")*SYSTEM_OF_UNITS::mm; 
  positionOfEndCapVolumeAB = RDBVars_EndCapOverallPars.getFloat("POSFORENDCAPVOLAB")*SYSTEM_OF_UNITS::mm;
  lengthOfEndCapVolumeC    = RDBVars_EndCapOverallPars.getFloat("LENGTHOFENDCAPVOLC")*SYSTEM_OF_UNITS::mm;
  positionOfEndCapVolumeC  = RDBVars_EndCapOverallPars.getFloat("POSITIONOFENDCAPVOLC")*SYSTEM_OF_UNITS::mm;//rename!!!

  if ( RDBVars_A_Wheel.getFloat("OUTERRADIUSOFINNERSUP") != RDBVars_B_Wheel.getFloat("OUTERRADIUSOFINNERSUP") )
    std::cout << "DD Database indicates different outer radii of inner support for A and B wheels." << " The code assumes otherwise. Using value of A wheel for both!" << std::endl;
  if ( RDBVars_A_Wheel.getFloat("INNERRADIUSOFOUTERSUP") != RDBVars_B_Wheel.getFloat("INNERRADIUSOFOUTERSUP") )
    std::cout << "DD Database indicates different inner radii of outer support for A and B wheels." << " The code assumes otherwise. Using value of A wheel for both!" << std::endl;
  endcapOuterRadiusOfInnerSupport_wheelAB = RDBVars_A_Wheel.getFloat("OUTERRADIUSOFINNERSUP")*SYSTEM_OF_UNITS::mm;
  endcapInnerRadiusOfOuterSupport_wheelAB = RDBVars_A_Wheel.getFloat("INNERRADIUSOFOUTERSUP")*SYSTEM_OF_UNITS::mm;
  endcapOuterRadiusOfInnerSupport_wheelC  = RDBVars_C_Wheel.getFloat("OUTERRADIUSOFINNERSUP")*SYSTEM_OF_UNITS::mm;
  endcapInnerRadiusOfOuterSupport_wheelC  = RDBVars_C_Wheel.getFloat("INNERRADIUSOFOUTERSUP")*SYSTEM_OF_UNITS::mm;

  endCapShiftForEachRotation = RDBVars_EndCapOverallPars.getFloat("SHIFTFOREACHROTATION")*SYSTEM_OF_UNITS::radian;

  endCapPhiOfFirstStraw = 0; // Temporary

 
  endCapInnerRadiusOfSupportA = RDBVars_A_Wheel.getFloat("INNERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;
  endCapInnerRadiusOfSupportB = RDBVars_B_Wheel.getFloat("INNERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;
  endCapInnerRadiusOfSupportC = RDBVars_C_Wheel.getFloat("INNERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;

  endCapOuterRadiusOfSupportA = RDBVars_A_Wheel.getFloat("OUTERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;
  endCapOuterRadiusOfSupportB = RDBVars_B_Wheel.getFloat("OUTERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;
  endCapOuterRadiusOfSupportC = RDBVars_C_Wheel.getFloat("OUTERRADIUSOFSUPPORT")*SYSTEM_OF_UNITS::mm;

  endCapRadialThicknessOfInnerSupportA = RDBVars_A_Wheel.getFloat("RADIALTHICKOFINNERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialThicknessOfInnerSupportB = RDBVars_B_Wheel.getFloat("RADIALTHICKOFINNERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialThicknessOfInnerSupportC = RDBVars_C_Wheel.getFloat("RADIALTHICKOFINNERSUP")*SYSTEM_OF_UNITS::mm;

  endCapRadialThicknessOfOuterSupportA = RDBVars_A_Wheel.getFloat("RADIALTHICKOFOUTERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialThicknessOfOuterSupportB = RDBVars_B_Wheel.getFloat("RADIALTHICKOFOUTERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialThicknessOfOuterSupportC = RDBVars_C_Wheel.getFloat("RADIALTHICKOFOUTERSUP")*SYSTEM_OF_UNITS::mm;

  endCapRadialDistFromRadToOuterSupportA = RDBVars_A_Wheel.getFloat("RADIALDISTRADTOOUTERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialDistFromRadToOuterSupportB = RDBVars_B_Wheel.getFloat("RADIALDISTRADTOOUTERSUP")*SYSTEM_OF_UNITS::mm;
  endCapRadialDistFromRadToOuterSupportC = RDBVars_C_Wheel.getFloat("RADIALDISTRADTOOUTERSUP")*SYSTEM_OF_UNITS::mm;

  endCapMainRadiatorThicknessA = RDBVars_A_Wheel.getFloat("MAINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;
  endCapMainRadiatorThicknessB = RDBVars_B_Wheel.getFloat("MAINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;
  endCapMainRadiatorThicknessC = RDBVars_C_Wheel.getFloat("MAINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;

  endCapThinRadiatorThicknessA = RDBVars_A_Wheel.getFloat("THINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;
  endCapThinRadiatorThicknessB = RDBVars_B_Wheel.getFloat("THINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;
  endCapThinRadiatorThicknessC = RDBVars_C_Wheel.getFloat("THINRADIATORTHICK")*SYSTEM_OF_UNITS::mm;

  endCapMiddleRadiatorThicknessB = RDBVars_B_Wheel.getFloat("MIDDLERADIATORTHICK")*SYSTEM_OF_UNITS::mm;
  if ( RDBVars_A_Wheel.getFloat("MIDDLERADIATORTHICK")*SYSTEM_OF_UNITS::mm != 0 )
    std::cout << "DD Database indicates a nonzero middle radiator thickness in the A wheel" << " where middle radiators doesn't exist. Ignoring parameter." << std::endl;
  if ( RDBVars_C_Wheel.getFloat("MIDDLERADIATORTHICK")*SYSTEM_OF_UNITS::mm != 0 )
    std::cout << "DD Database indicates a nonzero middle radiator thickness in the C wheel" << " where middle radiators doesn't exist. Ignoring parameter." << std::endl;

  endCapLengthOfWheelsA = RDBVars_A_Wheel.getFloat("LENGTHOFWHEELS")*SYSTEM_OF_UNITS::mm; 
  endCapLengthOfWheelsB = RDBVars_B_Wheel.getFloat("LENGTHOFWHEELS")*SYSTEM_OF_UNITS::mm; 

  endCapLengthOfWheelsC = RDBVars_C_Wheel.getFloat("LENGTHOFWHEELS")*SYSTEM_OF_UNITS::mm;

  endCapNumberOfStrawLayersPerWheelA = RDBVars_A_Wheel.getInt("NSTRAWLAYERSPERWHEEL");
  endCapNumberOfStrawLayersPerWheelB = RDBVars_B_Wheel.getInt("NSTRAWLAYERSPERWHEEL");
  endCapNumberOfStrawLayersPerWheelC = RDBVars_C_Wheel.getInt("NSTRAWLAYERSPERWHEEL");


  endcapNumberOfAWheels = RDBVars_A_Wheel.getInt("NWHEELSOFTYPE");
  endcapNumberOfBWheels = RDBVars_B_Wheel.getInt("NWHEELSOFTYPE");
  endcapNumberOfCWheels = RDBVars_C_Wheel.getInt("NWHEELSOFTYPE");



  endcapNumberOfStrawsInStrawLayer_AWheels = RDBVars_A_Wheel.getInt("NSTRAWSINSTRAWLAYER");
  endcapNumberOfStrawsInStrawLayer_BWheels = RDBVars_B_Wheel.getInt("NSTRAWSINSTRAWLAYER");
  endcapNumberOfStrawsInStrawLayer_CWheels = RDBVars_C_Wheel.getInt("NSTRAWSINSTRAWLAYER");

  includeECFoilHeatExchangerAndMembranes = (RDB_ECHeatExchAndFoil->size()!=0);
  
  if ( includeECFoilHeatExchangerAndMembranes ) {
    
    const GeoInpRecord& RDBECEndCapDepPars_EndCap_C = (*RDB_ECEndCapDepPars)[0];
    const GeoInpRecord& RDBECEndCapDepPars_EndCap_A = (*RDB_ECEndCapDepPars)[1];
  

    
    endCapPositionOfFirstWheelA[0] =  RDBECEndCapDepPars_EndCap_A.getDouble("ZPOSOFFIRSTAWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelA[1] =  RDBECEndCapDepPars_EndCap_C.getDouble("ZPOSOFFIRSTAWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelB[0] =  RDBECEndCapDepPars_EndCap_A.getDouble("ZPOSOFFIRSTBWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelB[1] =  RDBECEndCapDepPars_EndCap_C.getDouble("ZPOSOFFIRSTBWHEEL")*SYSTEM_OF_UNITS::mm;

    for (unsigned int iiWheel = 0; iiWheel < endcapNumberOfAWheels; iiWheel++) {
      const GeoInpRecord& row = (*RDB_ECWheelDepPars)[iiWheel];
      endCapDistanceBetweenWheelCentersA[0][iiWheel]=row.getDouble("DISTBETWWHEELCENTECC")*SYSTEM_OF_UNITS::mm;
      endCapDistanceBetweenWheelCentersA[1][iiWheel]=row.getDouble("DISTBETWWHEELCENTECA")*SYSTEM_OF_UNITS::mm;
    };
    for (unsigned int iiWheel = endcapNumberOfAWheels; iiWheel < (endcapNumberOfAWheels+endcapNumberOfBWheels); iiWheel++) {
      const GeoInpRecord& row = (*RDB_ECWheelDepPars)[iiWheel];
      endCapDistanceBetweenWheelCentersB[0][iiWheel]=row.getDouble("DISTBETWWHEELCENTECC")*SYSTEM_OF_UNITS::mm;
      endCapDistanceBetweenWheelCentersB[1][iiWheel]=row.getDouble("DISTBETWWHEELCENTECA")*SYSTEM_OF_UNITS::mm;
    };
  } else {
    
    endCapPositionOfFirstWheelA[0] = RDBVars_A_Wheel.getFloat("POSITIONOFFIRSTWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelA[1] = RDBVars_A_Wheel.getFloat("POSITIONOFFIRSTWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelB[0] = RDBVars_B_Wheel.getFloat("POSITIONOFFIRSTWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapPositionOfFirstWheelB[1] = RDBVars_B_Wheel.getFloat("POSITIONOFFIRSTWHEEL")*SYSTEM_OF_UNITS::mm;
    endCapDistanceBetweenWheelCentersA[0][0]=0.;
    endCapDistanceBetweenWheelCentersA[1][0]=0.;
    endCapDistanceBetweenWheelCentersB[0][endcapNumberOfAWheels]=0.;
    endCapDistanceBetweenWheelCentersB[1][endcapNumberOfAWheels]=0.;
    for (unsigned int iiWheel = 1; iiWheel < endcapNumberOfAWheels; iiWheel++) {
      endCapDistanceBetweenWheelCentersA[0][iiWheel]=RDBVars_A_Wheel.getFloat("DISTBETWWHEELCENTERS")*SYSTEM_OF_UNITS::mm;
      endCapDistanceBetweenWheelCentersA[1][iiWheel]=RDBVars_A_Wheel.getFloat("DISTBETWWHEELCENTERS")*SYSTEM_OF_UNITS::mm;
    };
    for (unsigned int iiWheel = (endcapNumberOfAWheels+1); iiWheel < (endcapNumberOfAWheels+endcapNumberOfBWheels); iiWheel++) {
      endCapDistanceBetweenWheelCentersB[0][iiWheel]=RDBVars_B_Wheel.getFloat("DISTBETWWHEELCENTERS")*SYSTEM_OF_UNITS::mm;
      endCapDistanceBetweenWheelCentersB[1][iiWheel]=RDBVars_B_Wheel.getFloat("DISTBETWWHEELCENTERS")*SYSTEM_OF_UNITS::mm;
    };
  };
  
  endCapDistanceBetweenWheelCentersC = RDBVars_C_Wheel.getFloat("DISTBETWWHEELCENTERS")*SYSTEM_OF_UNITS::mm;
  endCapPositionOfFirstWheelC = RDBVars_C_Wheel.getFloat("POSITIONOFFIRSTWHEEL")*SYSTEM_OF_UNITS::mm;
  
  
  endCapLayerZPositionA = new double[endCapNumberOfStrawLayersPerWheelA];
  endCapLayerZPositionB = new double[endCapNumberOfStrawLayersPerWheelB];
  endCapLayerZPositionC = new double[endCapNumberOfStrawLayersPerWheelC];
  
  for (unsigned int iiPlane = 0; iiPlane < endCapNumberOfStrawLayersPerWheelA; iiPlane++) {
    const GeoInpRecord& row = (*RDB_EndCapStrwLayZPos)[iiPlane];
    endCapLayerZPositionA[iiPlane] = row.getFloat("LAYERZPOSAWHEELS")*SYSTEM_OF_UNITS::mm; 
  };
  for (unsigned int iiPlane = 0; iiPlane < endCapNumberOfStrawLayersPerWheelB; iiPlane++) {
    const GeoInpRecord& row = (*RDB_EndCapStrwLayZPos)[iiPlane];
    endCapLayerZPositionB[iiPlane] = row.getFloat("LAYERZPOSBWHEELS")*SYSTEM_OF_UNITS::mm; 
  };
  for (unsigned int iiPlane = 0; iiPlane < endCapNumberOfStrawLayersPerWheelC; iiPlane++) {
    const GeoInpRecord& row = (*RDB_EndCapStrwLayZPos)[iiPlane];
    endCapLayerZPositionC[iiPlane] = row.getFloat("LAYERZPOSCWHEELS")*SYSTEM_OF_UNITS::mm;
  };
 

  
  if (includeECFoilHeatExchangerAndMembranes) {

    const GeoInpRecord& RDBVars_ECHeatExchAndFoil = (*RDB_ECHeatExchAndFoil)[0];

    // Faraday foils kapton
    endCapFaradayFoilThickness = RDBVars_ECHeatExchAndFoil.getDouble("ECFARADAYFOILTHICK")*SYSTEM_OF_UNITS::mm;
    
    // Heat Exchanger
    endCapRMinOfHeatExchanger  = RDBVars_ECHeatExchAndFoil.getDouble("ECHEATEXCHANGERRMIN")*SYSTEM_OF_UNITS::mm;
    endCapRMaxOfHeatExchanger  = RDBVars_ECHeatExchAndFoil.getDouble("ECHEATEXCHANGERRMAX")*SYSTEM_OF_UNITS::mm;
    endCapHeatExchangerThicknessA  = RDBVars_ECHeatExchAndFoil.getDouble("ECHEATEXCHTHICKNESSA")*SYSTEM_OF_UNITS::mm;
    endCapHeatExchangerThicknessB  = RDBVars_ECHeatExchAndFoil.getDouble("ECHEATEXCHTHICKNESSB")*SYSTEM_OF_UNITS::mm;
    
    // Membranes 
    const GeoInpRecord& RDBVars_ECMembranePars = (*RDB_ECMembranePars)[0];

    endCapRMinOfMbrane = RDBVars_ECMembranePars.getDouble("RMINOFENDCAPMBRANE")*SYSTEM_OF_UNITS::mm;
    endCapRMaxOfMbrane = RDBVars_ECMembranePars.getDouble("RMAXOFENDCAPMBRANE")*SYSTEM_OF_UNITS::mm;
    endCapZMinOfMbraneWheelA1 = RDBVars_ECMembranePars.getDouble("ZMINPOSMBRANEWHEELA1")*SYSTEM_OF_UNITS::mm;
    endCapZMaxOfMbraneWheelA1 = RDBVars_ECMembranePars.getDouble("ZMAXPOSMBRANEWHEELA1")*SYSTEM_OF_UNITS::mm;
    endCapZMinOfMbraneWheelA2 = RDBVars_ECMembranePars.getDouble("ZMINPOSMBRANEWHEELA2")*SYSTEM_OF_UNITS::mm;
    endCapZMaxOfMbraneWheelA2 = RDBVars_ECMembranePars.getDouble("ZMAXPOSMBRANEWHEELA2")*SYSTEM_OF_UNITS::mm;
    endCapZMinOfMbraneWheelB1 = RDBVars_ECMembranePars.getDouble("ZMINPOSMBRANEWHEELB1")*SYSTEM_OF_UNITS::mm;
    endCapZMaxOfMbraneWheelB1 = RDBVars_ECMembranePars.getDouble("ZMAXPOSMBRANEWHEELB1")*SYSTEM_OF_UNITS::mm;
    endCapZMinOfMbraneWheelB2 = RDBVars_ECMembranePars.getDouble("ZMINPOSMBRANEWHEELB2")*SYSTEM_OF_UNITS::mm;
    endCapZMaxOfMbraneWheelB2 = RDBVars_ECMembranePars.getDouble("ZMAXPOSMBRANEWHEELB2")*SYSTEM_OF_UNITS::mm;

    endCapThicknessOfMbrane = endCapZMaxOfMbraneWheelA1 - endCapZMinOfMbraneWheelA1;

  }
}

const GeoTrf::Transform3D &
TRT_DetDescrDB_ParameterInterface::partTransform(const std::string & partName) const
{
  if (m_placements) return m_placements->transform(partName);
  static const GeoTrf::Transform3D unitTransform = GeoTrf::Transform3D::Identity();
  return unitTransform;
}


bool
TRT_DetDescrDB_ParameterInterface::partPresent(const std::string & partName) const
{
  if (!m_placements) return true;
  return m_placements->present(partName);
}


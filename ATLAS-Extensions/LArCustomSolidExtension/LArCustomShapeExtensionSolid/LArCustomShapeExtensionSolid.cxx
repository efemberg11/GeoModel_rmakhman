#include "GeoModelKernel/GeoVG4ExtensionSolid.h"
#include "GeoModelKernel/GeoUnidentifiedShape.h"
#include "LArWheelSolid_type.h"
#include "LArWheelSolid.h"
#include "LArWheelSliceSolid.h"
#include "LArWheelSolid_type.h"
#include "GeoSpecialShapes/EMECData.h"
#include <iostream>
class LArCustomShapeExtensionSolid:public GeoVG4ExtensionSolid {

public:

  
  //Constructor:
  LArCustomShapeExtensionSolid();

  //Destructor:
  virtual ~LArCustomShapeExtensionSolid();

  //Convert to G4
  G4VSolid *newG4Solid(GeoUnidentifiedShape const*) const;

private:

  typedef std::map<std::string, G4VSolid*,std::less<std::string> > CustomSolidMap;
  mutable CustomSolidMap customSolids;
  
};

//Constructor:
LArCustomShapeExtensionSolid::LArCustomShapeExtensionSolid() {
  std::cout << "HELLO from LArCustomShapeExtensionSolid" << std::endl;
}
  //Destructor:
LArCustomShapeExtensionSolid::~LArCustomShapeExtensionSolid() {
  std::cout << "GOODBYE from LArCustomShapeExtensionSolid" << std::endl;
}

G4VSolid *LArCustomShapeExtensionSolid::newG4Solid(GeoUnidentifiedShape const* customShape) const{

  typedef std::pair<LArWheelSolid_t, int> LArWheelSolidDef_t;
  typedef std::map<std::string,  LArWheelSolidDef_t> TypeMap;
  static const TypeMap types = {
    /* entries w/o explicit Pos/Neg kept for backward compatibility */
    { "LAr::EMEC::InnerWheel::Absorber",           {InnerAbsorberWheel, 1} },
    { "LAr::EMEC::InnerWheel::Electrode",          {InnerElectrodWheel, 1} },
    { "LAr::EMEC::InnerWheel::Glue",               {InnerGlueWheel, 1} },
    { "LAr::EMEC::InnerWheel::Lead",               {InnerLeadWheel, 1} },
    
    { "LAr::EMEC::OuterWheel::Absorber",           {OuterAbsorberWheel, 1} },
    { "LAr::EMEC::OuterWheel::Electrode",          {OuterElectrodWheel, 1} },
    { "LAr::EMEC::OuterWheel::Glue",               {OuterGlueWheel, 1} },
    { "LAr::EMEC::OuterWheel::Lead",               {OuterLeadWheel, 1} },
    
    { "LAr::EMEC::Pos::InnerWheel::Absorber",      {InnerAbsorberWheel, 1} },
    { "LAr::EMEC::Pos::InnerWheel::Electrode",     {InnerElectrodWheel, 1} },
    { "LAr::EMEC::Pos::InnerWheel::Glue",          {InnerGlueWheel, 1} },
    { "LAr::EMEC::Pos::InnerWheel::Lead",          {InnerLeadWheel, 1} },
    
    { "LAr::EMEC::Pos::OuterWheel::Absorber",      {OuterAbsorberWheel, 1} },
    { "LAr::EMEC::Pos::OuterWheel::Electrode",     {OuterElectrodWheel, 1} },
    { "LAr::EMEC::Pos::OuterWheel::Glue",          {OuterGlueWheel, 1} },
    { "LAr::EMEC::Pos::OuterWheel::Lead",          {OuterLeadWheel, 1} },
    
    { "LAr::EMEC::Neg::InnerWheel::Absorber",      {InnerAbsorberWheel, -1} },
    { "LAr::EMEC::Neg::InnerWheel::Electrode",     {InnerElectrodWheel, -1} },
    { "LAr::EMEC::Neg::InnerWheel::Glue",          {InnerGlueWheel, -1} },
    { "LAr::EMEC::Neg::InnerWheel::Lead",          {InnerLeadWheel, -1} },
    
    { "LAr::EMEC::Neg::OuterWheel::Absorber",      {OuterAbsorberWheel, -1} },
    { "LAr::EMEC::Neg::OuterWheel::Electrode",     {OuterElectrodWheel, -1} },
    { "LAr::EMEC::Neg::OuterWheel::Glue",          {OuterGlueWheel, -1} },
    { "LAr::EMEC::Neg::OuterWheel::Lead",          {OuterLeadWheel, -1} },
    
    { "LAr::EMEC::InnerModule::Absorber",          {InnerAbsorberModule, 1} },
    { "LAr::EMEC::InnerModule::Electrode",         {InnerElectrodModule, 1} },
    { "LAr::EMEC::OuterModule::Absorber",          {OuterAbsorberModule, 1} },
    { "LAr::EMEC::OuterModule::Electrode",         {OuterElectrodModule, 1} },
    
    { "LAr::EMEC::Pos::InnerCone::Absorber",       {InnerAbsorberCone, 1} },
    { "LAr::EMEC::Pos::InnerCone::Electrode",      {InnerElectrodCone, 1} },
    { "LAr::EMEC::Pos::InnerCone::Glue",           {InnerGlueCone, 1} },
    { "LAr::EMEC::Pos::InnerCone::Lead",           {InnerLeadCone, 1} },
    
    { "LAr::EMEC::Neg::InnerCone::Absorber",       {InnerAbsorberCone, -1} },
    { "LAr::EMEC::Neg::InnerCone::Electrode",      {InnerElectrodCone, -1} },
    { "LAr::EMEC::Neg::InnerCone::Glue",           {InnerGlueCone, -1} },
    { "LAr::EMEC::Neg::InnerCone::Lead",           {InnerLeadCone, -1} },
    
    { "LAr::EMEC::Pos::OuterFrontCone::Absorber",  {OuterAbsorberFrontCone, 1} },
    { "LAr::EMEC::Pos::OuterFrontCone::Electrode", {OuterElectrodFrontCone, 1} },
    { "LAr::EMEC::Pos::OuterFrontCone::Glue",      {OuterGlueFrontCone, 1} },
    { "LAr::EMEC::Pos::OuterFrontCone::Lead",      {OuterLeadFrontCone, 1} },
    
    { "LAr::EMEC::Neg::OuterFrontCone::Absorber",  {OuterAbsorberFrontCone, -1} },
    { "LAr::EMEC::Neg::OuterFrontCone::Electrode", {OuterElectrodFrontCone, -1} },
    { "LAr::EMEC::Neg::OuterFrontCone::Glue",      {OuterGlueFrontCone, -1} },
    { "LAr::EMEC::Neg::OuterFrontCone::Lead",      {OuterLeadFrontCone, -1} },
    
    { "LAr::EMEC::Pos::OuterBackCone::Absorber",   {OuterAbsorberBackCone, 1} },
    { "LAr::EMEC::Pos::OuterBackCone::Electrode",  {OuterElectrodBackCone, 1} },
    { "LAr::EMEC::Pos::OuterBackCone::Glue",       {OuterGlueBackCone, 1} },
    { "LAr::EMEC::Pos::OuterBackCone::Lead",       {OuterLeadBackCone, 1} },
    
    { "LAr::EMEC::Neg::OuterBackCone::Absorber",   {OuterAbsorberBackCone, -1} },
    { "LAr::EMEC::Neg::OuterBackCone::Electrode",  {OuterElectrodBackCone, -1} },
    { "LAr::EMEC::Neg::OuterBackCone::Glue",       {OuterGlueBackCone, -1} },
    { "LAr::EMEC::Neg::OuterBackCone::Lead",       {OuterLeadBackCone, -1} }
    
  };

 EMECData data;
  EMECGEOMETRY emecgeometry;
  EMECWHEELPARAMETERS emecwheelparameters[2];
  EMECPARAMS emecparams;
  EMECMAGICNUMBERS emecmagicnumbers;
  EMECFAN emecfan;
  COLDCONTRACTION  coldcontraction;

  emecgeometry.Z0=368.95;
  emecgeometry.Z1=369.1;
  emecgeometry.DCF=368.9;
  emecgeometry.DCRACK=.15;
  emecgeometry.RLIMIT=203.4;
  emecgeometry.ZSHIFT=4.5;

  emecwheelparameters[0].ETAINT=3.2;
  emecwheelparameters[0].ETAEXT=2.5;
  emecwheelparameters[0].NABS=256;
  emecwheelparameters[0].NACC=6;
  emecwheelparameters[1].ETAEXT=1.375;
  emecwheelparameters[1].NABS=768;
  emecwheelparameters[1].NACC=9;

  emecmagicnumbers.ACTIVELENGTH=510;
  emecmagicnumbers.STRAIGHTSTARTSECTION=2;
  emecmagicnumbers.REFTOACTIVE=11;

  emecparams.PHIROTATION="off";
  emecparams.SAGGING="off";
  emecparams.INNERSLANTPARAM="default";
  emecparams.OUTERSLANTPARAM="default";

  coldcontraction.ABSORBERCONTRACTION=0.991;
  coldcontraction.ELECTRODEINVCONTRACTION=1.0036256;

  emecfan.LEADTHICKNESSINNER=2.2;
  emecfan.LEADTHICKNESSOUTER=1.69;
  emecfan.STEELTHICKNESS=0.2;
  emecfan.GLUETHICKNESS=0.1;
  emecfan.ELECTRODETOTALTHICKNESS=0.275;
    
  data.emecgeometry.push_back(emecgeometry);
  data.emecwheelparameters.push_back(emecwheelparameters[0]);
  data.emecwheelparameters.push_back(emecwheelparameters[1]);
  data.emecmagicnumbers.push_back(emecmagicnumbers);
  data.emecparams.push_back(emecparams);
  data.emecfan.push_back(emecfan);
  data.coldcontraction.push_back(coldcontraction);


  

  G4VSolid *theSolid=nullptr;
  
  if (customShape->name()=="LArCustomShape") {
    std::string customName = customShape->asciiData();
    CustomSolidMap::const_iterator it = customSolids.find(customName);
    if(it!=customSolids.end())
      theSolid = it->second;
    else {
      theSolid = nullptr;
      
      if(customName.find("Slice") != std::string::npos){
	theSolid = new LArWheelSliceSolid(customShape->asciiData(),&data);
      } else {
	theSolid = new LArWheelSolid(customShape->asciiData(), types.at(customShape->asciiData()).first, types.at(customShape->asciiData()).second,nullptr,&data); 
      }
      if ( nullptr == theSolid ) {
	std::string error = std::string("Can't create LArWheelSolid for name ") + customName + " in LArCustomShapeExtensionSolid";
	throw std::runtime_error(error);
      }
      if(theSolid != nullptr) customSolids[customName] = theSolid;
      
    }
    
  }
  
  return theSolid;
}


extern "C" LArCustomShapeExtensionSolid *createLArCustomShapeExtensionSolid() {

  return new LArCustomShapeExtensionSolid;
}

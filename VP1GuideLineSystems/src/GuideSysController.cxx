/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class GuideSysController                //
//                                                            //
//  Author: Thomas H. Kittelmann (Thomas.Kittelmann@cern.ch)  //
//  Initial version: July 2008                                //
//                                                            //
////////////////////////////////////////////////////////////////

#define VP1IMPVARNAME m_d

#include "VP1GuideLineSystems/GuideSysController.h"
#include "VP1GuideLineSystems/VP1Floor.h"
#include "ui_guidelinescontrollerform.h"
#include "ui_guides_settings_coordinateaxes_form.h"
#include "ui_guides_settings_etacones_form.h"
#include "ui_guides_settings_floorandletters_form.h"
#include "ui_guides_settings_grid_form.h"
#include "ui_guides_settings_lines_form.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"

#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'


#include <cmath>

//____________________________________________________________________
class GuideSysController::Imp {
public:
  GuideSysController * theclass;
  Ui::VP1GuidesControllerForm ui;
  Ui::VP1GuidesSysSettingsCoordinateAxesForm ui_axes;
  Ui::VP1GuidesSysSettingsEtaConeForm ui_etacones;
  Ui::VP1GuidesSysSettingsFloorAndLettersForm ui_floorandletters;
  Ui::VP1GuidesSysSettingsGridForm ui_grid;
  Ui::VP1LinesForm ui_lines;

  static SbColor4f color4f(const QColor& col, int transp_int) {
    return SbColor4f(std::max<float>(0.0f,std::min<float>(1.0f,col.redF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.greenF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,col.blueF())),
		     std::max<float>(0.0f,std::min<float>(1.0f,1.0-transp_int/100.0)));
  }


  bool last_showFloor;
  SbColor4f last_floorColourAndTransp;
  double last_floorExtent;
  double last_floorSpacing;
  double last_floorHeight;
  bool last_showLetters;
  double last_lettersZPos;
  double last_lettersVerticalPos;
  bool last_showAxes;
  double last_axesLength;
  SbVec3f last_axesPosition;
  double last_relAxesThickness;
  bool last_showCartesianGrid;
  bool last_showCylindricalGrid;
  SbColor4f last_gridColourAndTransp;
  double last_gridSpacing;
  double last_gridExtent;
  bool last_showEtaCone1;
  bool last_showEtaCone2;
  bool last_showEtaCone3;
  double last_etaConeValue1;
  double last_etaConeValue2;
  double last_etaConeValue3;
  double last_etaExtent;
  bool last_showTrackingVolumes;
  bool last_showInnerDetector;
  bool last_showCalorimeters;
  bool last_showMuonSpectrometer;
  bool last_showLines;
  SbVec3f last_lineDirection;
  double last_line_eta; // This is needed to update the display in possibleChange_lineDirection

};
#include <iostream>
//____________________________________________________________________
GuideSysController::GuideSysController(IVP1System * sys)
  : VP1Controller(sys,"GuideSysController"), m_d(new Imp)
{

  m_d->theclass = this;
  m_d->ui.setupUi(this);

  initDialog(m_d->ui_axes, m_d->ui.pushButton_settings_coordinateAxes,m_d->ui.checkBox_coordinateAxes);
  initDialog(m_d->ui_etacones, m_d->ui.pushButton_settings_etaCones,m_d->ui.checkBox_etaCones);
  initDialog(m_d->ui_floorandletters, m_d->ui.pushButton_settings_floorAndLetters,m_d->ui.checkBox_floorAndLetters);
  initDialog(m_d->ui_grid, m_d->ui.pushButton_settings_grid,m_d->ui.checkBox_grid);
  initDialog(m_d->ui_lines, m_d->ui.pushButton_settings_lines,m_d->ui.checkBox_lines);

  //Setup material buttons:
  m_d->ui_floorandletters.colorButton_floor->setColor(QColor::fromRgbF(0.5,0.5,0.5));
  m_d->ui_grid.colorButton_grid->setColor(QColor::fromRgbF(1.0,1.0,1.0));
  m_d->ui_floorandletters.matButton_letters->setMaterial(VP1MaterialButton::createMaterial(0.5,0.5,0.5,0.1));
  m_d->ui_axes.matButton_axes_x->setMaterial(VP1MaterialButton::createMaterial(1,0,0,0.15));
  m_d->ui_axes.matButton_axes_y->setMaterial(VP1MaterialButton::createMaterial(0,1,0,0.15));
  m_d->ui_axes.matButton_axes_z->setMaterial(VP1MaterialButton::createMaterial(0,0,2,0.15));
  m_d->ui_etacones.matButton_etaCone1->setMaterial(VP1MaterialButton::createMaterial(1,1,0,0.2,0.5));
  m_d->ui_etacones.matButton_etaCone2->setMaterial(VP1MaterialButton::createMaterial(1,0.56471,0.20784,0.2,0.5));
  m_d->ui_etacones.matButton_etaCone3->setMaterial(VP1MaterialButton::createMaterial(1,0.15686,0.08235,0.2,0.5));
	
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Setup connections which monitor changes in the controller so that we may emit signals as appropriate:  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

  addUpdateSlot(SLOT(possibleChange_showFloor()));
  connectToLastUpdateSlot(m_d->ui.checkBox_floorAndLetters);
  connectToLastUpdateSlot(m_d->ui_floorandletters.checkBox_floorenabled);

  addUpdateSlot(SLOT(possibleChange_floorColourAndTransp()));
  connectToLastUpdateSlot(m_d->ui_floorandletters.colorButton_floor);
  connectToLastUpdateSlot(m_d->ui_floorandletters.spinBox_floortransp);

  addUpdateSlot(SLOT(possibleChange_floorExtent()));
  connectToLastUpdateSlot(m_d->ui_floorandletters.doubleSpinBox_floorextent);

  addUpdateSlot(SLOT(possibleChange_floorSpacing()));
  connectToLastUpdateSlot(m_d->ui_floorandletters.doubleSpinBox_floorspacing);

  addUpdateSlot(SLOT(possibleChange_floorHeight()));
  connectToLastUpdateSlot(m_d->ui_floorandletters.doubleSpinBox_floorheight);

  addUpdateSlot(SLOT(possibleChange_showLetters()));
  connectToLastUpdateSlot(m_d->ui.checkBox_floorAndLetters);
  connectToLastUpdateSlot(m_d->ui_floorandletters.checkBox_acdesignations);

  addUpdateSlot(SLOT(possibleChange_lettersZPos()));
  connectToLastUpdateSlot(this,SIGNAL(floorExtentChanged(const double&)));
  connectToLastUpdateSlot(this,SIGNAL(floorSpacingChanged(const double&)));

  addUpdateSlot(SLOT(possibleChange_lettersVerticalPos()));
  connectToLastUpdateSlot(this,SIGNAL(floorHeightChanged(const double&)));

  addUpdateSlot(SLOT(possibleChange_showAxes()));
  connectToLastUpdateSlot(m_d->ui.checkBox_coordinateAxes);

  addUpdateSlot(SLOT(possibleChange_axesLength()));
  connectToLastUpdateSlot(m_d->ui_axes.checkBox_axes_shownegativeparts);
  connectToLastUpdateSlot(m_d->ui_axes.doubleSpinBox_axes_lengths_m);

  addUpdateSlot(SLOT(possibleChange_axesPosition()));
  connectToLastUpdateSlot(m_d->ui_axes.doubleSpinBox_axes_xpos_m);
  connectToLastUpdateSlot(m_d->ui_axes.doubleSpinBox_axes_ypos_m);
  connectToLastUpdateSlot(m_d->ui_axes.doubleSpinBox_axes_zpos_m);

  addUpdateSlot(SLOT(possibleChange_relAxesThickness()));
  connectToLastUpdateSlot(m_d->ui_axes.doubleSpinBox_axes_relthickness);

  addUpdateSlot(SLOT(possibleChange_showCartesianGrid()));
  connectToLastUpdateSlot(m_d->ui.checkBox_grid);
  connectToLastUpdateSlot(m_d->ui_grid.radioButton_grid_cartesian);

  addUpdateSlot(SLOT(possibleChange_showCylindricalGrid()));
  connectToLastUpdateSlot(m_d->ui.checkBox_grid);
  connectToLastUpdateSlot(m_d->ui_grid.radioButton_grid_cylindrical);

  addUpdateSlot(SLOT(possibleChange_gridColourAndTransp()));
  connectToLastUpdateSlot(m_d->ui_grid.colorButton_grid);
  connectToLastUpdateSlot(m_d->ui_grid.spinBox_gridtransp);

  addUpdateSlot(SLOT(possibleChange_gridSpacing()));
  connectToLastUpdateSlot(m_d->ui_grid.doubleSpinBox_gridspacing);

  addUpdateSlot(SLOT(possibleChange_gridExtent()));
  connectToLastUpdateSlot(m_d->ui_grid.doubleSpinBox_gridextent);

  addUpdateSlot(SLOT(possibleChange_showEtaCone1()));
  connectToLastUpdateSlot(m_d->ui.checkBox_etaCones);
  connectToLastUpdateSlot(m_d->ui_etacones.checkBox_etacone1);

  addUpdateSlot(SLOT(possibleChange_showEtaCone2()));
  connectToLastUpdateSlot(m_d->ui.checkBox_etaCones);
  connectToLastUpdateSlot(m_d->ui_etacones.checkBox_etacone2);

  addUpdateSlot(SLOT(possibleChange_showEtaCone3()));
  connectToLastUpdateSlot(m_d->ui.checkBox_etaCones);
  connectToLastUpdateSlot(m_d->ui_etacones.checkBox_etacone3);

  addUpdateSlot(SLOT(possibleChange_etaConeValue1()));
  connectToLastUpdateSlot(m_d->ui_etacones.doubleSpinBox_etaval1);

  addUpdateSlot(SLOT(possibleChange_etaConeValue2()));
  connectToLastUpdateSlot(m_d->ui_etacones.doubleSpinBox_etaval2);

  addUpdateSlot(SLOT(possibleChange_etaConeValue3()));
  connectToLastUpdateSlot(m_d->ui_etacones.doubleSpinBox_etaval3);

  addUpdateSlot(SLOT(possibleChange_etaExtent()));
  connectToLastUpdateSlot(m_d->ui_etacones.doubleSpinBox_etaconeextent);
  connectToLastUpdateSlot(m_d->ui_etacones.radioButton_etaconeextentisr);


  addUpdateSlot(SLOT(possibleChange_showLines()));
  connectToLastUpdateSlot(m_d->ui.checkBox_lines);
  
  addUpdateSlot(SLOT(possibleChange_lineDirection()));
  connectToLastUpdateSlot(m_d->ui_lines.doubleSpinBox_phi);
  connectToLastUpdateSlot(m_d->ui_lines.doubleSpinBox_theta);
  connectToLastUpdateSlot(m_d->ui_lines.doubleSpinBox_eta);
  connectToLastUpdateSlot(m_d->ui_lines.doubleSpinBox_length);
  

  initLastVars();
}

//____________________________________________________________________
GuideSysController::~GuideSysController()
{
  delete m_d;
}

//____________________________________________________________________
//Material access methods:
SoMaterial * GuideSysController::lettersMaterial() const { return m_d->ui_floorandletters.matButton_letters->handledMaterials().at(0); }
SoMaterial * GuideSysController::xAxisMaterial() const { return m_d->ui_axes.matButton_axes_x->handledMaterials().at(0); }
SoMaterial * GuideSysController::yAxisMaterial() const { return m_d->ui_axes.matButton_axes_y->handledMaterials().at(0); }
SoMaterial * GuideSysController::zAxisMaterial() const { return m_d->ui_axes.matButton_axes_z->handledMaterials().at(0); }
SoMaterial * GuideSysController::etaCone1Material() const { return m_d->ui_etacones.matButton_etaCone1->handledMaterials().at(0); }
SoMaterial * GuideSysController::etaCone2Material() const { return m_d->ui_etacones.matButton_etaCone2->handledMaterials().at(0); }
SoMaterial * GuideSysController::etaCone3Material() const { return m_d->ui_etacones.matButton_etaCone3->handledMaterials().at(0); }

//____________________________________________________________________
bool GuideSysController::showFloor() const
{
  return m_d->ui.checkBox_floorAndLetters->isChecked() &&
    m_d->ui_floorandletters.checkBox_floorenabled->isChecked();
}

//____________________________________________________________________
SbColor4f GuideSysController::floorColourAndTransp() const
{
  return Imp::color4f(m_d->ui_floorandletters.colorButton_floor->color(),m_d->ui_floorandletters.spinBox_floortransp->value());
}

//____________________________________________________________________
double GuideSysController::floorExtent() const
{
  return m_d->ui_floorandletters.doubleSpinBox_floorextent->value()*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
double GuideSysController::floorSpacing() const
{
  return m_d->ui_floorandletters.doubleSpinBox_floorspacing->value()*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
double GuideSysController::floorHeight() const
{
  return m_d->ui_floorandletters.doubleSpinBox_floorheight->value()*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
bool GuideSysController::showLetters() const
{
  return m_d->ui.checkBox_floorAndLetters->isChecked() &&
    m_d->ui_floorandletters.checkBox_acdesignations->isChecked();
}

//____________________________________________________________________
double GuideSysController::lettersZPos() const
{

  int nmax; double distmax;
  if (!VP1Floor::calcParsFromExtentAndSpacing( (VP1HelperClassBase*)this, floorExtent(), floorSpacing(), VP1Floor::nMax(), nmax, distmax )) {
    nmax = 10;
    distmax = 10*SYSTEM_OF_UNITS::m;
    message("lettersZPos  ERROR: Problems calculating floor nmax/distmax.");
  }
  return distmax*1.1;
}

//____________________________________________________________________
double GuideSysController::lettersVerticalPos() const
{
  return floorHeight()+1.5*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
bool GuideSysController::showAxes() const
{
  return m_d->ui.checkBox_coordinateAxes->isChecked();
}

//____________________________________________________________________
double GuideSysController::axesLength() const
{
  return m_d->ui_axes.doubleSpinBox_axes_lengths_m->value()*SYSTEM_OF_UNITS::m *
    (m_d->ui_axes.checkBox_axes_shownegativeparts->isChecked()?-1.0:1.0);
}

//____________________________________________________________________
SbVec3f GuideSysController::axesPosition() const
{
  return SbVec3f(m_d->ui_axes.doubleSpinBox_axes_xpos_m->value()*SYSTEM_OF_UNITS::m,
         m_d->ui_axes.doubleSpinBox_axes_ypos_m->value()*SYSTEM_OF_UNITS::m,
         m_d->ui_axes.doubleSpinBox_axes_zpos_m->value()*SYSTEM_OF_UNITS::m);
}

//____________________________________________________________________
double GuideSysController::relAxesThickness() const
{
  return m_d->ui_axes.doubleSpinBox_axes_relthickness->value();
}

//____________________________________________________________________
bool GuideSysController::showCartesianGrid() const
{
  return m_d->ui.checkBox_grid->isChecked() &&
    m_d->ui_grid.radioButton_grid_cartesian->isChecked();
}

//____________________________________________________________________
bool GuideSysController::showCylindricalGrid() const
{
  return m_d->ui.checkBox_grid->isChecked() &&
    m_d->ui_grid.radioButton_grid_cylindrical->isChecked();
}

//____________________________________________________________________
SbColor4f GuideSysController::gridColourAndTransp() const
{
  return Imp::color4f(m_d->ui_grid.colorButton_grid->color(),m_d->ui_grid.spinBox_gridtransp->value());
}

//____________________________________________________________________
double GuideSysController::gridSpacing() const
{
  return m_d->ui_grid.doubleSpinBox_gridspacing->value()*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
double GuideSysController::gridExtent() const
{
  return m_d->ui_grid.doubleSpinBox_gridextent->value()*SYSTEM_OF_UNITS::m;
}

//____________________________________________________________________
bool GuideSysController::showEtaCone1() const
{
  return m_d->ui.checkBox_etaCones->isChecked() &&
    m_d->ui_etacones.checkBox_etacone1->isChecked();
}

//____________________________________________________________________
bool GuideSysController::showEtaCone2() const
{
  return m_d->ui.checkBox_etaCones->isChecked() &&
    m_d->ui_etacones.checkBox_etacone2->isChecked();
}

//____________________________________________________________________
bool GuideSysController::showEtaCone3() const
{
  return m_d->ui.checkBox_etaCones->isChecked() &&
    m_d->ui_etacones.checkBox_etacone3->isChecked();
}

//____________________________________________________________________
double GuideSysController::etaConeValue1() const
{
  return std::max(1.0e-5,m_d->ui_etacones.doubleSpinBox_etaval1->value());
}

//____________________________________________________________________
double GuideSysController::etaConeValue2() const
{
  return std::max(1.0e-5,m_d->ui_etacones.doubleSpinBox_etaval2->value());
}

//____________________________________________________________________
double GuideSysController::etaConeValue3() const
{
  return std::max(1.0e-5,m_d->ui_etacones.doubleSpinBox_etaval3->value());
}

//____________________________________________________________________
double GuideSysController::etaExtent() const
{
  return m_d->ui_etacones.doubleSpinBox_etaconeextent->value() * SYSTEM_OF_UNITS::m
    * (m_d->ui_etacones.radioButton_etaconeextentisr->isChecked() ? 1.0 : -1.0);
}

//____________________________________________________________________
bool GuideSysController::showLines() const
{
  return m_d->ui.checkBox_lines->isChecked();
}


//____________________________________________________________________
SbVec3f GuideSysController::lineDirection() const
{
  double r = lineLength();
  double phi = m_d->ui_lines.doubleSpinBox_phi->value();
  double theta = m_d->ui_lines.doubleSpinBox_theta->value();
  SbVec3f direction(r*sin(theta)*cos(phi),r*sin(theta)*sin(phi), r*cos(theta));
  return direction;
}

//____________________________________________________________________
double GuideSysController::lineLength() const
{
  return m_d->ui_lines.doubleSpinBox_length->value() * SYSTEM_OF_UNITS::m;
}

//_____________________________________________________________________________________

//____________________________________________________________________
int GuideSysController::currentSettingsVersion() const
{
  return 3;
}

//____________________________________________________________________
void GuideSysController::actualSaveSettings(VP1Serialise&s) const
{

  //version 0 output an integer here.

  //Materials
  s.save(m_d->ui_floorandletters.matButton_letters);
  s.save(m_d->ui_axes.matButton_axes_x);
  s.save(m_d->ui_axes.matButton_axes_y);
  s.save(m_d->ui_axes.matButton_axes_z);
  s.save(m_d->ui_etacones.matButton_etaCone1);
  s.save(m_d->ui_etacones.matButton_etaCone2);
  s.save(m_d->ui_etacones.matButton_etaCone3);

  //Everything apart from projection surfaces:
  s.save(m_d->ui_grid.colorButton_grid);
  s.save(m_d->ui_grid.spinBox_gridtransp);
  s.save(m_d->ui_floorandletters.colorButton_floor);
  s.save(m_d->ui_floorandletters.spinBox_floortransp);
  s.save(m_d->ui_floorandletters.doubleSpinBox_floorheight);
  s.save(m_d->ui_floorandletters.checkBox_floorenabled);
  s.save(m_d->ui_floorandletters.doubleSpinBox_floorspacing);
  s.save(m_d->ui_floorandletters.doubleSpinBox_floorextent);
  s.save(m_d->ui_floorandletters.checkBox_acdesignations);
  //Version 0 output bool here
  s.save(m_d->ui_axes.checkBox_axes_shownegativeparts);
  s.save(m_d->ui_axes.doubleSpinBox_axes_lengths_m);
  s.save(m_d->ui_axes.doubleSpinBox_axes_relthickness);
  s.save(m_d->ui_axes.doubleSpinBox_axes_xpos_m);
  s.save(m_d->ui_axes.doubleSpinBox_axes_ypos_m);
  s.save(m_d->ui_axes.doubleSpinBox_axes_zpos_m);
  s.save(0/*backwards compat*/,m_d->ui_grid.radioButton_grid_cartesian,m_d->ui_grid.radioButton_grid_cylindrical);
  s.save(m_d->ui_grid.doubleSpinBox_gridspacing);
  s.save(m_d->ui_grid.doubleSpinBox_gridextent);
  s.save(m_d->ui_etacones.checkBox_etacone1);
  s.save(m_d->ui_etacones.doubleSpinBox_etaval1);
  s.save(m_d->ui_etacones.checkBox_etacone2);
  s.save(m_d->ui_etacones.doubleSpinBox_etaval2);
  s.save(m_d->ui_etacones.checkBox_etacone3);
  s.save(m_d->ui_etacones.doubleSpinBox_etaval3);
  s.save(m_d->ui_etacones.doubleSpinBox_etaconeextent);
  s.save(m_d->ui_etacones.radioButton_etaconeextentisr,m_d->ui_etacones.radioButton_etaconeextentisz);

  //Projection surfaces:

  s.save(m_d->ui.checkBox_coordinateAxes);//version 1+
  s.save(m_d->ui.checkBox_etaCones);
  s.save(m_d->ui.checkBox_floorAndLetters);//version 1+
  s.save(m_d->ui.checkBox_grid);//version 1+
  
  
  // Line from origin
  s.save(m_d->ui.checkBox_lines);
  s.save(m_d->ui_lines.doubleSpinBox_phi); 
  s.save(m_d->ui_lines.doubleSpinBox_phi);
  s.save(m_d->ui_lines.doubleSpinBox_eta);
  s.save(m_d->ui_lines.doubleSpinBox_length);
}

//____________________________________________________________________
void GuideSysController::actualRestoreSettings(VP1Deserialise& s)
{
  if (s.version()<0||s.version()>2) {
    message("Warning: State data in .vp1 file has unsupported version ("+str(s.version())+")");
    return;
  }

  if (s.version()==0)
    s.ignoreInt();

  //Materials
  s.restore(m_d->ui_floorandletters.matButton_letters);
  s.restore(m_d->ui_axes.matButton_axes_x);
  s.restore(m_d->ui_axes.matButton_axes_y);
  s.restore(m_d->ui_axes.matButton_axes_z);
  s.restore(m_d->ui_etacones.matButton_etaCone1);
  s.restore(m_d->ui_etacones.matButton_etaCone2);
  s.restore(m_d->ui_etacones.matButton_etaCone3);
  
  //Everything apart from projection surfaces:
  s.restore(m_d->ui_grid.colorButton_grid);
  s.restore(m_d->ui_grid.spinBox_gridtransp);
  s.restore(m_d->ui_floorandletters.colorButton_floor);
  s.restore(m_d->ui_floorandletters.spinBox_floortransp);
  s.restore(m_d->ui_floorandletters.doubleSpinBox_floorheight);
  s.restore(m_d->ui_floorandletters.checkBox_floorenabled);
  s.restore(m_d->ui_floorandletters.doubleSpinBox_floorspacing);
  s.restore(m_d->ui_floorandletters.doubleSpinBox_floorextent);
  s.restore(m_d->ui_floorandletters.checkBox_acdesignations);
  if (s.version()==0)
    s.ignoreBool();
  s.restore(m_d->ui_axes.checkBox_axes_shownegativeparts);
  s.restore(m_d->ui_axes.doubleSpinBox_axes_lengths_m);
  s.restore(m_d->ui_axes.doubleSpinBox_axes_relthickness);
  s.restore(m_d->ui_axes.doubleSpinBox_axes_xpos_m);
  s.restore(m_d->ui_axes.doubleSpinBox_axes_ypos_m);
  s.restore(m_d->ui_axes.doubleSpinBox_axes_zpos_m);
  s.restore(0/*backwards compat*/,m_d->ui_grid.radioButton_grid_cartesian,m_d->ui_grid.radioButton_grid_cylindrical);
  s.restore(m_d->ui_grid.doubleSpinBox_gridspacing);
  s.restore(m_d->ui_grid.doubleSpinBox_gridextent);
  s.restore(m_d->ui_etacones.checkBox_etacone1);
  s.restore(m_d->ui_etacones.doubleSpinBox_etaval1);
  s.restore(m_d->ui_etacones.checkBox_etacone2);
  s.restore(m_d->ui_etacones.doubleSpinBox_etaval2);
  s.restore(m_d->ui_etacones.checkBox_etacone3);
  s.restore(m_d->ui_etacones.doubleSpinBox_etaval3);
  s.restore(m_d->ui_etacones.doubleSpinBox_etaconeextent);
  s.restore(m_d->ui_etacones.radioButton_etaconeextentisr,m_d->ui_etacones.radioButton_etaconeextentisz);

  s.restore(m_d->ui.checkBox_lines);
  s.restore(m_d->ui_lines.doubleSpinBox_phi); 
  s.restore(m_d->ui_lines.doubleSpinBox_phi);
  s.restore(m_d->ui_lines.doubleSpinBox_eta);
  s.restore(m_d->ui_lines.doubleSpinBox_length);
}

void GuideSysController::possibleChange_lineDirection() {	
  // Bit of a hack possibly to do this here, but I want to be able to update the direction by changing either theta or eta
  double eta = m_d->ui_lines.doubleSpinBox_eta->value();
  double theta = m_d->ui_lines.doubleSpinBox_theta->value();
  
  if (m_d->last_line_eta != eta){
    // eta has changed, so update theta in the UI
    theta = 2*std::atan(std::exp(-1.0 * eta));
    m_d->ui_lines.doubleSpinBox_theta->setValue(theta);
  } else if ( theta!= std::acos(m_d->last_lineDirection[2] / lineLength() ) ){
    eta = -1.0 * std::log(std::tan(theta/2.0));
    m_d->ui_lines.doubleSpinBox_eta->setValue(eta);  
  }
  m_d->last_line_eta = m_d->ui_lines.doubleSpinBox_eta->value();
  if (changed( m_d->last_lineDirection , lineDirection() ) ) { 
    if (verbose()&&!initVarsMode()) messageVerbose("Emitting "+QString()+"( lineDirectionChanged"+toString(m_d->last_lineDirection)+" )"); 
    emit lineDirectionChanged(m_d->last_lineDirection); 
  }  
}


///////////////////////////////////////////////////////////////////////////
// Test for possible changes in values and emit signals as appropriate:
// (possibleChange_XXX() slots code provided by macros)
#define VP1CONTROLLERCLASSNAME GuideSysController
#include "VP1Base/VP1ControllerMacros.h"
POSSIBLECHANGE_IMP(showFloor)
POSSIBLECHANGE_IMP(floorColourAndTransp)
POSSIBLECHANGE_IMP(floorExtent)
POSSIBLECHANGE_IMP(floorSpacing)
POSSIBLECHANGE_IMP(floorHeight)
POSSIBLECHANGE_IMP(showLetters)
POSSIBLECHANGE_IMP(lettersZPos)
POSSIBLECHANGE_IMP(lettersVerticalPos)
POSSIBLECHANGE_IMP(showAxes)
POSSIBLECHANGE_IMP(axesLength)
POSSIBLECHANGE_IMP(axesPosition)
POSSIBLECHANGE_IMP(relAxesThickness)
POSSIBLECHANGE_IMP(showCartesianGrid)
POSSIBLECHANGE_IMP(showCylindricalGrid)
POSSIBLECHANGE_IMP(gridColourAndTransp)
POSSIBLECHANGE_IMP(gridSpacing)
POSSIBLECHANGE_IMP(gridExtent)
POSSIBLECHANGE_IMP(showEtaCone1)
POSSIBLECHANGE_IMP(showEtaCone2)
POSSIBLECHANGE_IMP(showEtaCone3)
POSSIBLECHANGE_IMP(etaConeValue1)
POSSIBLECHANGE_IMP(etaConeValue2)
POSSIBLECHANGE_IMP(etaConeValue3)
POSSIBLECHANGE_IMP(etaExtent)
POSSIBLECHANGE_IMP(showLines)  
//POSSIBLECHANGE_IMP(lineDirection) Implemented this manually so we can update eta/theta

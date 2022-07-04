/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class HitDisplaySysController           //
//                                                            //
//  Author:  Joseph F. Boudreau (boudreau@pitt.edu)           //
//  Initial version: March 2020                               //
//                                                            //
////////////////////////////////////////////////////////////////

#define VP1IMPVARNAME m_d

#include "GXHitDisplaySystems/HitDisplaySysController.h"
#include "ui_hitdisplaycontrollerform.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"
#include <cmath>

//____________________________________________________________________
class HitDisplaySysController::Imp {
public:
  HitDisplaySysController * theclass;
  Ui::GXHitDisplayControllerForm ui;

  bool last_showHitDisplays1;
  int  last_setPointSize;  
};
#include <iostream>
//____________________________________________________________________
HitDisplaySysController::HitDisplaySysController(IVP1System * sys)
  : VP1Controller(sys,"HitDisplaySysController"), m_d(new Imp)
{

  m_d->theclass = this;
  m_d->ui.setupUi(this);


	
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Setup connections which monitor changes in the controller so that we may emit signals as appropriate:  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

  addUpdateSlot(SLOT(possibleChange_showHitDisplays1()));
  connectToLastUpdateSlot(m_d->ui.hitDisplayCheckBox);

  addUpdateSlot(SLOT(possibleChange_setPointSize()));
  connectToLastUpdateSlot(m_d->ui.pointSizeSpinBox);
  
  connect(m_d->ui.pushButton_selectInput, SIGNAL(clicked()),
	  this, SIGNAL(inputFileChanged()));
  
  initLastVars();
}

//____________________________________________________________________
HitDisplaySysController::~HitDisplaySysController()
{
  delete m_d;
}


//____________________________________________________________________
bool HitDisplaySysController::showHitDisplays1() const
{
  return m_d->ui.hitDisplayCheckBox->isChecked();
}

int HitDisplaySysController::setPointSize() const {
  return m_d->ui.pointSizeSpinBox->value();
}

//____________________________________________________________________
int HitDisplaySysController::currentSettingsVersion() const
{
  return 2;
}

//____________________________________________________________________
void HitDisplaySysController::actualSaveSettings(VP1Serialise&s) const
{

  s.save(m_d->ui.hitDisplayCheckBox);//version 1+
  s.save(m_d->ui.pointSizeSpinBox);      //version 2+
}

//____________________________________________________________________
void HitDisplaySysController::actualRestoreSettings(VP1Deserialise& s)
{
  
  s.restore(m_d->ui.hitDisplayCheckBox);
  s.restore(m_d->ui.pointSizeSpinBox);

}


///////////////////////////////////////////////////////////////////////////
// Test for possible changes in values and emit signals as appropriate:
// (possibleChange_XXX() slots code provided by macros)
#define VP1CONTROLLERCLASSNAME HitDisplaySysController
#include "VP1Base/VP1ControllerMacros.h"
POSSIBLECHANGE_IMP(showHitDisplays1)
POSSIBLECHANGE_IMP(setPointSize)


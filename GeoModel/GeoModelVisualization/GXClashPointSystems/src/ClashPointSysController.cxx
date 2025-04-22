/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class ClashPointSysController           //
//                                                            //
//  Author:  Joseph F. Boudreau (boudreau@pitt.edu)           //
//  Initial version: March 2020                               //
//                                                            //
////////////////////////////////////////////////////////////////

#define VP1IMPVARNAME m_d

#include "GXClashPointSystems/ClashPointSysController.h"
#include "ui_clashpointcontrollerform.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"
#include <cmath>

//____________________________________________________________________
class ClashPointSysController::Imp {
public:
  ClashPointSysController * theclass;
  Ui::GXClashPointControllerForm ui;

  bool last_showClashPoints1;
  bool last_showClashPoints2;
  bool last_showClashPoints3;
  bool last_showClashPoints4;
  int  last_setPointSize;  
};
#include <iostream>
//____________________________________________________________________
ClashPointSysController::ClashPointSysController(IVP1System * sys)
  : VP1Controller(sys,"ClashPointSysController"), m_d(new Imp)
{

  m_d->theclass = this;
  m_d->ui.setupUi(this);


	
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Setup connections which monitor changes in the controller so that we may emit signals as appropriate:  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

  addUpdateSlot(SLOT(possibleChange_showClashPoints1()));
  connectToLastUpdateSlot(m_d->ui.checkBox_clashPoints_1);

  addUpdateSlot(SLOT(possibleChange_showClashPoints2()));
  connectToLastUpdateSlot(m_d->ui.checkBox_clashPoints_2);

  addUpdateSlot(SLOT(possibleChange_showClashPoints3()));
  connectToLastUpdateSlot(m_d->ui.checkBox_clashPoints_3);
    
  addUpdateSlot(SLOT(possibleChange_showClashPoints4()));
  connectToLastUpdateSlot(m_d->ui.checkBox_clashPoints_4);

  addUpdateSlot(SLOT(possibleChange_setPointSize()));
  connectToLastUpdateSlot(m_d->ui.pointSizeSpinBox);
  
  connect(m_d->ui.pushButton_selectInput, SIGNAL(clicked()),
	  this, SIGNAL(inputFileChanged()));
  
  initLastVars();
}

//____________________________________________________________________
ClashPointSysController::~ClashPointSysController()
{
  delete m_d;
}


//____________________________________________________________________
bool ClashPointSysController::showClashPoints1() const
{
  return m_d->ui.checkBox_clashPoints_1->isChecked();
}

//____________________________________________________________________
bool ClashPointSysController::showClashPoints2() const
{
  return m_d->ui.checkBox_clashPoints_2->isChecked();
}

//____________________________________________________________________
bool ClashPointSysController::showClashPoints3() const
{
  return m_d->ui.checkBox_clashPoints_3->isChecked();
}

//____________________________________________________________________
bool ClashPointSysController::showClashPoints4() const
{
  return m_d->ui.checkBox_clashPoints_4->isChecked();
}

int ClashPointSysController::setPointSize() const {
  return m_d->ui.pointSizeSpinBox->value();
}

//____________________________________________________________________
int ClashPointSysController::currentSettingsVersion() const
{
  return 2;
}

//____________________________________________________________________
void ClashPointSysController::actualSaveSettings(VP1Serialise&s) const
{

  s.save(m_d->ui.checkBox_clashPoints_1);//version 1+
  s.save(m_d->ui.checkBox_clashPoints_2);//version 1+
  s.save(m_d->ui.checkBox_clashPoints_3);//version 1+
  s.save(m_d->ui.checkBox_clashPoints_4);//version 1+
  s.save(m_d->ui.pointSizeSpinBox);      //version 2+
}

//____________________________________________________________________
void ClashPointSysController::actualRestoreSettings(VP1Deserialise& s)
{
  
  s.restore(m_d->ui.checkBox_clashPoints_1);
  s.restore(m_d->ui.checkBox_clashPoints_2);
  s.restore(m_d->ui.checkBox_clashPoints_3);
  s.restore(m_d->ui.checkBox_clashPoints_4);
  s.restore(m_d->ui.pointSizeSpinBox);

}


///////////////////////////////////////////////////////////////////////////
// Test for possible changes in values and emit signals as appropriate:
// (possibleChange_XXX() slots code provided by macros)
#define VP1CONTROLLERCLASSNAME ClashPointSysController
#include "VP1Base/VP1ControllerMacros.h"
POSSIBLECHANGE_IMP(showClashPoints1)
POSSIBLECHANGE_IMP(showClashPoints2)
POSSIBLECHANGE_IMP(showClashPoints3)
POSSIBLECHANGE_IMP(showClashPoints4)
POSSIBLECHANGE_IMP(setPointSize)


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

#include "GXTrackDisplaySystems/TrackDisplaySysController.h"
#include "ui_trackdisplaycontrollerform.h"
#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1Deserialise.h"
#include <cmath>

//____________________________________________________________________
class TrackDisplaySysController::Imp {
public:
  TrackDisplaySysController * theclass;
  Ui::GXTrackDisplayControllerForm ui;

  bool last_showTrackDisplays1;
  int  last_setLineWidth;  
};
#include <iostream>
//____________________________________________________________________
TrackDisplaySysController::TrackDisplaySysController(IVP1System * sys)
  : VP1Controller(sys,"TrackDisplaySysController"), m_d(new Imp)
{

  m_d->theclass = this;
  m_d->ui.setupUi(this);


	
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Setup connections which monitor changes in the controller so that we may emit signals as appropriate:  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

  addUpdateSlot(SLOT(possibleChange_showTrackDisplays1()));
  connectToLastUpdateSlot(m_d->ui.trackDisplayCheckBox);

  addUpdateSlot(SLOT(possibleChange_setLineWidth()));
  connectToLastUpdateSlot(m_d->ui.lineWidthSpinBox);
  
  connect(m_d->ui.pushButton_selectInput, SIGNAL(clicked()),
	  this, SIGNAL(inputFileChanged()));

  connect (m_d->ui.nextEventButton, SIGNAL(clicked()),
	   this, SIGNAL(nextEvent()));
  
  initLastVars();
}

//____________________________________________________________________
TrackDisplaySysController::~TrackDisplaySysController()
{
  delete m_d;
}


//____________________________________________________________________
bool TrackDisplaySysController::showTrackDisplays1() const
{
  return m_d->ui.trackDisplayCheckBox->isChecked();
}

int TrackDisplaySysController::setLineWidth() const {
  return m_d->ui.lineWidthSpinBox->value();
}

//____________________________________________________________________
int TrackDisplaySysController::currentSettingsVersion() const
{
  return 2;
}

//____________________________________________________________________
void TrackDisplaySysController::actualSaveSettings(VP1Serialise&s) const
{

  s.save(m_d->ui.trackDisplayCheckBox);//version 1+
  s.save(m_d->ui.lineWidthSpinBox);      //version 2+
}

//____________________________________________________________________
void TrackDisplaySysController::actualRestoreSettings(VP1Deserialise& s)
{
  
  s.restore(m_d->ui.trackDisplayCheckBox);
  s.restore(m_d->ui.lineWidthSpinBox);

}


///////////////////////////////////////////////////////////////////////////
// Test for possible changes in values and emit signals as appropriate:
// (possibleChange_XXX() slots code provided by macros)
#define VP1CONTROLLERCLASSNAME TrackDisplaySysController
#include "VP1Base/VP1ControllerMacros.h" 
POSSIBLECHANGE_IMP(showTrackDisplays1)
POSSIBLECHANGE_IMP(setLineWidth)



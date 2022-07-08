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


#ifndef CLASHPOINTSYSCONTROLLER_H
#define CLASHPOINTSYSCONTROLLER_H

#include "VP1Base/VP1Controller.h"
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/SbColor4f.h>

class HitDisplaySysController : public VP1Controller {

  Q_OBJECT

public:

  HitDisplaySysController(IVP1System * sys);
  virtual ~HitDisplaySysController();

  void actualRestoreSettings(VP1Deserialise&);
  int currentSettingsVersion() const;
  void actualSaveSettings(VP1Serialise&) const;

  //HitDisplays:
  bool showHitDisplays1() const;
  int  setPointSize() const;
  
signals:
  
  void showHitDisplays1Changed(bool);
  void setPointSizeChanged(int);
  void inputFileChanged();
  void nextEvent();
  
private:

  class Imp;
  Imp * m_d;

private slots:
  void possibleChange_showHitDisplays1();
  void possibleChange_setPointSize();
  
};


#endif

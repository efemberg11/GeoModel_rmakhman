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


#ifndef CLASHPOINTSYSCONTROLLER_H
#define CLASHPOINTSYSCONTROLLER_H

#include "VP1Base/VP1Controller.h"
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/SbColor4f.h>

class ClashPointSysController : public VP1Controller {

  Q_OBJECT

public:

  ClashPointSysController(IVP1System * sys);
  virtual ~ClashPointSysController();

  void actualRestoreSettings(VP1Deserialise&);
  int currentSettingsVersion() const;
  void actualSaveSettings(VP1Serialise&) const;

  //ClashPoints:
  bool showClashPoints1() const;
  bool showClashPoints2() const;
  bool showClashPoints3() const;
  bool showClashPoints4() const;

signals:
  
  void showClashPoints1Changed(bool);
  void showClashPoints2Changed(bool);
  void showClashPoints3Changed(bool);
  void showClashPoints4Changed(bool);
  void inputFileChanged();

private:

  class Imp;
  Imp * m_d;

private slots:
  void possibleChange_showClashPoints1();
  void possibleChange_showClashPoints2();
  void possibleChange_showClashPoints3();
  void possibleChange_showClashPoints4();

  
};


#endif

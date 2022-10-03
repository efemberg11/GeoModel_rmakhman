/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/



////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class TrackDisplaySysController         //
//                                                            //
//  Based on class HitDisplaySysController                    //
//                                                            //
////////////////////////////////////////////////////////////////


#ifndef TRACKDISPLAYSYSCONTROLLER_H
#define TRACKDISPLAYSYSCONTROLLER_H

#include "VP1Base/VP1Controller.h"
#include <Inventor/C/errors/debugerror.h>

class TrackDisplaySysController : public VP1Controller {

  Q_OBJECT

public:

  TrackDisplaySysController(IVP1System * sys);
  virtual ~TrackDisplaySysController();

  void actualRestoreSettings(VP1Deserialise&);
  int currentSettingsVersion() const;
  void actualSaveSettings(VP1Serialise&) const;

  //TrackDisplays:
  bool showTrackDisplays1() const;
  int  setLineWidth() const;
  
signals:
  
  void showTrackDisplays1Changed(bool);
  void setLineWidthChanged(int);
  void inputFileChanged();
  void nextEvent();
  
private:

  class Imp;
  Imp * m_d;

private slots:
  void possibleChange_showTrackDisplays1();
  void possibleChange_setLineWidth();
  
};


#endif

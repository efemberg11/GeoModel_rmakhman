/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GXTRACKDISPLAYSYSTEM_H
#define GXTRACKDISPLAYSYSTEM_H

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Header file for class GXTrackDisplaySystem                         //
//                                                                     //
//  Based on GXHitDisplaySystem                                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "VP1Base/IVP13DSystemSimple.h"

class GXTrackDisplaySystem : public IVP13DSystemSimple {

  Q_OBJECT

public:

  ///////////////////
  //  Constructor  //
  ///////////////////

  GXTrackDisplaySystem();

  //////////////////////////////////////////
  //  Reimplementations from base class:  //
  //////////////////////////////////////////

  virtual ~GXTrackDisplaySystem();
  void buildPermanentSceneGraph(StoreGateSvc* detstore, SoSeparator *root);
  void buildEventSceneGraph(StoreGateSvc*, SoSeparator *);
  void systemuncreate();
  QWidget * buildController();
  QByteArray saveState();
  void restoreFromState(QByteArray ba);

public slots:

  void selectInputFile();
  void showTrackDisplay1(bool);
  void setLineWidth(int);
  void nextEvent();
  
signals:
  void updateTransparencyType(unsigned type);
  
public:
  class Imp;
  Imp * m_d;
};

#endif

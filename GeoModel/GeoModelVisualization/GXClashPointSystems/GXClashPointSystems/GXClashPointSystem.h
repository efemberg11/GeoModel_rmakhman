/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GXCLASHPOINTSYSTEM_H
#define GXCLASHPOINTSYSTEM_H

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Header file for class GXClashPointSystem                           //
//                                                                     //
//  Author: Joseph F. Boudreau <boudreau@pitt.edu>                     //
//                                                                     //
//  Initial version: March 2020                                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "VP1Base/IVP13DSystemSimple.h"

class GXClashPointSystem : public IVP13DSystemSimple {

  Q_OBJECT

public:

  ///////////////////
  //  Constructor  //
  ///////////////////

  GXClashPointSystem();

  //////////////////////////////////////////
  //  Reimplementations from base class:  //
  //////////////////////////////////////////

  virtual ~GXClashPointSystem();
  void buildPermanentSceneGraph(StoreGateSvc* detstore, SoSeparator *root);
  void buildEventSceneGraph(StoreGateSvc*, SoSeparator *);
  void systemuncreate();
  QWidget * buildController();
  QByteArray saveState();
  void restoreFromState(QByteArray ba);

public slots:

  void selectInputFile();
  void showClashPoint1(bool);
  void showClashPoint2(bool);
  void showClashPoint3(bool);
  void showClashPoint4(bool);
  void setPointSize(int);

signals:
  void updateTransparencyType(unsigned type);
  
private:
  class Imp;
  Imp * m_d;
};

#endif

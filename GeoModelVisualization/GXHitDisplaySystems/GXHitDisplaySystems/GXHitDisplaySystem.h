/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GXHITDISPLAYSYSTEM_H
#define GXHITDISPLAYSYSTEM_H

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Header file for class GXHitDisplaySystem                           //
//                                                                     //
//  Author: Joseph F. Boudreau <boudreau@pitt.edu>                     //
//                                                                     //
//  Initial version: March 2020                                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "VP1Base/IVP13DSystemSimple.h"

class GXHitDisplaySystem : public IVP13DSystemSimple {

  Q_OBJECT

public:

  ///////////////////
  //  Constructor  //
  ///////////////////

  GXHitDisplaySystem();

  //////////////////////////////////////////
  //  Reimplementations from base class:  //
  //////////////////////////////////////////

  virtual ~GXHitDisplaySystem();
  void buildPermanentSceneGraph(StoreGateSvc* detstore, SoSeparator *root);
  void buildEventSceneGraph(StoreGateSvc*, SoSeparator *);
  void systemuncreate();
  QWidget * buildController();
  QByteArray saveState();
  void restoreFromState(QByteArray ba);

public slots:

  void selectInputFile();
  void showHitDisplay1(bool);
  void setPointSize(int);
  
signals:
  void updateTransparencyType(unsigned type);
  
private:
  class Imp;
  Imp * m_d;
};

#endif

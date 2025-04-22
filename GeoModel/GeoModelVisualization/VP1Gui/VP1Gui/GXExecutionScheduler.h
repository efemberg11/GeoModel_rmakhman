/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////
//                                                         //
//  Header file for class GXExecutionScheduler            //
//                                                         //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>  //
//                                                         //
//  Initial version: April 2007                            //
//                                                         //
/////////////////////////////////////////////////////////////

#ifndef VP1EXECUTIONSCHEDULER_H
#define VP1EXECUTIONSCHEDULER_H

// include C++
//#include <stddef.h> // this to fix the 'ptrdiff_t' does not name a type error with Qt (http://qt-project.org/forums/viewthread/16992)

// include VP1
#include "VP1Gui/VP1QtApplication.h"
#include "VP1Base/IVP1ChannelWidget.h"

// include Qt
#include <QObject>
#include <QStringList>
#include <QTime>
#include <QDir> 

class IVP1System;


class GXExecutionScheduler : public QObject {

  Q_OBJECT

public:





  //init/cleanup:
  static GXExecutionScheduler* init();
  static void cleanup(GXExecutionScheduler*);

  //Call when new event data are available (returns false when the user closes the program)
  bool interact();//

  GXExecutionScheduler(QObject * parent);

  virtual ~GXExecutionScheduler();

  void bringFromConstructedToReady(IVP1ChannelWidget*);
  void uncreateAndDelete(IVP1ChannelWidget*);

  bool isRefreshing() const;

  bool hasAllActiveSystemsRefreshed(IVP1ChannelWidget*) const;


signals:
  void refreshingStatusChanged(bool);



private:
  class Imp;
  Imp * m_d;
  void refreshSystem(IVP1System*);
  void eraseSystem(IVP1System*);
  void actualUncreateAndDelete(IVP1ChannelWidget*);

private slots:
  void processSystemForRefresh();
  void channelCreated(IVP1ChannelWidget*);
  void channelUncreated(IVP1ChannelWidget*);
  void startRefreshQueueIfAppropriate();
  void systemNeedErase();

};

#endif

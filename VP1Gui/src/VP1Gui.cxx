/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////
//                                                         //
//  Implementation of class VP1Gui                         //
//                                                         //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>  //
//                                                         //
//  Initial version: April 2007                            //
//                                                         //
/////////////////////////////////////////////////////////////

#include "VP1Gui/VP1Gui.h"
#include "VP1Gui/VP1ExecutionScheduler.h"
#include "VP1Base/VP1Msg.h"

#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QSslSocket>

#include <cassert>
#include <iostream>

//____________________________________________________________________
class VP1Gui::Imp {
public:
  Imp() : the_scheduler(nullptr) {}
  
  VP1ExecutionScheduler * the_scheduler;

};


//____________________________________________________________________
VP1Gui::VP1Gui()
 : m_d(new Imp)
{
}

//____________________________________________________________________
VP1Gui::~VP1Gui()
{
  delete m_d; m_d=0;
}


//____________________________________________________________________
void VP1Gui::init()
{
  VP1Msg::messageDebug("Start of VP1Gui::init()...");

  assert(!m_d->the_scheduler&&"Dont init twice!!");
  VP1Msg::message("");
  VP1Msg::message("===================================================");
  VP1Msg::message("               Launching the VP1 GUI");
  VP1Msg::message("===================================================");
  VP1Msg::message("");
  m_d->the_scheduler = VP1ExecutionScheduler::init();

  VP1Msg::messageDebug("end of VP1Gui::init().");
}

//____________________________________________________________________
void VP1Gui::cleanup()
{
  assert(m_d->the_scheduler&&"Cant cleanup before init");
  VP1ExecutionScheduler::cleanup(m_d->the_scheduler);
}

//____________________________________________________________________
bool VP1Gui::executeNewEvent( const int& run, const uint64_t& event, const unsigned& triggerType, const unsigned& time )
{
  return m_d->the_scheduler->executeNewEvent(run,event,triggerType,time);
}


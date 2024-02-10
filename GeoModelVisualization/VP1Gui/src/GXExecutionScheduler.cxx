/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////
//                                                         //
//  Implementation of class GXExecutionScheduler          //
//                                                         //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>  //
//                                                         //
//  Initial version: April 2007                            //
//                                                         //
//  History:                                               //
//  R.M.Bianchi <rbianchi@cern.ch> - Feb 2013              //
//                                                         //
/////////////////////////////////////////////////////////////


#include "VP1Gui/GXExecutionScheduler.h"
#include "VP1Gui/VP1Prioritiser.h"
#include "VP1Gui/GXMainWindow.h"
#include "VP1Gui/VP1ChannelManager.h"
#include "VP1Gui/VP1TabManager.h"

#include "VP1Base/VP1QtUtils.h"
#include "VP1Base/IVP1System.h"
#include "VP1Base/VP1QtUtils.h"
#include "VP1Base/IVP1ChannelWidget.h"
#include "VP1Base/VP1Msg.h"
#include "VP1Base/VP1HelperClassBase.h"
#include "VP1Base/VP1AthenaPtrs.h"
#include "VP1Base/VP1Settings.h"


#include <QApplication>
#include <QProgressBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCursor>
#include <QTimer>
#include <QSet>
#include <QStringList>
#include <QMessageBox>
#include <QCommandLineParser>
#include <QDebug>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/Qt/SoQt.h>

#include <set>
#include <cassert>
#include <iostream>     //For "widgetcount" output
#include <sstream>      // std::ostringstream
#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>



std::vector<std::string> qstringlistToVecString(QStringList list)
{
  std::vector<std::string> vec;
  foreach(QString str, list) {
    vec.push_back(str.toStdString());
  }
  return vec;
}



//___________________________________________________________________
class GXExecutionScheduler::Imp {
public:
  class GlobalEventFilter;//Used to animate mouse clicks (for screencasts);
  GlobalEventFilter * globalEventFilter;
  
  GXExecutionScheduler * scheduler;
  VP1Prioritiser * prioritiser;
  GXMainWindow* mainwindow;
  
  
  QTimer * refreshtimer;
  QTimer * splashScreenTimer;
  IVP1System* currentsystemrefreshing;
  bool allSystemsRefreshed;
  
  
  //When receiving erase requests for a system that is currently refreshing, we use this:
  bool eraseJustAfterRefresh;
  IVP1ChannelWidget* postponedUncreateAndDeleteCW;
  
  bool allVisibleRefreshed() const;
  bool allSoonVisibleRefreshed() const;
  
  static void warnIfWidgetsAlive();
  
  
};

// NOTE: Before arriving here, events are intercepted by the SoCooperativeSelection class,
//       grabbed directly from the SoQt windowing system
//___________________________________________________________________
class GXExecutionScheduler::Imp::GlobalEventFilter : public QObject {
public:
  GlobalEventFilter():m_lastPopup(QTime::currentTime()),
		      m_lastPopupWasQMenu(false){}

  bool eventFilter ( QObject * watched, QEvent * event ) {

    if (event->type()==QEvent::MouseButtonPress
	||event->type()==QEvent::MouseButtonDblClick
	||event->type()==QEvent::Wheel
	||event->type()==QEvent::KeyPress)
      {
	QTime t = QTime::currentTime();
	int timediff(abs(t.msecsTo(m_lastPopup)));
	int timecut(m_lastPopupWasQMenu?300:100);
	if (timediff>timecut) {
	  m_lastPopup = t;
	  QString txt;

	  // Grab events
	  QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
	  QKeyEvent*   keyEvent   = dynamic_cast<QKeyEvent*>(event);
	  QWheelEvent*   wheelEvent = dynamic_cast<QWheelEvent*>(event);

	  // Intercept mouse events
	  if (mouseEvent) {
	    qDebug() << "VP1Gui::GlobalEventFilter: mouse event intercepted.";
	    /*
	     * get 'modifiers'
	     * macOS:
	     * - shift:        QFlags<Qt::KeyboardModifier>(ShiftModifier)
	     * - option/alt:   QFlags<Qt::KeyboardModifier>(AltModifier)
	     * - control:      QFlags<Qt::KeyboardModifier>(MetaModifier)
	     * - command:      QFlags<Qt::KeyboardModifier>(ControlModifier)
	     */
	    qDebug() << "modifiers:" << mouseEvent->modifiers();
	    /*
	     * get the mouse position
	     */
	    qDebug() << "pos():" << mouseEvent->pos() << ", x():" << mouseEvent->x() << ", y():" << mouseEvent->y() << ", globalPos():" << mouseEvent->globalPos();

	    txt = event->type()==QEvent::MouseButtonDblClick?"Dbl-click"
	      :(mouseEvent->button()&Qt::LeftButton?"Left-click"
		:(mouseEvent->button()&Qt::RightButton?"Right-click":"Middle-click"));
	  }
	  // Intercept Wheel events
	  else if (wheelEvent) {
	    qDebug() << "VP1Gui::GlobalEventFilter: wheel event intercepted.";
	    //					QWheelEvent * wheelEvent = dynamic_cast<QWheelEvent*>(event);
	    //					if (wheelEvent)
	    txt = "Wheel event";
	  }

	  if (keyEvent) {
	    qDebug() << "VP1Gui::GlobalEventFilter: keyboard event intercepted.";
	    /*
	     * get the key pressed by the user
	     */
	    qDebug() << "key pressed (code):" << keyEvent->key() << "key pressed (text):" << keyEvent->text();
	    if (keyEvent->text() != "") txt = txt + "+" + keyEvent->text();
	    else txt = txt + "+" + (char) keyEvent->key();
	  }

	  // 	std::cout<<"Popup (dt="<<timediff<<") "<<txt.toStdString()<<". watched = "<<watched
	  // 		 <<" (on="<<watched->objectName().toStdString()<<")"
	  // 		 <<" (cn="<<watched->metaObject()->className()<<")"
	  // 		 <<std::endl;

	  QLabel * label = new QLabel(txt,0,Qt::Tool|Qt::FramelessWindowHint
				      |Qt::X11BypassWindowManagerHint|Qt::WindowStaysOnTopHint);
	  label->setStyleSheet("background-color: yellow;color: black;"
			       "font: bold 140%;border: 2px solid black");
	  //"border-radius: 3px;"
	  label->setFrameStyle(QFrame::StyledPanel);
	  label->setAttribute(Qt::WA_ShowWithoutActivating);
	  label->setFocusPolicy(Qt::NoFocus);
	  QPoint p(QCursor::pos().x()-label->sizeHint().width()/2,QCursor::pos().y()-label->sizeHint().height());
	  label->move(p);
	  QTimer::singleShot(0,label,SLOT(show()));
	  QTimer::singleShot(500, label, SLOT(deleteLater()));
	  m_lastPopupWasQMenu = (dynamic_cast<QMenu*>(watched)!=0);
	}
      }
    return false;//old
    // new, from Qt documentation.
    // FIXME: However, it is not possible to switch the "Show click events" option in VP1Light
    // when that is ON, because the click on the checkbox is not passed on properly.
    return QObject::eventFilter(watched, event);// return false;
  }
private:
  QTime m_lastPopup;
  bool m_lastPopupWasQMenu;
};

//___________________________________________________________________
GXExecutionScheduler::GXExecutionScheduler( QObject * parent)
  : QObject(parent), m_d(new Imp)
{

  m_d->scheduler = this;
  m_d->prioritiser = new VP1Prioritiser(this);
  m_d->mainwindow = new GXMainWindow(this);
	

  m_d->allSystemsRefreshed = false;
  m_d->currentsystemrefreshing=0;
  m_d->eraseJustAfterRefresh=false;
  m_d->postponedUncreateAndDeleteCW=0;
  m_d->refreshtimer = new QTimer(this);
  m_d->splashScreenTimer = new QTimer(this);
  m_d->splashScreenTimer->setSingleShot(true);
  m_d->splashScreenTimer->setInterval(1000);
	
  connect(m_d->refreshtimer, SIGNAL(timeout()), this, SLOT(processSystemForRefresh()));

  connect(m_d->splashScreenTimer, SIGNAL(timeout()), m_d->mainwindow, SLOT(quickSetupTriggered()));
  //Connect signals to ensure that prioritiser knows about present channels and their visibility:
  connect(m_d->mainwindow->channelManager(),SIGNAL(newChannelCreated(IVP1ChannelWidget*)),m_d->prioritiser, SLOT(channelCreated(IVP1ChannelWidget*)));
  connect(m_d->mainwindow->channelManager(),SIGNAL(channelUncreated(IVP1ChannelWidget*)),m_d->prioritiser, SLOT(channelUncreated(IVP1ChannelWidget*)));
  connect(m_d->mainwindow->channelManager(),SIGNAL(newChannelCreated(IVP1ChannelWidget*)),this, SLOT(channelCreated(IVP1ChannelWidget*)));
  connect(m_d->mainwindow->channelManager(),SIGNAL(channelUncreated(IVP1ChannelWidget*)),this, SLOT(channelUncreated(IVP1ChannelWidget*)));

  connect(m_d->mainwindow->tabManager(),SIGNAL(visibleChannelsChanged(const QSet<IVP1ChannelWidget*>&,const QSet<IVP1ChannelWidget*>&,const double&)),
	  m_d->prioritiser,SLOT(visibleChannelsChanged(const QSet<IVP1ChannelWidget*>&,const QSet<IVP1ChannelWidget*>&,const double&)));


  // Init and show the main window of VP1
  SoQt::init( m_d->mainwindow );


  m_d->mainwindow->show();

  bool checkDisplayMouseClicks = VP1QtUtils::expertSettingIsOn("general","ExpertSettings/GMEX_DISPLAY_MOUSE_CLICKS") ||
    VP1QtUtils::environmentVariableIsOn("GMEX_DISPLAY_MOUSE_CLICKS");
  if (checkDisplayMouseClicks) {
    m_d->globalEventFilter = new Imp::GlobalEventFilter;
    qApp->installEventFilter(m_d->globalEventFilter);
  } else {
    m_d->globalEventFilter = 0;
  }


}

//___________________________________________________________________
GXExecutionScheduler::~GXExecutionScheduler()
{
  m_d->refreshtimer->stop();
  delete m_d->mainwindow;
  delete m_d->prioritiser;
  delete m_d->globalEventFilter;
  delete m_d;
}

//___________________________________________________________________
GXExecutionScheduler* GXExecutionScheduler::init()
{

  QCoreApplication::setOrganizationName("ATLAS");
  QCoreApplication::setApplicationName("gmex");
  QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);



  // here we check if the main (and unique!) Qt application has been initialized already. If not we initialize it.
  if (qApp) {
    VP1Msg::message("GXExecutionScheduler::init ERROR: QApplication already initialized. Expect problems!!!");
  } else {
    //NB: Static to avoid scope-problems:
    static int argc=1;
    static char execpath[] = "/some/fake/executable/vp1";
    static char *argv[2];
    //    VP1Msg::messageDebug("setting argv[0]...");
    argv[0] = execpath;
    //    VP1Msg::messageDebug("setting argv[1]...");
    argv[1] = NULL;
    // instead of using the default Qt QApplication class,
    // we now use our custom inherited class where we
    // reimplemented the notify() method, in order to catch
    // C++ exceptions, especially while running it inside Athena.
    //new QApplication(argc, argv);
    new VP1QtApplication(argc, argv);
  }

  GXExecutionScheduler*scheduler = new GXExecutionScheduler(0);
  return scheduler;
}

//___________________________________________________________________
void GXExecutionScheduler::cleanup(GXExecutionScheduler*scheduler)
{
  if (scheduler) {
    delete scheduler;
    qApp->processEvents(); // Qt 5
    qApp->deleteLater(); // Qt 5
    
    Imp::warnIfWidgetsAlive();
    VP1HelperClassBase::warnUndeletedInstances();
    
    delete qApp;
    SoQt::done();
  }
}


//___________________________________________________________________
bool GXExecutionScheduler::interact()
{
  VP1Msg::messageDebug("GXExecutionScheduler::executeNewEvent()");



  assert(!m_d->refreshtimer->isActive());//fixme: -> if.


  VP1Msg::messageDebug("calling refreshtimer->start()...");
  m_d->refreshtimer->start();
  m_d->splashScreenTimer->start();


  //Flush event queue before reenabling controllers, etc.:
  qApp->processEvents();
  VP1Msg::messageDebug("qApp->processEvents() called.");

  //Enable various user input:
  m_d->mainwindow->groupBox_channelcontrols->setEnabled(true);


  qApp->exec();

  VP1Msg::messageDebug("Disabling user inputs...");
  //Disable various user input:
  m_d->mainwindow->groupBox_channelcontrols->setEnabled(false);


  if (m_d->refreshtimer->isActive()) {
    m_d->refreshtimer->stop();
  }

  VP1Msg::messageDebug("Erasing systems...");
  assert(!m_d->currentsystemrefreshing);
  foreach(IVP1System*s,m_d->prioritiser->getSystemsToEraseByPriority()) {
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents|QEventLoop::ExcludeSocketNotifiers);
    eraseSystem(s);
  }


  qApp->processEvents(QEventLoop::ExcludeUserInputEvents|QEventLoop::ExcludeSocketNotifiers);


  VP1Msg::messageDebug("mainwindow->mustQuit ? " + QString::number(m_d->mainwindow->mustQuit()) );
  return !m_d->mainwindow->mustQuit();
}

//___________________________________________________________________
void GXExecutionScheduler::eraseSystem(IVP1System*s) {

  VP1Msg::messageDebug("GXExecutionScheduler::eraseSystem()");

  assert(s->state()==IVP1System::REFRESHED);
  assert(!s->isRefreshing());

  QString base =  QString(s->name())+" from channel "+s->channel()->unique_name();
  m_d->mainwindow->statusBar()->showMessage( "Erasing system ["+base+"]" );
  VP1Msg::messageDebug("ERASING - " + base);

  s->disallowUpdateGUI();
  s->erase();//fixme: time?
  s->setState(IVP1System::ERASED);
  m_d->mainwindow->statusBar()->showMessage( "Post-erase update to channel ["+base+"]" );
  s->channel()->systemErased(s);//fixme: time?
  m_d->mainwindow->statusBar()->clearMessage();
}

//___________________________________________________________________
void GXExecutionScheduler::systemNeedErase() {
  IVP1System*s = static_cast<IVP1System*>(sender());
  assert(s);
  if (m_d->currentsystemrefreshing!=s) {
    eraseSystem(s);
  } else {
    assert(s->isRefreshing());
    m_d->eraseJustAfterRefresh=true;
  }
}

//___________________________________________________________________
void GXExecutionScheduler::processSystemForRefresh()
{
  if (m_d->currentsystemrefreshing)
    return;

  IVP1System * s = m_d->prioritiser->nextErasedActiveSystemByPriority();
  if (s) {
    refreshSystem(s);
  } else {
    assert(m_d->refreshtimer->isActive());//fixme: -> if. ???
    //if (refreshtimer->isActive())
    m_d->refreshtimer->stop();
  }
}

//___________________________________________________________________
void GXExecutionScheduler::channelCreated(IVP1ChannelWidget* cw)
{
  std::set<IVP1System*>::const_iterator it, itE = cw->systems().end();
  for (it=cw->systems().begin();it!=itE;++it) {
    qInfo() << "System name:" << (*it)->name();
    connect(*it,SIGNAL(inactiveSystemTurnedActive()),this,SLOT(startRefreshQueueIfAppropriate()));
    connect(*it,SIGNAL(needErase()),this,SLOT(systemNeedErase()));
#ifdef BUILDVP1LIGHT
    connect(*it,SIGNAL(signalLoadEvent(IVP1System*)),this,SLOT(passEvent(IVP1System*)));
#endif // BUILDVP1LIGHT
  }
  startRefreshQueueIfAppropriate();

}

//___________________________________________________________________
void GXExecutionScheduler::channelUncreated(IVP1ChannelWidget* cw)
{
  std::set<IVP1System*>::const_iterator it, itE = cw->systems().end();
  for (it=cw->systems().begin();it!=itE;++it)
    disconnect(*it,SIGNAL(inactiveSystemTurnedActive()),this,SLOT(startRefreshQueueIfAppropriate()));
}

//___________________________________________________________________
void GXExecutionScheduler::startRefreshQueueIfAppropriate()
{
  if (!m_d->refreshtimer->isActive()) m_d->refreshtimer->start();
}

//___________________________________________________________________
bool GXExecutionScheduler::isRefreshing() const
{
  return m_d->currentsystemrefreshing;
}
//___________________________________________________________________
void GXExecutionScheduler::refreshSystem(IVP1System*s)
{
  QString sysname = s->name();
  VP1Msg::messageDebug("GXExecutionScheduler::refreshSystem() - system: " + sysname);

  assert(s->state()==IVP1System::ERASED);
  assert(s->activeState()==IVP1System::ON);
  assert(!m_d->currentsystemrefreshing);

  m_d->currentsystemrefreshing = s;

  QString base =  QString(s->name())+" from channel "+s->channel()->unique_name();
  m_d->mainwindow->statusBar()->showMessage( "Refreshing system ["+base+"]" );

  s->allowUpdateGUI();
  s->setRefreshing(true);
  refreshingStatusChanged(true);
  s->channel()->emitRefreshInfoChanged();
  s->refresh(VP1AthenaPtrs::eventStore());
  s->setState(IVP1System::REFRESHED);
  s->setRefreshing(false);
  s->disallowUpdateGUI();

  m_d->mainwindow->statusBar()->showMessage( "Post-refresh update to channel ["+base+"]" );
  s->channel()->systemRefreshed(s);//fixme: time independently?

  m_d->mainwindow->statusBar()->clearMessage();
  m_d->currentsystemrefreshing = 0;
	
  refreshingStatusChanged(false);
  s->channel()->emitRefreshInfoChanged();

  VP1Msg::messageDebug("checking if all systems have refreshed...");
  if (hasAllActiveSystemsRefreshed(s->channel())) {

    VP1Msg::messageDebug("All systems refreshed! - last system refreshed! - system: " + sysname);
    s->channel()->lastOfActiveSystemsRefreshed();

    m_d->allSystemsRefreshed = true; // check if that is fine for multiple channels window: i.e. VP1 instances with a couple of 3DCkocktail windows, for example.

  } // end of hasAllActiveSystemsRefreshed()


  if (m_d->eraseJustAfterRefresh) {
    //Someone asked to erase the system while it was refreshing!
    m_d->eraseJustAfterRefresh=false;
    //Check that it still needs to (maybe it was flicked back to ON by the impatient user)
    if (s->activeState()==IVP1System::OFF)
      eraseSystem(s);
  }
  if (m_d->postponedUncreateAndDeleteCW) {
    actualUncreateAndDelete(m_d->postponedUncreateAndDeleteCW);
    m_d->postponedUncreateAndDeleteCW=0;
  }

  VP1Msg::messageDebug("end of refreshing the system: " + sysname);

}


//___________________________________________________________________
void GXExecutionScheduler::bringFromConstructedToReady(IVP1ChannelWidget*cw)
{
  assert(cw->state()==IVP1ChannelWidget::CONSTRUCTED);

  connect(cw,SIGNAL(message(const QString&)),m_d->mainwindow,SLOT(channelAddToMessageBox(const QString&)));

  std::set<IVP1System *>::iterator itsys, itsysE = cw->systems().end();
  for (itsys = cw->systems().begin();itsys!=itsysE;++itsys) {
    assert((*itsys)->state()==IVP1System::CONSTRUCTED);
    connect((*itsys),SIGNAL(sysmessage(const QString&)),m_d->mainwindow,SLOT(systemAddToMessageBox(const QString&)));
  }
  itsysE = cw->systems().end();
  for (itsys = cw->systems().begin();itsys!=itsysE;++itsys) {
    (*itsys)->setCanRegisterController(true);
    (*itsys)->create(VP1AthenaPtrs::detectorStore());
    (*itsys)->setCanRegisterController(false);
    (*itsys)->setState(IVP1System::ERASED);
  }
  assert(cw->state()==IVP1ChannelWidget::CONSTRUCTED);
  cw->setCanRegister(true,false);//Ok to register controllers during create.
  cw->create();
  cw->setCanRegister(false,false);
  cw->setState(IVP1ChannelWidget::READY);

  assert(cw->state()==IVP1ChannelWidget::READY);
  //Fixme: statusbar!!!!!
}

//___________________________________________________________________
void GXExecutionScheduler::uncreateAndDelete(IVP1ChannelWidget*cw)
{
  assert(cw->state()==IVP1ChannelWidget::READY);
  if (m_d->currentsystemrefreshing&&cw->systems().find(m_d->currentsystemrefreshing)!=cw->systems().end()) {
    assert(!m_d->postponedUncreateAndDeleteCW);
    m_d->postponedUncreateAndDeleteCW=cw;
  } else {
    actualUncreateAndDelete(cw);
  }
}

//___________________________________________________________________
void GXExecutionScheduler::actualUncreateAndDelete(IVP1ChannelWidget*cw)
{
  assert(cw->state()==IVP1ChannelWidget::READY);
  cw->setUpdatesEnabled(false);//Just because...

  //From this point on, we are not interested in signals from the channel (and its systems) that could disturb us:
  std::set<IVP1System *>::iterator itsys, itsysE = cw->systems().end();
  for (itsys = cw->systems().begin();itsys!=itsysE;++itsys) {
    (*itsys)->blockSignals(true);
    disconnect(*itsys,0,0,0);
  }
  cw->blockSignals(true);
  disconnect(cw,0,0,0);

  //Make sure that all systems gets in the ERASED state. Throw assert if any is presently refreshing (BAD PROGRAMMER!!)
  for (itsys = cw->systems().begin();itsys!=itsysE;++itsys) {
    assert(m_d->currentsystemrefreshing!=(*itsys));
    if ((*itsys)->state()==IVP1System::REFRESHED)
      eraseSystem(*itsys);
  }

  //Uncreate systems:
  for (itsys = cw->systems().begin();itsys!=itsysE;++itsys) {
    assert((*itsys)->state()==IVP1System::ERASED);
    (*itsys)->uncreate();
    (*itsys)->setState(IVP1System::UNCREATED);
  }

  //Uncreate channel:
  assert(cw->state()==IVP1ChannelWidget::READY);
  cw->uncreate();//Fixme: Return codes!!
  cw->setState(IVP1ChannelWidget::UNCREATED);

  assert(cw->state()==IVP1ChannelWidget::UNCREATED);

  cw->deleteControllers();//Also deletes system controllers.

  //Delete:
  cw->deleteLater();

}

//___________________________________________________________________
void GXExecutionScheduler::Imp::warnIfWidgetsAlive()
{
  QSet<QWidget*> w_ignore, wl(QApplication::allWidgets().begin(), QApplication::allWidgets().end());// = QApplication::allWidgets().toSet();
  //w_ignore<<qApp->desktop();
  foreach (QObject*o,qApp->children()) {
    if (o->isWidgetType())
      w_ignore << static_cast<QWidget*>(o);
  }
  foreach (QWidget * w, wl) {
    if (w->objectName().startsWith("internal clipboard"))
      w_ignore << w;
    if (w->objectName()=="empty_widget")
      w_ignore << w;
  }
  wl.subtract(w_ignore);
  if (!wl.empty()) {
    std::cout<<std::endl;
    std::cout<<"VP1 WARNING: "<<wl.count()<< " widget"<<(wl.count()>1?"s":"")<<" left at end of job:"<<std::endl;
    int i(0);
    foreach(QWidget*w,wl) {
      std::cout<<++i<<") Address="<<w<<", ObjectName="<<w->objectName().toStdString()<<", ClassName="<<w->metaObject()->className()<<std::endl;
    }
    std::cout<<std::endl;
  }
}

//___________________________________________________________________
bool GXExecutionScheduler::hasAllActiveSystemsRefreshed( IVP1ChannelWidget* cw ) const
{
  VP1Msg::messageDebug("GXExecutionScheduler::hasAllActiveSystemsRefreshed() - channelWidget: " + cw->name());

  std::set<IVP1System*>::const_iterator it, itE = cw->systems().end();
  int i=0;
  for (it=cw->systems().begin();it!=itE;++it) {

    /*
     * enum State { CONSTRUCTED=0, REFRESHED, ERASED, UNCREATED };
     * enum ActiveState { ON=0, OFF };//Whether it is part of the erase/refresh cycle.
     */
    if (VP1Msg::verbose()) {
      QString name = QString((*it)->name());
      QString active = QString::number((*it)->activeState());
      QString state = QString::number((*it)->state());
      std::cout << i << " - name: " << name.toStdString() << " - active: " << active.toStdString() << " - refreshed: " << state.toStdString() << std::endl;
    }
    if ((*it)->activeState()==IVP1System::ON&&(*it)->state()!=IVP1System::REFRESHED)
      return false;

    i++;
  }

  return true;
}

//___________________________________________________________________
bool GXExecutionScheduler::Imp::allVisibleRefreshed() const
{
  foreach(IVP1ChannelWidget*cw,mainwindow->tabManager()->visibleChannels())
    if (!scheduler->hasAllActiveSystemsRefreshed(cw))
      return false;
  return true;
}

//___________________________________________________________________
bool GXExecutionScheduler::Imp::allSoonVisibleRefreshed() const
{
  foreach(IVP1ChannelWidget*cw,mainwindow->tabManager()->soonVisibleChannels())
    if (!scheduler->hasAllActiveSystemsRefreshed(cw))
      return false;
  return true;
}






/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class VP1Serialise                     //
//                                                            //
//  Author: Thomas H. Kittelmann (Thomas.Kittelmann@cern.ch)  //
//  Initial version: June 2008                                //
//                                                            //
////////////////////////////////////////////////////////////////

#include "VP1Base/VP1Serialise.h"
#include "VP1Base/VP1QtInventorUtils.h"
#include "VP1Base/PhiSectionWidget.h"
#include "VP1Base/VP1MaterialButton.h"
#include "VP1Base/VP1ColorSelectButton.h"
#include "VP1Base/VP1CollectionWidget.h"
#include "VP1Base/VP1EtaPhiCutWidget.h"
#include "VP1Base/VP1DrawOptionsWidget.h"
#include "VP1Base/VP1CollectionSettingsButtonBase.h"

#include <Inventor/nodes/SoMaterial.h>

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QSet>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QToolBox>
#include <QRadioButton>

//____________________________________________________________________
class VP1Serialise::Imp {
public:
  Imp(VP1Serialise *tc) : theclass(tc), buffer(0), state(0),checkedUnused(false),version(0) {}
  VP1Serialise * theclass;
  QByteArray byteArray;
  QBuffer * buffer;
  QDataStream * state;
  bool checkedUnused;
  qint32 version;

  QSet<const QWidget*> handledWidgets;
  QSet<const QWidget*> ignoredWidgets;
  void handle(QWidget*w) {
    if (!w) {
      if(VP1Msg::debug()){
    	 theclass->messageDebug("VP1Serialize::handle() - Returning...");
      }
      return;
    }
    if (handledWidgets.contains(w))
      theclass->message("ERROR: Handled widget more than once: Type="
			+QString(w->metaObject()->className())
			+", name="+w->objectName());
    handledWidgets.insert(w);
  }
  void handle(const QWidget*w) { handle((QWidget*)(w)); }
  bool expectsPersistification(const QWidget*w);

  static unsigned numberOfInstantiations;
};

unsigned VP1Serialise::Imp::numberOfInstantiations = 0;
//____________________________________________________________________
unsigned VP1Serialise::numberOfInstantiations()
{
  return Imp::numberOfInstantiations;
}

//____________________________________________________________________
void VP1Serialise::decrementNumberOfInstantiations()
{
  --(Imp::numberOfInstantiations);
}

//____________________________________________________________________
VP1Serialise::VP1Serialise(qint32 version, IVP1System * sys)
  : VP1HelperClassBase(sys,"VP1Serialise"), m_d(new Imp(this))
{
  ++(Imp::numberOfInstantiations);

  m_d->byteArray.clear();
  m_d->buffer = new QBuffer(&m_d->byteArray);
  m_d->buffer->open(QIODevice::WriteOnly);
  m_d->state = new QDataStream(m_d->buffer);
  m_d->version = version;
  save(version);
}

//____________________________________________________________________
VP1Serialise::~VP1Serialise()
{
  if (!m_d->checkedUnused)
    message("WARNING: warnUnsaved(..) was never called!");

  m_d->buffer->close();
  delete m_d->state;
  m_d->state = 0;
  delete m_d->buffer;
  m_d->buffer = 0;
  delete m_d;
}

//____________________________________________________________________
QDataStream * VP1Serialise::stream()
{
  return m_d->state;
}


//____________________________________________________________________
qint32 VP1Serialise::version() const
{
  return m_d->version;
}


//____________________________________________________________________
QByteArray VP1Serialise::result()
{
  return m_d->byteArray;
}

//____________________________________________________________________
void VP1Serialise::save(QCheckBox*cb)
{
  if(VP1Msg::debug()){
  	messageDebug("VP1Serialise::save(QCheckBox) - name: " + cb->objectName());
  }
  m_d->handle(cb);
  save(cb->isChecked());
}

//____________________________________________________________________
void VP1Serialise::save(QGroupBox*gb)
{
  if(VP1Msg::debug()){
	 messageDebug("VP1Serialise::save(QGroupBox) - name: " + gb->objectName());
  }
  if (!gb->isCheckable())
    message("WARNING: Asked to handled GroupBox which is not checkable: "+gb->objectName());
  m_d->handle(gb);
  save(gb->isChecked());
}

//____________________________________________________________________
void VP1Serialise::save(QComboBox*cb)
{
  if(VP1Msg::debug()){
	 messageDebug("VP1Serialise::save(QComboBox) - name: " + cb->objectName());
  }
  m_d->handle(cb);
  save( cb->count() > 0 ? cb->currentText() : QString() );
}

//____________________________________________________________________
void VP1Serialise::save(QLineEdit* le)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(QLineEdit) - name: " + le->objectName());
  }
  m_d->handle(le);
  save( le->text() );
}

//____________________________________________________________________
void VP1Serialise::save(QDoubleSpinBox*sb,const double& unit)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(QDoubleSpinBox) - name: " + sb->objectName());
  }
  m_d->handle(sb);
  save(unit==1.0 ? sb->value() : sb->value() * unit );
}

//____________________________________________________________________
void VP1Serialise::save(QSpinBox*sb)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(QSpinBox) - name: " + sb->objectName());
  }
  m_d->handle(sb);
  save(sb->value());
}

//____________________________________________________________________
void VP1Serialise::save(QSlider*s)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(QSlider) - name: " + s->objectName());
  }
  m_d->handle(s);
  save(s->value());
}

//____________________________________________________________________
void VP1Serialise::save(bool b)
{
  if (VP1Msg::verbose()){
    messageVerbose("Saving bool "+str(b));
  }
  (*m_d->state) << b;
}

//____________________________________________________________________
void VP1Serialise::save(qint32 i)
{
  if (VP1Msg::verbose()){
    messageVerbose("Saving int "+str(i));
  }
  (*m_d->state) << i;
}

//____________________________________________________________________
void VP1Serialise::save(const double& dbl)
{
  if (VP1Msg::verbose()){
    messageVerbose("Saving double "+str(dbl));
  }
  (*(m_d->state)) << dbl;
}

//____________________________________________________________________
void VP1Serialise::save(const QString& s)
{
  if (VP1Msg::verbose()){
    messageVerbose("Saving string "+s);
  }
  (*(m_d->state)) << s;
}

//____________________________________________________________________
void VP1Serialise::save(const QByteArray& ba)
{
  if (VP1Msg::verbose()){
    messageVerbose("Saving byte array (length = "+QString::number(ba.count())+")");
  }
  (*(m_d->state)) << ba;
}

//____________________________________________________________________
void VP1Serialise::save(QToolBox*tb)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(QToolBox) - name: " + tb->objectName());
  }
  m_d->handle(tb);
  save( tb && tb->count() > 0 ? tb->currentIndex() : -1 );
}

//____________________________________________________________________
void VP1Serialise::saveByTitle(QToolBox* tb)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(QToolBox) - name: " + tb->objectName());
  }
  m_d->handle(tb);
  int i = tb ? tb->currentIndex() : -1;
  save( i>=0 && i<tb->count() ? tb->itemText(i) : QString() );
}

//____________________________________________________________________
void VP1Serialise::save(SoMaterial* m)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(SoMaterial)");
  }
  if (!m) {
    save(QByteArray());
    return;
  }
  m->ref();
  save(VP1QtInventorUtils::serialiseSoMaterial(m));
  m->unrefNoDelete();
}

//____________________________________________________________________
void VP1Serialise::save(const VP1MaterialButton*mb)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(VP1MaterialButton) - name: " + mb->objectName());
  }
  m_d->handle(mb);
  QList<SoMaterial*> mats = mb ? mb->handledMaterials() : QList<SoMaterial*>();
  save(mats.isEmpty() ? 0 : mats.at(0));
}

//____________________________________________________________________
void VP1Serialise::save(const QColor& c)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(QColor) - name: " + c.name());
  }
  if (VP1Msg::verbose()){
    messageVerbose("Saving color "+str(c));
  }
  (*m_d->state) << c;
}

//____________________________________________________________________
void VP1Serialise::save(VP1ColorSelectButton* cb)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(VP1ColorSelectButton) - name: " + cb->objectName());
  }
  m_d->handle(cb);
  save(cb ? cb->color() : QColor());
}

//____________________________________________________________________
void VP1Serialise::save(const PhiSectionWidget*phi)
{
  if(VP1Msg::debug()){
	  messageDebug("\nVP1Serialise::save(PhiSectionWidget) - name: " + phi->objectName());
  }
  m_d->handle(phi);
  if (VP1Msg::verbose()){
    messageVerbose("Saving phisection widget state");
  }
  (*(m_d->state)) << (phi ? phi->state() : QByteArray());
}

//____________________________________________________________________
void VP1Serialise::save( QRadioButton * rb0,
			 QRadioButton * rb1,
			 QRadioButton * rb2,
			 QRadioButton * rb3,
			 QRadioButton * rb4,
			 QRadioButton * rb5,
			 QRadioButton * rb6,
			 QRadioButton * rb7,
			 QRadioButton * rb8,
			 QRadioButton * rb9 )
{
  QList<QRadioButton *> l;
  l << rb0 << rb1 << rb2 << rb3 << rb4 << rb5 << rb6 << rb7 << rb8 << rb9;
  for (qint32 i = 0; i < l.count(); ++i) {
    if (l.at(i)) {
      if(VP1Msg::debug()){
        messageDebug("\nVP1Serialise::save(QRadioButton) - name: " + l.at(i)->objectName());
      }
      m_d->handle(l.at(i));
    }
  }
  qint32 ichecked(-1);
  for (qint32 i = 0; i < l.count(); ++i) {
    if (l.at(i)&&l.at(i)->isChecked()) {
      ichecked = i;
      break;
    }
  }
  save(ichecked);
}

//____________________________________________________________________
void VP1Serialise::save(const VP1CollectionWidget*cw)
{
  if(VP1Msg::debug()){
  	messageDebug("\nVP1Serialise::save(VP1CollectionWidget) - name: " + cw->objectName());
  	messageDebug("VP1Serialise::save(VP1CollectionWidget)- start...");
  }
  m_d->handle(cw);
  ignoreWidget(cw);//To ignore all children of the collection widget.
  QByteArray ba;
  QBuffer buffer(&ba);
  buffer.open(QIODevice::WriteOnly);
  QDataStream out(&buffer);
  out << (qint32)0;//version
  out << cw->states();
  buffer.close();
  save(ba);
  if(VP1Msg::debug()){
    messageDebug("VP1Serialise::save(VP1CollectionWidget)- end.");
  }
}

////____________________________________________________________________
//void VP1Serialise::save(const JetCollectionSettingsButton* jcb)
//{
//  m_d->handle(jcb);
//  ignoreWidget(jcb);//To ignore all children of the etaphicut widget.
//  save(jcb ? jcb->saveState() : QByteArray());
//}
//____________________________________________________________________
void VP1Serialise::save(const VP1CollectionSettingsButtonBase* jcb)
{
  if(VP1Msg::debug()){
  	messageDebug("\nVP1Serialise::save(VP1CollectionSettingsButtonBase) - name: " + jcb->objectName());
  	messageDebug("VP1Serialise::save(VP1CollectionSettingsButtonBase)- start...");
  }
  m_d->handle(jcb);
  ignoreWidget(jcb);//To ignore all children of the etaphicut widget.
  save(jcb ? jcb->saveState() : QByteArray());
  if(VP1Msg::debug()){
    messageDebug("VP1Serialise::save(VP1CollectionSettingsButtonBase)- end.");
  }
}


//____________________________________________________________________
void VP1Serialise::save(const VP1EtaPhiCutWidget*w)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(VP1EtaPhiCutWidget) - name: " + w->objectName());
  }
  m_d->handle(w);
  ignoreWidget(w);//To ignore all children of the etaphicut widget.
  save(w ? w->saveState() : QByteArray());
}

//____________________________________________________________________
void VP1Serialise::save(const VP1DrawOptionsWidget*w)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::save(VP1DrawOptionsWidget) - name: " + w->objectName());
  }
  m_d->handle(w);
  ignoreWidget(w);//To ignore all children of the draw options widget.
  save(w ? w->state() : QByteArray());
}

//____________________________________________________________________
void VP1Serialise::ignoreWidget(const QWidget*w)
{
  if(VP1Msg::debug()){
	 messageDebug("\nVP1Serialise::ignoreWidget(QWidget) - name: " + w->objectName());
  }
  if (w)
    m_d->ignoredWidgets.insert(w);
}

//____________________________________________________________________
void VP1Serialise::widgetHandled(const QWidget*w)
{
  m_d->handle(w);
}

//____________________________________________________________________
bool VP1Serialise::Imp::expectsPersistification(const QWidget*w)
{
  //NB: Same code as in VP1Deserialise::Imp::expectsPersistification
  if (!w)
    return false;

  //Fixme: Something faster than string based? Or only do this in verbose mode?

  if (w->inherits("QGroupBox"))
    return static_cast<const QGroupBox*>(w)->isCheckable();

  return w->inherits("QCheckBox")
    || w->inherits("QRadioButton")
    || w->inherits("QComboBox")
    || w->inherits("QAbstractSpinBox")
    || w->inherits("QSlider")
    || w->inherits("QToolBox")
    || w->inherits("PhiSectionWidget")
    || w->inherits("VP1EtaPhiCutWidget")
    || w->inherits("VP1DrawOptionsWidget")
    || w->inherits("QLineEdit")
    || w->inherits("VP1ColorSelectButton")
    || w->inherits("VP1MaterialButton");
}

//____________________________________________________________________
void VP1Serialise::disableUnsavedChecks()
{
  m_d->checkedUnused = true;
}

//____________________________________________________________________
void VP1Serialise::warnUnsaved(const QObject* object)
{
  //NB: Same code as in VP1Deserialise::warnUnrestored

  if (!m_d->checkedUnused)
    m_d->checkedUnused = true;

  if (!object)
    return;

  if (object->isWidgetType()&&m_d->ignoredWidgets.contains(static_cast<const QWidget*>(object)))
	  return;

  if (object->isWidgetType()&&!object->objectName().startsWith("qt_")) {
	  const QWidget * wid = static_cast<const QWidget*>(object);
	  if (!m_d->handledWidgets.contains(wid)&&m_d->expectsPersistification(wid)) {
		  QString s("WARNING Unsaved widget of type: "+QString(wid->metaObject()->className())+" and object name = "+wid->objectName());
		  if (VP1Msg::verbose()){
			  message(s);
      }
      if(VP1Msg::debug()){
			  messageDebug(s);
      }
	  }
  }
  //Call recursively on all "children":
  foreach(const QObject* o, object->children())
    warnUnsaved(static_cast<const QWidget*>(o));
}

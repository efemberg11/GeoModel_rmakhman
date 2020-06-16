/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////
//                                                         //
//  Header file for class GXMainWindow                    //
//                                                         //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>  //
//                                                         //
//  Initial version: April 2007                            //
//                                                         //
/////////////////////////////////////////////////////////////

#ifndef GXMainWindow_H
#define GXMainWindow_H

// include the related GUI
#include "ui_gxmainwindow.h"

// include VP1
#include "VP1Gui/VP1EventDisplayEditor.h"

// include Qt
#include <QQueue>
#include <QStringList>
#include <QMap>

class VP1ChannelManager;
class VP1TabManager;
class QStringList;
class IVP1ChannelWidget;
class GXExecutionScheduler;
class VP1PluginDialog;
class QProgressBar;
class QLabel;
class QComboBox;
class VP1StreamMenuUpdater;
class QMutex;


class GXMainWindow : public QMainWindow, public Ui::GXMainWindow
{
  Q_OBJECT

public:
  GXMainWindow(GXExecutionScheduler*,QWidget *parent = 0);
  ~GXMainWindow();


  // Set next event to the execution scheduler and updates event controls
  // We needed to separate this method from goToNextEvent() in order to be able
  // to skip the bootstrap event in the https mode
  void nextEvent();

  QStringList userRequestedFiles();
  
public slots:
  void loadPluginFile(QString filename);

  #ifdef BUILDVP1LIGHT
    void request_expertSettings();
  #endif
  void request_addEmptyTab();
  void request_channelInformation();
  void request_printChannel();
  QString request_saveChannelSnapshot(QString xLabel="");
  void request_saveasConfig();
  void request_saveConfig();
  void request_loadConfig();
  void request_loadPlugin();
  void unloadPlugin_continue();
  void pluginDialogClosed();
  void saveAllCurrentChannels();
  void makeAllChannelsEventDisplay();
  void getAllChannelsIntoSnapshots(QList<QPixmap>& list, QStringList& listNames);
  QPixmap getSingleChannelCustomSnapshot(IVP1ChannelWidget* tab, int width = 0);
  QPixmap getSingleChannelCustomSnapshot(QString tabName, int width = 0);

  void tabListChanged(QStringList);

  void selectedChannelChanged(IVP1ChannelWidget*);

  void loadConfigurationFromFile(QString file);
  void replaceConfigurationFile(QString file);

  void addToMessageBox( const QString&, const QString& extrastyleopts = "",
			const QString& title = "", const QString& titleextrastyleopts = "" );
  void systemAddToMessageBox( const QString& );
  void channelAddToMessageBox( const QString& );
  void helperAddToMessageBox( const QString& );
  void changeFontSize(int);


public:
  bool mustQuit() const;

  VP1ChannelManager * channelManager() const { return m_channelmanager; }
  VP1TabManager * tabManager() const { return m_tabmanager; }

  bool userRequestedExit() { return m_userRequestedExit; }
  //  int getEventTimestamp() { return m_timestamp; }

protected:
  VP1ChannelManager * m_channelmanager;
  VP1TabManager * m_tabmanager;
  QString m_currentconfigfile;
  //unsigned m_timestamp;
  //  bool m_betweenevents;

  void closeEvent(QCloseEvent *event);
  bool m_mustquit;

  QWidget* m_dummyemptycontroller;
  GXExecutionScheduler*m_scheduler;


  void addChannelIconsToComboBox(QComboBox* cb, const bool& isbasenames);

  QStringList m_currentunloadpluginfiles;
  VP1PluginDialog*m_plugindialog;

  void setupStatusBar();
  QLabel*m_statusbarlabel;

  QMap<QString,QString> availablePluginFiles() const;
  QMap<QString,QString> availableFiles(const QString& extension,
				       const QString& pathvar,
				       const QString& instareasubdir,
				       const QString& extradirenvvar,
				       bool currentdir = false) const;

public:
  QProgressBar*progressbar;
protected slots:
  void postInitUpdates();
  void showMenu_loadPlugin();
  void showMenu_loadPluginItemSelected();
  void showMenu_loadConfFile();
  void showMenu_loadConfFileItemSelected();
  void changeStyleActionTriggered();
  void changeFontSizeActionTriggered();
  void updateCentralStackWidget();
  void quickSetupTriggered();
  void updateEventControls();
  void help_openGMWebSite();
  void help_openAbout();

  void launch3DstereoEditor();


protected:
  QAction * m_action_infoAboutLoadedPlugins;
  QMenu * m_menu_loadPlugin;
  QMenu * m_menu_loadConfFile;
  QMenu * m_menu_changeStyle;
  QMenu * m_menu_changeFontSize;
  QAction * m_actionAdd_empty_tab;
  QAction * m_actionEnableExpertSettings;
  QAction * m_actionSave_current_tabs;
  QAction* m_action_openUsersGuide;
  QAction* m_action_openUsersSupport;
  QAction* m_action_openGMSite;
  QAction* m_action_openAbout;

  QString m_currentsaveimagepath;
  QString m_currentloadpluginpath;
  QString m_currentStream;

  //Fontsize:
  QFont m_defaultfont;
  double m_defaultfont_pointsize;
  int m_defaultfont_pixelsize;
  const QString m_settingsfile;

  bool m_userRequestedExit;

  QStringList m_userRequestedFiles;

  QMutex* m_mutex;

  // Event display editor
  VP1EventDisplayEditor* m_edEditor;
};


#endif


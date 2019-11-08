/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/
// Author: Joe Boudreau, repurposed from orginal work from 
//              Riccardo.Maria.Bianchi@cern.ch, Apr 2017
// Update: Nov 2019
//
// Qt includes
#include <QSettings>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QCommandLineParser>

//
#include "VP1Gui/VP1Gui.h"

// C++ includes
#include <iostream>
#include <stdlib.h>
#include <fstream>

int main(int argc, char** argv)
{
  
  QStringList arguments;
  for (int i = 0; i<=argc; i++){
    arguments << argv[i];
  }

  QCommandLineParser parser;

  // An database directory option
  QCommandLineOption databaseDirectoryOption(QStringList() << "d" << "database", "Use geometry database <file>.","default");
  parser.addOption(databaseDirectoryOption);

  // Print help
  QCommandLineOption helpOption(QStringList() << "h" << "help", "Show help.");
  parser.addOption(helpOption);

  // Process the command line arguments given by the user
  parser.process(arguments);

  bool helpIsSet = parser.isSet(helpOption);
  bool dbIsSet = parser.isSet(databaseDirectoryOption);
  QString dbDir = parser.value(databaseDirectoryOption);
  if (dbDir!="") setenv("GX_GEOMETRY_FILE",dbDir.toStdString().c_str(), true);
  
  //If help option is set, display help and exit VP1Light
  if(helpIsSet){
    qInfo() << "Usage";
    qInfo() << " ";
    qInfo() << "  vp1light [options]";
    qInfo() << " ";
    qInfo() << "Add path to geometry database file as options. Alternatively it can be set inside VP1Light.";
    qInfo() << " ";
    qInfo() << "Options:";
    qInfo() << "  -h, --help                   = Show help.";
    qInfo() << "  -d, --database <path-to-DB>  = Specify geometry database file.";
    return 0;
  }

  // Save settings
  QSettings settings("ATLAS", "VP1Light");
  // Disable expert settings by default
  if(settings.value("ExpertSettings/notFirstStart").toString().isEmpty()){
    settings.setValue("ExpertSettings/notFirstStart","1");
    settings.setValue("ExpertSettings/enableExpertSettings","");
  }



 VP1Gui *vp1gui = new VP1Gui( nullptr, nullptr,nullptr,nullptr, // Storegate to get rid of. 
			      std::vector<std::string>(),
			      "NONE", // initial cruise mode
			      0,      //m_initialCruiseSeconds,
			      "",     // sourcedir
			      "",     // local copy dir
			      0,      // file limit
			      std::vector<std::string>());
 vp1gui->init();
 while (  vp1gui->executeNewEvent(0,0,0,0));
 vp1gui->cleanup();
 delete vp1gui;
}

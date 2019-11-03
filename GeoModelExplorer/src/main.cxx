/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

// Author: Riccardo.Maria.Bianchi@cern.ch, Apr 2017
// Update: Apr 2019

#include "RunVP1Light/VP1LightRun.h"

// Qt includes
#include <QSettings>
#include <QString>
#include <QDir>
#include <QDebug>
// #include <QCoreApplication>
#include <QCommandLineParser>

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


  VP1LightRun vp1light("my first VP1Light test");

  vp1light.initialize();
  while ( vp1light.execute());
}

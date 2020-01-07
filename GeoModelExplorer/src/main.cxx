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
#include "VP1Gui/GXExecutionScheduler.h"

// C++ includes
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv)
{
  
  QStringList arguments;
  for (int i = 0; i<=argc; i++){
    arguments << argv[i];
  }

  QCommandLineParser parser;

  // Print help
  QCommandLineOption helpOption(QStringList() << "h" << "help", "Show help.");
  parser.addOption(helpOption);

  // Process the command line arguments given by the user
  parser.process(arguments);

  bool helpIsSet = parser.isSet(helpOption);

  QStringList inputList=parser.positionalArguments();
  
  int NGeomFiles=0;
  for (int i=0;i<inputList.size()-1;i++) {
    std::string input=inputList[i].toStdString();
    if (input.find(".db")!=std::string::npos) {
      setenv((std::string("GX_GEOMETRY_FILE")+std::to_string(NGeomFiles++)).c_str(),inputList[i].toStdString().c_str(), true);
    }
    else {
      qWarning() << "Warning, file " << input.c_str() << " type not recognized";
      return 0;
    }
  }
  
  


  //If help option is set, display help and exit VP1Light
  if(helpIsSet){
    qInfo() << "Usage";
    qInfo() << " ";
    qInfo() << "  vp1light [options] [dbfile1] [dbfile2]... [sharedlib1] [sharedlib2] [sharedlib3]...";
    qInfo() << "Options:";
    qInfo() << "  -h, --help                   = Show help.";
    return 0;
  }

  // Save settings
  QSettings settings("ATLAS", "VP1Light");
  // Disable expert settings by default
  if(settings.value("ExpertSettings/notFirstStart").toString().isEmpty()){
    settings.setValue("ExpertSettings/notFirstStart","1");
    settings.setValue("ExpertSettings/enableExpertSettings","");
  }

  GXExecutionScheduler *scheduler=GXExecutionScheduler::init();
  while (scheduler->interact())
  GXExecutionScheduler::cleanup(scheduler);
  
}

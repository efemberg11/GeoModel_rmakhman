/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/
// Author: Joe Boudreau, repurposed from orginal work from
//              - Riccardo Maria Bianchi, Apr 2017
//              - Sebastian Andreas Merkt, 2018
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
#include <cstdlib>

// utility functions to get preprocessor variables' values
#define STR_VALUE(arg) #arg
#define STR_NAME(name) STR_VALUE(name)



int main(int argc, char** argv)
{
  auto pManip= [] (const char * variable, const char *plus) {
		 const char *path=getenv(variable);
		 if (path) {
		   std::string newPath=std::string(path)+":"+plus;
		   setenv(variable, newPath.c_str(), true);
		 }
		 else {
		   setenv(variable, plus, true);
		 }
	       };
  // Preprocessor:  we are passing the install directory to this
  // class, from the CMake configuration.
  // Drivers will be loaded from this install directory.
  // However in some cases "linux" is part of the install directory
  // name and that may be defined in a macro to be 1. So if that is
  // the case we temporarily undef it.
#ifdef linux
#define waslinux linux
#undef linux
  const char * standardPlaces = GEOMODEL_INSTALL_PREFIX;
#define linux waslinux
#else
  const char * standardPlaces = GEOMODEL_INSTALL_PREFIX;
#endif
  std::cout << "standardPlaces: " << standardPlaces << std::endl;
  std::string gxpluginpath= std::string(standardPlaces)+"/lib/gxplugins";
  pManip("GXPLUGINPATH",gxpluginpath.c_str());
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

  // This handles the input geometry files and the plugins
  int NGeomFiles=0;
  for (int i=0;i<inputList.size()-1;i++) {
    std::string input=inputList[i].toStdString();
    if (input.find(".db")!=std::string::npos ||
	input.find(".so")!=std::string::npos ||
	input.find(".dylib")!=std::string::npos)
      {
      setenv((std::string("GX_GEOMETRY_FILE")+std::to_string(NGeomFiles++)).c_str(),inputList[i].toStdString().c_str(), true);
    }
  }

  // This handles the JSON files containing the geometry clash information
  int NJsonFiles=0;
  for (int i=0;i<inputList.size()-1;i++) {
    std::string input=inputList[i].toStdString();
    if (input.find(".json")!=std::string::npos){

      setenv((std::string("GX_JSON_FILE")+std::to_string(NJsonFiles++)).c_str(),inputList[i].toStdString().c_str(), true);
    }
  }


  //If help option is set, display help and exit VP1Light
  if(helpIsSet){
    qInfo() << "Usage";
    qInfo() << " ";
    qInfo() << "  vp1light [options] [dbfile1] [dbfile2]... [sharedlib1] [sharedlib2] [sharedlib3]... [clash1.json] [clash2.json]...";
    qInfo() << "Options:";
    qInfo() << "  -h, --help                   = Show help.";
    return 0;
  }

  // Save settings
  QSettings settings("ATLAS", "VP1Light"); // TODO: update labels!
  // Disable expert settings by default
  if(settings.value("ExpertSettings/notFirstStart").toString().isEmpty()){
    settings.setValue("ExpertSettings/notFirstStart","1");
    settings.setValue("ExpertSettings/enableExpertSettings","");
  }

  GXExecutionScheduler *scheduler=GXExecutionScheduler::init();
  while (scheduler->interact())
  GXExecutionScheduler::cleanup(scheduler);

}

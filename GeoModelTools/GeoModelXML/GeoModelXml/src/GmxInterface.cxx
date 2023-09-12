/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

//
//    Implement default GmxInterface, which just prints debug messages.
//    User should over-ride all three methods.
//    Use: create an instance of this, and pass a reference to it in the Gmx2Geo constructor.
//
//    Updates:
//    - 2022 Apr, Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
//                Fix GMX trying to access non-existent GeoModel nodes
//
#include "GeoModelXml/GmxInterface.h"
#include "OutputDirector.h"
#include "GeoModelKernel/GeoPVConstLink.h"
#include "GeoModelKernel/Query.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoVFullPhysVol.h"
#include "GeoModelKernel/GeoPublisher.h"
#include <atomic>
using namespace std;

int GmxInterface::sensorId(map<string, int> &/*index*/) const {
  static std::atomic<int> sequentialId;
  return sequentialId++;
}

int GmxInterface::splitSensorId(map<string, int> &/*index*/, std::pair<std::string, int> &/*extraIndex*/, map<string, int> &/*updatedIndex*/) const {
  static std::atomic<int> sequentialId;
  return sequentialId++;
}

void GmxInterface::setPublisher(GeoPublisher * publisher){
    msglog << MSG::DEBUG << "GmxInterface: setting a GeoPublisher" << endmsg;
    m_publisher = publisher;
}

void GmxInterface::publish(std::string& tableName, std::vector<std::string>& colNames, std::vector<std::string>& colTypes, std::vector<std::vector<std::variant<int,long,float,double,std::string>>>& tableData){
      if(!m_publisher) return;
      msglog << MSG::DEBUG << "GmxInterface: publishing Aux Tables" << endmsg;  
      m_publisher->storeDataTable(tableName,colNames,colTypes,tableData);
}

void GmxInterface::addSensorType(const string& clas, const string& type, const map<string, string>& params) {

    msglog << MSG::DEBUG << "GmxInterface::addSensorType called for class " << clas << "; type " << type << 
                         "\n    Parameter names and values:\n";
    for (map<string, string>::const_iterator p = params.begin(); p != params.end(); ++p) {
        msglog << "        " << p->first << " = " << p->second << endmsg;
    }
}

void GmxInterface::addSensor(const string& name, map<string, int> &index, int sequentialId, GeoVFullPhysVol *fpv) {
    std::stringstream idString;
    msglog << MSG::DEBUG << "GmxInterface::addSensor called for " << fpv->getLogVol()->getName() << ", type " << name << 
                         ". Indices:   ";		
    for (map<string, int>::iterator i = index.begin(); i != index.end(); ++i) {
        msglog << i->second << "   ";
        if(i!=index.begin()) idString <<"_";
        idString << i->first <<"_"<<i->second; 
    }
    //finally, add the sensor type
    idString << "_" << name;
    msglog << "\nSequential ID = " << sequentialId << endmsg;
    if (m_publisher) m_publisher->publishNode<GeoVFullPhysVol*,std::string>(fpv,idString.str());
}

void GmxInterface::addSplitSensor(const string& name, map<string, int> &index, std::pair<std::string, int> &extraIndex, int sequentialId, GeoVFullPhysVol *fpv,int splitLevel) {
    std::stringstream idString;
    msglog << MSG::DEBUG << "GmxInterface::addSensor called for " << fpv->getLogVol()->getName() << ", type " << name << 
                         ". Indices:   ";		
    for (map<string, int>::iterator i = index.begin(); i != index.end(); ++i) {
        msglog << i->second << "   ";
        if(i!=index.begin()) idString <<"_";
        idString << i->first <<"_"<<i->second; 
    }
    msglog << "\n and additional Indices " << extraIndex.first << " : "<<
      extraIndex.second;
      idString<<"_split_"<<splitLevel;

    //finally, add the sensor type
    idString << "_" << name;
    msglog << "\nSequential ID = " << sequentialId << endmsg;
    if (m_publisher) m_publisher->publishNode<GeoVFullPhysVol*,std::string>(fpv,idString.str());
   
}

void GmxInterface::addAlignable(int level, map<std::string, int> &index, GeoVFullPhysVol *fpv, 
                                GeoAlignableTransform * transform) {
   
    std::stringstream idString;
    // debug message
    msglog << MSG::DEBUG 
        << "GmxInterface::addAlignable called for a physvol. Logvol name " 
        << fpv->getLogVol()->getName() << ". Level = " << level << ". Indices:   ";
    for (map<string, int>::iterator i = index.begin(); i != index.end(); ++i) {
        msglog << i->second << "   ";
        if(i!=index.begin()) idString <<"_";
        idString << i->first <<"_"<<i->second;
    }
    msglog << endmsg;
    //--------------

    //publish
    if (m_publisher) m_publisher->publishNode<GeoAlignableTransform *,std::string>(transform,idString.str());
}

void GmxInterface::addSplitAlignable(int level, map<std::string, int> &index, std::pair<std::string, int> &extraIndex, GeoVFullPhysVol *fpv, 
                                GeoAlignableTransform * transform) {
   
    std::stringstream idString;
    // debug message
    msglog << MSG::DEBUG 
        << "GmxInterface::addAlignable called for a physvol. Logvol name " 
        << fpv->getLogVol()->getName() << ". Level = " << level << ". Indices:   ";
    for (map<string, int>::iterator i = index.begin(); i != index.end(); ++i) {
        msglog << i->second << "   ";
        if(i!=index.begin()) idString <<"_";
        idString << i->first <<"_"<<i->second;
    }
    msglog << "\n and additional Indices " << extraIndex.first << " : "<<
    extraIndex.second;
    idString<<"_split_"<<extraIndex.first<<"_"<<extraIndex.second;
    msglog << endmsg;
    //--------------

    //publish
    if (m_publisher) m_publisher->publishNode<GeoAlignableTransform *,std::string>(transform,idString.str());
}


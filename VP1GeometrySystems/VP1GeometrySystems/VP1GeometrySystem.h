/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef VP1GEOMETRYSYSTEM_H
#define VP1GEOMETRYSYSTEM_H

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  Header file for class VP1GeometrySystem                            //
//                                                                     //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>              //
//                                                                     //
//  Derived from V-atlas geometry system by Joe Boudreau.              //
//  Origins of initial version dates back to ~1996, initial VP1        //
//  version by TK (May 2007) and almost entirely rewritten Oct 2007    //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "VP1Base/IVP13DSystemSimple.h"
#include "VP1GeometrySystems/VP1GeoFlags.h"
#include "VP1GeometrySystems/VolumeHandle.h"//fixme
#include "GeoModelKernel/GeoPhysVol.h"
#include <set>
#include <map>
#include <QStack>
#include <QString>

class VP1GeometrySystem : public IVP13DSystemSimple {

  Q_OBJECT

public:


  VP1GeometrySystem(QString name = "Geo" );
  virtual ~VP1GeometrySystem();

  //Method that channel can use to override defaults:
  void setGeometrySelectable(bool);
  void setZoomToVolumeOnClick(bool);


  QWidget * buildController();

  void systemcreate(StoreGateSvc*detstore);
  void buildPermanentSceneGraph(StoreGateSvc* detstore, SoSeparator *root);
  void buildEventSceneGraph(StoreGateSvc*, SoSeparator *) {}
  void userPickedNode(SoNode* pickedNode, SoPath *pickedPath);
  void systemuncreate();

  QByteArray saveState();
  void restoreFromState(QByteArray);

public slots:

  void setCurvedSurfaceRealism(int);//Accepts values in the range 0..100.
protected slots:
  void checkboxChanged();
  void updateTransparency();

  void resetSubSystems(VP1GeoFlags::SubSystemFlags);
  void autoExpandByVolumeOrMaterialName(bool,QString);//volname: (false,namestr), matname: (true,namestr)
  void actionOnAllNonStandardVolumes(bool);//true: zap, false: expand.

  void volumeStateChangeRequested(VolumeHandle*,VP1GeoFlags::VOLSTATE);
  void volumeResetRequested(VolumeHandle*);

  void setShowVolumeOutLines(bool);

  void saveMaterialsToFile(QString,bool);//(filename,onlyChangedMaterials)
  void loadMaterialsFromFile(QString);//filename

  GeoPhysVol *newWorld()  const; 
  void saveTrees();
  
protected:
  class Imp;
  Imp * m_d;
};

#endif

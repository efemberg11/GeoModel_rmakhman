/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_PERSISTIFIERFWD_H
#define GEOMODELKERNEL_PERSISTIFIERFWD_H

// ****************************************************************
// *** PERSISTIFICATION SETTINGS FOR GeoModel BOOLEAN SHAPES ******
// ****************************************************************
// Note:
// -- The instructions below are needed
// -- to correctly persistify/restore boolean shapes.
// -- They must be declared before including GeoModelKernel files.
// ****************************************************************
// This variable is used by GeoModel boolean shapes to switch ON
// the persistification mode
#define _GeoShapePersistification_On_
// fwd declaration of namespace::class we want to use to persistify
// the GeoModel boolean shapes
namespace GeoModelIO {
class ReadGeoModel;
}
// the 'Persistifier' class name is used by GeoModel boolean shapes
// to grant access to friend classes
typedef GeoModelIO::ReadGeoModel Persistifier;
// ****************************************************************
// ****************************************************************

#endif

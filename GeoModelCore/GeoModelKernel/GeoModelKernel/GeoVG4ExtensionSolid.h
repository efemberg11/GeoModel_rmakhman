/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVG4SOLIDPLUGIN_H
#define GEOMODELKERNEL_GEOVG4SOLIDPLUGIN_H

/**
 * @class: GeoVG4SolidPlugin
 *
 * @brief Abstact Base Class for a G4Solid plugin.
 *	  The G4Solid plugin builds the raw G4Solid
 */

class G4VSolid;
class GeoUnidentifiedShape;
class GeoVG4SolidPlugin 
{
 public:

   //! Default constructor.
   GeoVG4SolidPlugin() {}
     
   virtual ~GeoVG4SolidPlugin() {}

   //! Create the specified solid. .
   virtual G4VSolid *newG4Solid(const GeoUnidentifiedShape *shape) const=0;

 private:

  // Illegal operations
  GeoVG4SolidPlugin(const GeoVG4SolidPlugin &right)=delete;
  GeoVG4SolidPlugin & operator=(const GeoVG4SolidPlugin &right)=delete;

};

#endif

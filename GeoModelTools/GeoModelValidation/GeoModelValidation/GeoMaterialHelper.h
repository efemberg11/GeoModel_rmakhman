/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

///////////////////////////////////////////////////////////////////
// GeoMaterialHelper.h, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////

#ifndef GEOMATERIALHELPER_H
#define GEOMATERIALHELPER_H

#include <GeoModelKernel/GeoDefinitions.h>
#include <GeoModelKernel/GeoMaterial.h>
#include <GeoModelKernel/GeoVPhysVol.h>


namespace GeoModelTools {

    /**
    @class GeoMaterialHelper
    
    A helper class that browses the GeoModel tree for material estimates
        
    @author sarka.todorova@cern.ch
    */

    using GeoMaterialPtr = GeoIntrusivePtr<const GeoMaterial>;
    using MaterialComponent = std::pair<GeoMaterialPtr, double>;
    
    class GeoMaterialHelper {
      
      public:
        /** Default constructor*/
        GeoMaterialHelper() = default;
        /** Destructor*/
        virtual ~GeoMaterialHelper() = default;
    
        /** Evaluate mass ( of a GeoModel tree/branch/volume ) */
        double evaluateMass(PVConstLink gv, bool inclusive = true) const;

        /** Collect and blend material : return blended material (the client takes the ownership) and effective volume */
        MaterialComponent collectMaterial(PVConstLink gv) const;

        /** hardcoded dummy materials : TODO : find generic criterium ( density ? radiation length ? )  */
        bool dummy_material(PVConstLink gv) const;

      private:
	        /** Internal recursive loop over material components */
	        void collectMaterialContent( PVConstLink gv, std::vector< MaterialComponent >& materialContent) const; 
   };
 

} // end of namespace GeoModelTools

#endif


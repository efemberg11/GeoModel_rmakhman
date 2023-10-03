/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

///////////////////////////////////////////////////////////////////
// GeoMaterialHelper.h, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////

#ifndef GEOMATERIALHELPER_H
#define GEOMATERIALHELPER_H

#include <GeoModelKernel/GeoDefinitions.h>

class GeoMaterial;
class GeoVPhysVol;

namespace GeoModelTools {

    /**
    @class GeoMaterialHelper
    
    A helper class that browses the GeoModel tree for material estimates
        
    @author sarka.todorova@cern.ch
    */

    typedef  std::pair< const GeoMaterial*, double >  materialComponent;
    
    class GeoMaterialHelper {
      
      public:
        /** Default constructor*/
        GeoMaterialHelper()
       {}

        /** Destructor*/
        virtual ~GeoMaterialHelper(){}
    
        /** Evaluate mass ( of a GeoModel tree/branch/volume ) */
        float evaluateMass(const GeoVPhysVol* gv, bool inclusive = true) const;

        /** Collect and blend material : return blended material (the client takes the ownership) and effective volume */
	std::pair< const GeoMaterial* , double > collectMaterial(const GeoVPhysVol* gv) const;

      private:
	/** Internal recursive loop over material components */
	void collectMaterialContent( const GeoVPhysVol* gv, std::vector< GeoModelTools::materialComponent >& materialContent ) const; 
   };
 

} // end of namespace GeoModelTools

#endif


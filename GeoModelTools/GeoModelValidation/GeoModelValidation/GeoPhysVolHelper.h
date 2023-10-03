/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

///////////////////////////////////////////////////////////////////
// GeoPhysVolHelper.h, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////

#ifndef GEOPHYSVOLHELPER_H
#define GEOPHYSVOLHELPER_H

#include <GeoModelKernel/GeoDefinitions.h>

class GeoVPhysVol;
class GeoPhysVol;
class GeoShape;

namespace GeoModelTools {

  /**
    @class GeoPhysVolHelper
    
    A helper class that browses the GeoModel tree for validation purposes
        
    @author sarka.todorova@cern.ch
    */
    
    class GeoPhysVolHelper {
      
      public:
        /** Default constructor*/
        GeoPhysVolHelper()
       {}

        /** Destructor*/
        virtual ~GeoPhysVolHelper(){}

	/** Recursive comparison of trees/branches/volumes : 
	    in quiet mode (printFullInfo=False) , returns the indicator of first encountered difference ( 0 if none),
	    in verbose mode (printFullInfo=True), returns the indicator of last encountered difference ( 0 if none) */
        /** TODO : implicitly resolve non-material top level hierarchy */
        /** TODO : make the comparison independent from the naming scheme */
	int compareGeoVolumes( const GeoVPhysVol* gv1, const GeoVPhysVol* gv2, float tolerance, bool printFullInfo = false,  int level=0 , int returnCode=0) const; 
 
	/** save tree/branch/volume into sqlite database file */
        void saveToDb( const GeoVPhysVol* gv, std::string filename); 
	//void saveFullToDb( const GeoVFullPhysVol* gv, std::string filename); 

	/** retrieve tree/branch/volume from sqlite database file */
        GeoPhysVol* retrieveFromDb( std::string filename); 

	/** shape comparison */
	bool compareShapes( const GeoShape* gs1, const GeoShape* gv2, float tolerance ) const; 
    
	/** dumps from GeoModel tree for debugging  */
	void printInfo(const GeoVPhysVol* pv) const;
	void printChildren(const GeoVPhysVol* pv, int level) const;
	void decodeShape(const GeoShape* sh) const;

      private:
        mutable int m_diff;
        /** check of rotation invariance */
	bool identity_check(GeoTrf::RotationMatrix3D rotation, float tol) const;

   };
 
} // end of namespace GeoModelTools

#endif


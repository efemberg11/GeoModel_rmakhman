/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELFUNCSNIPPETS_TRANSFORMSORTER_H
#define GEOMODELFUNCSNIPPETS_TRANSFORMSORTER_H

#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"
#include "GeoModelKernel/Units.h"

#include <memory>


namespace GeoTrf {
      /// @brief: Helper struct to construct sets of Tranform3D or sets of
      ///         smart pointers of Transform3D. Two Transforms are considered to
      ///         be equivalent if their translation difference is less 
      ///         than 0.1 microns in each vector component and if their angles
      ///         coincide within 0.01 degrees.  
      struct TransformSorter {
      
          /// @brief  Compare smart pointers of GeoTransforms
          bool operator()(const std::unique_ptr<Transform3D>& a, 
                          const std::unique_ptr<Transform3D>& b) const;
          bool operator()(const std::shared_ptr<Transform3D>& a,
                          const std::shared_ptr<Transform3D>& b) const;
        
         
          template<class GeoObjType>
          bool operator()(const GeoIntrusivePtr<GeoObjType>& a,
                          const GeoIntrusivePtr<GeoObjType>& b) const {
                return (*this) (a.get(), b.get());
          }
    
          bool operator()(const ::GeoTransform* a,
                          const ::GeoTransform* b) const;
    

          bool operator()(const Transform3D& a, const Transform3D& b) const;
          bool operator()(const RotationMatrix3D&a, const RotationMatrix3D& b) const;

          int compare(const Transform3D&a, const Transform3D& b) const;
          int compare(const RotationMatrix3D&a, const RotationMatrix3D& b) const;

          /// @brief  Compares two N-dimension vectors component wise
          /// @param vecA Vector a to compare
          /// @param vecB Vector b to compare
          /// @return 0 if all components are the same within the tolerance
          ///        -1 if the first differing component of vecA is smaller than the one of vecB
          ///         1 otherwise
          template <int N> int compare(const VectorN<N>& vecA, const VectorN<N>& vecB) const {
              constexpr double transTolerance = 0.1 * GeoModelKernelUnits::micrometer;
              for (int d =0 ; d < N ; ++d) {
                  const double diffValue = vecA[d] - vecB[d];
                  if (std::abs(diffValue) > transTolerance) {
                     return diffValue < 0. ? -1 : 1;
                  }
              }
              return 0;
          }
          template <int N> bool operator()(const VectorN<N>& vecA, const VectorN<N>& vecB) const {
              return compare(vecA, vecB) < 0;
          }
  };
}

#endif
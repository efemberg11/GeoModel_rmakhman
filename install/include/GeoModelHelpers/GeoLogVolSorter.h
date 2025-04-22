/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GeoModelHelpers_GEOLOGVOLSORTER_H
#define GeoModelHelpers_GEOLOGVOLSORTER_H

#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoIntrusivePtr.h"


/*  
 * Helper struct to assess whether two logical volumes are the same allbeit they are 
 * different instances in memory. They're considered as the same if
 *        - Their GeoMaterial pointer is the same instance
 *        - The comparison of their shapes is equivalent
*/
class GeoLogVolSorter {
    public:
        /**
         * Comparison operator instantiated if std::set<> or std::map of 
         * GeoIntrusivePtr< (const) GeoIntrusivePtr> are defined
        */
        template <class LogVolType>
        bool operator()(const GeoIntrusivePtr<LogVolType>& a,
                        const GeoIntrusivePtr<LogVolType>& b) const{
            return (*this) (a.get(), b.get());
        }
        /**
         *  Standard comparison operator between two volume instances. 
         *  Returns true if the compare function returns a value < 0
        */
        bool operator()(const GeoLogVol* a, const GeoLogVol* b) const;
        
        /** @brief Compares two logical volumes
         *        1) Check whether the material pointers are the same 
         *        2) Return the decision of the GeoShapeSorter
         */
        int compare(const GeoLogVol*a, const GeoLogVol* b) const;
};
#endif
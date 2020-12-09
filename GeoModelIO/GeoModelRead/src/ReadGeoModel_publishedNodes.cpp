


// local includes
#include "GeoModelRead/ReadGeoModel.h"

// GeoModelKernel includes
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoAlignableTransform.h"

// C++ includes
#include <map>
#include <stdlib.h> /* exit, EXIT_FAILURE */

namespace GeoModelIO {

    /*
    template <typename T> std::vector<T> ReadGeoModel::getPublishedFPV( std::string publisherName ) 
    {
        std::string keyType = "";
        std::vector<std::vector<std::string>> vecFPVs = m_dbManager->getPublishedFPVTable( publisherName );
        unsigned ii = 0;
        for( auto const record : vecFPVs ) {
            // record[0] is the record's ID in the DB table, we skip that.
            std::string keyStr  = record[1];
            std::string volID   = record[2];
            if(0==ii) keyType   = record[3];
            std::cout << "keyStr: " << keyStr << ", volID: " << volID << ", keyType: " << keyType << std::endl;
            ++ii;

            if ("uint" == keyType) {
                unsigned int key = std::stoul( keyStr );
                std::map<unsigned int, GeoFullPhysVol*> mapFPV;
                return mapFPV;
            } else if ( "int" == keyType ) {
                int key = std::stoi( keyStr );
            } else if ( "string" == keyType ) {
                // OK! it's string already
            } else {
                std::cout << "ERROR! Key type '" << keyType << "' is not currently supported.\n"
                          << "If in doubt, please ask to 'geomodel-developers@cern.ch'.\n"
                          << "Exiting...\n";
                exit(EXIT_FAILURE);
            }
        }
    }
   */ 
    //std::map<unsigned int, GeoAlignableTransform*> ReadGeoModel::getPublishedAXF(std::string publisherName /*optional variable*/) {
    //auto ReadGeoModel::getPublishedAXF(std::string publisherName /*optional variable*/) {
    //}

} // closes GeoModelIO namespace

/* vim: set ft=cpp: */ // VIM modeline options
/*
 * ReadGeoModel.tpp
 *
 * Created on: Sep 8, 2020
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 * The file contains the definitions of the templated functions of the ReadGeoModel class.
 *
 */

#include "GeoModelKernel/GeoFullPhysVol.h"


namespace GeoModelIO {

    template <typename T, class N> std::map<T,N> ReadGeoModel::getPublishedNodes( std::string publisherName /*optional variable*/) 
    {
        std::map<T, N> mapNodes;
        std::string keyType = "";

        std::vector<std::vector<std::string>> vecRecords;
        if constexpr ( std::is_same_v<GeoFullPhysVol*, N> ) {
            vecRecords = m_dbManager->getPublishedFPVTable( publisherName );
        } else if constexpr ( std::is_same_v<GeoAlignableTransform*, N> ) {
            vecRecords = m_dbManager->getPublishedAXFTable( publisherName );
        } else {
            std::cout << "ERROR! The node type '" << typeid(N).name() 
                << "' is not currently supported.\n"
                << "If in doubt, please ask to 'geomodel-developers@cern.ch'.\n"
                << "Exiting...\n";
            exit(EXIT_FAILURE);
        }
        unsigned ii = 0;
        for( auto const &record : vecRecords ) {
            // record[0] is the record's ID in the DB table, we skip that.
            std::string keyStr  = record[1];
            std::string volID   = record[2];
            if(0==ii) keyType   = record[3];//this is the same for all records. TODO: it should be stored in a metadata table
            ++ii;
            N volPtr = nullptr;
            if constexpr ( std::is_same_v<GeoFullPhysVol*, N> ) {
                volPtr = dynamic_cast<GeoFullPhysVol*>( getVPhysVol(std::stoul(volID), 2,1) ); //always table=2, and we should have copyN=1 because FullPhysVols are not sharable 
            } else if constexpr ( std::is_same_v<GeoAlignableTransform*, N> ) {
                volPtr = getBuiltAlignableTransform(std::stoul(volID));
            } else {
                std::cout << "ERROR! The node type '" << typeid(N).name() 
                    << "' is not currently supported.\n"
                    << "If in doubt, please ask to 'geomodel-developers@cern.ch'.\n"
                    << "Exiting...\n";
                exit(EXIT_FAILURE);
            }

            if constexpr ( std::is_same_v<unsigned, T> ) {
                unsigned int key = std::stoul( keyStr );
                mapNodes.insert( {key, volPtr} );
            } 
            else if constexpr ( std::is_same_v<int, T> ) {
                int key = std::stoi( keyStr );
                mapNodes.insert( {key, volPtr} );
            } 
            else if constexpr ( std::is_same_v<std::string, T> ) {
                // OK! key is string already, so we use keyStr.
                mapNodes.insert( {keyStr, volPtr} );
            } 
            else {
                std::cout << "ERROR! Key type '" << keyType << "' is not currently supported.\n"
                          << "If in doubt, please ask to 'geomodel-developers@cern.ch'.\n"
                          << "Exiting...\n";
                exit(EXIT_FAILURE);
            }
        }
        return mapNodes;
    }

} // closes namespace

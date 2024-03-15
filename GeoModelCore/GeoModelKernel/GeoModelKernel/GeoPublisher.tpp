/* vim: set ft=cpp: */ // VIM modeline settings


template<typename Iter> void GeoPublisher::printInsertionStatus(Iter it, bool success)
{   
      std::cout << "GeoModelKernel::GeoPublisher : Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

template<class N, typename T> void GeoPublisher::publishNode(N node,T keyT) 
{
    std::any key = keyT;
    static_assert(std::is_same_v<GeoVFullPhysVol*, N> || std::is_same_v<GeoAlignableTransform*, N>,
                  "ERROR!!! The node type is not currently supported by 'GeoPublisher'.\n If in doubt, please ask to `geomodel-developers@cern.ch'.\n");
    if constexpr (std::is_same_v<GeoVFullPhysVol*, N>) {
        const auto [iter, success] = m_publishedFPV.insert( {node, key} );
        if(!success) printInsertionStatus(iter, success);
    } else if constexpr (std::is_same_v<GeoAlignableTransform*, N>) {
        const auto [iter, success] = m_publishedAXF.insert( {node, key} );
        if(!success) printInsertionStatus(iter, success);
    }
}



/*
  Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
 */
#include "GeoModelKernel/Query.h"
#include "GeoModelKernel/throwExcept.h"

#include <stdlib.h>
#include <iostream>


#define RETURN_FAILURE(MSG) {                                     \
    std::cerr<<__FILE__<<":"<<__LINE__<<" -- "<<MSG<<std::endl;   \
    return EXIT_FAILURE;                                          \
}
int main() {

    {
        /// Test that the default constructor generates an invalid query
        Query<int> emptyQuery{};
        if (emptyQuery.isValid()){
            RETURN_FAILURE("Empty query is not supposed to be valid." );
        }
    }
    {
        /// Test that the nullopt constructor does what it's supposed to do
        Query<int>emptyQuery{std::nullopt};
        if (emptyQuery.isValid()){
            RETURN_FAILURE("std::nullopt query is not supposed to be valid." );
        }
    }

    Query<int> initQuery{5};
    if (!initQuery.isValid()){
        RETURN_FAILURE("Query is supposed to be valid... And have value of 5." );
    }
    if (initQuery != 5) {
        RETURN_FAILURE("Query is valid but has not 5 but instead "<<initQuery);
    }
    if (initQuery != 5) {
        RETURN_FAILURE("Query is valid but has not 5 but instead "<<initQuery);
    }
    Query<int> copyQuery{initQuery};
    if (!copyQuery.isValid()){
        RETURN_FAILURE("The copy query is constructed from another valid one but is invalid...");
    }
    if (copyQuery.value_or(-9999) != initQuery.value_or(9999)){
        RETURN_FAILURE("The copy query stores "<<copyQuery<<" while the other one has "<<initQuery);
    }
    copyQuery = std::nullopt;
    if (copyQuery.isValid()){
        RETURN_FAILURE("After setting back to nullopt it's supposed to be invalid." );
    }
    copyQuery = 7;
    if (!copyQuery.isValid()){
        RETURN_FAILURE("Expect the query to be valid." );
    }
    if (copyQuery != 7) {
        RETURN_FAILURE("Expect the query to be 7 and not "<<copyQuery); 
    }
    copyQuery = std::move(initQuery);
    if (copyQuery != 5) {
        RETURN_FAILURE("Expect the query to be 5 and not "<<copyQuery); 
    }
    /// Apparently, the move on an optional does not invalidate the optional (https://stackoverflow.com/questions/51805059/why-does-moving-stdoptional-not-reset-state)
    ///if (initQuery.isValid()) {
    ///    RETURN_FAILURE("Initial query needs to be invalid and not "<<initQuery); 
    ///}
    initQuery = copyQuery;
    if (!initQuery.isValid()) {
        RETURN_FAILURE("Initial query needs to be valid again"); 
    }
    if (copyQuery.value_or(-9999) != initQuery.value_or(9999)){
        RETURN_FAILURE("The copy query stores "<<copyQuery<<" while the other one has "<<initQuery);
    }
    std::cout<<__FILE__<<": - Test is passed. "<<std::endl;
    return EXIT_SUCCESS;
}


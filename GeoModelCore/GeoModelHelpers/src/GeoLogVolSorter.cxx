/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/GeoLogVolSorter.h"
#include "GeoModelHelpers/throwExcept.h"
#include "GeoModelHelpers/GeoShapeSorter.h"

bool GeoLogVolSorter::operator()(const GeoLogVol* a, const GeoLogVol* b) const{
    return compare(a, b) < 0;
}
int GeoLogVolSorter::compare(const GeoLogVol*a, const GeoLogVol* b) const{
    if (!a || !b) {
        THROW_EXCEPTION("Nullptr given to the comparator");
    }
    if (a->getMaterial() != b->getMaterial()) {
        return a->getMaterial()->getName() < b->getMaterial()->getName() ? -1 : 1;
    }
    static const GeoShapeSorter shapeSorter{};
    return shapeSorter.compare(a->getShape(), b->getShape());
}

/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelXml/Element2ShapeItem.h"
#include <mutex>

namespace{
    std::mutex s_mutex{};
}

bool Element2ShapeItem::s_applyDepluication = false;
std::set<Element2ShapeItem*> Element2ShapeItem::s_instances{};

Element2ShapeItem::Element2ShapeItem() {
    std::lock_guard guard{s_mutex};
    s_instances.insert(this);
    setShapeDeduplication(s_applyDepluication);
}
Element2ShapeItem::~Element2ShapeItem() {
    std::lock_guard guard{s_mutex};
    s_instances.erase(this);
}
void Element2ShapeItem::deduplicateShapes(const bool enable) {
    std::lock_guard guard{s_mutex};
    s_applyDepluication = enable;
    for (Element2ShapeItem* item : s_instances) {
        item->setShapeDeduplication(enable);
    }
}

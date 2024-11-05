// Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration


#include <GeoModelKernel/GeoFullPhysVol.h>
#include <GeoModelKernel/GeoTransform.h>
#include <GeoModelKernel/GeoPhysVol.h>
#include <GeoModelKernel/GeoBox.h>
#include <iostream>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"

#define PRINT(OBJ)							\
    std::cout<<__FILE__<<":"<<__LINE__<<" Print reference count of "<<(RCBase*)OBJ;   \
    if (OBJ) std::cout<<" "<<typeid(*OBJ).name()<<" "<<OBJ->refCount();               \
    std::cout<<std::endl;                                                             


#define CHECKCOUNT(OBJ, EXPECT)                                                                   \
    if (OBJ->refCount() != EXPECT)  {                                                             \
         PRINT(OBJ);                                                                              \
         std::cerr<<__FILE__<<":"<<__LINE__<<" Expect a reference count of "<<EXPECT<<std::endl;  \
         return EXIT_FAILURE;                                                                     \
    }                                                                                             \
    PRINT(OBJ);                                                                                   \
    std::cout<<__FILE__<<":"<<__LINE__<<" Pass ref counter check of "<<EXPECT<<std::endl;         \

int main(int argc, char *argv[]){
    GeoMaterial* material = new GeoMaterial("Snow", 1.45);
    GeoBox* uniCave = new GeoBox(100., 100., 100.);
    /// Create the logical volume forklift which should increase the counters by one unit
    GeoLogVol* logVol = new GeoLogVol("Forklift", uniCave, material);

    CHECKCOUNT(material, 1);
    CHECKCOUNT(uniCave, 1);

    /// The logical volume should be also incremented by one unit then
    GeoIntrusivePtr<GeoFullPhysVol> myPhysVol{new GeoFullPhysVol(logVol)};
    CHECKCOUNT(myPhysVol, 1);
    CHECKCOUNT(logVol, 1);    
    /// Temporarilly create a new logical volume pointer
    {
        GeoIntrusivePtr<GeoLogVol> logInt{logVol};
        CHECKCOUNT(logInt, 2);        
    }
    /// Another check that the logical volume is back to 1
    CHECKCOUNT(logVol, 1);
    {
        /// Derived constructor from the mother
        GeoIntrusivePtr<GeoVFullPhysVol> physVol2{myPhysVol};
        CHECKCOUNT(physVol2, 2);
       
        /// Test move constructor
        GeoIntrusivePtr<GeoVPhysVol> physVol3{std::move(physVol2)};
        CHECKCOUNT(physVol3, 2);       
        /// Test the copy assignment
        GeoIntrusivePtr<GeoVPhysVol> physVol4{};
        physVol4 = physVol3;
        CHECKCOUNT(physVol4, 3);
        /// Reset
        physVol4.reset();
        // We should be back to 2
        CHECKCOUNT(physVol3, 2);
        /// Move assignment
        physVol4 = std::move(physVol3);
        CHECKCOUNT(physVol4, 2);
    }

    GeoIntrusivePtr<GeoPhysVol> world{new GeoPhysVol(nullptr)};
    world->add(new GeoTransform(GeoTrf::Translate3D(0.,0.,0 )));
    {
        GeoIntrusivePtr<GeoFullPhysVol> childVol{new GeoFullPhysVol(nullptr)};
        world->add(childVol);
        CHECKCOUNT(world, 1);
        CHECKCOUNT(childVol, 2);
        CHECKCOUNT(world, 1);
        childVol->getAbsoluteTransform();
        CHECKCOUNT(childVol, 2);        
        CHECKCOUNT(world, 1);
        childVol->getX();
        CHECKCOUNT(world, 1);
        childVol->getDefX();
        CHECKCOUNT(world, 1);
        childVol->getDefAbsoluteTransform();
        CHECKCOUNT(world, 1);
    }
    {
        GeoIntrusivePtr<GeoPhysVol> childVol{new GeoPhysVol(nullptr)};
        world->add(childVol);
        CHECKCOUNT(world, 1);
        CHECKCOUNT(childVol, 2);
        CHECKCOUNT(world, 1);
        childVol->getX();
        CHECKCOUNT(world, 1);
        childVol->getDefX();
        CHECKCOUNT(world, 1);
    }
    CHECKCOUNT(world, 1);
    
    
    return EXIT_SUCCESS;
}
#pragma clang diagnostic pop

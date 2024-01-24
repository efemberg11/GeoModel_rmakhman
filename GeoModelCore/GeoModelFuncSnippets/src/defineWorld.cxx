/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelFuncSnippets/defineWorld.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoVolumeCursor.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/Units.h"

GeoIntrusivePtr<GeoPhysVol> createGeoWorld(const double worldBoxX, 
                                           const double worldBoxY,
                                           const double worldBoxZ) {
    

    constexpr double gr =   GeoModelKernelUnits::gram;
    constexpr double mole = GeoModelKernelUnits::mole;
    constexpr double cm3 =  GeoModelKernelUnits::cm3;

    // Define the chemical elements
    GeoIntrusivePtr<GeoElement>  Nitrogen{new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0031 *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Oxygen{new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9949 *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Argon{new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.9624  *gr/mole)};
    GeoIntrusivePtr<GeoElement>  Hydrogen{new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00782503081372 *gr/mole)};

    constexpr double densityOfAir=0.001290 *gr/cm3;
    GeoIntrusivePtr<GeoMaterial> air{new GeoMaterial("GeoModelAir", densityOfAir)};
    air->add(Nitrogen  , 0.7494);
    air->add(Oxygen, 0.2369);
    air->add(Argon, 0.0129);
    air->add(Hydrogen, 0.0008);
    air->lock();


    GeoIntrusivePtr<GeoBox> worldBox{new GeoBox(worldBoxX, worldBoxY, worldBoxZ)};
    GeoIntrusivePtr<GeoLogVol> worldLog{new GeoLogVol("WorldLog", worldBox, air)};
    GeoIntrusivePtr<GeoPhysVol> world{new GeoPhysVol(worldLog)};

    return world;    
}

GeoIntrusivePtr<GeoPhysVol> resizeGeoWorld(GeoIntrusivePtr<GeoPhysVol> world)
{
    if (world)
    {
        //resize the world volume to the real needed volume
        
        // get number of children volumes
        unsigned int nChild=world->getNChildVols();
        
        //Dimensions of the bounding boxes
        double xmin=0.,ymin=0.,zmin=0.,xmax=0.,ymax=0.,zmax=0.;
        double xworld=0.,yworld=0.,zworld=0.;
        
        // loop over all children volumes
        for (unsigned int i=0; i<nChild; i++)
        {
            PVConstLink nodeLink = world->getChildVol(i);
            if ( dynamic_cast<const GeoVPhysVol*>( &(*( nodeLink ))) )
            {
                const GeoVPhysVol *childVolV = &(*( nodeLink ));
                
                if ( dynamic_cast<const GeoPhysVol*>(childVolV) ) {
                    const GeoPhysVol* childVol = dynamic_cast<const GeoPhysVol*>(childVolV);
                    childVol->getLogVol()->getShape()->extent(xmin, ymin, zmin, xmax, ymax, zmax);
                }
                else if ( dynamic_cast<const GeoFullPhysVol*>(childVolV) ) {
                    const GeoFullPhysVol* childVol = dynamic_cast<const GeoFullPhysVol*>(childVolV);
                    childVol->getLogVol()->getShape()->extent(xmin, ymin, zmin, xmax, ymax, zmax);
                   
                }
                xworld=std::max({xworld,std::abs(xmin),std::abs(xmax)});
                yworld=std::max({yworld,std::abs(ymin),std::abs(ymax)});
                zworld=std::max({zworld,std::abs(zmin),std::abs(zmax)});
            }
        }

        GeoIntrusivePtr<GeoPhysVol> resizedWorld{createGeoWorld(xworld, yworld, zworld)};
       
        for (unsigned int i=0; i< world->getNChildNodes(); i++){
            const GeoGraphNode * node = *(world->getChildNode(i));
            
            
            if (dynamic_cast<const GeoVPhysVol*>( node ))
            {
                if ( dynamic_cast<const GeoFullPhysVol*>(node) ){
                    const GeoFullPhysVol* nodeFullPhysVol = dynamic_cast<const GeoFullPhysVol*>(node);
                    resizedWorld->add((GeoGraphNode *)nodeFullPhysVol);
                    
                }
                else  if ( dynamic_cast<const GeoPhysVol*>(node) ){
                    const GeoPhysVol* nodePhysVol = dynamic_cast<const GeoPhysVol*>(node);
                    resizedWorld->add((GeoGraphNode *)nodePhysVol);
                    
                }
            }
            else if (dynamic_cast<const GeoNameTag*>( node )){
                const GeoNameTag* nodeTag = dynamic_cast<const GeoNameTag*>(node);
                resizedWorld->add((GeoGraphNode *)nodeTag);
                
            }
            
            else if (dynamic_cast<const GeoAlignableTransform*>( node )){
                const GeoAlignableTransform* nodeAT = dynamic_cast<const GeoAlignableTransform*>(node);
                resizedWorld->add((GeoGraphNode *)nodeAT);
                
            }
            
            else if (dynamic_cast<const GeoSerialTransformer*>( node )){
                const GeoSerialTransformer* nodeST = dynamic_cast<const GeoSerialTransformer*>(node);
                resizedWorld->add((GeoGraphNode *)nodeST);
                
            }
            
            else if (dynamic_cast<const GeoSerialDenominator*>( node )){
                const GeoSerialDenominator* nodeSD = dynamic_cast<const GeoSerialDenominator*>(node);
                resizedWorld->add((GeoGraphNode *)nodeSD);
                
            }
            else if (dynamic_cast<const GeoTransform*>( node )){
                const GeoTransform* nodeTransform = dynamic_cast<const GeoTransform*>(node);
                resizedWorld->add((GeoGraphNode *)nodeTransform);
                
            }else if (dynamic_cast<const GeoIdentifierTag*>( node )){
                const GeoIdentifierTag* nodeIT = dynamic_cast<const GeoIdentifierTag*>(node);
                resizedWorld->add((GeoGraphNode *)nodeIT);
                
            }
        }

        return resizedWorld;
        
    }
    return world;
}

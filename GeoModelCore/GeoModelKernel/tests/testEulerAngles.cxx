// Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration



#include "GeoModelKernel/Units.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include <cstdlib>
#include <iostream>

bool isIdentity(const GeoTrf::RotationMatrix3D& mat) {
    for (int i = 0; i < 3; ++i) {
        if (std::abs(GeoTrf::Vector3D::Unit(i).dot(mat*GeoTrf::Vector3D::Unit(i)) - 1 ) > std::numeric_limits<float>::epsilon()){
            return false;
        }
    }
    return true;
}

constexpr double toDeg = 1. / GeoModelKernelUnits::deg;
int main() {
    for (int alpha = 0; alpha < 360 ; alpha+=15) {
        GeoTrf::GeoRotation rotX{alpha*GeoModelKernelUnits::deg, 0. ,0.};
        if (std::abs(rotX.determinant() - 1) > std::numeric_limits<float>::epsilon()) {
            std::cerr<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the z-axis with angle "<<alpha<<" is not a rotation "<<std::endl;
            return EXIT_FAILURE;
        }
        std::cout<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the z-axis with angle "<<alpha<<" "<<std::endl<<rotX<<std::endl;


        GeoTrf::GeoRotation rotY{0., alpha*GeoModelKernelUnits::deg, 0.};
        if (std::abs(rotY.determinant() - 1) > std::numeric_limits<float>::epsilon()) {
            std::cerr<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the x-axis with angle "<<alpha<<" is not a rotation "<<std::endl;
            return EXIT_FAILURE;
        }
        std::cout<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the x-axis with angle "<<alpha<<" "<<std::endl<<rotY<<std::endl;
        

        GeoTrf::GeoRotation rotZ{0., 0., alpha*GeoModelKernelUnits::deg};
        if (std::abs(rotZ.determinant() - 1) > std::numeric_limits<float>::epsilon()) {
            std::cerr<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the z-axis with angle "<<alpha<<" is not a rotation "<<std::endl;
            return EXIT_FAILURE;
        }    
        std::cout<<"testEulerAngles() "<<__LINE__<<" The GeoTrf::GeoRotation around the z-axis with angle "<<alpha<<" "<<std::endl<<rotZ<<std::endl;
    }
    
    for (int psi = 0; psi <= 360 ; psi+= 15) {
        for (int phi = 0 ; phi <= 360; phi+= 15) {
            for (int theta = 0; theta<= 360; theta+= 15) {
                GeoTrf::GeoRotation rot(phi * GeoModelKernelUnits::deg,
                                        theta*GeoModelKernelUnits::deg,
                                        psi *GeoModelKernelUnits::deg);
                if (std::abs(rot.determinant() - 1.) > std::numeric_limits<float>::epsilon()) {
                    std::cerr<<"testEulerAngles() "<<__LINE__<<" The rotation angles psi: "<<psi<<", phi: "<<phi<<", theta: "<<theta
                            <<" plugged into the GeoTrf::GeoRotation do not result into a rotation matrix "<<std::endl<<rot<<
                            std::endl<< GeoTrf::get3DRotMatZ(psi *GeoModelKernelUnits::deg) *  
                                        GeoTrf::get3DRotMatX(theta *GeoModelKernelUnits::deg) * 
                                        GeoTrf::get3DRotMatZ(phi *GeoModelKernelUnits::deg) <<std::endl;
                    return EXIT_FAILURE;
                }
                GeoTrf::EulerAngles angles = GeoTrf::getGeoRotationAngles(rot);
                GeoTrf::GeoRotation angleRot{angles};
                if (!isIdentity(angleRot.inverse()* rot)) {
                    std::cout<<"testEulerAngles() "<<__LINE__<<" Injected rotation angles phi: "<<phi<<", theta: "<<theta<<" psi: "<<psi
                              <<std::endl<<rot<<std::endl;
                    std::cout<<"testEulerAngles() "<<__LINE__<<" Extracted rotation angles  "<<angles.phi*toDeg<<"/"
                             <<angles.theta*toDeg<<"/"<<angles.psi*toDeg<<std::endl<<angleRot<<std::endl;
                    return EXIT_FAILURE;
                }
                
                const GeoTrf::CoordEulerAngles coordAngles{phi*GeoModelKernelUnits::deg, 
                                                           theta*GeoModelKernelUnits::deg, 
                                                           psi*GeoModelKernelUnits::deg};
                const GeoTrf::GeoRotation coordRot{coordAngles};
                if (std::abs(coordRot.determinant() -1.) > std::numeric_limits<float>::epsilon()) {
                        std::cerr<<"testEulerAngles() "<<__LINE__<<" The rotation angles alpha: "<<psi<<", beta: "<<phi<<", gamma: "<<theta
                                <<" plugged into the GeoTrf::GeoRotation do not result into a rotation matrix "<<std::endl<<coordRot<<std::endl<<
                                 GeoTrf::get3DRotMatZ(coordAngles.gamma *GeoModelKernelUnits::deg) *  
                                 GeoTrf::get3DRotMatY(coordAngles.beta *GeoModelKernelUnits::deg) * 
                                 GeoTrf::get3DRotMatX(coordAngles.alpha *GeoModelKernelUnits::deg)<<std::endl;

                }
                if (!isIdentity(coordRot * GeoTrf::GeoRotation{coordAngles.inverse()})){
                    std::cout<<"testEulerAngles() "<<__LINE__<<"The inverse of "<<coordAngles<<" "
                             <<coordAngles.inverse()<<" does not lead to Identity rotation "<<std::endl;
                    return EXIT_FAILURE;
                }

                const GeoTrf::CoordEulerAngles calcCoordAngles = GeoTrf::getCoordRotationAngles(coordRot);
                const GeoTrf::GeoRotation extCoordRot{calcCoordAngles};
                if (!isIdentity(extCoordRot.inverse()* coordRot)) {
                    std::cout<<"testEulerAngles() "<<__LINE__<<" Injected rotation angles alpha: "<<coordAngles.alpha*toDeg
                              <<", beta: "<<coordAngles.beta*toDeg<<" gamma: "<<coordAngles.gamma*toDeg
                              <<std::endl<<coordRot<<std::endl;
                    std::cout<<"testEulerAngles() "<<__LINE__<<" Extracted rotation angles  "<<calcCoordAngles.alpha*toDeg<<"/"
                             <<calcCoordAngles.beta*toDeg<<"/"<<calcCoordAngles.gamma*toDeg<<std::endl<<extCoordRot<<std::endl;
                    return EXIT_FAILURE;
                }
                if (!isIdentity(extCoordRot * GeoTrf::GeoRotation{calcCoordAngles.inverse()})){
                    std::cout<<"testEulerAngles() "<<__LINE__<<"The inverse of "<<calcCoordAngles<<" "
                             <<calcCoordAngles.inverse()<<" does not lead to Identity rotation "<<std::endl;
                    return EXIT_FAILURE;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
#!/bin/bash
mkdir -p import
cd import

git init
git config core.sparsecheckout true

echo Control/CxxUtils/CxxUtils/features.h                                                                                   >  .git/info/sparse-checkout
echo Control/CxxUtils/CxxUtils/restrict.h                                                                                  >> .git/info/sparse-checkout
echo Control/CxxUtils/CxxUtils/sincos.h                                                                                    >> .git/info/sparse-checkout
echo Control/CxxUtils/CxxUtils/vec.h                                                                                       >> .git/info/sparse-checkout


echo DetectorDescription/GeoModel/GeoSpecialShapes/GeoSpecialShapes/EMECData.h                                             >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/GeoSpecialShapes/toEMECData.h                                           >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/GeoSpecialShapes/LArWheelCalculatorEnums.h                              >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/GeoSpecialShapes/LArWheelCalculator.h                                   >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/GeoSpecialShapes/vec_parametrized_sincos.h                              >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/PortableMsgStream/PortableMsgStream.h                                   >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator.cxx                                              >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculatorGeometry.cxx                                      >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorFactory.cxx               >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOff.cxx            >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOn.cxx             >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/FanCalculatorFactory.cxx                    >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/ModuleFanCalculator.cxx                     >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/sincos_poly.cxx                             >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorFactory.h                 >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOff.h              >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/DistanceCalculatorSaggingOn.h               >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/FanCalculatorFactory.h                      >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/ModuleFanCalculator.h                       >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/WheelFanCalculator.h                        >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/IFanCalculator.h                            >> .git/info/sparse-checkout
echo DetectorDescription/GeoModel/GeoSpecialShapes/src/LArWheelCalculator_Impl/IDistanceCalculator.h                       >> .git/info/sparse-checkout



echo Simulation/G4Utilities/Geo2G4/src/LArFanSection.cxx                                                                   >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSliceSolid.cxx                                                              >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSliceSolidDisToIn.cxx                                                       >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSliceSolidDisToOut.cxx                                                      >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSliceSolidInit.cxx                                                          >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolid.cxx                                                                   >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolidDisToIn.cxx                                                            >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolidDisToOut.cxx                                                           >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolidInit.cxx                                                               >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/G4ShiftedCone.cxx                                                                   >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArFanSection.h                                                                     >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSliceSolid.h                                                                >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolid_type.h                                                                >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/LArWheelSolid.h                                                                     >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/G4ShiftedCone.h                                                                     >> .git/info/sparse-checkout
echo Simulation/G4Utilities/Geo2G4/src/G4ShiftedCone.icc                                                                   >> .git/info/sparse-checkout


git remote add -f origin https://gitlab.cern.ch/atlas/athena.git
git pull origin master
rm -rf .git






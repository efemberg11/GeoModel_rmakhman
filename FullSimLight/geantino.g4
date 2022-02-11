########################################################################
##  SET ALL VERBOSE LEVELS TO 0
## ---------------------------------------------------------------------
/control/verbose 0
/run/verbose 0
/event/verbose 0
/tracking/verbose 0
########################################################################
##  SET NUMBER OF THREADS (will be ignored in case of sequential Geant4)
## ---------------------------------------------------------------------
/run/numberOfThreads 1
/control/cout/prefixString G4Worker_
########################################################################
##  MAGNETIC FIELD
## ---------------------------------------------------------------------
/mydet/setField 0 tesla
########################################################################
##  Geantino Maps Configuration
## ---------------------------------------------------------------------
/gmaps/rmin 0 mm
/gmaps/rmax 200 mm
/gmaps/zmin -3000 mm
/gmaps/zmax 3000 mm
/gmaps/xmin -200 mm
/gmaps/xmax 200 mm
/gmaps/ymin -200 mm
/gmaps/ymax 200 mm
/gmaps/etamin -6
/gmaps/etamax 6
/gmaps/volumeslist Pixel SCT TRT
########################################################################
##  INIT
## ---------------------------------------------------------------------
/run/initialize
########################################################################
##  PRIMARY GENERATOR
## ---------------------------------------------------------------------
/gps/verbose 2
/gps/particle geantino
#/gps/pos/type Plane
#/gps/pos/shape Square
#/gps/pos/centre 1 2 1 cm
#/gps/pos/halfx 2 cm
#/gps/pos/halfy 2 cm
/gps/ang/type iso
/gps/energy 10 GeV
########################################################################
##  EXECUTE with 100000 events
## ---------------------------------------------------------------------
/run/beamOn 100000

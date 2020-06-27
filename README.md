
# GeoModel --- A user-friendly C++ Toolkit for HEP Detector Description

GeoModel is a user-friendly C++ Toolkit and Suite for HEP Detector Description with minimal dependencies.

## Dependencies

The philosophy of the whole GeoModel Suite is to keep the external dependencies at the minimum.

Currently, these are the dependencies:

- GeoModel base classes (GeoModelCore, GeoModelIO):
  - Eigen -- a math library
  -

## Build

### Build tree

By default, only the base classes will be built:

- GeoModelCore
  - GeoModelKernel
  - GeoGenericFunctions
- GeoModelIO
  - GeoModelDBManager
  - GeoModelWrite
  - GeoModelRead
  - TFPersistification
- GeoModelTools
  - ExpressionEvaluator
  - GeoModelJSONParser
  - GeoModelXMLParser
  - GMCAT

On request, additional packages, libraries, and tools can be built:

- GeoModelVisualization (`gmex`)
- GeoModelExamples
- GeoModelG4
  - GeoMaterial2G4
  - GeoModel2G4
- FullSimLight

!!! note
    Some of the optional packages bring in additional dependencies, for example libraries to handle 3D graphics and Geant4 for standalone detector simulations.
    In particular, these are the additional dependencies and the packages that bring them:

    - GeoModelVisualization: Qt5, Coin3D, SoQt
    - Examples about the classes handling the GeoModel->Geant4 translation: Geant4
    - GeoModelG4: Geant4

### Build options

While configuring the build, you can enable the build of the optional packages. For example, if you want to build the examples, you can set the variable `GEOMODEL_BUILD_EXAMPLES` to `1` at CMake configuration time, in this way:

```
cmake -DGEOMODEL_BUILD_EXAMPLES=1 ../GeoModel/
```


#### Visualization

```
GEOMODEL_BUILD_VISUALIZATION=1
```

will enable the build of the geometry visualization tool, `GeoModelExplorer`, or `gmex`. This brings in additional dependencies for the GUI and to handle 3D graphics: Qt5, Coin3D, and SoQt.


#### Examples


There are other variables to control the build. In particular:

```
GEOMODEL_BUILD_EXAMPLES=1
```

will enable the build of all the examples, except those requiring Geant4.

```
GEOMODEL_BUILD_EXAMPLES_W_GEANT4=1
```

will enable the build of the examples requiring Geant4 as well.


#### GeoModel --> Geant4 interface

```
GEOMODEL_BUILD_GEOMODELG4=1
```

will build the interface classes which translate GeoModel nodes into Geant4 entities.
This brings in an additional dependency on Geant4.


#### Standalone detector simulation

```
GEOMODEL_BUILD_FULLSIMLIGHT=1
```

will build the Geant4-based application which let users run standalone simulations on a complete detector geometry or on a piece of that. This brings in an additional dependency on Geant4.



!!! note
    When used during the CMake configuration step, all the variables must be prefixed by `-D`, like in the first example of this section.

    For example,

    ```
    cmake -DGEOMODEL_BUILD_VISUALIZATION=1 -DGEOMODEL_BUILD_EXAMPLES=1 ../GeoModel
    ```

    will build the base classes, the visualization tool (`gmex`), and the examples (the ones that not require Geant4).

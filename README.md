# GeoModel --- A user-friendly C++ Toolkit for HEP Detector Description

GeoModel is a user-friendly C++ Toolkit and Suite for HEP Detector Description with minimal dependencies.

## Dependencies

The philosophy of the whole GeoModel Suite is to keep the external dependencies at the minimum.

The core packages only depend on these third-party libraries:

- Eigen3, as the maths library (header-only)
- SQLite, as the underlying format to store detector description data
- fmt, as formatting library (header-only)



## Structure

### Build tree

By default, only the base classes are built:

- GeoModelCore
  - GeoModelKernel
  - GeoGenericFunctions
- GeoModelIO
  - GeoModelDBManager
  - GeoModelWrite
  - GeoModelRead
  - TFPersistification


On request, additional packages, libraries, and tools can be built:

- GeoModelTools
  - ExpressionEvaluator
  - GeoModelJSONParser
  - GeoModelXMLParser
  - GMCAT
- GeoModelVisualization (`gmex`)
- GeoModelExamples
- GeoModelG4
  - GeoMaterial2G4
  - GeoModel2G4
- FullSimLight

!!! note

    Some of the optional packages bring in additional dependencies, for example libraries to handle 3D graphics and Geant4 for standalone detector simulations.
    In particular, these are the additional dependencies and the packages that bring them:

    - GeoModelTools: Xerces-C, nlohmann_json
    - GeoModelVisualization: Qt5, Coin3D, SoQt, nlohmann_json
    - Examples about the classes handling the GeoModel->Geant4 translation: Geant4
    - GeoModelG4: Geant4
    - FullSimLight: Geant4


### Build

For the build instruction, you can refer to the documnentation folder:

`documentation/dev/`

## License

GeoModel is distributed under the [Apache 2.0 license](LICENSE)

### Core Developers and Maintainers

GeoModel is developed and maintained by (in alphabetical order): 

- Marilena Bandieramonte *(University of Pittsburgh, US)*
- Riccardo Maria Bianchi *(University of Pittsburgh, US)*
- Joseph Boudreau *(University of Pittsburgh, US)*
- Andrea Dell'Acqua *(CERN, CH)*
- Vakhtang Tsulaia *(LBNL, US)*


### Contacts

For questions and requests, you can open an "Issue".







# GeoModel - A Detector Description Toolkit for HEP experiments


GeoModel is a **complete toolkit** to write and manage the **detector description** for High Energy Physics experiments.


## Intro

The GeoModel Toolkit offers classes that provide geometrical primitives for describing detectors, as well as a set of tools for writing, accessing, handling, manipulating, dumping, restoring, visualizing, inspecting, and debugging the detector geometry.

GeoModel also offers many **memory-saving techniques** which lets developers to optimally describe their detectors while keeping the memory footprint very low.

GeoModel is a **pure C++ toolkit** with only **minimal and ligthweight external dependencies**.

The GeoModel Toolkit is also **highly modular**: 

you can pick the modules and tools you need, and **you will use only what you really need**, without the fear of introducing extra external potentially heavyweight dependencies when not needed.

## Features

### Geometry Description

A complete set of shapes and geometrical primitives to describe your detector

A set of 'nodes' to build your detector tree

A smart mechanism to handle detector alignment, with multiple alignment costants kept in cache and synchronized

### Visualization 

A complete, standalone visualization tool to interactively check, inspect, debug your detector geometry in an easy way

### GDML support

An exporter to export a GeoModel geometry into GDML

An importer to import a GDML geometry into GeoModel *(work in progress)*

### Standalone Detector Simulation

#### Quick Feedback with a Geant4-based simulation 

An interactive graphical interface to steer a standalone Geant4-based simulation of your detector, to get quick feedback on your detector while you're still working on it

#### Mass calculation

A standalone mass calculator tool, to quickly compute the masses of your detector from the command line

#### Geantino Maps

A standalone tool to generate geantino maps of your newly built detector



----

## External Dependencies

The external dependencies are minimal:

- The [Eigen](https://eigen.tuxfamily.org) math library for the core packages

Additional external dependencies:

- The lighweight [SQLite](https://www.sqlite.org/) file-based database for I/O, when the `I/O` module is used
- The [`nlohmann_json`](https://github.com/nlohmann/json) JSON parser library to import external data, when the `Visualization` and `Tools` modules are used
- The Apache [`Xerces-C`](https://xerces.apache.org/xerces-c/) XML parser library, when the XML interface of the `Tools` module is used
- The open-source ("Community") version of the [Qt graphics framework](https://www.qt.io/) for the GUI, when the `Visualization` module is used
- The [Coin](https://github.com/coin3d/coin) and [SoQt](https://github.com/coin3d/soqt) libraries for 3D graphics, when the `Visualization` module is used
- The [Geant4](https://geant4.web.cern.ch/) detector simulation toolkit, when the standalone detector simulation module `FullSimLight` is used
- The [`fmt`](https://github.com/fmtlib/fmt) formatting library for advanced string formatting, when the `I/O` module is used

----

<i>Last update:</i> 
{% if git.status %}
  <i> {{ git.date.strftime("%b %d, %Y -- %H:%M") or now() }} </i>
{% else %}
  <i> {{ now() }} </i> 
{% endif %}




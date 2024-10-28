

# GeoModel - A Detector Description Toolkit for HEP experiments


GeoModel is a complete toolkit to write and manage detector description software for High Energy Physics experiments.


The GeoModel Toolkit offers C++ classes that provide geometrical primitives for describing detector geometry (e.g., shapes, logical/physical volumes, transformations), as well as a set of tools for writing, accessing, handling, manipulating, dumping, restoring, visualizing, inspecting, and debugging detector geometries.

GeoModel also offers several memory-saving techniques that allow code developers to optimally describe their detectors with minimal memory footprint requirements.

GeoModel is a pure C++ toolkit with only minimal and ligthweight external dependencies.

The GeoModel Toolkit is also highly modular: you can pick the modules and tools you need, and you will use only what you really need, without the fear of introducing extra, potentially heavyweight, dependencies on external libraries.

----

The external dependencies of the GeoModel toolkit are minimal:

- Eigen math library for the core packages
- SQLite and the `fmt` formatting library for I/O
- The `Xerces-C` XML parser and the `nlohmann_json` JSON parser to import external data, when the XML interface is chosen
- The Qt framework, and the Coin3D and SoQt libraries (only when when the visualization module is used)
- Geant4, when the FullSimLight standalone detector simulation tool is used

----

<i>Last update:</i> 
{% if git.status %}
  <i> {{ git.date.strftime("%b %d, %Y -- %H:%M") or now() }} </i>
{% else %}
  <i> {{ now() }} </i> 
{% endif %}




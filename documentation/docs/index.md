

# GeoModel - A Detector Description Toolkit for HEP experiments


GeoModel is a complete toolkit to write and manage the detetcor description for High Energy Physics experiments.


The GeoModel Toolkit offers classes that provide geometrical primitives for describing detectors, as well as a set of tools for writing, accessing, handling, manipulating, dumping, restoring, visualizing, inspecting, and debugging the detector geometry.

GeoModel also offers many memory-saving techniques which lets developers to optimally describe their detectors while keeping the memory footprint very low.

GeoModel is a pure C++ toolkit with only minimal and ligthweight external dependencies.

The GeoModel Toolkit is also highly modular: 

you can pick the modules and tools you need, and you will use only what you really need, without the fear of introducing extra external potentially heavyweight dependencies when not needed.

----

The external dependencies are minimal:

- The Eigen math library for the core packages
- SQLite and the `fmt` formatting library for I/O, when I/O is used
- The `Xerces-C` XML parser and the `nlohmann_json` JSON parser to import external data, when the XML interface is chosen
- The Qt framework for the GUI, when the visualization module is used
- The Coin3D and SoQt libraries for 3D graphics, when the visualization module is used
- Geant4, when the standalone detector simulation tool is used


----

<i>Last update:</i> 
{% if git.status %}
  <i> {{ git.date.strftime("%b %d, %Y -- %H:%M") or now() }} </i>
{% else %}
  <i> {{ now() }} </i> 
{% endif %}




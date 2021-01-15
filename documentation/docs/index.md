

# GeoModel - A Detector Description Toolkit for HEP experiments



The GeoModel Toolkit offers classes that provide geometrical primitives for describing detectors, and a set of tools for accessing, handling, manipulating, dumping, restoring, visualizing, inspecting, and debugging the detector geometry.

The external dependencies are minimal:

- The Eigen math library for the core packages
- The `Xerces-C` XML parser and the `nlohmann_json` JSON parser to import external data
- The Qt framework for the visualization
- The `fmt` formatting library

----

<i>Last update:</i> 
{% if git.status %}
  <i> {{ git.date.strftime("%b %d, %Y -- %H:%M") or now() }} </i>
{% else %}
  <i> {{ now() }} </i> 
{% endif %}




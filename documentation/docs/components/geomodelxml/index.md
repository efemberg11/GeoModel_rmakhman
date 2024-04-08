## Introduction

`GeoModelXML` is a toolkit built on top of `GeoModel`, which allows quick development of detector geometries using structured XML files containing the detector description, built up from a set of standard shapes and operations known about by the toolkit.

While this has limitations with respect to the programmatic approach used by `GeoModel` itself, it allows a clear separation between _data_ decribing the geometry, and _code_ used to construct it from that data. While the programmatic approach to describing geometries - backed by the full capabilities of the C++ language - is unquestionably more powerful, the contraints imposed by `GeoModelXML` can be beneficial for certain development models in terms of ensuring maintainability, and the data/code separation allows for clear versioning strategies.  

{!components/geomodelxml/quickstart.md!}

## Previous documentation

The original `GeoModelXML` manual, which has lots of useful information and details - but is in places rather specific to its initial version as part of the ATLAS experiment software - can be found [here](old.md)
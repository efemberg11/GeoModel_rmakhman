# Gmx2Geo

`Gmx2Geo` is the main top-level class of `GeoModelXML`, and as the name suggests, it is the interface through which `GeoModel` objects are created from the `GeoModelXML` processing of the input XML files. It takes as inputs:

- The XML files to be processed
- A "world" volume into which the geometry tree to be built will be inserted
- a `gmxInterface` to be used for communication with downstream code (or simply debugging)
- flags to steer behvaiour (e.g. whether to read from a file or a string from a database)
- a list of additional `processors` to be run on top of the "standard list" in `GmxUtil` (the main "driver" class)

The primary uses of `Gmx2Geo` are therefore in intergrating `GeoModelXML` in a more general software framework as the method of building `GeoModel` detector description, or in writing plugins which can be used by the other `GeoModel` tools such as `gmcat`, `gmex`, etc. 

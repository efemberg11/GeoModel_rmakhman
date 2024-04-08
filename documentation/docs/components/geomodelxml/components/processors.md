`GeoModelXML` uses `processors` to append specific information that should be associated with an XML element into a vector that gets passed the various steps. The standard list of `processors` applied (in `GmxUtil`) is:

## Standard processors

- *addbranch*: define a volume hierarchy to actually be built
- *logvol/logvolref*: create a `logvol` (or use one that previously been defined)
- *assembly/assemblyref*: create an `assembly` (or use one that has already been defined) 
- *transform*: create a geometrical transformation (rotation, translation, etc)
- *multicopy*: create multiple instances of a given `logvol` or `assembly` according to a given transformation rule to be apllied for each instance
- *replicaX/replicaY/replicaZ/replicaRPhi/replicaXYarrays*: similar to `multicopy` - create multuple instances of a given `logvol` or `assembly` with a defined periodic structure
- *index*: create a set of indices for addressing a volume

More information on the meanings can be found in the sections on [building a geometry](../../buildinggeometry/) and [defining readout](../../readout/).

## Supported shapes

The following shapes are supported by `GeoModeXML`. The names reflect the `Geant4` shapes on which they are based (and may eventually create). 

- simplepolygonbrep
- twistedtrap
- ellipticaltube
- torus
- box
- cons
- generictrap
- para
- pcon
- pgon
- trap
- trd
- tube
- tubs

In addition, the following boolean operations are defined, and can be applied to any of the above shapes, to create new solid volume possibilities.

- intersection
- union
- subtraction

Note that boolean operations can be very CPU intensive to navigate.

To-do: add the arguments for each type?
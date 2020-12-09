
## Material Geometry

Material geometry consists of a set of classes that bears a large resemblance to the material geometry within GEANT4. These classes, however, are designed to take a minimal size in memory. This requirement determines the basic data structure used to hold the data for the geometry description. That structure is a graph of nodes consisting of both volumes and their properties. The tree is built directly and accessed in a way that provides users access to volumes and, simultaneously, to the properties accumulated during graph traversal that apply to the volumes. See the [Actions section](), below.


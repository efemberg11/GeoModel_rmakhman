
## Transformations

Transformations (class `GeoTransform` and `GeoAlignableTransform`) are graph nodes that can be inserted into the geometry graph prior to the insertion of a physical volume.  Transformations in the geometry graph are interpreted as affecting the position of the physical volume that follows.  They do not affect the position of subsequent physical volumes.

No transformations are inserted before a physical volume, the physical volume remains at the center of its mother volume.  

If one transformation is inserted, the physical volume is moved relative to the mother volume according to the transformation (which includes both a rotation and a translation).

If more than one transformation is inserted, then the composition of all transformations is applied to the physical volume.  The *last* transformation to be added (the one closest to the physical volume) is applied *first*.  

Instances of `GeoTransform` and `GeoAlignableTransform` may be shared within a geometry graph.  

The two classes both provide access to a default transformation and to an actual transformation.  The major difference between them is that these transformations may differ for the `GeoAlignableTransform` but not for the `GeoTransform`.  In addition, the misalignments may be set or cleared for `GeoAlignableTransform`.  

The full interface for these classes is shown below.

{!components/kernel/reference/RCBase/GeoGraphNode/GeoTransform/GeoTransform.md!}

{!components/kernel/reference/RCBase/GeoGraphNode/GeoTransform/GeoAlignableTransform.md!}

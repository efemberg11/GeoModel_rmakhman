# Troubleshooting

Some typical errors that can happen, and how to debug and/or solve them. This is an evolving list and may differ depending on the specific version of `GeoModelXML`, as bugs are solved and others revealed.

- **`evaluate: invalid expression. Last good expression was ...`**:

    This error indicates an issue when XML is being parsed. Check for typos or invalid syntax in defining parameters, providing names to `logvolref`, etc.

- **`WARNING: No valid FullPhysicalVolume found` or `WARNING: no GeoAlignableTransform found!`**:

   This error can occur when `alignable` properties are not correctly set by all elements consistently. Normally this needs to be set in three places, 
    - a) a `multicopy` or `transform` which should set `alignable="n"` where `n` is the "level" at which it is alignable (scheme defined by the client code, not `GeoModelXML`, so the user can choose any value they deem appropriate).
    - b) a `transformation` applied within the `multicopy`/`transform` should set `alignable="true"` so that a `GeoAlignableTransform` will be created for this transformation, allowing alignment corrections to be applied.
    - c) the `logvol` or `assembly` to which the above `multicopy`/`transform` and `transformation` are being applied should set `alignable="true"` when it is created, so that a `GeoFullPhysVol` will be created, which can be associated with alignment corrections. 

- **`unable to open external entity ...`**: 
   
   This error can happen when a tool cannot properly resolve relative paths to entities within an XML file. Generating a single file from the separate files as follows can help make a geometry more "portable":
   ```
   xmllint --noent --valid PATH/TO/FILES/MySplitUpGeometry.gmx > MyCombinedGeometry.xml
   ```

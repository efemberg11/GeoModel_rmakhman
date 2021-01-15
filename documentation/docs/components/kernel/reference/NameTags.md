
## Name Tags, Identifier Tags and Serial Denominators.

To minimize the use of memory, name information is not stored within physical volumes.  In the case of a very large and complicated geometry, the need to denominate many millions of volumes is deemed largely unnecessary.  However, certain pieces of the geometry are important and do deserve to be named.  For these pieces, we have created two types of objects:  name tags, and serial denominators.

Each physical volume is associated with a name, and with an absolution name.  The name of the object is a simple string; while the absolute name is a “/” separated string that looks like a Unix directory:  `/WORLD/INNERDET/SCT/BARRELSUPPORT`, for example. When a physical volume is placed into a tree, by default its name is `ANONYMOUS`.

The default name can be modified by placing a name tag into the geometry graph immediately prior to a physical volume.  A name tag is applied only to the physical volume that is inserted after the name tag into the geometry graph; it does not affect subsequent physical volumes.  The class representing a name tag is `GeoNameTag`:


{!components/kernel/reference/RCBase/GeoGraphNode/GeoNameTag.md!}


Another way of modifying the default name is through an object that automatically generates name strings for all subsequent volumes added to a specific physical volume.  
This object is called a `GeoSerialDenominator`:


{!components/kernel/reference/RCBase/GeoGraphNode/GeoSerialDenominator.md!}

The generation of names using `GeoNameTag` and `GeoSerialDenominator` applies also to the virtual volumes when parameterization is used.  If a name tag is used with parameterization, then all of the virtual physical volumes of the parameterization are given the name tag.  In case a serial denominator is used, each parameterized volume is given a name consisting of the base name plus an additional serial number.


In many cases it is useful to assign an identifier, or "serial number", to physical volumes.  One use case is to enable a simulation engine such as GEANT[^NT1] to make a correspondence between a piece of geometry designated as sensitive, and the readout element corresponding to that piece.  For such cases, we provide a GeoIdentifierTag, similar to a GeoNameTag.  It provides a way of labeling physical volumes with an unsigned int.  GeoIdentifierTags, however, apply only to the physical volume that immediately follows the tag in the geometry graph.  Its interface follows:

[^NT1]: https://geant4.web.cern.ch/


{!components/kernel/reference/RCBase/GeoGraphNode/GeoSerialIdentifier.md!}


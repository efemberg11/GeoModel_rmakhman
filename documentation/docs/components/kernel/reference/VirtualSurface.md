
## Virtual Surfaces

### Introduction

Virtual surface classes are light weight classes describing geometrical objects devoid of any material properties. They are introduced for particle tracking and fast simulation purposes. Like `GeoVFullPhysVol`, the `GeoVSurface` class caches its absolute position locally and responds to alignment updates further up the tree. Virtual surface nodes may be parented within *physical volumes*, but they do not have any child nodes of any kind. The node actions are adapted to work with surface classes as well.

{{ imgutils_image_caption('RCBase/GeoVSurfaceShape/SurfaceClassRef.png', 
   alt='The GeoVSurface inheritance', 
   cap='Figure: GeoVSurface inheritance relations.',
   urlFix=False) 
}}

{% include 'components/kernel/reference/RCBase/GeoVSurfaceShape/GeoVSurface.md' %}
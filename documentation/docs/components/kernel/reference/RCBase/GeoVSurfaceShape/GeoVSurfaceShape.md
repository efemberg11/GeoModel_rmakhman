### GeoVSurfaceShape


The abstract class GeoVSurfaceShape defines the interface for subclasses, specifically the `isOnSurface` method. 

```cpp
// === GeoVSurfaceShape ===

   //Constructor:
   GeoVSurfaceShape () = default;

   //Public Methods:
   //Executes a GeoShapeAction on the surface shape
   virtual void exec (GeoShapeAction *action) const = 0;
   //Determines whether the point(Px,Py,Pz) is on the surface, can be overriden by child methods
   virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const = 0;
```

All surface shapes are initialized at X-O-Y plane. In *gmex* when visualized, the side initially facing at +Z direction is in red color, while the side initially facing at -Z direction is in green color. The following subsections show specific examples of virtual surface shapes.
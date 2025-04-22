### GeoVSurface


The class GeoVSurface takes [Virtual Surface Shape](#geovsurfaceshape) as input to the constructor, and is used to embed the shape within the geometry tree.  It provides access to the SurfaceShape and can determine whether a triplet of coordinates lies on and within the surface. 


```cpp
// === GeoVSurface ===

   //Constructor:
   GeoVSurface(const GeoVSurfaceShape *SurfShape);

   //Public Methods:
   const GeoVSurfaceShape* getShape () const
   virtual void exec(GeoNodeAction *action) const;
   bool isOnSurface(const double Px, const double Py, const double Pz) const;
```
For example, if we want to construct a rectangle virtual surface node, we just need to first construct the surface shape and then construct the virtual surface node:

```cpp
   GeoRectSurface* rectangle = new GeoRectSurface(5, 9); // 5 is X half length and 9 is Y half length
   GeoVSurface* surface = new GeoVSurface(rectangle);    // construct a virtual surface node in rectangle shape   
```
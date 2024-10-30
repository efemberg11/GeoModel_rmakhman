### GeoRectSurface


```cpp
// === GeoRectSurface ===

   // Constructor:
   GeoRectSurface (double XHalfLength, double YHalfLength);


   // Public Methods:
   double area() const;
   double getXHalfLength () const;
   double getYHalfLength () const;
   void exec (GeoShapeAction *action) const override final;
   virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const override final;
```

A `GeoRectSurface` represents a rectangle virtual surface initialized at the XOY plane. It is specified by a *half-length* in X-direction and a *half-length* in Y-direction, which are input parameters of the constructor.  Methods `getXHalfLength()`, `getYHalfLength()` return the shape boundaries of the surface. `area()` returns the area.


{{ imgutils_image_caption('RCBase/GeoVSurfaceShape/Rect.png', 
   alt='The GeoRectSurface shape', 
   cap='Figure: Shape boundaries of rectangle surface.',
   urlFix=False) 
}}


### GeoTrapezoidSurface


```cpp
// === GeoTrapezoidSurface ===

  // Constructor:
  GeoTrapezoidSurface (double XHalfLengthMin, double XHalfLengthMax, double YHalfLength);


  // Public Methods:
  double area() const;
  double getXHalfLengthMin () const;
  double getXHalfLengthMax () const;
  double getYHalfLength () const;
  void exec (GeoShapeAction *action) const override final;
  virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const override final;
```

A `GeoTrapezoidSurface` represents a trapezoid virtual surface initialized at XOY plane. It is specified by *X half-length minimum* (or bottom half-length) and *X half-length maximum* (or top half-length) in X-direction and *Y half-length* in Y-direction, which are input parameters of the constructor. The methods `getXHalfLengthMin()`, `getXHalfLengthMax`, and `getYHalfLength()` return the shape boundaries of the surface. `area()` returns the area of the trapezoid.


{{ imgutils_image_caption('RCBase/GeoVSurfaceShape/Trapezoid.png', 
   alt='The GeoTrapezoidSurface shape', 
   cap='Figure: Shape boundaries of trapezoid surface.',
   urlFix=False) 
}}

### GeoDiamondSurface


```cpp
// === GeoDiamondSurface ===

  // Constructor:
  GeoDiamondSurface (double X_bottom_half, double X_mid_half, double X_top_half, double Y_bottom_half, double Y_top_half);


  // Public Methods:
  double area() const;

  double getXbottomHalf () const;
  double getXmidHalf () const;
  double getXtopHalf () const;
  double getYbottomHalf () const;
  double getYtopHalf () const;
  void exec (GeoShapeAction *action) const override final;
  virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const override final;
```

A `GeoDiamondSurface` represents a diamond virtual surface initialized at XOY plane. It is specified by *X half-length bottom*, *X half-length middle*, *X half-length top* in X-direction and *Y half-length bottom*, *Y half-length top* in Y-direction, which are input parameters of the constructor. The public methods `exec` and `isOnSurface` inherits methods from `GeoVSurfaceShape`, methods `getXbottomHalf()`, `getXmidHalf()`, `getXtopHalf()`, `getYbottomHalf()`, and `getYtopHalf()` return the shape boundaries of the surface. `area()` returns the area of the diamond.


{{ imgutils_image_caption('RCBase/GeoVSurfaceShape/Diamond.png', 
   alt='The GeoDiamondSurface shape', 
   cap='Figure: Shape boundaries of diamond surface.',
   urlFix=False) 
}}


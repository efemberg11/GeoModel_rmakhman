### GeoAnnulusSurface


```cpp
// === GeoAnnulusSurface === 

  // Constructor:
  // The default position of the cylinder center is at (0,0,0)
  // Ox, Oy: the deviation of the focusing point from the default position
  // radius_in: the inner radius of the annulus
  // radius_out: the outer radius of the annulus
  // phi: the span angle of the deviated circular sector, when phi = 2*PI, it is an annulus
  GeoAnnulusSurface(double Ox, double Oy, double radius_in, double radius_out, double phi);


  // Public Methods:
  double getOx() const;
  double getOy() const;
  double getRadiusIn() const;
  double getRadiusOut() const;
  double getPhi() const;
  void exec (GeoShapeAction *action) const override final;
  virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const override final;
```

A `GeoAnnulusSurface` represents an annulus (or circular sector) virtual surface initialized at XOY plane. It is specified by *Ox (X-position of deviated center)*, *Oy (Y-position of deviated center)*, *Inner Radius*, *Outer Radius*, and  *Span Angle $\Phi$*, all input parameters of the constructor. The methods `getOx()`, `getOy()`, `getRadiusIn()`, `getRadiusOut()`, and `getPhi()` return the shape boundaries of the surface. When $\phi = 2\pi$, the circular section returns to an annulus.


{{ imgutils_image_caption('RCBase/GeoVSurfaceShape/Annulus.png', 
   alt='The GeoAnnulusSurface shape', 
   cap='Figure: Shape boundaries of annulus surface.',
   urlFix=False) 
}}

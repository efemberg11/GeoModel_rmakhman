
### GeoPcon

```cpp
//=== GeoPcon ===

  // Constructor:
  GeoPcon (double SPhi, double DPhi)

  // Public Methods:
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane)
  double getSPhi() const
  double getDPhi() const
  unsigned int getNPlanes ()
  bool isValid () const
  const double & getZPlane (unsigned int i) const
  const double & getRMinPlane (unsigned int i) const
  const double & getRMaxPlane (unsigned int i) const
```

`GeoPcon` represents a polycone, which is a union of simple cone sections. The polycone subtends a fixed angle in $\phi$ (`DPhi`) beginning at $\phi_0$ (`SPhi`), and is specified at $n$ locations in $z$. At each $z$ location, the inner and outer radius is given.

When the polycone is constructed, only $\phi_0$ and $\phi$ are given; then, the information at each $z$ location is given, one plane at a time, by using the `addPlane()` method.  At least two planes must be given, otherwise the polycone is not valid and methods such as `volume()` will fail and throw an exception.  The `isValid()` method can be used to determine whether the polycone has at least two planes.

!!! note

    A polycone (`GeoPcon`) with exactly two planes is equivalent geometrically to a cone section (`GeoCons`).


<figure>
  <img src="/reference/RCBase/GeoShape/GeoPcon.png" width="400" />
  <figcaption>Figure 2: A GeoPcon object, representing a "polycone", which is a union of cone sections.</figcaption>
</figure>




### GeoPgon

```cpp
//=== GeoPgon ===

  // Constructor:
  GeoPgon (double SPhi, double DPhi, unsigned int NSides)

  // Public Methods:
  double getSPhi() const
  double getDPhi() const
  unsigned int NSides() const
  unsigned int getNPlanes () const
  const double & getZPlane (unsigned int i) const
  const double & getRMinPlane (unsigned int i) const
  const double & getRMaxPlane (unsigned int i) const
  bool isValid () const
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane)
```

`GeoPgon` is similar to a `GeoPcon` (polycone).  Like a `GeoPcon` it subtends a fixed angle in $\phi$ (`dPhi`) beginning at $\phi_0$ (`sPhi`), and is further specified by giving inner and outer radii at $n$ locations in $z$. However the `GeoPgon` object has a polygonal cross section, and the solid angle $\phi$ is segmented into a fixed number of sides. 

The constructor takes $\phi$ , $\phi_0$, and the number of sides in the cross-section as arguments; then, the information at each $z$ location is given, one plane at a time, by using the `addPlane()` method.  At least two planes must be given, otherwise the polygon is not valid and methods such as `volume()` will fail and throw an exception.  The `isValid()` method can be used to determine whether the polygon has at least two planes.

*Note:  Visualization of GeoPgon is on the to-do list.*

<figure>
  <img src="https://dummyimage.com/600x400/eee/aaa" width="300" />
  <figcaption>Figure 4: GeoPgon object, representing a polycone with a polygonal cross section.</figcaption>
</figure>




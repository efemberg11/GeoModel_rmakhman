

### GeoTubs

```cpp
//=== GeoTubs ===

  // Constructor:
  GeoTubs (double RMin, double RMax, double ZHalfLength, double SPhi, double DPhi)

  // Public Methods:
  double getRMin() const
  double getRMax() const
  double getZHalfLength() const
  double getSPhi() const
  double getDPhi() const
```

A `GeoTubs` is a tube section; a tube that subtends some plane angle (less than $360^\circ$) in $\phi$.  The `GeoTubs` is constructed by providing the inner radius (`RMin`), outer radius (`RMax`), *half-length* in $z$ (`ZHalfLength`), as well as the starting $\phi$ and $\Delta \phi$.  

Member functions provide access to these quantities.

<figure>
  <img src="/kernel/reference/RCBase/GeoShape/GeoTubs.png" width="400" />
  <figcaption>Figure 7: A GeoTubs object, representing a tube section.</figcaption>
</figure>



### GeoTrd

```cpp
//=== GeoTrd ===

  // Constructor:
  GeoTrd ( double XHalfLength1, double XHalfLength2,
           double YHalfLength1, double YHalfLength2,
           double ZHalfLength)

  // Public Properties:
  double getXHalfLength1() const
  double getXHalfLength2() const
  double getYHalfLength1() const
  double getYHalfLength2() const
  double getYHalfLength() const
```

A `GeoTrd` is a simple trapezoid.  Two faces at $\pm \Delta z$ are parallel to each other and to the $x-y$ plane, and each centered on the $z$-axis.  At $-\Delta z$ ($+\Delta z$), the *half-length* of the edges parallel to the $x$-axis is `XHalfLength1` (`XHalfLength2`) and the *half-length* of the edges parallel to the $y$-axis is `YHalfLength1` (`YHalfLength2`).  

The constructor fills the object with these values and the accessors return them.


<figure>
  <img src="/kernel/reference/RCBase/GeoShape/GeoTrd.png" width="400" />
  <figcaption>Figure 8: A GeoTrd object, representing a simple trapezoid.</figcaption>
</figure>



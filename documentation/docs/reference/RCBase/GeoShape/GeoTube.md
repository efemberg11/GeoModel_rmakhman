
### GeoTube

```cpp
//=== GeoTube ===

  // Constructor:
  GeoTube (double RMin, double RMax, double ZHalfLength)

  // Public Methods:
  double getRMin() const
  double getRMax() const
  double getZHalfLength() const
```

The `GeoTube` class represents a tube, specified by an inner radius (`RMin`), an outer radius (`RMax`) and a *half-length* in $z$ (`ZHalfLength`).  

The constructor fills these quantities and the accessors return them.


<figure>
  <img src="/reference/RCBase/GeoShape/GeoTube.png" width="400" />
  <figcaption>Figure 2: A GeoTube object, representing a tube or "pipe".</figcaption>
</figure>




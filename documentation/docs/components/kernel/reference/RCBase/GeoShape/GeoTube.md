
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



{{ imgutils_image_caption('RCBase/GeoShape/GeoTube.png', 
   alt='The GeoTube shape', 
   cap='Figure N: A GeoTube object, representing a tube or "pipe".',
   urlFix=False) 
}}





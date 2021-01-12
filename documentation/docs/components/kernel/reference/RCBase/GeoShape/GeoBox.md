
### GeoBox

```cpp
// === GeoBox ===

   //Constructor:
   GeoBox (double XHalfLength, double YHalfLength, double ZHalfLength)

   //Public Methods:
   double getXHalfLength() const
   double getYHalfLength() const
   double getZHalfLength() const
```

The constructor fills the box with the $x$, $y$, and $z$ *half-lengths*, and the accessors return those quantities.


{{ imgutils_image_caption('RCBase/GeoShape/GeoBox.png', 
   alt='The GeoBox shape', 
   cap='Figure 2: A GeoBox object, representing a rectangular prism or “box”.',
   urlFix=False) 
}}





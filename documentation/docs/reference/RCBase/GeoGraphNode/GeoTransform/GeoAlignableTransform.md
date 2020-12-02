

### GeoAlignableTransform


```cpp
//=== GeoAlignableTransform ===

  // Constructor:
  GeoAlignableTransform (const HepTransform3D & Transform)

  // Public Methods:
  void setDelta (const HepTransform3D & Delta)
  void clearDelta ()
     
  // Public Methods from GeoTransform:
  HepTransform3D getTransform () const
  HepTransform3D getDefTransform () const
  const HepTransform3D * getDelta () const

  // Public Methods from GeoGraphNode:
  void exec (GeoNodeAction * action) 
```



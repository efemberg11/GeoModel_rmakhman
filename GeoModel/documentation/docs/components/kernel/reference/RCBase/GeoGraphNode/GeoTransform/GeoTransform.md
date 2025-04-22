

### GeoTransform


```cpp
//=== GeoTransform ===

  // Constructor:
  GeoTransform (const HepTransform3D & Transform)

  // Public Methods:
  HepTransform3D getTransform () const
  HepTransform3D getDefTransform () const
  const HepTransform3D * getDelta () const

  // Public Methods from GeoGraphNode
  void exec (GeoNodeAction * action)
```



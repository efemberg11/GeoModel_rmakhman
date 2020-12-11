

### GeoLogVol


```cpp
//=== GeoLogVol ===

  // Constructor:
  GeoLogVol ( const std::string & Name, 
              const GeoShape * Shape, 
              const GeoMaterial * Material)

  // Public Methods:
  const std::string & getName () const
  const GeoShape * getShape () const
  const GeoMaterial * getMaterial () const
```

A `GeoLogVol` is an agglomeration of a name, a shape, and a material.  These constituents are provided as arguments to the constructor, which increments the reference count of both the material and the shape.  These reference counts are decremented when the `GeoLogVol` is destroyed.

The `GeoLogVol` provides const access to its constituents through the three access methods list above.


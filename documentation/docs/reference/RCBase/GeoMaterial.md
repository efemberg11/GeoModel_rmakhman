### GeoMaterial

```cpp
// GeoMaterial
  Constructor:
    GeoMaterial (const std::string & Name, double Density) const
  Public Methods:
    void add (GeoElement * element, double fraction = 1.0) void add (GeoMaterial * material, double fraction) void lock ()
    std::string getName () const
    double getDensity () const
    unsigned int getID() const
    unsigned int getNumElements () const
    const GeoElement * getElement (unsigned int i) const double getFraction (int i) const
    double getRadLength () const
    double getIntLength () const
    double getDeDxConstant () const
    double getDeDxI0 () const
    double getDeDxMin () const
```

`GeoMaterial` is a class that describes a material, which is a list of elements. It is created “empty”; subsequently, elements are added one-by-one until the material is "locked". When the material is locked, no further editing is possible, and a series of derived quantities (radiation length, interaction length, etc.) is computed for the material.

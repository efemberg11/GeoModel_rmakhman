### GeoElement

```cpp
// GeoElement
  //Constructor:
    GeoElement (const std::string & Name, const std::string & Symbol, double Z, double A)
  //Public Methods:
    std::string getName() const std::string getSymbol() const double getZ() const
    double getA() const
    double getN() const
```

`GeoElement` has a constructor which takes a name, a chemical symbol, an atomic number, and an atomic weight[^gel1]. The public methods provide access to this information. The `getN()` method returns the effective number of nucleons in the material, Z+A.

[^gel1]: The atomic weight should be specified using units such as g/cm3 (also known as CLHEP[^gmat2] units).



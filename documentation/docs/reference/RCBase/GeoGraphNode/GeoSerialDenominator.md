

### GeoSerialDenominator

```cpp
//=== GeoSerialDenominator ===

  // Constructors:
  GeoSerialDenominator (const std::string & BaseName)

  // Public Methods:
  const std::string & getBaseName() const

  // Public Methods from GeoGraphNode
  void exec (GeoNodeAction * action)
```


Its interface is also simple:  one constructor, and one accessor which retrieves the base name.  If the base name is `"BASE`, then the serial denominator generates names like `BASE0`, `BASE1`, `BASE2`, etc.  
This continues until the last child, or until a GeoNameTag is encountered, or until a new `GeoSerialDenominator` is encountered.



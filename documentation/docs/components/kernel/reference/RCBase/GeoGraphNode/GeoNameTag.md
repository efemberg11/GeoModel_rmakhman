
### GeoNameTag

```cpp
//=== GeoNameTag ===

  // Constructors:
  GeoNameTag (const std::string & BaseName)

  // Public Methods:
  const std::string & getName() const

  // Public Methods from GeoGraphNode
  void exec (GeoNodeAction * action)
```


The interface is simple, consisting only of a constructor taking a name-string, and an accessor which retrieves it.


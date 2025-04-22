# GeoModelTools 

Tools and utilities for GeoModel-based detector description projects. This includes an expression evaluator package (`ExpressionEvaluator`, formerly based on CLHEP, now moved to the "Partow's Mathematical Expression Library"), an XML parser (`GeoModelXMLParser`),
a JSON Parser (`GeoModelJSONParser`), a package to build GeoModel objects from an XML input data file (`GeoModelXML`), a tool to produce GeoModel SQLite data files from multiple inputs (`GMCAT`), a tool to audit GeoModel trees (`GMSTATISTICS`) and a tool to convert a GDML geometry representation to GeoModel.

`GeoModelTools` can be built as part of the GeoModel suite, or as a single package (provided that GeoModel's `GeoModelCore` and `GeoModelIO` are installed already on the system). 

Third-party dependencies:

- [Eigen3](http://eigen.tuxfamily.org/)
- [XercesC](https://xerces.apache.org/xerces-c/)
- [nlohmann_json](https://github.com/nlohmann/json)

## Quick installation 

### macOS 

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git 
brew install geomodel-tools 
```

The command above will install the `geomodel` base libraries as well.


### Ubuntu 

_(coming soon...)_

## Build GeoModelTools 

For the build instructions, you can refer to the [documentation folder](/documentation/docs/dev), or to the web-based render at the documentation website: <https://geomodel.web.cern.ch/>

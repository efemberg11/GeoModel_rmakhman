# LArPlugin

LAr plugin builds GeoModel description of the ATLAS Liquid Argon Calorimeter. It uses several XML files as input. A sample set of such XML inputs is located in the `SampleInput` directory. Once the plugin library has been built and installed to some $INSTALL directory, you can run it with `gmex` by following these instructions:

 * Setup runtime for `gmex`
 * `mkdir geomodel-xml`
 * `cp ${path_to_GeoModelPlugins_source_root_dir}/LArPlugin/SampleInput/*.xml geomodel-xml/LAr`
 * `cp ${path_to_GeoModelDataManagers_source_root_dir}/GeoXmlMatManager/SampleInput/*.xml geomodel-xml/Materials`
 * `export GEOMODEL_XML_DIR=$PWD/geomodel-xml`
 * `gmex $INSTALL/lib/libLArGeoPlugin.1.(so,dylib)`

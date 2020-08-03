# LArPlugin

LAr plugin builds GeoModel description of the ATLAS Liquid Argon Calorimeter. It uses several XML files as input. A sample set of such XML inputs is located in the `SampleInput` directory. Once the plugin library has been built and installed to some $INSTALL directory, you can run it with `gmex` by following these instructions:

 * Setup runtime for running `gmex`
 * `export GEOMODEL_XML_DIR=${path_to_the_GeoModelPlugins_root_diractory}/LArPlugin/SampleInput`
 * `gmex $INSTALL/libLArGeoPlugin.1.(so,dylib)`
 

# TRTPlugin

LAr plugin builds GeoModel description of the ATLAS Transition Radiation Tracker. It uses two XML files as input. A sample set of such XML files can be found in the `SampleInput` subdirectory. Once the TRT plugin library has been built and installed into some $INSTALL path, you can run it with `gmex` by following these instructions:

 * Setup runtime for `gmex`
 * `mkdir geomodel-xml`
 * `cp ${path_to_GeoModelPlugins_source_root_dir}/TRTPlugin/SampleInput/*.xml geomodel-xml`
 * `cp ${path_to_GeoModelDataManagers_source_root_dir}/GeoXmlMatManager/SampleInput/*.xml geomodel-xml`
 * `export GEOMODEL_XML_DIR=$PWD/geomodel-xml`
 * `gmex $INSTALL/lib/libTRTGeoPlugin.1.(so,dylib)`

 

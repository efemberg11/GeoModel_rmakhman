# GeoModelStatistics

GeoModelStatistics (`gmstatistics`) is a command line tool that allows to extract information on the memory footprint of your detector description code. It takes one or more plugins (.dylib/.so) as input.

*gmstatistics* has the following parameters: 

``` bash
./gmstatistics [-p] [plugin1.dylib] [plugin2.dylib] 

``` 

## Examples

If you want to analyse the memory occupancy of your plugin (i.e. PixelGeoPlugin) you can type:

```bash
./gmstatistics ../lib/libPixelGeoPlugin.0.dylib 
                    
```

This is the output retrieved by gmstatistics:

```bash
                     GeoLogVol            9998 instances              2239552 bytes
                  GeoTransform            9856 instances              1419264 bytes
                        GeoBox            9835 instances               472080 bytes
                    GeoPhysVol            7941 instances              1905840 bytes
                    GeoNameTag            4904 instances               235392 bytes
              GeoIdentifierTag            4293 instances                68688 bytes
                GeoFullPhysVol            4088 instances              1896832 bytes
         GeoAlignableTransform            2059 instances               494160 bytes
    GeoAlignableTransform[216]            2058 instances                32928 bytes
                 GeoShapeShift            1914 instances               306240 bytes
           GeoShapeSubtraction            1190 instances                57120 bytes
                   GeoMaterial             812 instances               116928 bytes
               GeoMaterial[56]             794 instances                48752 bytes
                       GeoTube             746 instances                35808 bytes
              GeoMaterial[120]             571 instances                25408 bytes
                 GeoShapeUnion             492 instances                23616 bytes
               GeoMaterial[72]             447 instances                35904 bytes
                       GeoTubs             190 instances                12160 bytes
               GeoMaterial[64]             167 instances                 5280 bytes
                 GeoLogVol[32]             132 instances                 4544 bytes
                    GeoElement              97 instances                 7760 bytes
              GeoMaterial[128]              87 instances                 2080 bytes
                GeoPhysVol[48]              71 instances                 2720 bytes
               GeoMaterial[32]              60 instances                 1920 bytes
                GeoPhysVol[40]              49 instances                 3392 bytes
          GeoSimplePolygonBrep              45 instances                 3600 bytes
                       GeoTrap              44 instances                 4928 bytes
              GeoMaterial[136]              36 instances                 2144 bytes
      GeoSimplePolygonBrep[40]              29 instances                  960 bytes
      GeoSimplePolygonBrep[64]              29 instances                  960 bytes
                GeoPhysVol[56]              22 instances                 2560 bytes
                        GeoTrd              20 instances                 1280 bytes
      GeoSimplePolygonBrep[32]              16 instances                 1120 bytes
                       GeoCons              15 instances                 1200 bytes
      GeoSimplePolygonBrep[48]              11 instances                  832 bytes
            GeoFullPhysVol[56]              10 instances                17408 bytes
                       GeoPgon              10 instances                 1280 bytes
      GeoSimplePolygonBrep[56]               9 instances                  576 bytes
                   GeoPgon[48]               9 instances                  144 bytes
                   GeoPgon[56]               9 instances                  144 bytes
                       GeoPara               8 instances                  640 bytes
            GeoFullPhysVol[40]               7 instances                20992 bytes
                  GeoPgon[104]               7 instances                  112 bytes
      GeoSimplePolygonBrep[72]               3 instances                  192 bytes
                   GeoPgon[72]               2 instances                   32 bytes
                   GeoPgon[80]               2 instances                   32 bytes
                GeoNameTag[32]               1 instances                   32 bytes
                GeoElement[56]               1 instances                   16 bytes
                   GeoPgon[96]               1 instances                   16 bytes
Total GeoModel object allocation:  9.51557MB

```

<!--To analyse a GDML geometry and save it in SQLite format with  *gdml2gm* :-->
<!--``` bash-->
<!--./gdml2gm -f myGeometry.gdml -o myGeometry.db-->
<!--``` -->
<!---->
<!--To visualize a GDML geometry with  *gmex*, the user has to set the following environment variable: GDML_FILE_NAME. It has to point to the GDML file that the user wants to plug into *gmex*. Then it is sufficient to call gmex together with the *libGDMLtoGM.dylib/so* library:-->
<!---->
<!--``` bash-->
<!--export GDML_FILE_NAME=<path-to-my-geometry-file>/myGeometry.gdml-->
<!--./gmex ../lib/libGDMLtoGM.dylib   -->
<!--``` -->

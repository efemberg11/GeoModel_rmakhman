
# How to dump the detector description to a file with `dump-geo`

## Use cases - How To

## How to set a specific Geometry tag

You can use `dump-geo` to dump a given version of the ATLAS geometry, by setting a specific geometry tag. For example:

```
dump-geo -detdescr=ATLAS-R3-2021-01-00-00
```

a file called `geometry_ATLAS-R3-2021-01-00-00.db`, containing the geometry configured by the given tag, will be created in the run folder.




## How to use `dump-geo` as a jobOption

You can also use `dump-geo` as an Athena jobOption. For example:

```
# for the NSW symmetric Run3 layout
athena DumpGeo/dump-geo.py -c "DetDescrVersion='ATLAS-R3S-2021-01-00-00‘"

# for the NSW asymmetric Run3 layout.
athena DumpGeo/dump-geo.py -c "DetDescrVersion='ATLAS-R3-2021-01-00-00‘"
```


## How to use a jobOption file with `dump-geo`

You can use `dump-geo` to load a custom jobOption to modify the geometry, then dump that to a local file:

```
dump-geo jobOption.py
```

where the jobOption.py can contain code to modify the geometry; for example:

```
# jobOption.py
from LArGeoAlgsNV.LArGeoAlgsNVConf import LArDetectorToolNV
LArDetectorToolNV.EMECVariantInner="Cone"
LArDetectorToolNV.EMECVariantOuter="Cone"
```

## How to set a specific Muon layout

You can set a specific Muon layout.

```
dump-geo -detdescr=...
```

Then, you can override the layout for muon only:

```
dump-geo -muonLayout=...
```

**Note:** the underlying ATLAS layout should contain the same types of detectors, e.g. when you want to override with a muon layout without any CSCs, you should also use an ATLAS layout without any CSCs in `-detdescr` to set the geometry flags all correct.

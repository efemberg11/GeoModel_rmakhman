When building geometries in `GeoModelXML` it is assumed that at least some parts will be instrumented for readout in order to be be able to function as a particle detector. While most of the modeling of the readout is handled outside of `GeoModel`, it is important that the relevant information about the sensitive elements of the detector is propagated correctly, and that the sensitive volumes are handled correctly in the geometry tree.

## Making volumes sensitive

In order to make a given volume sensitive, a `readout` scheme must first be applied. Since as mentioned the detector readout is handled externally, there are few constraints on what information the reaout scheme can contain; customed-named single parameters or parameter lists can be defined with either numerical or string data. This information will be made avaiable via [`GmxInterface`](../components/gmxinterface/)[^abbr] for external tools, and also propagated to dedicated tables in the SQLite files created by e.g. `gmcat`.

The sensitive detectors can be defined by creating a `readoutgeometry` element. Information can be split over classes and types, by defining one or more `sensorclass` elements with information that is common to all types, and which should contain one or more `sensortype` elements which add additional type-specific information. The desired informtion can be defined like in the example below:
```
<readoutgeometry>
  <sensorclass name="MySensor">
    <param name="ToTBits" value="4"/>
    <param name="is3D" value="0"/>

    <!-- main Quad type -->
    <sensortype name="MySensor1">
      <param name="rows" value="200"/>
      <param name="columns" value="400"/>
      <param name="pitchEta" value="0.050"/>
      <param name="pitchPhi" value="0.050"/>
      <param name="thickness" value="0.1"/>
    </sensortype>

    <sensortype name="MySensor2">
      <param name="rows" value="400"/>
      <param name="columns" value="600"/>
      <param name="pitchEta" value="0.025"/>
      <param name="pitchPhi" value="0.025"/>
      <param name="thickness" value="0.15"/>
    </sensortype>
  </sensorclass>
</readoutgeometry>
```

The `sensitive` property is used to assign a specific readout scheme to a `logvol`:

```
<logvol name="MySensor" shape="MySensorShape" material="SiMetal" sensitive="MySensor1">
```

NB that the `sensitive` property is always applied to `logvols` and not to assemblies. Any `logvol` which has the `sensitive` property set will have an associated `GeoFullPhysVol` rather than `GeoPhysVol` assocated [(see GeoModelKernel for more details)](../../kernel/overview/#physical-volumes-and-the-geometry-graph).

## Identifying sensitive volumes

To identify specific instances of a sensitive volumes, it can be useful to define hierarchical indices to represent that a given sensor is e.g. the 3rd sensor in $\eta$ on the 2nd barrel layer, etc. This can be done by defining a `positionindex` element, and adding various indices which may want to be counted:
```
<positionindex>
  <addindex name="layer"/>
  <addindex name="eta_module"/>
</positionindex>
```
These can be set for a `logvol` or `assembly` via an `index` element directly, if the structure of the geometry allows:

```
<assembly name="Layer1" >
   <index ref="layer" value="2"/>
   [rest of assembly definition]
</assembly>   
```
However, it is frequently the case that such volume's index will not be a priori easily known, due to the use of `multicopy` etc. In which case, automated internal couters of the form `CNL_X` can be used. These are dynamically assigned to the various levels in the geometry hierarchy which are iterated over, and so the number `X`  is the `depth` in the hierarchy. They can be used either directly, or as part of a function, to define the index, and will dynamically evaluate to the current iteration when queried.

```
<assembly name="Layer1" >
   <index ref="eta_module" value="CNL_3"/>
   [multicopy over modules, that is 3rd in hierarchy] 
</assembly>   
```

Setting these indices correctly can be "fiddly" if there are strong external constraints on the ranges/values, and so one should be aware that quite some trial and error may be involved in getting them correct.

## Splitting volumes into multiple readout elements

Occasionally it may be beneficial to split a single `GeoModel` volume into multiple readout elements downstream. This can be achieved by setting the `splitLevel` property of the `logvol`, which can then produce multiple readout instances which share the same `GeoFullPhysVol`. Whenever the `splitlevel` property is encountered, it will call `addSplitSensor` (rather than `addSensor`) for each of the `n` elements into which the single volume should be split, and propagate additional information that can be used to set the indices correctly following the splitting.

*Currently the `splitLevel` assumes that the volumes should be split in the $\eta$ coordinate, and so the indices to be adjusted are assumed to the `eta_module`. Some small code developments would be required to allow this to be fully configurable.*

[^abbr]: these can be accessed via helper functions like `getParameter`/`getParameters` (for single values and vectors respectively)  
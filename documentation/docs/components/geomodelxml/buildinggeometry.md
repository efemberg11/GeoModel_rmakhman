# Building a Geometry

## Creating a volume

When building a geometry, we are generally considering a set of `volumes` positioned in a 3D space. We define these by creating a `logvol`:

```
<logvol name="MyLogVol" shape="MyShape" material="MyMaterial"/>
```

a `logvol` should have a `name` (names are generally needed for identifying elements and referring to them elsewhere in the code), a `shape` defining its extent in space, and a `material` which specificies its composition and density.

A `shape` to be used in this `logvol` can be defined as follows:

```
<shapes>
  <!-other shapes can be defined here too-->
  <box name=“MyShape” xhalflength=“100.” yhalflength=“100.” zhalflength=“500.” />
<\shapes>
```
This will create a `shape` of type `box`, which is defined by 3 parameters: its half-length in `x`,`y`, and `z`, set to 100, 100, and 500 mm, respecively. A full list of supported shapes can be found [here](../components/processors/#supported-shapes).

A `material` to be used in the `logvol` can be defined as follows, building up chemical molecules from elements, to make materials which can themselves be mixed in different fractions to create new materials:

```
<materials>
<!- a more complicated definition to show some possibilities ->
  <element name="Carbon" shortname="C" Z="6" A=“12.011"/>
  <element name="Argon" shortname="N" Z="18" A="39.72"/>
  <element name="Oxygen" shortname="O" Z="8" A=“15.9994"/>
  <chemical name="CO2">
    <elemcontent ref="Carbon" quantity="1"/>
    <elemcontent ref="Oxygen" quantity="2"/>
  </chemical>
  <material name="CO2Gas" density="1.98e-3">
    <chemicalref fraction="1" ref="CO2"/>
  </material>
  <material name="ArgonGas" density="1.789e-3" >
    <elementref fraction="1.00000" ref="Argon"/>
  </material>
  <material name="MyMaterial" density="1.79e-3" >
    <materialref ref="ArgonGas" fraction="0.93" />
    <materialref ref="CO2Gas" fraction="0.07" />
  </material>
</materials>

```

## Placing a volume

By default, volumes are created at the origin of the reference frame. To place a volume within 3D space, it should be placed within a `transform` element and have a `transformation` applied. For example, to place the centre of the box we created at a position 250 mm along the z-axis:

```
<transform>
  <transformation name="MyTransformation">
    <translation z="250">
  </transformation>
  <logvolref ref="MyLogVol">
</transform>
```
Here we use a `logvolref` to refer to a previously defined `logvol`. This can also be done with a `transformation` that has been previously defined (for example to allow re-use), e.g.

```
<transformation name="MyTransformation">
  <translation z="250">
</transformation>

<transform>
  <transformationref ref="MyTransformation">
  <logvolref ref="MyLogVol">
</transform>
```
Multiple operations can be combined in a single `transformation`, e.g. to combine a translation and a rotation:

```
<transformation name="MyTranslateRotate" >
  <translation x=“600." y="600." z="600." />
  <rotation xcos="1/sqrt(3.)" ycos="1/sqrt(3.)" zcos="1/sqrt(3.)" angle="PI/4." />
</transformation>
```
When defining rotations, the parameters are the cosines defining the axis around which the rotation should be performed, and the angle through which to be rotated. The cosines are `0` unless set, and so a rotation about the z-axis can be performed like `<rotation zcos="1.0" angle="PI/4." />`.

## Combining volumes

Since a full detetor will typically consist of very many volumes, some nested within others, it is necessary to be able to combine them. It is possible to simply place one `logvol` inside another (NB they must be full contained and not protruding otherwise this will cause a `clash`):

```
<!- assuming you've defined "MyBigBox", "MySmallBox", "MySmallVolume", ","MyMaterial" already->
<logvol name="MyBigVolume" shape="MyBigBox" material="MyMaterial">
  <logvolref ref="MySmallVolume"/>
</logvol>
```
This can be done for mutliple `logvols`, and can also include a `transformation`, e.g:
```
<!- assuming you've defined "MyBigBox", "MySmallBox", "MySmallVolume", ","MyMaterial" already->
<logvol name="MyBigVolume" shape="MyBigBox" material="MyMaterial">
  <transform>
    <transformation name"MyZShift">
      <translation z="5"/>
    </transformation>
    <logvolref ref="MySmallVolume"/>
  </transform>
  <transform>
    <transformation name"MyNegativeZShift">
      <translation z="-5"/>
    </transformation>
    <logvolref ref="MySmallVolume"/>
  </transform>
</logvol>
```
This generally works very well, but note that the containing `logvol` by definition requires a `shape`. Sometimes this can be inconvenient; e.g. one may need to define a very complicated shape to encompass all the volumes contained, without generating clashes with other volumes. A good solution can be to create an `assembly` which simply binds a set of volumes together, without requiring an "outer" `logvol` and `shape`:

```
<!- assuming you've defined "MyBigBox", "MySmallBox", "MySmallVolume", ","MyMaterial" already->
<assembly name="MyBigVolume">
  <transform>
    <transformation name"MyZShift">
      <translation z="5"/>
    </transformation>
    <logvolref ref="MySmallVolume"/>
  </transform>
  <transform>
    <transformation name"MyNegativeZShift">
      <translation z="-5"/>
    </transformation>
    <logvolref ref="MySmallVolume"/>
  </transform>
</assembly>
```
An `assembly` can consist of a single positioned `logvol` as a convenient way to bind a `transformation` to a volume, and an `assembly` can also contain mutliple other `assemblies` or `assemblyrefs`:

```
<!- assuming you've defined "MyBigBox", "MySmallBox", "MySmallVolume", ","MyMaterial" already->

<assembly name="MySmallVolumePos">
  <transform>
    <transformation name="MyZShift">
      <translation z="5"/>
    </transformation>
    <logvolref ref="MySmallVolume">
  </transform>
</assembly>

<assembly name="MySmallVolumeNeg">
  <transform>
    <transformation name="MyNegativeZShift">
      <translation z="-5"/>
    </transformation>
    <logvolref ref="MySmallVolume">
  </transform>
</assembly>

<assembly name="MyBigVolume">
  <assemblyref ref="MySmallVolumePos"/>
  <assemblyref ref="MySmallVolumeNeg"/>
</assembly>
```

## Defining variables

You may notice that in the previous examples, we have used explicit numerical values for e.g. the magnitude of translations. In general, as stated in the [Good practise guidelines](../#good-practise-guidelines), it is better to always use defined variables where possible. This can be done as follows:

```
<defines>
  <var name=“PI” value=“3.14159265359” />
  <vector name=“digits” value=“0 1 2 3 4 5 6 7 8 9” />
  <matrix name=“ints” coldim=“3” value=“ 1 2 3 4 5 6 7 8 9” />
</defines>
```
These can also use mathematical operations, and other already-defined variables in their definition:

```
<defines>
  <var name="position1" value="1">
  <var name="position2" value="3.5">
  <var name="position3" value="pos1+pos2">
  <var name="position4" value="(pos1+pos2)/(pos2 - pos1)">
</defines>
```
Addressing specific components of vectors (matrices) can be done via an `_N` (`_N_M`) notation:

```
<defines>
  <var name=“PI” value=“3.14159265359” />
  <vector name=“digits” value=“0 1 2 3 4 5 6 7 8 9” />
  <matrix name=“ints” coldim=“3” value=“ 1 2 3 4 5 6 7 8 9” />
</defines>

<defines>
  <var name=“PIby2” value=“PI/digits_2” />
  <var name=“AlsoPIby2” value=“PI/ints_0_1”/>
</defines>
```

## Repeated positionings

Detector geometries are in general built of repeated structures, and so it is of great convenience (nigh-on essential) to be able to place identical volumes in a series different positions, reflecting e.g. detector modules placed at regular z-intervals. This can be achieved through the use of a `multicopy`. The `multicopy` will place `n` instances of the `logvol`/`assembly` it is applied to, applying the same `transformation` each time. For instance, placing a volume 5 times with a regular z-interval of 5mm:

```
<multicopy name="MyVolsAlongZ" n="5">
  <transformation name="MyZShift">
    <translation z="5"/>
  </transformation>
  <logvolref ref="MyLogVol">
</multicopy>
```
It is also possible to use `multicopy` to place volumes with more complex sets of positions via the use of a `loopvar`. The `loopvar` is a vector of values, for which the `multicopy` will use element `_N` for the n$^{th}$ copy:

```
<defines>
  <vector name=“MyZPositions” value=“-11 -10 -5 1 4” />
</defines>

<multicopy name="MyVolsAlongZ" n="5" loopvar="MyZPositions">
  <transformation name="MyZShift">
    <translation z="MyZPositions"/> <!--This will evaluate to MyZPositions_N-->
  </transformation>
  <logvolref ref="MyLogVol">
</multicopy>
```
The `loopvar` variable can also be used in formulae:

```
<defines>
  <vector name=“MyScaleFactors” value=“1 2 3 4 5” />
</defines>

<multicopy name="MyVolsAlongZ" n="5" loopvar="MyScaleFactors">
  <transformation name="MyZShift">
    <translation z="10*MyScaleFactors"/>
  </transformation>
  <logvolref ref="MyLogVol">
</multicopy>
```
**NB** that when using a `multicopy` without a `loopvar`, there is an *implicit* multiplication by 0 ... n in the iterations. When a `loopvar` is defined the periodicity is fully *explicit* and so the following:

```
<defines>
  <vector name=“MyZPositions” value=“0 1 2 3 4” />
</defines>

<multicopy name="MyVolsAlongZ" n="5" loopvar="MyZPositions">
  <transformation name="MyZShift">
    <translation z="5"/>
  </transformation>
  <logvolref ref="MyLogVol">
</multicopy>
```
would result in 5 copies placed at the same position (hence clashing), while the following would produce the same result as using no `loopvar`:

```
<defines>
  <vector name=“MyZPositions” value=“0 1 2 3 4” />
</defines

<multicopy name="MyVolsAlongZ" n="5" loopvar="MyZPositions">
  <transformation name="MyZShift">
    <translation z="5*MyZPositions"/>
  </transformation>
  <logvolref ref="MyLogVol">
</multicopy>
```
*[NS: Should also document the `ReplicaXYArray` etc here, but I've not used these so might be better that someone else does it ;-)]*

## Working with shapes

While the [built-in shapes](../components/processors/#supported-shapes) give considerable options for the type of volumes that can be constructed, some shapes cannot be directly described by the built-in options. In this case `boolean` solids can be useful. In the following, the three types of boolean operations are shown - `union`, `subtraction`, and `intersection`:

```
<shapes>
  <tube name="Cylinder” rmin="100" rmax="200" zhalflength=“1000"/>
  <box name="Box" xhalflength="300" yhalflength="300" zhalflength=“500"/>

   <union name="BoxAndCylinder" >
     <shaperef ref="Box" />
     <transformation name="trivialShift">
       <translation z="0."/>
    </transformation>
    <shaperef ref="Cylinder" />
  </union>  

  <subtraction name="BoxMinusCylinder" >
    <shaperef ref="Box" />
    <transformation name="trivialShift" >
      <translation z="0."/>
    </transformation>
    <shaperef ref="Cylinder" />
  </subtraction>

  <intersection name="BoxIntersectCylinder" >
    <shaperef ref="Box" />
    <transformation name="rotations" >
      <rotation xcos="1" angle="PI/4."/>
      <rotation ycos="1" angle="PI/4."/>
      <rotation zcos="1" angle="PI/4."/>
    </transformation>
    <shaperef ref="Cylinder" />
  </intersection>
</shapes>
```
Note that extensive use of `boolean` operations can lead to high computational overheads, and so their use should be carefully considered (e.g. best avoided for volumes for which very many copies will created).

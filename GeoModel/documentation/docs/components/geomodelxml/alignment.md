Since geometries described with `GeoModel` typically aim to represent a detector which will (or might be) physically constructed in future, it is important to be able to model issues which commonly face detectors once constructed. A significant issue for many detectors is that contruction tolerances and/or effects due to changing temperature, magnetic fields, etc, can result in measurable differences between the real and expected positions of detector elements.

While these effects can be explicitly modeled in the geometry *if known*, it is more typical to include such effects by allowing for *misalignments* to be applied to the geometry, which represent external corrections to be applied to the positions of volumes, having been determined by an *alignment procedure* (typically a minimization of certain parameters with high sensitivity to the positions of sensor elements).

## Making volumes alignable

In `GeoModelXML`, elements are made alignable through using the `alignable` property. This has several functions.

- When creating a `logvol` or `assembly`, this property indicates that a `GeoFullPhysVol`[^abbr1] should be created rather than a `GeoPhysVol`[^abbr2] [(see GeoModelKernel for more details)](../../kernel/overview/#physical-volumes-and-the-geometry-graph).

```
<logvol name="SingleBox" shape="BoxShape" material="Air" alignable="true"/>
```

- When creating a `transformation`, this property indicates that a [`GeoAlignableTransform` should be created rather than a `GeoTransform`](../../kernel/overview/#physical-volumes-and-the-geometry-graph).

```
<transformation name="RotateABox" alignable="true">
  <rotation zcos="1.0" angle="-PI/4"/>
</transformation>
```
Since often it is known when creating a volume that is *should* be aligned, but not necessarily the full picture of how it may later be positioned via `transformations`, it is also possible to define an empty `transformation` to which alignments can be applied (since the final position will be the product of all `transformations` applied):
```
<logvol name="SingleBox" shape="BoxShape" material="Air" alignable="true"/>

<assembly name="AlignableBox">
  <transform>
    <transformation name="DummyTransform" alignable="true">
      <translation/>
    </transformation>
    <logvolref ref="SingleBox"/>
  </transform>
</assembly>
```  

- The final use of `alignable` is a little different; when defining a `transform`, `multicopy`, or `ReplicaArray`, the `alignable` property expects an `int` rather than a `bool`. This is because this use of `alignable` propagates the objects which are to be made alignable to [`GmxInterface`](../components/gmxinterface/), in order that they can be accessed and used externally to `GeoModel`, e.g. within an experiment's simulation or reconstruction framework. The value passed as an `int` represents the alignment "level"; different levels may be treated differently as they can be defined to distinguish e.g. large, medium, and small structures which should be processed one after the other in the alignment procedures of the experiment.

Putting this all together, you should have something that looks like the following:

```
<logvol name="SingleBox" shape="BoxShape" material="Air" alignable="true"/>

<assembly name="AlignableBox">
  <transform alignable="1">
    <transformation name="RotateABox" alignable="true">
      <rotation zcos="1.0" angle="-PI/4"/>
    </transformation>
    <logvolref ref="SingleBox"/>
  </transform>
</assembly>
```
If all three of the points discussed above have been successfully applied, you should see be able to see a `DEBUG` message from `GmxInterface` (if the output verbosity is high enough):

```
GmxInterface::addAlignable called for a physvol. Logvol name [...]
```

## Known issues

Currently alignment with `GeoModelXML` is in a development and testing phase, and so issues are in the process of being discovered and solved. The current known issues are:

- when using `splitLevel` to [split up volumes into multiple readout elements](../readout/#splitting-volumes-into-multiple-readout-elements), only the first of these currently gets a `GeoAlignableTransform` passed on. Since these split volumes should always be physically bound, they should not need alignment relative to each other and so can all use a single `GeoAlignableTransform`. This propagation of a common `GeoAlignableTransform` for split volumes is not yet implemented.
- when defining a `multicopy` without a `loopvar`, `GeoAlignableTransforms` are not being properly created. Until this is fixed, it is suggested that a `loopvar` is used in all cases (can be a ["dummy" which simply implements the *implicit* behaviour](../buildinggeometry/#repeated-positionings)) 
- using the same name for the `bool` and `int` `alignable` properties can be confusing; a new `alignmentLevel` (or similar) property should be defined that can optionally be used in place of the `int` version, and eventually replace it as the standard.

[^abbr1]:Class definition states: *"This class represent a physical volume whose absolute default and aligned positions are known and held in cache."*

[^abbr2]:Class definition states: *"A GeoPhysVol is a physical volume for normal recursive access only. This means, normally, that it represents a piece of detector that nobody cares about except for tracing engines like GEANT"*
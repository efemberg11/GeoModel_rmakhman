


## Materials and Elements

### Introduction


Two classes are used for describing materials: `GeoElement` and `GeoMaterial`.

Elements are declared by specifying a name, chemical symbol, and atomic number and mass; the latter being specified in atomic mass units.

Materials are constructed with a name and density and are not ready-for-use until one or more elements have been added, e.g.:

```cpp
water->add(hydrogen,0.11);
water->add(oxygen,0.89);
water->lock();
```

The `add()` method takes an element and its mass fraction, and is overloaded to accept also a material and its mass fraction. The `lock()` method protects the material against further addition of elements, and re-normalizes the mass fractions so that they sum to unity

The material responds to various queries about its composition, and in addition can return a series of derived quantities of physical interest. The interaction length and radiation length of a material are familiar and are described in the Particle Data Book[^ME1].


Ionization energy loss in materials follows the Bethe-Bloch formula and is governed by two constants, an overall normalization term and the ionization potential, which can be computed from the atomic number and density of the material; the calculation is also described in the Particle Data Book cited above[^ME1]. The calculation does not include small corrections to the energy loss due to chemical binding of elements. These constants are available through the methods `getDeDxConstant()` and `getDeDxI0()`. The method `getDeDxMin()` returns the minimum ionization energy loss and is derived from the other methods.

Both materials and elements are reference-counted; the reference count of an element is incremented when it added to a material and decremented when a referencing material is destroyed; materials are reference counted when they are used in logical volumes and decremented when the referencing logical volume is destroyed.

[^ME1]: http://pdg.lbl.gov/


{!kernel/reference/RCBase/GeoElement.md!}

{!kernel/reference/RCBase/GeoMaterial.md!}




# GeoModelKernel Class Reference

This section describes in more detail the classes in the geometry kernel. In most cases we provide the class interface. In cases where part of the interface is used only internally by the geometry kernel itself and not by other users. In such cases we present a simplified picture of the interfaces.

Detailed descriptions of the geometry kernel classes follow.

## Reference counting


!!! warning
    The custom reference counting used in GeoModel was developed when no smart pointers were available for C++. That part will be the object of a major refactoring in the near future.


Many objects need to be allocated in the description of a complicated geometry. For efficient use of memory, these should be shared as often as possible. The extensive sharing of objects in a geometry system calls for a way of destroying the objects as soon as they are not used—by any other object that references them. The scheme for doing this is called reference counting. In the GeoModelKernel classes, it is achieved by mixing in a abstract base class, RCBase:

```cpp
//RCBase
  Constructor:
    RCBase()
  Public Methods:
    void ref ()
    void unref ()
    unsigned int refCount ()
```

`RCBase` is inherited by many of the classes in the geometry system. Reference-counted objects can only be created using operator new, and cannot be created on the stack. The methods `ref()` and `unref()` can be called to increase or decrease the reference count of an object. When the reference count decreases to zero, the object deletes itself. The accessor `refCount()` returns the current reference count.


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

The material responds to various queries about its composition, and in addition can return a series of derived quantities of physical interest. The interaction length and radiation length of a material are familiar and are described in the Particle Data Book[^n1].

[^n1]: http://pdg.lbl.gov/

Ionization energy loss in materials follows the Bethe-Bloch formula and is governed by two constants, an overall normalization term and the ionization potential, which can be computed from the atomic number and density of the material; the calculation is also described in the Particle Data Book cited above[^n1]. The calculation does not include small corrections to the energy loss due to chemical binding of elements. These constants are available through the methods `getDeDxConstant()` and `getDeDxI0()`. The method `getDeDxMin()` returns the minimum ionization energy loss and is derived from the other methods.

Both materials and elements are reference-counted; the reference count of an element is incremented when it added to a material and decremented when a referencing material is destroyed; materials are reference counted when they are used in logical volumes and decremented when the referencing logical volume is destroyed.

{!reference/RCBase/GeoElement.md!}
{!reference/RCBase/GeoMaterial.md!}

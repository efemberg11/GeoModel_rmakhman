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



##	Shapes and Logical Volumes.

### Introduction

The shape classes in the geometry kernel are data structures designed to describe several geometrical primitives. Table 1 describes the different shapes presently provided within the geometry kernel.  This set is extensible; one only needs to derive a new shape from the base class and insure that it fits the pattern described below. Shapes are reference- counted objects, as described in [1.3.2.6][#].


| Class   | Shape |
| ------- | ----- |
| GeoBox  | Box |
| GeoCons | Cone Section |
| GeoPara |Parallelapiped |
| GeoPcon | Polycone |
| GeoPgon | Polygon |
| GeoTrap | Trapezoid  (complex) |
| GeoTrd  | Trapezoid (simple) |
| GeoTube | Tube |
| GeoTubs | Tube Section |

Table 1: Existing geometrical shapes in the GeoModel kernel.


All shapes provide access to their geometry attributes (height, width, & cetera), and in addition perform several other services:

* They calculate their volume
* They can combine themselves using Boolean operations
* They allow themselves to be identified through a built-in type identification scheme.

The volume calculation is an analytic calculation provided by each subclass.


One or more Boolean operation upon shapes creates a binary expression tree.  This tree can be navigated later and the Boolean volumes declared to clients who can cope with them:  GEANT4, notably, and certain visualization systems. Several Boolean operations may be combined in a single line of code:

```cpp
GeoShape *A, *B, *C;
const GeoShape & D = (*A).add((*B) .subtract (*C));
```

A shape’s reference count is incremented either when the shape is used by a `GeoLogVol`, or in a Boolean expression.  In the above example, `D` has been `new`’d, so has an unnamed temporary.  The reference count of the temporary is incremented when it is combined with `A` to make `D`.  When `D`’s reference count falls to zero, `D` is deleted, and the temporary is deleted.


Shapes can also be shifted about before they are used in a Boolean operation.  The operation looks like this:

```cpp
GeoShape *A, *B;
HepTransform3D offset;
const GeoShape & D = A->subtract (B<<offset);
```

The type identification scheme works by comparing the result of a static method with the result of a pure virtual method:

```cpp
GeoShape *A;
if (A->typeId() == GeoBox::GetClassTypeId() ) {
	//
	// It’s a box!
	//
}
```

Both methods return a coded integer.  When the class returns the same integer as the object, a match has occurred.  Alternately one can use the methods i`getClassType()` and `type()`, which return meaningful strings like "Box", "Cons"... These are human-readable but slower to compare than unsigned integers.


All GeoShapes have the following interface:


```cpp
// GeoShape
// Public Methods:
   virtual double volume () const
   const GeoShapeUnion & add ()  const
   const GeoShapeSubtraction & subtract(const GeoShape & shape) const
   const GeoShapeIntersection &   intersection(const GeoShape & shape) const
   const GeoShapeShift & operator << (const HepTransform3D &) const
   virtual const std::string & type () const
   virtual ShapeType typeID ()  const

//Static Public Methods
   const std::string & getClassType ()
   const ShapeType getClassTypeID ()
```

The classes `GeoShapeShift`, `GeoShapeUnion`, `GeoShapeSubtraction`, and `GeoShapeIntersection` are internal and should be considered for experts.  We will not discuss them further.  
We now present the interfaces to specific shapes.  In general these shapes are by default constructed as symmetrically around the origin as possible.  


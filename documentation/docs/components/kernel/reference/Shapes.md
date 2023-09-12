

##	Shapes


### Introduction

The shape classes in the geometry kernel are data structures designed to describe several geometrical primitives. Table 1 describes the different shapes presently provided within the geometry kernel.  This set is extensible; one only needs to derive a new shape from the base class and insure that it fits the pattern described below. Shapes are reference-counted objects, as described in [Reference Counting](#reference-counting).


In the table here below, a list of the existing geometrical shapes in the GeoModelKernel package is given:


| Class   | Shape |
| ------- | ----- |
| [GeoBox](#geobox)  | Box |
| [GeoCons](#geocons) | Cone Section |
| GeoEllipticalTube | |
| GeoGenericTrap | |
| [GeoPara](#geopara) |Parallelepiped |
| [GeoPcon](#geopcon) | Polycone |
| [GeoPgon](#geopgon) | Polygon |
| GeoSimplePolygonBrep | |
| GeoTessellatedSolid | |
| GeoTorus | |
| [GeoTrap](#geotrap) | Trapezoid  (complex) |
| [GeoTrd](#geotrd)  | Trapezoid (simple) |
| [GeoTube](#geotube) | Tube |
| [GeoTubs](#geotubs) | Tube Section |
| [GeoTwistedTrap](#geotwistedtrap) | Twisted Trapezoid |
| GeoUnidentifiedShape | |


In addition to the geometrical shapes listed above, a set of *boolean operator* shapes is also added:

| Class   | Operation |
| ------- | ----- |
| GeoShapeIntersection | Intersection |
| GeoShapeShift | Shift |
| GeoShapeSubtraction | Subtraction |
| GeoShapeUnion | Intersection |



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

Both methods return a coded integer.  When the class returns the same integer as the object, a match has occurred.  Alternately one can use the methods `getClassType()` and `type()`, which return meaningful strings like "Box", "Cons"... These are human-readable but slower to compare than unsigned integers.


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

{% include 'components/kernel/reference/RCBase/GeoShape/GeoBox.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoCons.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoPara.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoPcon.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoPgon.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoTrap.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoTrd.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoTube.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoTubs.md' %}

{% include 'components/kernel/reference/RCBase/GeoShape/GeoTwistedTrap.md' %}





## Material Geometry

Material geometry consists of a set of classes that bears a large resemblance to the material geometry within GEANT4. These classes, however, are designed to take a minimal size in memory. This requirement determines the basic data structure used to hold the data for the geometry description. That structure is a graph of nodes consisting of both volumes and their properties. The tree is built directly and accessed in a way that provides users access to volumes and, simultaneously, to the properties accumulated during graph traversal that apply to the volumes. See the [Actions]() section, below.

The requirement of minimizing the memory consumption has led us to foresee a system in which objects (as well as classes) in the detector description can be re-used. This is called shared instancing and is described below. It essentially means that an element, compound, volume, or entire tree of volumes may be referenced by more than one object in the detector description.   Shared instancing can make the deletion of objects difficult unless special measures are taken.  We have used reference counting in order to facilitate clean-up and make it less error prone. See the section [How Objects are Created and Destroyed]().

Before creating hierarchies of volumes representing positioned pieces of detectors, we need to create lower-level primitives, such as elements, materials, and shapes. So, we will discuss these first.

### Materials

Materials are represented within the geometry kernel class library by the class [GeoMaterial](/kernel/reference/#geomaterial), and are built up by combining different elements, specifying each element and its fraction-by-mass.  Material constants such as the radiation length and the interaction length, as well as constants for ionization energy loss, are available through the interface but do not need to be provided to the constructor.  Instead, they are computed from the material’s element list.

The class [GeoElement](/kernel/reference/#geoelement) is used to represent elements.  Their constructor requires a name, symbol, and effective Z and A. These properties can also be retrieved from the element.

`GeoMaterial` objects are created by specifying a name and a density.  The material is “empty” until elements are added, one by one, using the `GeoMaterial::add()` method, which is overloaded so that one may provide either elements or prebuilt materials.  After all materials are added, the `GeoMaterial::lock()` method must be called, which prevents further elements or materials from being added.

Material classes, as well as all other classes, use the units defined in `GeoModelKernel/Units.h` header file. These units have originally been taken from the CLHEP library. One should normally give units when specifying densities, lengths, volumes, or other quantities in the methods of all of the classes in this library.  Therefore, when specifying water, one should use a constructor call like this:

```cpp
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits
...
GeoMaterial *water = new GeoMaterial(“H20”, 1.0*SYSTEM_OF_UNITS::gram/SYSTEM_OF_UNITS::cm3);
```

To finish constructing this material, water, one needs to follow the constructor with the following lines

```cpp
GeoElement *hydrogen = new GeoElement(“Hydrogen”,“H”,1.0, 1.010);
GeoElement *oxygen = new GeoElement(“Oxygen”,  “O”, 8.0, 16.0);
water->add(hydrogen,0.11);
water->add(oxygen,0.89);
water->lock();
```

The materials are then used to together with shapes to form logical volumes, discussed below.

### Shapes

Shapes are created using the new operator. Essentially, shapes within this system are required to store and provide access to the geometrical constants that describe their geometrical form. This data is, insofar as possible, to be specified on the constructor.

Here is how one builds a box:

```cpp
double length = 100*SYSTEM_OF_UNITS::cm;
double width  = 200*SYSTEM_OF_UNITS::cm;
double depth  =  33*SYSTEM_OF_UNITS::cm;

GeoBox* box = new GeoBox(length, width, depth);
```

Most objects can be constructed along similar lines; exceptions are objects with multiple planes such as polycones and polygons; their interface allows one to add planes successively. For the polycone, for example, the shape is built as follows:

```cpp
double dphi = 10*SYSTEM_OF_UNITS::deg;
double sphi = 40*SYSTEM_OF_UNITS::deg;

// Create polycone object
GeoPcon* polycone = new GeoPcon(dphi,sphi);

// Add planes successively
double z0    =  0.;
double rmin0 =  5.*SYSTEM_OF_UNITS::cm;
double rmax0 = 10.*SYSTEM_OF_UNITS::cm;
polycone->addPlane(z0, rmin0, rmax0);

double z1    = 10.*SYSTEM_OF_UNITS::cm;
double rmin1 =  6.*SYSTEM_OF_UNITS::cm;
double rmax1 = 12.*SYSTEM_OF_UNITS::cm;
polycone->addPlane(z1, rmin1, rmax1);

double z2    = 15.*SYSTEM_OF_UNITS::cm;
double rmin2 =  5.*SYSTEM_OF_UNITS::cm;
double rmax2 = 10.*SYSTEM_OF_UNITS::cm;
polycone->addPlane(z2, rmin2, rmax2);
```

This creates a polycone whose projection subtends an angle of 10 degrees between 40 degrees and 50 degrees, with planes at z=0, z=10, and z=15, with minimum and maximum radii there of (5,10), (6, 12), and (5,10).

The shapes can provide their data to a client through their accessors, and in addition support several other operations. Boolean operations on shapes are possible.  They can be accomplished through Boolean operators in class [GeoShape](/kernel/reference/#introduction_1):

```cpp
GeoShape       * donut  = new GeoTube();
GeoShape       * hole   = new GeoTube();
const GeoShape & result = (donut->subtract(*hole));
```

The result of a Boolean operation is a shape in a boolean expression tree that can, for example, be decoded when the geometry is declared to GEANT4.

Another method that shapes can carry out is to compute their volume.  This is useful in the context of mass inventory, in which the mass of the detector model is computed, usually for the purpose of comparing with an actual installed detector. One needs to call the `volume()` method which is defined for all shape types.

Finally, we mention a type identification scheme for shapes.  The scheme relies on two static and two virtual methods which together can be used as follows:

```cpp
// Test if the shape is a box:
if (myShape->typeId()==GeoBox::classTypeId()) {
	...
}
```

The methods `typeId()` and `classTypeId()` return unsigned integers, making the type identification very  fast.  Alternately one can use the methods `type()` and `classType()`, which work in the same way, except that these methods return `std::string`-s:  “Box”, “Tubs”, “Cons”, etc.
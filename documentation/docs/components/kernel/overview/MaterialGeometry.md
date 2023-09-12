
## Material Geometry

Material geometry consists of a set of classes that bears a large resemblance to the material geometry within GEANT4. These classes, however, are designed to take a minimal size in memory. This requirement determines the basic data structure used to hold the data for the geometry description. That structure is a graph of nodes consisting of both volumes and their properties. The tree is built directly and accessed in a way that provides users access to volumes and, simultaneously, to the properties accumulated during graph traversal that apply to the volumes. See the [Actions](/components/kernel/overview/#actions) section, below.

The requirement of minimizing the memory consumption has led us to foresee a system in which objects (as well as classes) in the detector description can be re-used. This is called shared instancing and is described below. It essentially means that an element, compound, volume, or entire tree of volumes may be referenced by more than one object in the detector description.   Shared instancing can make the deletion of objects difficult unless special measures are taken.  We have used reference counting in order to facilitate clean-up and make it less error prone. See the section [How Objects are Created and Destroyed](/components/kernel/overview/#how-objects-are-created-and-destroyed).

Before creating hierarchies of volumes representing positioned pieces of detectors, we need to create lower-level primitives, such as elements, materials, and shapes. So, we will discuss these first.

### Materials

Materials are represented within the geometry kernel class library by the class [GeoMaterial](/components/kernel/reference/#geomaterial), and are built up by combining different elements, specifying each element and its fraction-by-mass.  Material constants such as the radiation length and the interaction length, as well as constants for ionization energy loss, are available through the interface but do not need to be provided to the constructor.  Instead, they are computed from the material’s element list.

The class [GeoElement](/components/kernel/reference/#geoelement) is used to represent elements.  Their constructor requires a name, symbol, and effective Z and A. These properties can also be retrieved from the element.

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
GeoElement *hydrogen = new GeoElement("Hydrogen",
                                      "H",
                                      1.0,
                                      1.010);
GeoElement *oxygen = new GeoElement("Oxygen",
                                    "O",
                                    8.0,
                                    16.0);
water->add(hydrogen,0.11);
water->add(oxygen,0.89);
water->lock();
```

The materials are then used to together with shapes to form logical volumes, discussed below.

### Shapes

Shapes are created using the new operator. Essentially, shapes within this system are required to store and provide access to the geometrical constants that describe their geometrical form. This data is, insofar as possible, to be specified on the constructor.

Shapes are extensible and custom shapes can be built.

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

The shapes can provide their data to a client through their accessors, and in addition support several other operations. Boolean operations on shapes are possible.  They can be accomplished through Boolean operators in class [GeoShape](/components/kernel/reference/#introduction_1):

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

### Logical Volumes

Logical volumes represent, conceptually, a specific manufactured piece that can be placed in one or more locations around the detector.  A logical volume is created by specifying a name tag for the volume, a shape, and a material:

```cpp
const GeoLogVol *myLog = new  GeoLogVol("MyLogVol",
                                        myShape,
                                        gNitrogen);
```

### Physical Volumes and the Geometry Graph

Having created elements, materials, shapes, and logical volumes, you are now ready to create and locate placed volumes called physical volumes. Before you start, you will need to know that there are two kinds of these:

 * Regular Physical Volumes, designed to be small.
 * Full Physical Volumes, designed to hold in cache complete information about how the volume is located with respect to the world volume, its formatted name string and other important information.

There is a common abstract [base class](/components/kernel/overview/#geomodel-kernel-overview) for all of these:  `GeoVPhysVol`.  In addition both the full physical volumes have another layer of abstraction, `GeoVFullPhysVol`. All physical volumes allow access to their children.

The concrete subclasses that you have at your disposition for detector description are called [GeoPhysVol](/components/kernel/reference/#geophysvol) and [GeoFullPhysVol](/components/kernel/reference/#geofullphysvol).  Both of these have a method to add either volumes or volume properties:

```cpp
GeoPhysVol* myVol;
myVol->add(aTransformation);
myVol->add(anotherVolume);
```

When you add a transformation, you change the position of the subsequent volume with respect to the parent.  If you add no transformation, you will not shift the daughter relative to the parent and commonly will create a daughter which is centered directly in the parent. If you add more than one transformation to the volume before adding a parent, they will be multiplied.  The last transformation to be added is applied first to the child. Transformations are discussed next. Like logical volumes, they may be shared.

Like physical volumes, transformations come in two types:

* Regular transformations designed to be small.
* Alignable transformations, which allow one to add a misalignment to the system.  Misaligning a transformation changes the position of all volumes “under” the transformation and clears the absolute location caches of all full physical volumes.

When you create a transformation, you must choose the type.

The model of the raw geometry is a tree of nodes, property nodes and volume nodes. The tree can be thought of as a tree of volumes, each one “having” a set of properties (inherited from property nodes throughout the tree). The subsystem engineer judiciously chooses which of the volumes are to contain full, cached, position information – usually, these first-class volumes are to be associated with a detector. He or she also judiciously decides which of the transformations are to be alignable—usually these are the transformations which position something that ultimately has a detector bolted, glued, riveted or otherwise clamped onto a sensitive piece.  Then, the developer can apply several techniques for keeping track of these pointers so that the important volumes can later be connected to detector elements, and the alignable transformations can be connected to the alignment database for periodic updating.

Finally, we provide three mechanisms for giving names to volumes:

* Do nothing.  The volume will be called "ANON".
* Add a [GeoNameTag](/components/kernel/reference/#geonametag) object to the graph before adding a volume.  The next volume to be added will be given the `GeoNameTag`’s name.
* Add a [GeoSerialDenominator](/components/kernel/reference/#geoserialdenominator) object to the graph before adding more volumes. The volumes will be named according to the base name of the `GeoSerialDenominator`, plus given a serial number: 0, 1, 2, 3, ...

In effect this last method can be thought of as a way of parametrizing the name of the volume.

### Actions

There are two ways of getting raw geometry information out of the model.  Suppose that one has access to a particular physical volume (it could be the “World” physical volume). One can access its children, there names, and their transformations with respect to the parent in the following way:

```cpp
PVConstLink myVol;
for(int c=0; c<myVol->getNChildVols(); ++c) {
	PVConstLink child = myVol->getChildVol(c);
	GeoTrf::Transform3D xf = getXToChildVol(c);
}
```

One could then iterate in a similar way over the grand children, by using a double loop.  Ultimately one would probably to visit all the volumes, whatever their depth in the tree, so probably this would call on some form of recursion.  An easy way would be to embed the small sample of code shown above in a recursive subroutine or method.  That would be fine and is conceptually simple. However, within the geometry model’s kernel, we have provided an alternate, probably better way to visit the entire tree.

That mechanism involves a [GeoVolumeAction]().  A `GeoVolumeAction` is a way (for applications programmers) to obtain recursive behavior without writing any recursive routines.  It’s a class with a handler routine (`handleVPhysVol()`) which is called for each node before (or after) it is called on its children.  This can descend to an arbitrary depth in the tree.  The `GeoVolumeAction` is an abstract base class and should be subclassed by programmers to suit their needs.  Another class `TemplateVolAction` is provided as a template that one can take and modify.  To run it, one does this:

```cpp
PVConstLink myVol;
TemplateVolAction tempVolAction;
myVol->apply(tempVolAction);
```

The `handleVPhysVol()` function within the `TemplateVolAction` is where the work is supposed to get done.  It will be invoked repeatedly, once for each node in the tree.  Within that routine, one can access the physical volume as a subroutine parameter, and information about the transformation and the path to the node through the base class for actions, `GeoVolumeAction`.  The action can be designed to run from the bottom up or from the top down.

Incidentally, there is another kind of action in the library called [GeoNodeAction]().  `GeoNodeActions` visit all nodes in the geometry tree, including naming nodes, transformation nodes, and perhaps other property nodes that may be added later to the model.  Since usually an application programmer wants to see volumes and their properties, the `GeoVolumeAction` is more suited to casual users than the `GeoNodeAction`, which is considered mostly internal.  However the usage is similar, except that node actions are “exec’d” while volume actions are “applied”.  Here for example is how we can rewrite the loop over children using volume actions:

```cpp
PVConstLink myVol;
for(int c=0; c<myVol->getNChildVols(); ++c) {
	GeoAccessVolumeAction av(c);
	myVol->exec(&ac);
	PVConstLink child = av.getVolume();
	GeoTrf::Transform3D xf = av.getTransform();
}
```

This, it turns out, will execute faster than the loop shown above, which (internally) will run the action, twice: once, in order to locate the daughter volume and then a second time, to locate its transform.

### How Objects are Created and Destroyed

We now come to the important topic of how objects in this system are created and destroyed.  The geometry kernel uses a technique called reference counting.  Reference counting, shortly stated, is a way to perform an automatic garbage collection of nodes that are no longer in use.  This is important when describing a large tree of information, much of which is ideally to be shared — used again and again in many places.

You may have noticed, in several code examples used throughout the GeoModel Kernel Overview section many of the objects have been created using operator `new`. You may have also noticed, if you’ve tried to play around with the kernel classes, that statements which allocate most kernel classes on the stack, such as

```cpp
GeoBox box(100*SYSTEM_OF_UNITS::cm
           , 100*SYSTEM_OF_UNITS::cm
           , 100*SYSTEM_OF_UNITS::cm);
```

are not allowed. Who is going to clean up the memory after all these `new `operations?  And why does the compiler disallow allocation on the stack?

Let's consider this example:

```cpp
const GeoBox* worldBox = new GeoBox(100*SYSTEM_OF_UNITS::cm
                                    , 100*SYSTEM_OF_UNITS::cm
                                    , 100*SYSTEM_OF_UNITS::cm);
const GeoLogVol* worldLog = new GeoLogVol("WorldLog"
                                          , worldBox
                                          , worldMaterial);
GeoPhysVol* worldPhys = new GeoPhysVol(worldLog);
```

Each of the three objects (`worldBox`, `worldLog`, and `worldPhys`) are created with a reference count.  `worldBox`’s is initially zero, at the time it is created.  `worldLog`’s is also zero when it is created.  However, when `worldLog` is created, the reference count of `worldBox` increases to one, since now it is referenced somewhere — namely by the logcal volume `worldLog`. Now, when the physical volume `worldPhys` is created, the reference count of the logical volume will increase to one — since it is used once by a single physical volume.

Each time a physical volume is positioned within another physical volume, its reference count increases. Suppose we look now at a sub-tree of physical volumes that is used five times.  At a run boundary, it may happen that a piece of the tree is torn down.  When the first node referencing the physical volume is destroyed, it decreases the volumes reference count, from five to four.  When the next node referencing the physical volume is destroyed, the reference count goes from four to three. And so forth.

When the very last node referencing the physical volume is destroyed, this means that the physical volume itself has outlived its usefulness and *should disappear*.  And that is what happens.  The destruction of objects is carried out automatically when the reference count falls to zero.  And in fact, the only way to delete an object is to arrange for all of its references to disappear.  This is because the destructor of all reference counted objects is private.

This scheme applies to elements, materials, shapes, logical volumes, physical volumes, full physical volumes, and instances of all other classes which also inherit from the `RCBase` class.

So far, we have described what happens to an object when it is no longer used by any other node in the tree.  However, what about the top of the tree, which has no nodes that refer to it?  Since the destructors of our physical volumes are private, how do you arrange to get it to go away?

Reference counts can also be manipulated manually, by using the methods `ref()` and `unref()`.  The physical volume at the head of the tree, often known as the “world” physical volume, can be referenced manually using this call:

```cpp
worldPhys->ref(); // increments reference count by one
```

Later, you can destroy the world volume and trigger a global collection of garbage by using this call:

```cpp
worldPhys->unref() // decrements reference count by one
```

When the reference count becomes 0, the world physical volume deletes itself, decreasing the reference counts of its logical volume and any children. These will then begin dereferencing and possibly deleting their own children, until all the memory has been freed.

Suppose now that you want to arrange for a node to not be deleted automatically in this fashion — even when nobody references it any more. In order to do this, simply call the `ref()` method on this object. That way, the reference counts starts at 1 and will not fall to zero until you call `unref()`, manually.

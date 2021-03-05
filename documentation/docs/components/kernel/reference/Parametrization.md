

## Parametrization

### Introduction

A principle goal in the design of the geometry kernel has been to limit memory usage.  A powerful way of doing this is to parameterize the volumes rather than to create them, say, inside of a single, double, or other multiple loop.

Parameterizations are mathematical recipes for creating volumes.  There are three main ingredients to these recipes:


* `GENFUNCTIONS`, which are mathematical function-objects; they allow one to perform function arithmetic in the same way that one performs floating point arithmetic.

* `TRANSFUNCTIONS`, which, together with `GENFUNCTIONS` and `HepTransform3D`, allow one to expand and  parametrize elements of the Euclidean group (i.e., rigid body transformations).

* `GeoSerialTransformer`, a kind of `GeoGraphNode` that allows a particular physical volume to be placed according to a `TRANSFUNCTION` expansion of a rigid body transformation.


An example will demonstrate how this is useful. The example is taken from the actual ATLAS detector description, from the GeoModel description of the liquid argon barrel calorimeter:


```cpp
#include "GeoGenericFunctions/AbsFunction.hh"
#include "GeoGenericFunctions/Variable.hh"
#include "GeoModelKernel/GeoXF.h"  
#include "GeoModelKernel/GeoSerialTransformer.h"

using namespace Genfun;
using namespace GeoXF;

// Define some constants:

GeoPhysVol  *pV, *world;
int          N;
double       c1, c2, r, z;

// Construct a simple linear function of a variable i:

Variable      i;
GENFUNCTION   g = c1+c2*i;

// Parameterize a transformation using this function:

TRANSFUNCTION xf = Pow(HepRotateZ3D(1),g)*
			       HepTranslateX3D(r)*
			       HepTranslateZ3D(z);

// Use the parametrized transformation to place a volume N times:

GeoSerialTransformer *st=new GeoSerialTransformer(pV, &xf, N);
world->add(st);
```

The file `AbsFunction.hh`, from `GeoModel/GeoModelCore/GeoGenericFunctions`---which has been adapted from the [CLHEP](proj-clhep.web.cern.ch) package---, defines the interface to `GENFUNCTIONS` and must be included.  In addition, if specific functions such as trig functions, higher transcendental functions or physics-specific functions are required, header files for these function-objects should be included from the same area.   

The headers `GeoXF.h` and `GeoSerialTransformer.h` are  needed for the classes `TRANSFUNCTION` and  `GeoSerialTransformer`, respectively.

`GENFUNCTIONS` and `TRANSFUNCTIONS` both live within namespaces, which we access with the `using` statements near the top of the example.  After defining the variables used in this example, we construct a simple linear function of an input variable i:

```cpp
Variable    i;
GENFUNCTION g =  c1+c2*i;
```

This example is very simple, but shows already how we can use these classes to build symbolic expressions.  A variety of functions lives already within `GeoModel/GeoModelCore/GeoGenericFunctions`.  The set is user-extensible, and the extension procedure is amply described within the [CLHEP `GeoGenericFunctions` package documentation](https://proj-clhep.web.cern.ch/proj-clhep/doc/CLHEP_1_7/UserGuide/GenericFunctions/index.html).  Addition, subtraction, multiplication, division, composition, and direct product operations are all valid. 

The next step, in which `TRANSFUNCTIONS` are constructed, parametrizes the rigid body transformation.  The `TRANSFUNCTION`, `xf`, has a function call operator that can be used to evaluate a particular rigid body transformation as a function of an input argument, like this:

```cpp
HepTransform3D tx = xf(j);
```

The expansion of the `TRANSFUNCTION` is as follows.  Let  $X_i (i  = 1, 2, … N)$ represent any transformation, either a rotation, a translation, or even some combination of these.  The rotations may be about a coordinate axis, or even some other axis.  Furthermore, let us denote by $f_i(x) ( \textrm{where}\ i =1, 2… N)$  a function of a single variable.  Then, the expansion of an arbitrary function is:

$$
T(x) = X_1^{f_1(x)} * X_2^{f_2(x)} * X_3^{f_3(x)} \cdots X_n^{f_n(x)}
$$

In this expression, $T(x)$ is the resulting transformation, which is now a function of the single input parameter, $x$. The expansion is both simple, and completely general.  A single term in this expansion (for example $X_2^{f_2(x)}$), will be referred to as an exponentiated transformation.  It is implemented in terms of the class `Pow`, which has the following constructor:

```cpp
Pow(const HepTransform3D &, GENFUNCTION f);
```

Exponentiated transformations are simple transfunctions, and can be composed to make other `TRANSFUNCTIONS`.  The `TRANSFUNCTION` interface also allows one to compose fixed transformations with exponentiated transformations. 

The interface to `GENFUNCTION` and `TRANSFUNCTION` provide all the necessary operations on these types of object.  The interfaces to these types of objects are not simple to read, so we will not attempt to explain them in this document.  Instead, one should assume that all well-defined mathematical properties that apply to functions are properties of `GENFUNCTIONS`, and all mathematical properties that apply to parameterizations of elements of the Euclidean group are properties of `TRANSFUNCTIONS`.

Once one has a a `TRANSFUNCTION` in hand, it can be used together with a `GeoSerialTransformer` object to repeatedly place a physical volume.  To do this, use the following constructor:

```cpp
GeoSerialTransformer(const GeoPhysVol *pVol,
                     TRANSFUNCTION xf,
                     unsigned int N);
```


In this constructor, `pVol` is the volume to be repeatedly placed, `xf` is the `TRANSFUNCTION` that specifies how to place it, and `N` is the desired number of copies.

The `GeoSerialTransformer` can then be added to the geometry graph.  During any subsequent volume traversal, the geometry graph will appear to contain multiple physical volumes at different locations.  However, only the memory of a single physical volume and a `TRANSFUNCTION` is actually allocated.  

During node traversal (using `GeoNodeActions`) one can recover, from the geometry graph, the actual recipe for generating volumes.  This is sometimes useful; for example, in case one wishes to create a GEANT4 parameterization from a GeoModel parameterization.





!!! note

    One further word about parameterizations is in order: parameterizations, as they are usually understood, allows for the shape or composition of an object to vary as a function of copy number.  This is presently not a part of the `GeoModelKernel`.  

    However, we intend to include this in subsequent releases.  The basic design strategy is to start with a concrete shape class, such as GeoBox, and to use this a basis of a new class for parameterizing boxes.  In the new class—call it `GeoBoxParameterization`—we replace all of the floating point member data with `GENFUNCTION` member data, and all of the floating point constructor arguments with `GENFUNCTION` constructor arguments.  In this way we create a very flexible recipe for generating a box.  

    The same technique can be used to vary an objects composition as a function of copy number.




### GeoSerialTransformer


```cpp
//=== GeoSerialTransformer ===

  // Constructors:
  GeoSerialTransformer (const GeoVPhysVol * volume, const GeoXF::Function * func, unsigned int copies)

  // Public Methods:
  HepTransform3D getTransform (int  i) const
  unsigned int getNCopies() const
  PVConstLink getVolume() const;
 
  // Public Methods from GeoGraphNode
  void exec (GeoNodeAction * action)
```


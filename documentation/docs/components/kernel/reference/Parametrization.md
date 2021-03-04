

## Parametrization


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

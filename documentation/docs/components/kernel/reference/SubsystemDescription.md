

## Base classes for subsystem description

The geometry kernel contains three base classes for subsystem description.  These provide require a minimum amount of functionality from subclasses—just enough to function within a reasonable framework.  The classes are:  `GeoVDetectorElement`, an abstract base class for a separately-alignable piece of a detector subsystem,  `GeoVDetectorFactory`, an abstract base class for an algorithm which builds the geometry, including the detector elements and other pieces of nondescript support material, and a `GeoVDetectorManager`, which is stored in the transient detector store and provides access to the geometry, both material geometry and readout geometry. Both classes should be extended by subsystems people to provide a bona fide readout geometry interface. 

The interfaces to these classes are shown here:


### GeoVDetectorElement

```cpp
//== GeoVDetectorElement ==

  // Constructor:
  GeoVDetectorElement (const GeoVFullPhysVol * fullPhysVol)

  // Public Methods:
  const GeoVFullPhysVol * getMaterialGeom () const

  // Virtual Public Methods to be overridden by subclasses:
  Identifier identify() const;
```

The constructor for a `GeoVDetectorElement` requires a full physical volume, which should be unique and which should live under a unique branch.  This is how all detector elements know where they are.


The `getMaterialGeom()` method provides access to the full physical volume.  This is mostly for subclasses to use, when they compute derived position information.

The `identify()` method provides an identifier for the volume. 

!!! note 

    Note that the coupling to the actual identification scheme is loose because the `identify()` method is pure virtual and the `Identifier` class is forward-declared. 





#### GeoVDetectorFactory

```cpp
//== GeoVDetectorFactory ==

  // Constructor:
  GeoVDetectorFactor()

  // Virtual Public Methods to be overridden by subclasses:
  void create (GeoPhysVol * world)
  const GeoVDetectorManager *getDetectorManager() const;
```

`GeoVDetectorFactories` create geometry and map it into readout geometry.  For the simulation, access to the raw geometry is required.  The interface to `GeoVDetectorFactory` contains two pure virtual functions which must be implemented in the subclass.

The method  `create (GeoPhysVol * world)` creates the material and readout geometry, and attaches the raw geometry to the physical volume tree, under the world volume, which is passed in from the calling routine.

The method `getDetectorManager()` returns a detector manager, which contains all of the constructed geometry.  It is permissible to return a pointer to a subclass of GeoVDetectorManager (the so-called covariant return type mechanism).  This is proper C++ and the correct way of avoiding dynamic casting.



#### GeoVDetectorManager


```cpp 
//== GeoVDetectorManager ==

  // Constructor:
  GeoVDetectorManager()

  // Virtual Public Methods to be overridden by subclasses:
  unsigned int getNumTreeTops () const
  PVConstLink getTreeTop (unsigned int i) const
```


`GeoVDetectorManagers` hold the results of geometry construction and are stored in the transient detector store.  For the simulation, access to the raw geometry is required.  Experience has shown that subsystems cannot be cleanly implemented in terms of a single, top-level volume; instead several volumes must exist at the top level, for topological reasons, hence the iterative access to top level volumes, or tree-tops.  The interface to `GeoVDetectorManager` contains several pure virtual functions which must be implemented in the subclass.

The method `getNumTreeTops()` returns the number of top-level physical volumes in the raw geometry for the subsystem.

The method  `getTreeTop(unsigned int i)` returns the $i^{th}$ top-level physical volume for the subsystem.








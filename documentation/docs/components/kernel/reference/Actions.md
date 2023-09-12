

## Actions

The principle way of accessing the physical volume tree is through actions which facilitate a recursive tree traversal and accumulate certain volatile information, such as a volume’s absolute position, during the traversal.  An action is applied to a node in the tree, which applies it in turn to its children.  

One type of action, `GeoNodeAction`,  is passed to all children.  Another type, `GeoVolumeAction`, is only passed only to daughter physical volumes (this includes “virtual” physical volumes that come from parameterizations).   Casual users should consider subclassing `GeoVolumeAction` in order to recurse through all of the physical volumes in the geometry graph.  “Power users” will occasionally need to access the geometry graph directly, in order to visit nodes such as `GeoSerialTransformer` nodes.   Such users should consider subclassing `GeoNodeAction`.  This section describes the two types of existing actions in more detail.



### Volume Actions and associated classes


#### GeoVolumeAction

`GeoVolumeAction` is a base class specifically designed for user subclassing, and all users are encouraged to use it for data access.  

Its interface is here:

```cpp
//=== GeoVolumeAction ===

  // Enumerated Types:
  enum GeoVolumeAction::Type {TOP_DOWN, BOTTOM_UP}

  // Constructor:
  GeoVolumeAction (Type type  = TOP_DOWN)

  // Public Methods:
  void handleVPhysVol (const GeoVPhysVol * )
  void terminate ()
  const GeoTraversalState * getState () const
```

In order to subclass this, follow this checklist:

1.	Write a class inheriting from `GeoVolumeAction`.  You should decide whether your class should walk through the volumes from the top down, which is the default, or from the bottom up.  If you want a bottom up tree traversal you need to initialize the base class, accordingly, in the constructor for your new class.

2.	Override the `handleVPhysVol()` method in order to do something with each volume you encounter during geometry graph traversal.  You may, of course, add member data or additional methods in order to carry out the action, or to access results.  

3.	If you wish for the action to hit every node in the tree, you don’t need to do anything special.  If you wish it to terminate early, call the `terminate()` method from with your `handleVPhysVol()`.  Geometry graph traversal will immediately terminate.

4.	The action keeps track of its traversal state, accumulating such information as absolute transformation to the current node, and the path to the current node.  The state can be retrieved from the `getState()` method.  This may be called from with your `handleVPhysVol()` method in order to, say, find out where you are in global coordinates.



A GeoVolumeAction upon a tree of physical volumes is initiated when the following line of code is invoked:

```cpp
GeoPhysVol        *vol;
MyGeoVolumeAction action;
vol->apply(&action);
```

Your `handleVPhysVol()` routine can obtain information about the current node from two sources:  first, from the physical volume itself, which is passed as an argument to the routine, and secondly from the state (class `GeoTraversalState`) which is available from the `getState()` method.  We examine that next.


#### GeoTraversalState

```cpp
//=== GeoTraversalState ===

  // Const Public Methods:
  const HepTransform3D & getTransform () const
  const std::string & getName () const
  Query<unsigned int> getId() const
  const HepTransform3D & getDefTransform () const
  const std::string & getAbsoluteName () const
  const HepTransform3D & getDefAbsoluteTransform () const
  const HepTransform3D & getAbsoluteTransform () const
  const GeoNodePath * getPath () const
```

The interface is simple and self-explanatory; only the getPath() method and the information it returns needs further explanation.   The path (class GeoNodePath) is an ordered stack of nodes that shows how the current node was reached.  

Its interface is shown here:

#### GeoNodePath

```cpp
//== GeoNodePath ==

  // Public Methods:
  unsigned int getLength ()
  const GeoVPhysVol * getItem(unsigned int i)
  const GeoVPhysVol * getHead ()
  const GeoVPhysVol * getTail ()
```


The head node, or node from which the action was initiated, and tail node, or last node in the path, are available.  The total length of the path can be retrieved, as well as arbitrary item along the path.  


#### TemplateVolAction

TemplateVolAction is a class which has been added only as an illustration for how to write volume actions.  The header file is shown here below: 

```cpp
#ifndef TemplateVolAction_h
#define TemplateVolAction_h 1

class TemplateVolAction:public GeoVolumeAction	
{

public:
 
  // Constructor
  TemplateVolAction ();

  // Destructor
  ~TemplateVolAction ();

  // Action routine
  virtual void handleVPhysVol (const GeoVPhysVol *);

};
#endif
```

while the source file is shown in the following:

```cpp
#include "GeoModelKernel/TemplateVolAction.h"

TemplateVolAction::TemplateVolAction ()
:GeoVolumeAction (GeoVolumeAction::TOP_DOWN)
{
}

TemplateVolAction::~TemplateVolAction ()
{
}

void TemplateVolAction::handleVPhysVol (const GeoVPhysVol *)
{
  /* 
   * Your procedure here.  This one does nothing...
   */
}
```



### Node Actions


`GeoNodeAction` classes do more than traverse the tree of physical volumes.  These actions stop and execute on every graph node in the geometry graph, not the physical volumes represented within or generated by the geometry graph.  Three kinds of node actions are used internally by the kernel:

- `GeoCountVolAction`,
- `GeoAccessVolumeAction`,
- `GeoClearAbsPosAction`.

`GeoCountVolAction` counts the number of physical volumes below some node, down do a depth. `GeoAccessVolumeAction` is for retrieving a particular volume.  `GeoClearAbsPosAction` can be used to invalidate a cache of absolute position information below some node. The first two have potential usefulness outside of the geometry kernel itself, and will be discussed below.

The following methods are available on all `GeoNodeAction` classes, and control the depth limit:

- `void setDepthLimit(int limit)`;
- `Query<unsigned int> getDepthLimit() const`;
- `void clearDepthLimit()`,

Specific `GeoNodeAction` classes may set specific defaults for their depth limits.   See the documentation on these actions.



#### GeoCountVolAction

```cpp
//== GeoCountVolAction ==

  // Constructor:
  GeoCountVolAction ();

  // Public Methods:
  const  unsigned int getCount() const;
```

When this action is executed upon a node it counts daughter volumes.  The count includes only physical volumes.  It contains virtual, or parameterized volumes as well as actual volumes.
The depth limit for this action by default is 1:  only the volumes directly beneath the top volume are counted.  The final count does not include the top volume, only the children. The depth limit can be reset or cleared using the methods `clearDepthLimit()` or `setDepthLimit()` from the base class.  

Here is a typical use case:

```cpp
GeoPhysVol *world;
GeoCountVolAction cv;
cv->setDepthLimit(2);
world->exec(&cv);
int count = cv.getCount ();
```


#### GeoAccessVolumeAction

```cpp
//== GeoAccessVolumeAction ==

  // Constructor:
  GeoAccessVolumeAction (unsigned int Index)

  // Public Methods:
  PVConstLink getVolume () const
  const HepTransform3D & getTransform () const
  const HepTransform3D & getDefTransform () const
  const std::string & getName () const
  Query<unsigned int> getId() const
```

`GeoAccessVolumeAction` is used to retrieve physical volume and some of its properties from within the geometry tree.  

The constructor needs to provide the index of the child volume which is sought.  The public methods can be used once the action has executed and return a link to the daughter volume, the transform to the daughter, the default transform to the daughter, and the name of the daughter. 

A typical use case is shown here:

```cpp
int index
GeoFullPhysVol *vol;
GeoAccessVolumeAction av (index);
vol->exec (&av);
std::string name = av->getName();
```

Besides the name, the transformations (default and misaligned), can be retrieved along with the volume.  The action executes to a depth of 1; i.e. it is used only for accessing direct descendents.

A simpler alternative to accessing daughter volumes in this way is to use the methods in `GeoVPhysVol` and subclasses to retrieve information about their daughters.  This is simpler, and uses the action internally.  Using the action directly, however, can be faster if accessing different kinds of information at the same time since the volume location can be performed just once. 




### For Power Users: How to Make Your Own `GeoNodeAction`

The basic structure of the GeoModel is:  a geometry graph which emulates a physical volume tree.   The graph is constructed by adding a combination of physical volumes, transformations, name tags, serial denominators and serial transformers into physical volumes.  The information is usually retrieved by scanning the physical volume tree as opposed to the geometry graph.  The class `GeoVolumeAction` exists in order to be subclassed by users for this purpose.  


However, in certain more rare cases users will need to navigate the geometry graph directly.  For example, when declaring the geometry to the simulation, one may wish to translate GeoModel parametrizations from `GeoSerialTransformer` nodes into `G4Parametrizations`.  In that case one is interested in accessing the parametrization directly and not the virtual volumes that they generate.   These ''power users'' can access all nodes in the geometry tree by subclassing `GeoNodeAction`.  In this subsection, we examine how to do this.  


#### GeoNodeAction

GeoNodeAction has the following interface:

```cpp
//== GeoNodeAction ==

  // Constructor:
  GeoNodeAction()

  // Public Methods:
  GeoNodePath * getPath () const
  Query<unsigned int>  getDepthLimit () const
  void terminate ()
  bool shouldTerminate () const
  void setDepthLimit (unsigned int limit)
  void clearDepthLimit ()

  // Virtual Public Methods to be overridden in subclasses:
  void handleNode (const GeoGraphNode * )
  void handleTransform (const GeoTransform * )
  void handlePhysVol (const GeoPhysVol * )
  void handleFullPhysVol (const GeoFullPhysVol * )
  void handleNameTag (const GeoNameTag * )
  void handleIdentifierTag(const GeoIdentifierTag *)
  void handleSerialDenominator (const GeoSerialDenominator * )
  void handleSerialTransformer (const GeoSerialTransformer  * )
```


When subclassing the `GeoNodeAction`, the principle task is to write a handle method for each type of `GeoGraphNode` object that you wish to visit.  These methods are shown in the above table under the rubric “Virtual Public Methods to be overridden in subclasses”.  They are called in sequence each time a specific kind of graph node is encountered, from top to bottom.

In addition, the action can specify its own depth by calling the methods `setDepthLimit()` and `clearDepthLimit()`.  In addition the action can be terminated by any time by calling the `terminate()` method; usually this should be done within one of the handler methods.  The path (class `GeoNodePath`, discussed in section 2.8.1) can also be accessed from anywhere within the class, most notably within the handler methods.






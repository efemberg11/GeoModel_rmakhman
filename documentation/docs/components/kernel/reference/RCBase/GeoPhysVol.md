### GeoPhysVol

```cpp
// GeoPhysVol
  //Constructor:
    GeoPhysVol (const GeoLogVol * LogVol)
  //Public Methods:
    void add (GeoGraphNode * graphNode)
  //Public Methods from GeoVPhysVol
    bool isShared() const
    std::optional<unsigned int> indexOf (PVConstLink daughter) const
    PVConstLink getParent () const
    const GeoLogVol * getLogVol () const
    unsigned int getNChildVols () const
    PVConstLink  getChildVol (unsigned int index) const
    GeoTrf::Transform3D getXToChildVol (unsigned int index) const
    GeoTrf::Transform3D getDefXToChildVol (unsigned int index) const
    std::string getNameOfChildVol (unsigned int i) const
    std::optional<unsigned int> getIdOfChildVol() const
    void apply (GeoVolumeAction * action) const
    unsigned int getNChildVolAndST() const
    GeoTrf::Transform3D getX    (const GeoVAlignmentStore* store=nullptr) const
    GeoTrf::Transform3D getDefX (const GeoVAlignmentStore* store=nullptr) const
    unsigned int getNChildNodes() const
    const GeoGraphNode * const * getChildNode (unsigned int i) const
    const GeoGraphNode * const * findChildNode (const GeoGraphNode *n) const
  //Public Methods from GeoGraphNode
    void exec (GeoNodeAction * action)
    void dockTo (GeoVPhysVol* pv)
```

`GeoPhysVol (const GeoLogVol * LogVol)` Constructs the physical volume from the logical volume. The reference count of the logical volume is incremented, and will be decremented again when the physical volume is destroyed.

`void add (GeoGraphNode * graphNode)` Adds a graph node to the physical volume.  The reference count of the graph node is incremented.

`bool isShared() const` Accessor to determine whether the physical volume is shared; i.e, used more than once in the geometry description.

`std::optional<unsigned int> indexOf (PVConstLink daughter) const` Accessor to determine the index of a daughter physical volume, in other words, in position within the list of daughters.  The result only counts physical volumes as daughters, not their properties.  

`PVConstLink getParent () const` Returns the parent physical volume.  In case the parent is not unique (i.e., if the physical volume is shared), return NULL.

`const GeoLogVol * getLogVol () const` Returns the logical volume.

`unsigned int getNChildVols () const` Returns the number of child volumes.  This includes only physical volumes and does not count property nodes.  It does, however, include virtual physical volumes from a parametrization.

`PVConstLink  getChildVol (unsigned int index) const` Returns the specified child volume.

`GeoTrf::Transform3D getXToChildVol (unsigned int index) const` Returns the transformation to the specified child volume.  The transformation is relative to this object; it is *not* an absolute transformation to a global coordinate system.

`GeoTrf::Transform3D getDefXToChildVol (unsigned int index) const` Returns the default transformation of the specified child volume, relative to this object.

`std::string getNameOfChildVol (unsigned int i) const` Returns the name of the child volume, relative to this object.

`std::optional<unsigned int> getIdOfChildVol() const` Returns the identifier of the child volume.

`void apply (GeoVolumeAction * action) const` Applies a volume action to the volume.  This action normally recursively visits each daughter volume.

`unsigned int getNChildVolAndST() const` Returns the number of child physical volumes and serial transformers.

`GeoTrf::Transform3D getX    (const GeoVAlignmentStore* store=nullptr) const` Returns the transformation of the given physical volume to its parent physical volume. If the given physical volume is shared, then an exception is thrown.

`GeoTrf::Transform3D getDefX (const GeoVAlignmentStore* store=nullptr) const` Returns the default transformation of the given physical volume to its parent physical volume. If the given physical volume is shared, then an exception is thrown.

`unsigned int getNChildNodes() const` Returns the number of child graph nodes.

`const GeoGraphNode * const * getChildNode (unsigned int i) const` Returns the child graph node, relative to this object.

`const GeoGraphNode * const * findChildNode (const GeoGraphNode *n) const` Checks if the object contains child node *n*. If it does, then the function returns the pointer to *n*, otherwise it returns `nullptr`.

`void exec (GeoNodeAction * action)` Applies a node action to the volume.  This action normally recursively visits each graph node in the geometry graph and includes handler functions even for property nodes.

`void dockTo (GeoVPhysVol* pv)` Caches *pv* as a pointer to the parent physical volume.
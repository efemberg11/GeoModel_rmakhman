### GeoMaterial

```cpp
// GeoMaterial
  //Constructor:
    GeoMaterial (const std::string & Name, double Density) const
  //Public Methods:
    void add (GeoElement * element, double fraction = 1.0) void add (GeoMaterial * material, double fraction) void lock ()
    std::string getName () const
    double getDensity () const
    unsigned int getID() const
    unsigned int getNumElements () const
    const GeoElement * getElement (unsigned int i) const double getFraction (int i) const
    double getRadLength () const
    double getIntLength () const
    double getDeDxConstant () const
    double getDeDxI0 () const
    double getDeDxMin () const
```

`GeoMaterial` is a class that describes a material, which is a list of elements. It is created “empty”; subsequently, elements are added one-by-one until the material is "locked". When the material is locked, no further editing is possible, and a series of derived quantities (radiation length, interaction length, etc.) is computed for the material.


`GeoMaterial (const std::string & Name, double Density) const` Constructs the material with a name and a density[^gmat1]


`void add (GeoElement * element, double fraction = 1.0)`  Adds an element to the material, with a specified mass fraction.

`void add (GeoMaterial * material, double fraction)`  Adds a material to the material, with a specified mass fraction.  This is useful for combining precombined materials, such as argon + ethane.

`void lock ()` Locks the material against further editing, and computes all derived quanties such as radiation length and interaction length.

`std::string getName () const` Accesses the name of the material.

`double getDensity () const` Returns the density of the material[^gmat1].

`unsigned int getID() const`  Returns the id of the material.  The id is generated automatically by counting instances of materials.

`unsigned int getNumElements () const`  Returns the number of elements in a material.

`const GeoElement * getElement (unsigned int i) const` Returns a pointer to the ith element.

`double getFraction (int i) const` Returns the fraction of the ith element.

`double getRadLength () const`  Returns the radiation length of the material, computed from the density, the list of constituents, and their properties.

`double getIntLength () const` Returns the interaction length of the material, computed from the density the list of constituents, and their properties.

The following methods refer to ionization energy loss, specifically, the following formulation:


$$
\frac{dE}{dx} = \frac{K}{\beta^2} (ln (\frac{2m_e c^2 \beta^2 \gamma^2}{I_0}) - \beta^2 )
$$


`double getDeDxConstant () const`  Returns the constant, $K$, which depends upon the material properties (mostly the density).

`double getDeDxI0 () const`  Returns the effective ionization potential $I_0$, which is a property of the material.

double getDeDxMin () const  Returns an approximation for the ionization of a minimum ionizing particle ($\beta\gamma=3.4$), given by: $K \times 11.528$.




[^gmat1]: The density is normally specified using `GeoModel` units, ported from CLHEP[^gmat2].  The native unit of mass is the MeV, the native unit for length is the mm.  A material with a density of 1 g/cm3 has a density of 1.7 x 10-22, in these units.  

[^gmat2]: CLHEP - A Class Library for High Energy Physics,  <https://proj-clhep.web.cern.ch/proj-clhep/>

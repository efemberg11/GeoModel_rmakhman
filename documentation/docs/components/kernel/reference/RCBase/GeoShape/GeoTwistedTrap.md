
### GeoTwistedTrap

```cpp
//=== GeoTwistedTrap ===

  // Constructor:
  GeoTwistedTrap(double  PhiTwist,   // twist angle
                 double  Dz,     // half z length
                 double  Theta,  // direction between end planes
                 double  Phi,    // defined by polar and azim. angles
                 double  Dy1,    // half y length at -pDz
                 double  Dx1,    // half x length at -pDz,-pDy
                 double  Dx2,    // half x length at -pDz,+pDy
                 double  Dy2,    // half y length at +pDz
                 double  Dx3,    // half x length at +pDz,-pDy
                 double  Dx4,    // half x length at +pDz,+pDy
                 double  Alph    // tilt angle
                 )

  // Public Methods:
 double getY1HalfLength() const 
 double getX1HalfLength() const 
 double getX2HalfLength() const 
 double getY2HalfLength() const 
 double getX3HalfLength() const 
 double getX4HalfLength() const 
 double getZHalfLength()  const 
 double getPhiTwist()     const 
 double getTheta()        const 
 double getPhi()          const 
 double getTiltAngleAlpha() const 
```

The `GeoTwistedTrap` class represents a twisted trapezoid.  Two faces at $\pm \Delta z$ (`Dz`)  are parallel to each other and to the $x-y$ plane.  The centers of the faces are offset by a vector whose polar and azimuthal angles respectively are $\theta$ and $\phi$.  At $-\Delta z$, two edges parallel to the $x$-axis are offset by $\pm \Delta y_1$ (`Dy1`) from the face’s center, and these two faces have *half-lengths* of $\Delta x_{1}$ (`Dx1`) and $\Delta x_{2}$ (`Dx2`). The face at $+\Delta z$ are similar:  two edges parallel to the $x$-axis are offset by $\pm \Delta y_2$ (`Dy2`) from the face’s center, and these two faces have *half-lengths* of $\Delta x_{3}$ (`Dx3`) and $\Delta x_{4}$ (`Dx4`).

The face at $+ \Delta z$ is twisted with respect to the one at $- \Delta z$  of an angle defined by $\phi Twist$. The tilt angle $\alpha$ defines the angle with respect to the y axis from the centre of the top and bottom trapezoids. It transforms the two trapezoids from isosceles to scalene. 

The constructor fills the `GeoTwistedTrap` with these values and the accessors return them.

{{ imgutils_image_caption('RCBase/GeoShape/GeoTwistedTrap.png', 
   alt='The GeoTwistedTrap shape', 
   cap='Figure:  GeoTwistedTrap object, representing a twisted trapezoid.',
   urlFix=False) 
}}

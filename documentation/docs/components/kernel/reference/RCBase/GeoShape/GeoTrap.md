
### GeoTrap

```cpp
//=== GeoTrap ===

  // Constructor:
  GeoTrap (double ZHalfLength, double Theta, double Phi, 
           double Dydzn, double Dxdyndzn, double Dxdypdzn,
           double Angleydzn, double Dydzp, double Dxdyndzp, 
           double Dxdypdzp, double Angleydzp)

  // Public Methods:
  double getZHalfLength() const
  double getTheta() const
  double getPhi() const
  double getDydzn() const
  double getDxdyndzn() const
  double getDxdypdzn() const
  double getAngleydzn() const
  double getDydzp() const
  double getDxdyndzp() const
  double getDxdypdzp() const
  double getAngleydzp() const
```

The `GeoTrap` class represents a very general kind of trapezoid.  Two faces at $\pm \Delta z$ (`ZHalfLength`)  are parallel to each other and to the $x-y$ plane.  The centers of the faces are offset by a vector whose polar and azimuthal angles respectively are $\theta$ and $\phi$.  At $-\Delta z$, two edges parallel to the $x$-axis are offset by $\pm \Delta y_n$ (`Dydzn`) from the face’s center, and these two faces have *half-lengths* of $\Delta x_{\Delta y_n \Delta z_n}$ (`Dxdyndzn`) and $\Delta x_{\Delta y_p \Delta z_n}$ (`Dxdypdzn`). The face at $+\Delta z$ are similar:  two edges parallel to the $x$-axis are offset by $\pm \Delta y_p$ (`Dydzp`) from the face’s center, and these two faces have *half-lengths* of $\Delta x_{\Delta y_n \Delta z_p}$ (`Dxdyndzp`) and $\Delta x_{\Delta y_p \Delta z_p}$ (`Dxdypdzp`).

The two edges not parallel to the $x$-axis make an angle of $\alpha_n$ (`Angleydzn`) and $\alpha_p$ (`Angleydzp`) with respect to the $y$-axis on the bottom face (at $-\Delta z$) and the top face (at $+\Delta z$), respectively.

The constructor fills the `GeoTrap` with these values and the accessors return them.


{{ imgutils_image_caption('RCBase/GeoShape/GeoTrap.png', 
   alt='The GeoTrap shape', 
   cap='Figure: A GeoTrap object, representing a very general kind of trapezoid.',
   urlFix=False) 
}}





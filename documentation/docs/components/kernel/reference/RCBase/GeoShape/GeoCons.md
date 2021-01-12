### GeoCons


```cpp
// === GeoCons ===

   // Constructor:
   GeoCons (double RMin1, double RMin2,   
            double RMax1, double RMax2, 
            double DZ,  double SPhi,  double DPhi)

   // Public Methods:
   double getRMin1() const
   double getRMin2() const
   double getRMax1() const
   double getRMax2() const
   double getDZ() const
   double getSPhi() const
   double getDPhi() const
```

A `GeoCons` represents a cone section positioned with its axis along the $z$ direction, and is specified by a starting value of $\phi$, a total subtended angle in $\phi$, a *half-width* in $z$, and minimum and maximum values of radius at both extremities in $z$.  The constructor specifies the values of these constants, and the accessors return them.


{{ imgutils_image_caption('RCBase/GeoShape/GeoCons.png', 
   alt='The GeoCons shape', 
   cap='Figure: A GeoCons Object, representing a cone section.',
   urlFix=False) 
}}




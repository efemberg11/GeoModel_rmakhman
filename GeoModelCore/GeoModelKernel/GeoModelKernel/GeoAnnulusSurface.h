/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 *
 * Created on: July, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */

#ifndef GEOMODELKERNAL_GEOANNULUSSURFACE_H
#define GEOMODELKERNAL_GEOANNULUSSURFACE_H

#include "GeoModelKernel/GeoVSurfaceShape.h"

class GeoAnnulusSurface : public GeoVSurfaceShape
{
    public:
      
      // Constructor for a GeoAnnulusSurface
      // The default position of the cyllinder center is at (0,0,0)
      // Ox, Oy: the deviation of the focusing point from the default position
      // radius_in: the inner radius of the annulus
      // radius_out: the outer radius of the annulus
      // phi: the span angle of the deviated circular sector, when phi = 2*PI, it is an annulus
      GeoAnnulusSurface(double Ox, double Oy, double radius_in, double radius_out, double phi);

      // Returns the area of the annulus
      // Proposed method: using Monte Carlo method to calculate the area
      //                  it would need isInside() method to check if a point is inside the annulus 
      //double area() const;

      // Returns Ox
      double getOx() const
      {
          return m_Ox;
      }

      // Returns Oy
      double getOy() const
      {
          return m_Oy;
      }

      // Returns the inner radius of the annulus
      double getRadiusIn() const
      {
          return m_radius_in;
      }

      // Returns the outer radius of the annulus
      double getRadiusOut() const
      {
          return m_radius_out;
      }

      // Returns the span angle of the deviated circular sector
      double getPhi() const
      {
          return m_phi;
      }

      // Determine if a point is inside the annulus
      // x, y: the coordinates of the point
      // coming soon!
      //bool isInside(double x, double y) const;

      virtual const std::string & type() const{
        return s_classType;
      }

      virtual ShapeType typeID() const{
        return s_classTypeID;
      }

      void exec (GeoShapeAction *action) const override final;

      virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const override final;
    
    protected:

      virtual ~GeoAnnulusSurface() = default;
      static const std::string s_classType;
      static const ShapeType s_classTypeID;

      double m_Ox{0.0};
      double m_Oy{0.0};
      double m_radius_in{0.0};
      double m_radius_out{0.1};
      double m_phi{2*M_PI};
};

#endif

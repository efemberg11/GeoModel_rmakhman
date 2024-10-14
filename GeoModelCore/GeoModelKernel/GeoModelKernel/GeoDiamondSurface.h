/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 *
 * Created on: Oct, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */


#ifndef GEOMODELKERNEL_GEODIAMONDSURFACE_H
#define GEOMODELKERNEL_GEODIAMONDSURFACE_H

#include "GeoModelKernel/GeoVSurfaceShape.h"

class GeoDiamondSurface : public GeoVSurfaceShape
{
 public:
  //    Constructor for the Trapezoid Surface
  GeoDiamondSurface (double X_bottom_half, double X_mid_half, double X_top_half, double Y_bottom_half, double Y_top_half);

  //    Returns the area of the surface
  double area() const;

  //    Bottom Half Length in the x direction
  double getXbottomHalf () const
  {
      return m_XbottomHalf;
  }

  //    Midddle Half Length in the x direction
  double getXmidHalf () const
  {
    return m_XmidHalf;
  }

  //    Top Half Length in the x direction
  double getXtopHalf () const
  {
    return m_XtopHalf;
  }
  
  //    Bottom Half Length in the y direction
  double getYbottomHalf () const
  {
    return m_YbottomHalf;
  }

  //    Top Half Length in the y direction
  double getYtopHalf () const
  {
    return m_YtopHalf;
  }  

  virtual const std::string & type () const{
     return s_classType;
  }

  virtual ShapeType typeID () const{
     return s_classTypeID;
  }

  //     Executes a GeoShapeAction
  void exec (GeoShapeAction *action) const override final;

 protected:
  virtual ~GeoDiamondSurface() = default;
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;
  double m_XbottomHalf{0.};
  double m_XmidHalf{0.};
  double m_XtopHalf{0.};
  double m_YbottomHalf{0.};
  double m_YtopHalf{0.};
};


#endif

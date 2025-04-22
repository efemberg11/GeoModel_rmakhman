/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Implementation of class GXHepVisUtils                    //
//                                                            //
//  Author: Thomas H. Kittelmann (Thomas.Kittelmann@cern.ch)  //
//  Initial version: November 2008                            //
//  Update: Giorgi Gvaberidze (ggvaberi@cern.ch)              //
//  Update  version: November 2010                            //
//  Truncated to almost zero: J. Boudreau June 2024           //
//                                                            //
////////////////////////////////////////////////////////////////

#include "GXHepVis/GXHepVisUtils.h"

#include "GXHepVis/nodes/SoCons.h"
#include "GXHepVis/nodes/SoGenericBox.h"
#include "GXHepVis/nodes/SoLAr.h"
#include "GXHepVis/nodes/SoPcons.h"
#include "GXHepVis/nodes/SoPolyhedron.h"
#include "GXHepVis/nodes/SoTransparency.h"
#include "GXHepVis/nodes/SoTubs.h"
#include "GXHepVis/nodes/SoTessellated.h"

//____________________________________________________________________
void GXHepVisUtils::initAllCustomClasses()
{
  SoCons::initClass();
  SoGenericBox::initClass();
  SoLAr::initClass();
  SoPcons::initClass();
  SoPolyhedron::initClass();
  SoTransparency::initClass();
  SoTubs::initClass();
  SoTessellated::initClass();
}

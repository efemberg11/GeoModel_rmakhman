/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/


////////////////////////////////////////////////////////////////
//                                                            //
//  Header file for class GXHepVisUtils                      //
//                                                            //
//  Description: Utilities connected to the GXHepVis         //
//               classes.                                     //
//                                                            //
//  Author: Thomas H. Kittelmann (Thomas.Kittelmann@cern.ch)  //
//  Initial version: November 2008                            //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef GXHEPVISUTILS_H
#define GXHEPVISUTILS_H

class SoGroup;
class SoNode;

class GXHepVisUtils {
public:

  static void initAllCustomClasses();//calls ::initClass() on all custom classes.

  
private:

  GXHepVisUtils();
  ~GXHepVisUtils();

};

#endif

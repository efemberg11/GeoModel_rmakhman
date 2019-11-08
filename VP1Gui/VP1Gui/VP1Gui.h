/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////
//                                                         //
//  Header file for class VP1Gui                           //
//                                                         //
//  Author: Thomas Kittelmann <Thomas.Kittelmann@cern.ch>  //
//                                                         //
//  Initial version: April 2007                            //
//                                                         //
/////////////////////////////////////////////////////////////

#ifndef VP1GUI_H
#define VP1GUI_H

//NB: There should never be any Qt (or Athena of course) includes in this file!!!

#include <string>
#include <vector>
#include <cstdint> // for uint64_t (unsigned long long)

class VP1Gui {
public:

  VP1Gui();

  ~VP1Gui();

  //Called in the first algorithm refresh to launch the gui:
  void init();

  //Called every algorithm refresh:
  bool executeNewEvent( const int& run,
			const uint64_t & event,
			const unsigned& triggerType = 0,//When available
			const unsigned& time = 0 //When available (seconds since 1970)
			);
  //Called in algorithm finalise:
  void cleanup();


private:

  VP1Gui(const VP1Gui & );
  VP1Gui & operator= (const VP1Gui & );

  class Imp;
  Imp * m_d;

};

#endif

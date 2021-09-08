/*
    Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

//
//    Use gaudi messaging service when in Athena; use cout when "stand-alone".
//

#ifndef OUTPUTDIRECTOR_H
#define OUTPUTDIRECTOR_H

#include <iostream>
#include <string>

namespace MSG {
  // enum((VERBOSE=1, DEBUG, INFO, WARNING, ERROR, FATAL, ALWAYS))}
  const std::string PKG_NAME("GeoModelXML-");
  const std::string VERBOSE(PKG_NAME + "VERBOSE: ");
  const std::string DEBUG(PKG_NAME + "DEBUG: ");
  const std::string INFO(PKG_NAME + "INFO: ");
  const std::string WARNING(PKG_NAME + "WARNING: ");
  const std::string ERROR(PKG_NAME + "ERROR: ");
  const std::string FATAL(PKG_NAME + "FATAL: ");
  const std::string ALWAYS(PKG_NAME + "ALWAYS: ");
}
// Consider changing this to a longer, less clashing sort of name, in the code
#define msglog std::cout
#define endmsg std::endl

#endif // OUTPUTDIRECTOR_H

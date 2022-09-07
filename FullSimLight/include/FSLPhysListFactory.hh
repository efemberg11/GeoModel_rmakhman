/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/
#ifndef __FSLPhysListFactory_HH__
#define __FSLPhysListFactory_HH__

#include "G4VModularPhysicsList.hh"

#include <string>



class FSLPhysListFactory
{
public:
    FSLPhysListFactory();
    ~FSLPhysListFactory();

    static G4VModularPhysicsList *GetPhysList(const std::string physListNameOrPluginPath);

    static bool GetActivateRegionsFlag() {return fActivateRegionsFlag;}

    static const FSLPhysListFactory* GetInstance();

private:

    static bool fActivateRegionsFlag;
    
    static const FSLPhysListFactory* fgInstance;
};

#endif //__FSLPhysListFactory_HH__
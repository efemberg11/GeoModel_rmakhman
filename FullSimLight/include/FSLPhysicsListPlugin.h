/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/
#ifndef __FSLPhysicsListPlugin_h__
#define __FSLPhysicsListPlugin_h__

class G4VModularPhysicsList;

class FSLPhysicsListPlugin
{
public:
    FSLPhysicsListPlugin()=default;
    ~FSLPhysicsListPlugin()=default;

    virtual G4VModularPhysicsList* GetPhysicsList() const {return nullptr;}
    virtual bool GetActivateRegionsFlag() const {return false;}

};


#endif //__FSLPhysicsListPlugin_h__
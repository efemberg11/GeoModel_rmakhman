#include "FullSimLight/FSLUserActionPlugin.h"
#include <iostream>

#include "TrksRunAction.h"
#include "TrksSteppingAction.h"
#include "TrksTrackingAction.h"
#include "TrksEventAction.h"

class TracksPlugin:public FSLUserActionPlugin {
    
public:
    
    TracksPlugin();

    G4UserRunAction* getRunAction() const;
    G4UserEventAction* getEventAction() const;
    G4UserTrackingAction* getTrackingAction() const;
    G4UserSteppingAction* getSteppingAction() const;
};

TracksPlugin::TracksPlugin()
{

}

G4UserRunAction* TracksPlugin::getRunAction() const
{
    return new TrksRunAction;
}

G4UserEventAction* TracksPlugin::getEventAction() const
{
    return new TrksEventAction;
}

G4UserTrackingAction* TracksPlugin::getTrackingAction() const
{
    return new TrksTrackingAction;
}


G4UserSteppingAction* TracksPlugin::getSteppingAction() const
{
    return new TrksSteppingAction;
}

extern "C" TracksPlugin *createGenerateTracksPlugin() {
    return new TracksPlugin();
}


#ifndef _FSLUSERACTIONPLUGIN_H_
#define _FSLUSERACTIONPLUGIN_H_
class G4UserEventAction;
class G4UserRunAction;
class G4UserSteppingAction;
class G4UserTrackingAction;
class G4UserStackingAction;

class FSLUserActionPlugin {
 
 public:

  // Constructor
  FSLUserActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserActionPlugin()=default;
  
  // Typically a user action may inherit one or more of the five action
  // types.  These routines should be overriden for each one. 
  virtual G4UserEventAction    *getEventAction()    const { return nullptr;}
  virtual G4UserRunAction      *getRunAction()      const { return nullptr;}
  virtual G4UserSteppingAction *getSteppingAction() const { return nullptr;}
  virtual G4UserTrackingAction *getTrackingAction() const { return nullptr;}
  virtual G4UserStackingAction *getStackingAction() const { return nullptr;}

  
 private:
  
  FSLUserActionPlugin (const FSLUserActionPlugin &)=delete;
  FSLUserActionPlugin & operator=(const FSLUserActionPlugin &)=delete;

};


#endif

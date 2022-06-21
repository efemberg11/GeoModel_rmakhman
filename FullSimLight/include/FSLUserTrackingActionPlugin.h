#ifndef _FSLUSERTRACKINGACTIONPLUGIN_H_
#define _FSLUSERTRACKINGACTIONPLUGIN_H_
class G4UserTrackingAction;
class FSLUserTrackingActionPlugin {
 
 public:

  // Constructor
  FSLUserTrackingActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserTrackingActionPlugin()=default;
  
  virtual G4UserTrackingAction *getUserTrackingAction() const=0;
  
 private:
  
  FSLUserTrackingActionPlugin (const FSLUserTrackingActionPlugin &)=delete;

  FSLUserTrackingActionPlugin & operator=(const FSLUserTrackingActionPlugin &)=delete;
};


#endif

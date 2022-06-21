#ifndef _FSLUSERSTEPPINGACTIONPLUGIN_H_
#define _FSLUSERSTEPPINGACTIONPLUGIN_H_
class G4UserSteppingAction;
class FSLUserSteppingActionPlugin {
 
 public:

  // Constructor
  FSLUserSteppingActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserSteppingActionPlugin()=default;
  
  virtual G4UserSteppingAction *getUserSteppingAction() const=0;
  
 private:
  
  FSLUserSteppingActionPlugin (const FSLUserSteppingActionPlugin &)=delete;

  FSLUserSteppingActionPlugin & operator=(const FSLUserSteppingActionPlugin &)=delete;
};


#endif

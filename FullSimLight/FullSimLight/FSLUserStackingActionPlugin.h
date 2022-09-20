#ifndef _FSLUSERSTACKINGACTIONPLUGIN_H_
#define _FSLUSERSTACKINGACTIONPLUGIN_H_
class G4UserStackingAction;
class FSLUserStackingActionPlugin {
 
 public:

  // Constructor
  FSLUserStackingActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserStackingActionPlugin()=default;
  
  virtual G4UserStackingAction *getUserStackingAction() const=0;
  
 private:
  
  FSLUserStackingActionPlugin (const FSLUserStackingActionPlugin &)=delete;

  FSLUserStackingActionPlugin & operator=(const FSLUserStackingActionPlugin &)=delete;
};


#endif

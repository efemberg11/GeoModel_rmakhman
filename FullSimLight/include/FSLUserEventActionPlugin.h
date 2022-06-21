#ifndef _FSLUSEREVENTACTIONPLUGIN_H_
#define _FSLUSEREVENTACTIONPLUGIN_H_
class G4UserEventAction;
class FSLUserEventActionPlugin {
 
 public:

  // Constructor
  FSLUserEventActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserEventActionPlugin()=default;
  
  virtual G4UserEventAction *getUserEventAction() const=0;
  
 private:
  
  FSLUserEventActionPlugin (const FSLUserEventActionPlugin &)=delete;

  FSLUserEventActionPlugin & operator=(const FSLUserEventActionPlugin &)=delete;
};


#endif

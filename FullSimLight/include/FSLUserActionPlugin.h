#ifndef _FSLUSERACTIONPLUGIN_H_
#define _FSLUSERACTIONPLUGIN_H_
class G4VUserActionInitialization;
class FSLUserActionPlugin {
 
 public:

  // Constructor
  FSLUserActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserActionPlugin()=default;
  
  virtual G4VUserActionInitialization *getUserActionInitialization() const=0;
  
 private:
  
  FSLUserActionPlugin (const FSLUserActionPlugin &)=delete;

  FSLUserActionPlugin & operator=(const FSLUserActionPlugin &)=delete;
};


#endif

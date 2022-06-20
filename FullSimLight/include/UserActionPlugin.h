#ifndef _USERACTIONPLUGIN_H_
#define _USERACTIONPLUGIN_H_
class G4VUserActionInitialization;
class UserActionPlugin {

 public:

  // Constructor
  UserActionPlugin()=default;
  
  // Destructor
  virtual ~UserActionPlugin()=default;
  
  virtual const G4VUserActionInitialization *getUserActionInitialization() const=0;
  
 private:
  
  UserActionPlugin (const UserActionPlugin &)=delete;

  UserActionPlugin & operator=(const UserActionPlugin &)=delete;
};


#endif

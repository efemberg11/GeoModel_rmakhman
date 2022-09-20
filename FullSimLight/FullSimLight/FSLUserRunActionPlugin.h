#ifndef _FSLUSERRUNACTIONPLUGIN_H_
#define _FSLUSERRUNACTIONPLUGIN_H_
class G4UserRunAction;
class FSLUserRunActionPlugin {
 
 public:

  // Constructor
  FSLUserRunActionPlugin()=default;
  
  // Destructor
  virtual ~FSLUserRunActionPlugin()=default;
  
  virtual G4UserRunAction *getUserRunAction() const=0;
  
 private:
  
  FSLUserRunActionPlugin (const FSLUserRunActionPlugin &)=delete;

  FSLUserRunActionPlugin & operator=(const FSLUserRunActionPlugin &)=delete;
};


#endif

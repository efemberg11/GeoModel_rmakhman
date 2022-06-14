#ifndef _MAGFIELDPLUGIN_H_
#define _MAGFIELDPLUGIN_H_
class G4MagneticField;
class MagFieldPlugin {

 public:

  // Constructor
  MagFieldPlugin()=default;
  
  // Destructor
  virtual ~MagFieldPlugin()=default;
  
  virtual const G4MagneticField *getField() const=0;
  
 private:
  
  MagFieldPlugin (const MagFieldPlugin &)=delete;

  MagFieldPlugin & operator=(const MagFieldPlugin &)=delete;
};


#endif

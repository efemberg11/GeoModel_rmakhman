# Plugin Support

`fullSimLight` provides the user support with writing plugins by providing the abstract classes which one has to override. These abstract classes serve as a link to interface various `Geant4` functionality with simulation using FullSimLight. Header files containing these abstract classes are automatically installed with FullSimLight installation.

## User Actions & Event Generators
<!---->
The header file which contains the abstract classes to create plugins with User actions is `FSLUserActionPlugin.h`. 


```c++
#ifndef _FSLUSERACTIONPLUGIN_H_
#define _FSLUSERACTIONPLUGIN_H_
class G4UserEventAction;
class G4UserRunAction;
class G4UserSteppingAction;
class G4UserTrackingAction;
class G4UserStackingAction;
class G4VUserPrimaryGeneratorAction;

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
  #A primary generator plugin inherits from this.
  virtual G4VUserPrimaryGeneratorAction *getPrimaryGeneratorAction() const {return nullptr;}

 private:
  
  FSLUserActionPlugin (const FSLUserActionPlugin &)=delete;
  FSLUserActionPlugin & operator=(const FSLUserActionPlugin &)=delete;

};


#endif
```

## Sensitive Detectors
<!---->
The header file which contains the abstract classes to create plugins with Sensitive detectors is `FSLSensitiveDetectorPlugin.h`. 


```c++
#ifndef __FSLSensitiveDetectorPlugin_h__
#define __FSLSensitiveDetectorPlugin_h__
#include <vector>
#include <string>


class G4VSensitiveDetector;


class FSLSensitiveDetectorPlugin {

 public:
  
  // Public type definitions:
  typedef std::vector<std::string>::const_iterator ConstIterator;
  
  // Constructor
  FSLSensitiveDetectorPlugin();

  // Destructor
  virtual ~FSLSensitiveDetectorPlugin();

  // Get the senstive detector
  virtual G4VSensitiveDetector *getSensitiveDetector() const=0;

  // Get the name of the hit collection into which hits go:
  virtual std::string getHitCollectionName() const=0;

  // Get the list of volume to which we attach this senstive detector
  ConstIterator beginLogVolumeNames() const;
  ConstIterator endLogVolumeNames() const;

  // Add a logical volume for this senstive detector:
  void addLogicalVolumeName(const std::string & logVolName);
  
 private:

  std::vector<std::string> m_logVolNames;
    
  
};


inline FSLSensitiveDetectorPlugin::FSLSensitiveDetectorPlugin() {
}

inline FSLSensitiveDetectorPlugin::~FSLSensitiveDetectorPlugin() {
}

inline void FSLSensitiveDetectorPlugin::addLogicalVolumeName(const std::string & logVolName) {
  m_logVolNames.push_back(logVolName);
}

inline FSLSensitiveDetectorPlugin::ConstIterator FSLSensitiveDetectorPlugin::beginLogVolumeNames() const {
  return m_logVolNames.begin();
}

inline FSLSensitiveDetectorPlugin::ConstIterator FSLSensitiveDetectorPlugin::endLogVolumeNames() const {
  return m_logVolNames.end();
}

#endif

```

## Physics List
<!---->
The header file which contains the abstract classes to create plugins with Physics lists is `FSLPhysicsListPlugin.h`. 


```c++
#ifndef __FSLPhysicsListPlugin_h__
#define __FSLPhysicsListPlugin_h__

class G4VModularPhysicsList;

class FSLPhysicsListPlugin
{
public:
    FSLPhysicsListPlugin()=default;
    ~FSLPhysicsListPlugin()=default;

    virtual G4VModularPhysicsList* GetPhysicsList() const {return nullptr;}
    virtual bool GetActivateRegionsFlag() const {return false;}

};


#endif //__FSLPhysicsListPlugin_h__

```


## Magnetic Field
<!---->
The header file which contains the abstract classes to create plugins with Magnetic fields is `MagFieldPlugin.h`. 


```c++
#ifndef _MAGFIELDPLUGIN_H_
#define _MAGFIELDPLUGIN_H_
class G4MagneticField;
class MagFieldPlugin {

 public:

  // Constructor
  MagFieldPlugin()=default;
  
  // Destructor
  virtual ~MagFieldPlugin()=default;
  
  virtual G4MagneticField *getField() =0;
  
 private:
  
  MagFieldPlugin (const MagFieldPlugin &)=delete;

  MagFieldPlugin & operator=(const MagFieldPlugin &)=delete;
};


#endif


```

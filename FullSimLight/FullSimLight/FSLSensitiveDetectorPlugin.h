// ---------------------------------------------------------------//
//                                                                //
//   class FSLSensitiveDetectorPlugin                             //
//                                                                //
//   Joe Boudreau May 2022                                        //
//                                                                //
//   Base class for plugins which are loaded by FullSimLight.     //
//                                                                //
//   Should construct a sensitive detector and provide a list of  //
//   logical volumes to which it should attach.                   //
//                                                                //
//                                                                //
// ---------------------------------------------------------------//
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

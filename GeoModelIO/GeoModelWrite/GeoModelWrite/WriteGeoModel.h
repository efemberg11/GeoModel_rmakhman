/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

/*
 * author: Riccardo.Maria.Bianchi@cern.ch, 2017
 *
 * major updates:
 * - Aug 2018 - R.M.Bianchi
 * - Feb 2019 - R.M.Bianchi
 * - May 2020 - R.M.Bianchi
 * - Aug 2020 - R.M.Bianchi // added support for GeoPublisher
 */

#ifndef GeoModelWrite_WriteGeoModel_H
#define GeoModelWrite_WriteGeoModel_H

// local includes
#include "GeoModelDBManager/GMDBManager.h"

// GeoModel includes
#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoXF.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoDefinitions.h" 

// C++ includes
#include <vector>
#include <string>
#include <unordered_map>

// FWD declarations
class GeoPublisher;

namespace GeoModelIO {

/**
 * \class WriteGeoModel
 *
 * WriteGeoModel acts on objects of the GeoModel tree
 * persitifying them offline.
 */
class WriteGeoModel  : public GeoNodeAction
{
public:
	/**
	 * @brief Constructor
	 *
	 * Constructor sets up connection with db and opens it
	 * @param path - absolute path to db file
	 */
	WriteGeoModel(GMDBManager& db);

	/**
	 * @brief Destructor
	 */
	~WriteGeoModel();

	virtual void handlePhysVol (const GeoPhysVol *vol); //	Handles a physical volume.
	virtual void handleFullPhysVol (const GeoFullPhysVol *vol);
	virtual void handleSerialDenominator (const GeoSerialDenominator *sD); //	Handles a Serial Denominator.
	virtual void handleSerialTransformer (const GeoSerialTransformer *obj);
	virtual void handleTransform (const GeoTransform *);
	virtual void handleNameTag (const GeoNameTag *);

	void saveToDB(GeoPublisher* store = nullptr);
    void saveToDB( std::vector<GeoPublisher*>& vecStores);

private:

	// define copy constructor, needed for the GeoModelAction subclass
	WriteGeoModel(const WriteGeoModel &right);

	// define assignment operator, needed for the GeoModelAction subclass
	WriteGeoModel & operator=(const WriteGeoModel &right);

	void handleVPhysVolObjects(const GeoVPhysVol* vol);
	void handleReferencedVPhysVol (const GeoVPhysVol *vol); // Handles a physical volume referenced by a SerialTrasformer

	void showMemoryMap();

  std::vector<std::string> getParentNode();

	unsigned int storeShape(const GeoShape* shape);
	unsigned int storeMaterial(const GeoMaterial* mat);
	unsigned int storeElement(const GeoElement* el);
	unsigned int storeTranform(const GeoTransform* node);

	unsigned int storeObj(const GeoMaterial* pointer, const std::string &name, const double &density, const std::string &elements);
  unsigned int storeObj(const GeoElement* pointer, const std::string &name, const std::string &symbol, const double &elZ, const double &elA);
  unsigned int storeObj(const GeoShape* pointer, const std::string &type, const std::string &parameters);
  unsigned int storeObj(const GeoLogVol* pointer, const std::string &name, const unsigned int &shapeId, const unsigned int &materialId);
	unsigned int storeObj(const GeoPhysVol* pointer, const unsigned int &logvolId, const unsigned int parentId = 0, const bool isRootVolume = false );
	unsigned int storeObj(const GeoFullPhysVol* pointer, const unsigned int &logvolId, const unsigned int parentId = 0, const bool isRootVolume = false );
  unsigned int storeObj(const GeoSerialDenominator* pointer, const std::string &baseName);
  unsigned int storeObj(const GeoSerialTransformer* pointer, const unsigned int &functionId, const unsigned int &volId, const std::string &volType, const unsigned int &copies);
  unsigned int storeObj(const GeoXF::Function* pointer, const std::string &expression);
	unsigned int storeObj(const GeoTransform* pointer, const std::vector<double> &parameters);
	unsigned int storeObj(const GeoAlignableTransform* pointer, const std::vector<double> &parameters);
  unsigned int storeObj(const GeoNameTag* pointer, const std::string &name);

  unsigned int addRecord(std::vector<std::vector<std::string>>* container, const std::vector<std::string> values) const;
  
  unsigned int addMaterial(const std::string &name, const double &density, const std::string &elements);
  unsigned int addElement(const std::string &name, const std::string &symbol, const double &elZ, const double &elA);
  unsigned int addNameTag(const std::string &name);
	unsigned int addAlignableTransform(const std::vector<double> &params);
	unsigned int addTransform(const std::vector<double> &params);
  unsigned int addFunction(const std::string &expression);
  unsigned int addSerialTransformer(const unsigned int &funcId, const unsigned int &physvolId, const std::string volType, const unsigned int &copies);
  unsigned int addShape(const std::string &type, const std::string &parameters);
  unsigned int addSerialDenominator(const std::string &baseName);
	unsigned int addPhysVol(const unsigned int &logVolId, const unsigned int &parentPhysVolId, const bool &isRootVolume);
	unsigned int addFullPhysVol(const unsigned int &logVolId, const unsigned int &parentPhysVolId, const bool &isRootVolume);
  unsigned int addLogVol(const std::string &name, const unsigned int &shapeId, const unsigned int &materialId);
  void addChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &childId, const unsigned int &parentCopyNumber, const unsigned int &childPos, const std::string &childType, const unsigned int &childCopyN);

	unsigned int getChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &copyN);

  unsigned int setVolumeCopyNumber(const unsigned int& volId, const std::string& volType);
  unsigned int getLatestParentCopyNumber(const unsigned int& parentId, const std::string& parentType);

	void storeChildPosition(const unsigned int& parentId, const std::string& parentType, const unsigned int& childVol, const unsigned int& parentCopyNumber, const unsigned int& childPos, const std::string& childType, const unsigned int& childCopyN);

	bool isAddressStored(const std::string &address);
	void storeAddress(const std::string &address, const unsigned int &id);

  unsigned int getStoredIdFromAddress(const std::string &address);

	std::string getAddressStringFromPointer(const GeoMaterial* pointer);
	std::string getAddressStringFromPointer(const GeoElement* pointer);
	std::string getAddressStringFromPointer(const GeoShape* pointer);
	std::string getAddressStringFromPointer(const GeoLogVol* pointer);
	std::string getAddressStringFromPointer(const GeoPhysVol* pointer);
	std::string getAddressStringFromPointer(const GeoVPhysVol* pointer);
	std::string getAddressStringFromPointer(const GeoSerialDenominator* pointer);
	std::string getAddressStringFromPointer(const GeoSerialTransformer* pointer);
	std::string getAddressStringFromPointer(const GeoXF::Function* pointer);
	std::string getAddressStringFromPointer(const GeoTransform* pointer);
	std::string getAddressStringFromPointer(const GeoNameTag* pointer);

	std::string getQStringFromOss(std::ostringstream &oss);

	std::vector<double> getTransformParameters(GeoTrf::Transform3D); // TODO: to be moved to Eigen (GeoTrf) and to be moved to an Utility class, so we can use it from TransFunctionRecorder as well.
  std::string getShapeParameters(const GeoShape*);

  std::string getGeoTypeFromVPhysVol(const GeoVPhysVol* vol);

  unsigned int getIdFromNodeType(const std::string &nodeType);

  void storePublishedNodes(GeoPublisher* store);
  template <typename TT> void storeRecordPublishedNodes(const TT storeMap, std::vector<std::vector<std::string>>* cachePublishedNodes);

  std::string m_dbpath;
	GMDBManager* m_dbManager;

    // work caches
  std::unordered_map<std::string, unsigned int> m_parentChildrenMap;
  std::unordered_map<std::string, unsigned int> m_volumeCopiesMap;
  std::unordered_map<std::string, unsigned int> m_memMap;
  std::unordered_map<std::string, unsigned int> m_memMap_Tables;

	// keep track of the number of visited tree node
	unsigned int m_len;
	unsigned int m_len_nChild;

    // store True if we have found the Root volume
	bool m_rootVolumeFound;
    
    // store True if we are visiting an unconnected tree 
    // (for example, one used in a GeoSerialTransformer)
	bool m_unconnectedTree;

    // caches for GeoModel nodes to be saved into the DB
  std::vector<std::vector<std::string>> m_logVols;
	std::vector<std::vector<std::string>> m_physVols;
	std::vector<std::vector<std::string>> m_fullPhysVols;
	std::vector<std::vector<std::string>> m_materials;
	std::vector<std::vector<std::string>> m_elements;
	std::vector<std::vector<std::string>> m_transforms;
	std::vector<std::vector<std::string>> m_alignableTransforms;
	std::vector<std::vector<std::string>> m_serialDenominators;
	std::vector<std::vector<std::string>> m_serialTransformers;
	std::vector<std::vector<std::string>> m_functions;
	std::vector<std::vector<std::string>> m_nameTags;
  std::vector<std::vector<std::string>> m_shapes;
  
    // caches for Metadata to be saved into the DB
    std::vector<std::string> m_rootVolume;
	std::vector<std::vector<std::string>> m_childrenPositions;
	std::vector<std::vector<std::string>> m_publishedAlignableTransforms_String;
	std::vector<std::vector<std::string>> m_publishedFullPhysVols_String;

    // cache to store the node that could not have persistified. 
    // Usually, that means that persistification code has not been developed
    // for the particular GeoModel node
  std::vector<std::string> m_objectsNotPersistified;

};

} /* namespace GeoModelIO */
#endif // GeoModelWrite_WriteGeoModel_H

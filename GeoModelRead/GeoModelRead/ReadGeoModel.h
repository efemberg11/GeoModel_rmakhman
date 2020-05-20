/*
 * GeoModelReadIn.h
 *
 *  Created on: May 20, 2016
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 * major updates:
 * - 2019 Feb, R.M.Bianchi
 * - 2020 May, R.M.Bianchi
 */

#ifndef GeoModelRead_ReadGeoModel_H_
#define GeoModelRead_ReadGeoModel_H_


// ****************************************************************
// *** PERSISTIFICATION SETTINGS FOR GeoModel BOOLEAN SHAPES ******
// ****************************************************************
// Note:
// -- The instructions below are needed
// -- to correctly persistify/restore boolean shapes.
// -- They must be declared before including GeoModelKernel files.
// ****************************************************************
// This variable is used by GeoModel boolean shapes to switch ON
// the persistification mode
#define _GeoShapePersistification_On_
// fwd declaration of namespace::class we want to use to persistify
// the GeoModel boolean shapes
namespace GeoModelIO { class ReadGeoModel; }
// the 'Persistifier' class name is used by GeoModel boolean shapes
// to grant access to friend classes
typedef GeoModelIO::ReadGeoModel Persistifier;
// ****************************************************************
// ****************************************************************



#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelKernel/GeoXF.h"

// C++ includes
#include <string>
#include <set>
#include <tuple>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>


// FWD declarations
class GeoVPhysVol;
class GeoPhysVol;
class GeoFullPhysVol;
class GeoLogVol;
class GeoShape;
class GeoMaterial;
class GeoElement;
class GeoNameTag;
class GeoSerialDenominator;
class GeoAlignableTransform;
class GeoTransform;
class GeoSerialTransformer;
class GeoGraphNode;
class GeoShapeSubtraction;
class GeoBox;


using namespace GeoGenfun;
using namespace GeoXF;



typedef const Function & TRANSFUNCTION;
// containers for boolean shapes' information
typedef std::tuple<unsigned int/*shape ID*/, GeoShape*, unsigned int/*opA ID*/, unsigned int/*opB ID*/> tuple_shapes_boolean_info;
typedef std::vector<tuple_shapes_boolean_info> type_shapes_boolean_info;


namespace GeoModelIO {

class ReadGeoModel {
public:

	ReadGeoModel(GMDBManager* db, unsigned long* progress = nullptr);
	virtual ~ReadGeoModel();

	GeoPhysVol* buildGeoModel();

private:

  void buildAllShapes();
  void buildAllElements();
  void buildAllMaterials();
  void buildAllLogVols();
  void buildAllPhysVols();
  void buildAllFullPhysVols();
  //  void buildAllFunctions(); // FIXME:
  void buildAllTransforms();
  void buildAllAlignableTransforms();
  void buildAllSerialDenominators();
  void buildAllSerialTransformers();
  void buildAllNameTags();

  
  std::string getEnvVar( std::string const & key ) const;

	GeoPhysVol* buildGeoModelByCalls();
	GeoPhysVol* buildGeoModelOneGo();

  GeoBox* buildDummyShape();

  void loopOverAllChildrenInBunches();
  void loopOverAllChildrenInBunchesNew();
  
	void loopOverAllChildren(QStringList keys);
	void processParentChildren(const QString &parentKey);
	void processChild(GeoVPhysVol* parentVol, bool &isRootVolume, const QStringList &child);
  
  void loopOverAllChildrenRecords(std::vector<std::vector<std::string>> records);
  void processParentChild(const std::vector<std::string> &parentchild);

	GeoPhysVol* getRootVolume();

	GeoVPhysVol* parseChildren(GeoVPhysVol* vol, QMap<unsigned int, QStringList> children, int depth = 0);

  GeoVPhysVol* buildVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber);
  GeoVPhysVol* buildNewVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber);
  GeoVPhysVol* buildActualVPhysVol(const unsigned int id, const unsigned int tableId, unsigned int logVol_ID=0);

	GeoLogVol* buildLogVol(const unsigned int id);
	GeoShape* buildShape(const unsigned int id, type_shapes_boolean_info* shapes_info_sub);
	GeoMaterial* buildMaterial(const unsigned id);
	GeoElement* buildElement(const unsigned int id);
	GeoAlignableTransform* buildAlignableTransform(const unsigned int id);
	GeoTransform* buildTransform(const unsigned int id);
	GeoSerialTransformer* buildSerialTransformer(const unsigned int id);
	TRANSFUNCTION buildFunction(const unsigned int id);


  void checkNodePtr(GeoGraphNode* nodePtr, std::string varName="", std::string funcName="", std::string funcSignature="");//TODO: to be moved to an utility class
  void volAddHelper(GeoVPhysVol* vol, GeoGraphNode* volChild);

  // methods for shapes
  std::string getShapeType(const unsigned int shapeId);
  bool isShapeOperator(const unsigned int shapeId);
  bool isShapeOperator(const std::string type);
  bool isShapeBoolean(const unsigned int shapeId);
  bool isShapeBoolean(const std::string type);
  void createBooleanShapeOperands(type_shapes_boolean_info* shapes_info_sub);
  std::pair<unsigned int, unsigned int> getBooleanShapeOperands(const unsigned int shape);
  GeoShape* addEmptyBooleanShapeForCompletion(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub);
  GeoShape* getBooleanReferencedShape(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub);


  // caching methods
  bool isBuiltShape(const unsigned int id);
  void storeBuiltShape(const unsigned int, GeoShape* node);
  GeoShape* getBuiltShape(const unsigned int id);

  bool isBuiltTransform(const unsigned int id);
  void storeBuiltTransform(const unsigned int, GeoTransform* node);
  GeoTransform* getBuiltTransform(const unsigned int id);
  
  bool isBuiltAlignableTransform(const unsigned int id);
  void storeBuiltAlignableTransform(const unsigned int, GeoAlignableTransform* node);
  GeoAlignableTransform* getBuiltAlignableTransform(const unsigned int id);

  bool isVPhysVolBuilt(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber);
  void storeVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber, GeoGraphNode* node);
  GeoGraphNode* getVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber);

  bool isBuiltLog(const unsigned int id);
  void storeBuiltLog(const unsigned int id, GeoLogVol* nodePtr);
  GeoLogVol* getBuiltLog(const unsigned int id);

  bool isBuiltMaterial(const unsigned int id);
  void storeBuiltMaterial(const unsigned int id, GeoMaterial* nodePtr);
  GeoMaterial* getBuiltMaterial(const unsigned int id);

  bool isBuiltElement(const unsigned int id);
  void storeBuiltElement(const unsigned int id, GeoElement* nodePtr);
  GeoElement* getBuiltElement(const unsigned int id);

  bool isBuiltFunc(const unsigned int id);
  void storeBuiltFunc(const unsigned int id, TRANSFUNCTION nodePtr);
  TRANSFUNCTION getBuiltFunc(const unsigned int id);

  bool isBuiltPhysVol(const unsigned int id);
  void storeBuiltPhysVol(const unsigned int id, GeoPhysVol* nodePtr);
  GeoPhysVol* getBuiltPhysVol(const unsigned int id);

  bool isBuiltFullPhysVol(const unsigned int id);
  void storeBuiltFullPhysVol(const unsigned int id, GeoFullPhysVol* nodePtr);
  GeoFullPhysVol* getBuiltFullPhysVol(const unsigned int id);

  bool isBuiltSerialDenominator(const unsigned int id);
  void storeBuiltSerialDenominator(const unsigned int id, GeoSerialDenominator* nodePtr);
  GeoSerialDenominator* getBuiltSerialDenominator(const unsigned int id);

  bool isBuiltNameTag(const unsigned int id);
  void storeBuiltNameTag(const unsigned int id, GeoNameTag* nodePtr);
  GeoNameTag* getBuiltNameTag(const unsigned int id);

  bool isBuiltSerialTransformer(const unsigned int id);
  void storeBuiltSerialTransformer(const unsigned int id, GeoSerialTransformer* nodePtr);
  GeoSerialTransformer* getBuiltSerialTransformer(const unsigned int id);


  // Utility functions
	void checkInputString(QString input);
  std::vector<std::string> splitString(const std::string& s, char delimiter);
	void printTrf(GeoTrf::Transform3D t);
	void printTransformationValues(QStringList t);
	QList<double> convertQstringListToDouble(QStringList listin);
  std::vector<std::string> toStdVectorStrings(QStringList qlist);
  QStringList toQStringList(std::vector<std::string> vec);
  void printStdVectorStrings(std::vector<std::string> vec); //TODO: move it to utility class


	// input arguments
	std::string _dbName;
	GMDBManager* m_dbManager;
	bool m_deepDebug;
	bool m_debug;
  bool m_timing;
  bool m_runMultithreaded;
  int m_runMultithreaded_nThreads;

	// callback handles
	unsigned long* m_progress;

	// data containers
	QHash<QString, QMap<unsigned int, QStringList>> m_allchildren; // key = "parentId:parentTable", item = list of children parameters, inserted by child position
//  std::map<std::string, std::>
  
  QHash<unsigned int, QStringList> m_functions;

  std::vector<std::vector<std::string>> m_physVolsStd;
  std::vector<std::vector<std::string>> m_fullPhysVolsStd;
  std::vector<std::vector<std::string>> m_transformsStd;
  std::vector<std::vector<std::string>> m_alignableTransformsStd;
  std::vector<std::vector<std::string>> m_serialDenominatorsStd;
  std::vector<std::vector<std::string>> m_serialTransformersStd;
  std::vector<std::vector<std::string>> m_nameTags;
  std::vector<std::vector<std::string>> m_logVols;
  std::vector<std::vector<std::string>> m_materials;
  std::vector<std::vector<std::string>> m_elements;
  std::vector<std::vector<std::string>> m_shapes;
  //  std::vector<std::vector<std::string>> m_functions; // FIXME:
  
  std::vector<std::vector<std::string>> m_allchildrenStd;
  
	QHash<unsigned int, QString> m_tableID_toTableName; // to look for node's type name starting from a table ID
	QHash<QString, unsigned int> m_tableName_toTableID; // to look for table ID starting from node's type name

	QStringList m_root_vol_data;

  QHash<QString, GeoGraphNode*> m_memMap; // FIXME: move to std::
//  std::unordered_map<unsigned int, GeoShape*> m_memMapShapes; // FIXME: move all these to vectors
//  std::unordered_map<unsigned int, GeoTransform*> m_memMapTransforms;
//  std::unordered_map<unsigned int, GeoLogVol*> m_memMapLogs;
//  std::unordered_map<unsigned int, GeoMaterial*> m_memMapMats;
//  std::unordered_map<unsigned int, GeoElement*> m_memMapEls;
	// std::unordered_map<unsigned int, GeoPhysVol*> m_memMapPhysVols;
	// std::unordered_map<unsigned int, GeoFullPhysVol*> m_memMapFullPhysVols;
	// std::unordered_map<unsigned int, TRANSFUNCTION> m_memMapFuncs;

  std::vector<GeoPhysVol*> m_memMapPhysVols;
  std::vector<GeoFullPhysVol*> m_memMapFullPhysVols;
  std::vector<GeoTransform*> m_memMapTransforms;
  std::vector<GeoAlignableTransform*> m_memMapAlignableTransforms;
  std::vector<GeoSerialDenominator*> m_memMapSerialDenominators;
  std::vector<GeoSerialTransformer*> m_memMapSerialTransformers;
  std::vector<GeoNameTag*> m_memMapNameTags;
  std::vector<GeoLogVol*> m_memMapLogVols;
  std::vector<GeoMaterial*> m_memMapMaterials;
  std::vector<GeoElement*> m_memMapElements;
  std::vector<GeoShape*> m_memMapShapes;
  //  std::vector<TRANSFUNCTION> m_memMapFunctions; // FIXME:

  
  std::set<std::string> m_unknown_shapes;

};

} /* namespace GeoModelIO */
#endif /* GeoModelRead_ReadGeoModel_H_ */

/*
 * GeoModelReadIn.h
 *
 *  Created on: May 20, 2016
 *      Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 * major updates: Feb 2019 rbianchi
 */

#ifndef GeoModelRead_ReadGeoModel_H_
#define GeoModelRead_ReadGeoModel_H_

#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelKernel/GeoXF.h"

// C++ includes
#include <string>
#include <set>
#include <mutex>


// FWD declarations
class GeoVPhysVol;
class GeoPhysVol;
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

using namespace GeoGenfun;
using namespace GeoXF;

typedef const Function & TRANSFUNCTION;


namespace GeoModelIO {

class ReadGeoModel {
public:

	ReadGeoModel(GMDBManager* db, unsigned long* progress = nullptr);
	virtual ~ReadGeoModel();

	// void printRecords();
	GeoPhysVol* buildGeoModel();

private:

	GeoPhysVol* buildGeoModelByCalls();
	GeoPhysVol* buildGeoModelOneGo();

	// GeoPhysVol* loopOverAllChildren(QStringList keys);
	void loopOverAllChildren(QStringList keys);
	void processParentChildren(const QString &parentKey);
	void processChild(GeoVPhysVol* parentVol, bool &isRootVolume, const QStringList &child, std::mutex &mux);

	// GeoPhysVol* loopOverAllChildrenInBunches(); 
	void loopOverAllChildrenInBunches(); 
	// void loopOverChildrenKeys(const unsigned int &start, const unsigned int &end); 


	GeoPhysVol* getRootVolume();

	GeoVPhysVol* parseChildren(GeoVPhysVol* vol, QMap<unsigned int, QStringList> children, int depth = 0);
	GeoVPhysVol* parseVPhysVol(QStringList values, QString nodeType, int depth = 0);

	GeoVPhysVol* buildVPhysVol(QString id, QString tableId, QString copyNumber, std::mutex &mux);
	GeoVPhysVol* buildNewVPhysVol(QString id, QString tableId, QString copyN, std::mutex &mux);

	GeoLogVol* buildLogVol(QString id);
	GeoShape* buildShape(QString id);
	GeoMaterial* buildMaterial(QString id);
	GeoElement* buildElement(QString id);
	GeoSerialDenominator* parseSerialDenominator(QStringList values);
	GeoSerialDenominator* buildSerialDenominator(QString id);
	GeoAlignableTransform* parseAlignableTransform(QStringList values);
	GeoAlignableTransform* buildAlignableTransform(QString id);
	GeoTransform* parseTransform(QStringList values);
	GeoTransform* buildTransform(QString id);
	GeoSerialTransformer* parseSerialTransformer(QStringList values);
	GeoSerialTransformer* buildSerialTransformer(QString id, std::mutex &mux);
	TRANSFUNCTION parseFunction(const std::string& expr);
	TRANSFUNCTION buildFunction(QString id);
	GeoNameTag* parseNameTag(QStringList values);
	GeoNameTag* buildNameTag(QString id);

	GeoShape* getShape(QString id);

	void volAddHelper(GeoVPhysVol* vol, GeoGraphNode* volChild);

	bool isNodeBuilt(const QString id, const QString tableId, const QString copyNumber);
	GeoGraphNode* getNode(const QString id, const QString tableId, const QString copyNumber);
	void storeNode(const QString id, const QString tableId, const QString copyNumber, GeoGraphNode* node, std::mutex &mux);

	void checkInputString(QString input);

	void printTrf(GeoTrf::Transform3D t);
	void printTransformationValues(QStringList t);
	QList<double> convertQstringListToDouble(QStringList listin);


	// input arguments
	std::string _dbName;
//	GeoPhysVol* _root;
	GMDBManager* m_dbManager;
	bool m_deepDebug;

	// callback handles
	unsigned long* m_progress;

	// data containers
	QHash<QString, QMap<unsigned int, QStringList>> m_allchildren; // key = "parentId:parentTable", item = list of children parameters, inserted by child position
	QHash<unsigned int, QStringList> m_physVols;
	QHash<unsigned int, QStringList> m_fullPhysVols;
	QHash<unsigned int, QStringList> m_logVols;
	QHash<unsigned int, QStringList> m_shapes;
	QHash<unsigned int, QStringList> m_materials;
	QHash<unsigned int, QStringList> m_elements;
	QHash<unsigned int, QStringList> m_transforms;
	QHash<unsigned int, QStringList> m_alignableTransforms;
	QHash<unsigned int, QStringList> m_serialDenominators;
	QHash<unsigned int, QStringList> m_serialTransformers;
	QHash<unsigned int, QStringList> m_functions;
	QHash<unsigned int, QStringList> m_nameTags;

	QHash<unsigned int, QString> m_tableid_tableName;

	QStringList m_root_vol_data;

	QHash<QString, GeoGraphNode*> m_memMap;

	std::set<std::string> m_unknown_shapes;

};

} /* namespace GeoModelIO */
#endif /* GeoModelRead_ReadGeoModel_H_ */

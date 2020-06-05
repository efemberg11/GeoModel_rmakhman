// author: Riccardo.Maria.Bianchi@cern.ch - 2017
// major updates: Aug 2018

#ifndef GMDBManager_H
#define GMDBManager_H

//#include "GeoModelKernel/GeoNodeAction.h"
//#include "GeoModelKernel/GeoGraphNode.h"

// include Qt
#include <QSqlDatabase>
#include <QStringList>
#include <QVariant>
#include <QString>
#include <QMap>

#include <sqlite3.h>


// include C++
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

/**
 * \class GMDBManager
 *
 * \brief SQL Database Manager class for GeoModel
 *
 * GMDBManager sets up the connection with SQL database
 * and performs some basics queries.
 */
class GMDBManager
{
public:
	/**
	 * @brief Constructor
	 *
	 * Constructor sets up connection with db and opens it
	 * @param path - absolute path to db file
	 */
  GMDBManager(const std::string &path);
//  GMDBManagerStd(const std::string &path);

	/**
	 * @brief Destructor
	 *
	 * Close the db connection
	 */
	~GMDBManager();
  void DestructorGMDBManagerStd();


	/**
	 * @brief Returns true if the DB is correctly open
	 */
  bool isOpen() const;

	/**
	 * @brief Print names of all GeoPhysVol objects in db
	 */
	void printAllPhysVols() const;

	/**
	 * @brief Print names of all GeoFullPhysVol objects in db
	 */
	void printAllFullPhysVols() const;

	/**
	 * @brief Print names of all GeoLogVol objects in db
	 */
	void printAllLogVols() const;

	/**
	 * @brief Print names of all GeoMaterial objects in db
	 */
	void printAllMaterials() const;

	/**
	 * @brief Print names of all GeoElement objects in db
	 */
	void printAllElements() const;

	/**
	 * @brief Print names of all GeoShape objects in db
	 */
	void printAllShapes() const;

	/**
	 * @brief Print names of all GeoSerialDenominators objects in db
	 */
	void printAllSerialDenominators() const;


	/**
	 * @brief Print names of all GeoSerialDenominators objects in db
	 */
	void printAllSerialTransformers() const;

	/**
	 * @brief Print names of all Function objects in db
	 */
	void printAllFunctions() const;

	/**
	 * @brief Print names of all GeoTransform objects in db
	 */
	void printAllTransforms() const;

	/**
	 * @brief Print names of all GeoAlignableTransform objects in db
	 */
	void printAllAlignableTransforms() const;

	/**
	 * @brief Print names of all GeoNameTag objects in db
	 */
	void printAllNameTags() const;

	/**
	 * @brief Print the db table storing all the children positions per parent
	 */
	void printAllChildrenPositions() const;

	/**
	 * @brief Print the db table storing all the children positions per parent
	 */
	void printAllNodeTypes() const;

	/**
	 * @brief Print the DB table storing the ID of the PhysVol that is the Root volume
	 */
	void printRootVolumeId() const;

	/**
	 * @brief Print the current version number of the DB schema
	 */
	void printDBVersion() const;
  
  
  
  void printAllDBTables();
  int execQuery(std::string queryStr);



	// ADD methods
  /* single-item adders are not efficient, so they are not used at the moment. We might want to enable them again later if we'll have specific use-cases.
	QVariant addShape(const QString &type, const QString &parameters);
	QVariant addMaterial(const QString &name, const QString &density, const QString &elements);
	QVariant addElement(const QString &name, const QString &symbol, const QString &elZ, const QString &elA);
	QVariant addLogVol(const QString &name, const QVariant &shapeId, const QVariant &materialId);
	QVariant addPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume = false);
	QVariant addFullPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume = false);
	QVariant addSerialDenominator(const QString &baseName);
	QVariant addFunction(const QString expression);
  QVariant addSerialTransformer(const unsigned int &funcId, const unsigned int &physvolId, const std::string &volType, const unsigned int &copies);
	QVariant addTransform(QVector<double> parameters);
	QVariant addAlignableTransform(QVector<double> parameters);
	QVariant addNameTag(const QString &name);
   */

//  bool addListOfRecords(const QString geoType, const std::vector<QStringList> records);
  bool addListOfRecords(const std::string geoType, const std::vector<std::vector<std::string>> records);
  
//  bool addListOfRecordsToTable(const QString tableName, const std::vector<QStringList> records);
  bool addListOfRecordsToTableStd(const std::string tableName, const std::vector<std::vector<std::string>> records);
  bool addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::string>> records);
  
//  bool addListOfRecordsToTableOld(const QString tableName, const std::vector<QStringList> records); // for the old SQlite only
  bool addListOfChildrenPositions(const std::vector<std::vector<std::string>> &records);

	bool addRootVolume(const std::vector<std::string> &values);
  void addChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &childId, const unsigned int &parentCopyNumber, const unsigned int &childPos, const std::string &childType, const unsigned int &childCopyN);

  
	void addDBversion(const QString);
  void addDBversion(std::string version);


	// GET methods

  std::string getDBFilePath();

  std::vector<std::string> getRootPhysVol();

  std::vector<std::string> getItem(std::string geoType, unsigned int id);
  std::vector<std::string> getItem(unsigned int tableId, unsigned int id);
  
	
  std::vector<std::string> getItemFromTableName(std::string tableName, unsigned int id);
  
  std::vector<std::string> getItemAndType(unsigned int tableId, unsigned int id);

  std::string getNodeTypeFromTableId(unsigned int id);

  QMap<unsigned int, QStringList> getVPhysVolChildren(const unsigned int &id, const std::string &nodeType, const unsigned int &copyN);

	/// methods to dump the DB
	QHash<QString, QMap<unsigned int, QStringList>> getChildrenTable();
  std::vector<std::vector<std::string>> getChildrenTableStd();

//  QHash<unsigned int, QStringList> getTableFromNodeType(QString nodeType);
  std::vector<std::vector<std::string>> getTableFromNodeType(std::string nodeType);
  
  std::unordered_map<unsigned int, std::string> getAll_TableIDsNodeTypes();
  std::unordered_map<std::string, unsigned int> getAll_NodeTypesTableIDs();
  

private:

	bool initDB();
	bool createTables();
  bool createTablesStd();
  
  void checkIsDBOpen();


//  void loadTestData(); // for debug only

	void loadTableNamesFromDB();
  std::vector<std::string> getTableColNamesFromDB(std::string tableName) const;

  std::string getTableNameFromTableId(unsigned int tabId);
  
  unsigned int getTableIdFromNodeType(const std::string &nodeType);
  void storeNodeType(std::string nodeType, std::string tableName);
	
  QString getTableNameFromNodeType(QString nodeType); // TODO: to be removed
  std::string getTableNameFromNodeType(std::string nodeType);

  QSqlQuery selectAllFromTable(std::string tableName) const;
  QSqlQuery selectAllFromTableSortBy(std::string tableName, std::string sortColumn="") const;

  void storeTableColumnNames(std::vector<std::string> input);
  
  std::vector<std::string> getTableColumnNames(const std::string &tableName);
//  void printTableColNamesFromDB(const std::string &tableName) const;

  void printAllRecords(const std::string &tableName) const;

  int getTableColIndex(const std::string &tableName, const std::string &colName);

  bool storeRootVolume(const unsigned int &id, const std::string &nodeType);

	void showError(const QSqlError &err) const;

  std::string m_dbpath;
	
  QSqlDatabase m_db;
  /// Pointer to SQLite connection
  sqlite3* m_dbSqlite;
  /// Variable to store error messages from SQLite
  char *m_SQLiteErrMsg;
  
  
	bool m_dbIsOK;

	bool m_deepDebug;

  std::unordered_map<std::string, std::vector<std::string>> m_tableNames; /// stores the column names for each table
  std::unordered_map<std::string, std::string> m_childType_tableName;
};

#endif // GMDBManager_H

// author: Riccardo.Maria.Bianchi@cern.ch - 2017
// major updates: 
// - Aug 2018, R.M.Bianchi
// - Nov 2020, R.M.Bianchi

#ifndef GMDBManager_H
#define GMDBManager_H

// include C++
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <typeindex> // std::type_index, needs C++11


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

  bool initDB();

  int loadGeoNodeTypesAndBuildCache();


	/**
	 * @brief Returns true if the DB is correctly open
	 */
//  bool isOpen() const;
  bool checkIsDBOpen() const;

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
	 * @brief Print the db table storing all the 'published' GeoVFullPhysVol nodes.
     * @details By default, the method prints out the table 'PublishedGeoFullPhysVols'.
     * @param suffix If 'suffix' is provided, then the table 'PublishedGeoFullPhysVols-suffix' will be printed.
     * @param suffix Optional parameter. If 'suffix' is provided, then the table 'PublishedGeoFullPhysVols-suffix' will be printed. Of course, developers must be first checked the related DB table has been created (e.g., through the tools offered by GeoModelKernel::GeoVStore and its implementation).
     * @note The 'suffix' parameter is optional. If not provided, the default table will be printed.
	 */
	void printAllPublishedFullPhysVols(const std::string suffix = "") const;
	
	/**
	 * @brief Print the db table storing all the 'published' GeoAlignableTransform nodes
     * @details By default, the method prints out the table 'GeoAlignableTransforms'.
     * @param suffix Optional parameter. If 'suffix' is provided, then the table 'GeoAlignableTransforms-suffix' will be printed. Of course, developers must be first checked the related DB table has been created (e.g., through the tools offered by GeoModelKernel::GeoVStore and its implementation).
     * @note The 'suffix' parameter is optional. If not provided, the default table will be printed.
	 */
	void printAllPublishedAlignableTransforms(const std::string suffix = "") const;
	
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

  void printAllRecords(const std::string &tableName) const;

  void printAllDBTables();

  void createTableDataCaches();
  void getAllDBTables();
  void getAllDBTableColumns();

  int execQuery(std::string queryStr);


  bool addListOfRecords(const std::string geoType, const std::vector<std::vector<std::string>> records);


  bool addListOfChildrenPositions(const std::vector<std::vector<std::string>> &records);
  
  /**
   * @brief Save the list of 'published' GeoAlignableTransform nodes to the DB.
   * @details The method gets a list of records and stores them in the default table 'PublishedAlignableTransforms".
   * @parameter suffix Optional parameter. If provided, the list of records will be saved in a new table named 'PublishedAlignableTransforms-suffix'.
     * @note The 'suffix' parameter is optional. If not provided, the records will be saved in the default table.
   */
  bool addListOfPublishedAlignableTransforms(const std::vector<std::vector<std::string>> &records, std::string suffix = "");
  
  /**
   * @brief Save the list of 'published' GeoVFullPhysVol nodes to the DB.
   * @details The method gets a list of records and stores them in the default table 'PublishedFullPhysVols".
   * @parameter suffix Optional parameter. If provided, the list of records will be saved in a new table named 'PublishedFullPhysVols-suffix'.
     * @note The 'suffix' parameter is optional. If not provided, the records will be saved in the default table.
   */
  bool addListOfPublishedFullPhysVols(const std::vector<std::vector<std::string>> &records, std::string suffix = "");

	bool addRootVolume(const std::vector<std::string> &values);

	// GET methods

  std::string getDBFilePath();

  std::vector<std::string> getRootPhysVol();


  std::vector<std::string> getItemFromTableName(std::string tableName, unsigned int id);

  std::vector<std::string> getItemAndType(unsigned int tableId, unsigned int id);

  std::string getNodeTypeFromTableId(unsigned int id);


	/// methods to dump the DB
  std::vector<std::vector<std::string>> getChildrenTable();
  
  std::vector<std::vector<std::string>> getPublishedFPVTable( std::string suffix = "" );
  std::vector<std::vector<std::string>> getPublishedAXFTable( std::string suffix = "" );

  std::vector<std::vector<std::string>> getTableFromNodeType(std::string nodeType);

  std::unordered_map<unsigned int, std::string> getAll_TableIDsNodeTypes();
  std::unordered_map<std::string, unsigned int> getAll_NodeTypesTableIDs();

  std::vector<std::vector<std::string>> getTableRecords(std::string tableName) const; // TODO: should be private?

  /**
   * @brief Create a custom DB table to store auxiliary data.
   * @param tableName The name of the custom table
   * @param tableColNames A vector of strings defining the names of the table's columns
   * @param tableColTypes A vector of strings defining the types of the table's columns
   * @param records A vector of strings storing the table's rows' data
   */
  bool createCustomTable(const std::string tableName, const std::vector<std::string> tableColNames, const std::vector<std::string> tableColTypes, const std::vector<std::vector<std::string>> &records );
  bool createCustomTable(const std::string tableName, const std::vector<std::string> tableColNames, const std::vector<std::string> tableColTypes, const std::vector<std::vector<std::variant<int,long,float,double,std::string>>> &records );



private:

  /**
   * @brief Create all the default DB tables.
   */
	bool createTables();

  /**
   * @brief Create a custom DB table to store a list of published nodes. 
   * @param tableName The table's name.
   * @param keyType The type of the 'key' that identifies the linked node. 
   */
  bool createTableCustomPublishedNodes(const std::string tableName, const std::string nodeType, const std::type_info* keyType);
  
  bool addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::string>> records);
  bool addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::variant<int,long,float,double,std::string>>> records);
//  bool addListOfRecordsToTableOld(const QString tableName, const std::vector<QStringList> records); // for the old SQlite only

  void addDBversion(std::string version);



//  void loadTestData(); // for debug only

  std::string getTableNameFromTableId(unsigned int tabId);

  unsigned int getTableIdFromNodeType(const std::string &nodeType);
  void storeNodeType(std::string nodeType, std::string tableName);

  std::string getTableNameFromNodeType(const std::string &nodeType);

  void storeTableColumnNames(std::vector<std::string> input);

  std::vector<std::string> getTableColumnNames(const std::string &tableName);


  int getTableColIndex(const std::string &tableName, const std::string &colName);

  bool storeRootVolume(const unsigned int &id, const std::string &nodeType);

  std::string m_dbpath;

  bool m_dbIsOK;

  bool m_debug;

  std::unordered_map<std::string, std::vector<std::string>> m_tableNames; /// stores the column names for each table
  std::unordered_map<std::string, std::string> m_childType_tableName;

  std::vector<std::string> m_cache_tables; /// cache for the list of tables in the DB
  std::unordered_map<unsigned int, std::string> m_cache_tableId_tableName; /// cache for tableID-->tableName
  std::unordered_map<unsigned int, std::string> m_cache_tableId_nodeType; /// cache for tableID-->nodeType
  std::unordered_map<std::string, std::string> m_cache_nodeType_tableName; /// cache for nodeType-->tableName
  std::unordered_map<std::string, unsigned int> m_cache_nodeType_tableID; /// cache for nodeType-->tableID

protected:
  class Imp;
  Imp * m_d;

};


#endif // GMDBManager_H

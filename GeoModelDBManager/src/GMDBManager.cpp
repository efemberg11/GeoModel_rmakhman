
/*
* author: Riccardo.Maria.Bianchi@cern.ch - 2017
* major updates:
*  - Aug 2018, R.M.Bianchi
*  - May 2020, R.M.Bianchi
*/

#include <GeoModelDBManager/GMDBManager.h>

// fmt library
#define FMT_HEADER_ONLY 1
#include "fmt/format.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QDebug>


#include <stdlib.h> /* exit, EXIT_FAILURE */
#include <sstream>



static std::string dbversion = "0.4.0"; // removed "parent" info from [Full]PhysVols tables (May 2020)


// FIXME: move this to utility class/file
std::vector<std::string> toStdVectorStrings(QStringList qlist)
{
  std::vector<std::string> vec;
  foreach(QString qstr, qlist) {
    vec.push_back(qstr.toStdString());
  }
  return vec;
}

// FIXME: should go to an utility class
std::string joinVectorStrings(std::vector<std::string> vec, std::string sep="") {
  std::string s;
  unsigned int ii = 0;
  for (const auto &piece : vec) {
    ++ii;
    if (ii == vec.size()) {
      s += (piece);
    } else {
      s += (piece + sep);
    }
  }
  return s;
}



//GMDBManager::GMDBManager(const std::string &path) : m_dbpath(path), m_SQLiteErrMsg(0), m_dbIsOK(false), m_debug(false)
//{
//  // TODO: move to a more organic way
//  #ifdef GEOREAD_DEEP_DEBUG
//    m_debug = true;
//   #endif
//
//  m_dbIsOK = false;
//
//  m_db = QSqlDatabase::addDatabase("QSQLITE");
//  m_db.setDatabaseName( QString::fromStdString(path));
//
//  if (!m_db.open())
//  {
//    std::cout << "Error: connection with database failed!\n";
//    showError( m_db.lastError() );
//    m_dbIsOK = false;
//  }
//  else
//  {
//    m_dbIsOK = true;
//  }
//
////  // check if DB has tables, if not create them
//  if (m_dbIsOK) {
//    if ( ! (initDB()) ) {
//      std::cout << "Error: database initialization failed" << std::endl;
//      m_dbIsOK = false;
//    }
//  }
//
//  // populate DB with fake data, only for debug
//  //loadTestData(); // TODO: we should move initDB() here, only for debug and Write
//}

//GMDBManager::~GMDBManager()
//{
//  m_db.close();
//}


//bool GMDBManager::isOpen() const
//{
//  if (! m_dbIsOK )
//    return false;
//  return m_db.isOpen();
//}

GMDBManager::GMDBManager(const std::string &path) : m_dbpath(path), m_dbSqlite(nullptr), m_SQLiteErrMsg(0), m_dbIsOK(false), m_debug(false)
{
  // Check if the user asked for running in serial or multi-threading mode
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_DBMANAGER_DEBUG")) {
    m_debug = true;
    std::cout << "You defined the GEOMODEL_IO_DEBUG variable, so you will see a verbose output." << std::endl;
  }

  m_dbIsOK = false;

  // FIXME: TODO: we should check the existence of the file, otherwise SQLite will create a new file from scratch
  // Save the connection result
  int exit = sqlite3_open(path.c_str(), &m_dbSqlite);

  // Test if there was an error
  if (exit == SQLITE_OK) {
    std::cout << "The Geometry Database '"<< path << "' has been opened successfully!" << std::endl;
    m_dbIsOK = true;
  } else {
    std::cout << "DB Open Error: " << sqlite3_errmsg(m_dbSqlite) << std::endl;
    m_dbIsOK = false;
  }

  if (m_dbIsOK) {
    if ( ! (initDB()) ) {
      std::cout << "Error: database initialization failed" << std::endl;
      m_dbIsOK = false;
    }
  }

  // populate DB with fake data, only for debug
  //loadTestData(); // TODO: we should move initDB() here, only for debug and Write
}


GMDBManager::~GMDBManager()
{
  sqlite3_close(m_dbSqlite);
  m_dbSqlite = nullptr;
}


void GMDBManager::printAllMaterials() const
{
	printAllRecords("Materials");
}
void GMDBManager::printAllElements() const
{
	printAllRecords("Elements");
}

void GMDBManager::printAllShapes() const
{
	printAllRecords("Shapes");
}

void GMDBManager::printAllSerialDenominators() const
{
	printAllRecords("SerialDenominators");
}

void GMDBManager::printAllLogVols() const
{
	printAllRecords("LogVols");
}
void GMDBManager::printAllPhysVols() const
{
	printAllRecords("PhysVols");
}
void GMDBManager::printAllFullPhysVols() const
{
	printAllRecords("FullPhysVols");
}
void GMDBManager::printAllSerialTransformers() const
{
	printAllRecords("SerialTransformers");
}
void GMDBManager::printAllFunctions() const
{
	printAllRecords("Functions");
}
void GMDBManager::printAllTransforms() const
{
	printAllRecords("Transforms");
}
void GMDBManager::printAllAlignableTransforms() const
{
	printAllRecords("AlignableTransforms");
}
void GMDBManager::printAllNameTags() const
{
	printAllRecords("NameTags");
}
void GMDBManager::printAllChildrenPositions() const
{
	printAllRecords("ChildrenPositions");
}
void GMDBManager::printAllNodeTypes() const
{
	printAllRecords("GeoNodesTypes");
}
void GMDBManager::printRootVolumeId() const
{
	printAllRecords("RootVolume");
}
void GMDBManager::printDBVersion() const
{
	printAllRecords("dbversion");
}




void GMDBManager::printAllRecords(const std::string &tableName) const
{
  // --- print table name
  std::cout << tableName << " in db:" << std::endl;
  // --- print table column names
  std::cout << "- " <<  joinVectorStrings(m_tableNames.at(tableName), ", ") << std::endl;
  // --- print records
  std::vector<std::vector<std::string>> records;
//  std::vector<std::string> nodeParams;
  records = getTableRecords(tableName);
  for( auto& row : records) {
    std::cout << "* ";
    for( auto& item : row) {
      std::cout << item << ", ";
    }
    std::cout << std::endl;
  }
  std::cout << "---" << endl;
	// TODO: I want to have a symbol like '---' to mean empty line when query gives 0 results.
}


// FIXME: TODO: we now return all records as text, but should we get double/int instead when appropriate? In that case, we should create dedicated methods for all tables, I guess.
// TODO: fill a cache and returns that if asked a second time
std::vector<std::vector<std::string>> GMDBManager::getTableRecords(std::string tableName) const
{
  // container to be returned
  std::vector<std::vector<std::string>> records;
  // get the query statetement ready to be executed
  sqlite3_stmt* stmt = nullptr;
  if ("ChildrenPositions" == tableName) {
    stmt = selectAllFromTableChildrenPositions();
  }
  else {
    stmt = selectAllFromTable(tableName);
  }
  // execute the query and loop over all rows and all columuns
  if ( stmt )
  {
    int ctotal = sqlite3_column_count(stmt); // Count the Number of Columns in the Table
    int res = 0;
    while ( 1 )
    {
      res = sqlite3_step(stmt); // Execute SQL Statement.
      if ( res == SQLITE_ROW )
      {
        std::vector<std::string> nodeParams; // stores the data items contained in a single row
        for ( int i = 0; i < ctotal; i++ )  // Loop times the number of columns in the table
        {
          std::string s = (char*)sqlite3_column_text(stmt, i);  // Read each Column in the row as text FIXME: is there a method to get the right type, e.g. double, instead of text?
          nodeParams.push_back( s );
        }
        records.push_back(nodeParams);
      }
      
      if ( res == SQLITE_DONE || res==SQLITE_ERROR)
      {
        if (res == SQLITE_ERROR) {
          std::string errmsg(sqlite3_errmsg(m_dbSqlite));
          sqlite3_finalize(stmt);
          throw errmsg;
        }
        break;
      }
    }
  }
  // finalize
  sqlite3_finalize(stmt);
  return records;
}


std::vector<std::vector<std::string>> GMDBManager::getTableFromNodeType(std::string nodeType)
{

  std::string tableName = getTableNameFromNodeType(nodeType);
  return getTableRecords(tableName);
  
//  // QHash<unsigned int, QStringList> records;
//  // QStringList nodeParams;
//  std::vector<std::vector<std::string>> records;
//  std::vector<std::string> nodeParams;
//
//  int nCols = (m_tableNames.at(tableName)).size();
//
//  QSqlQuery query = selectAllFromTable(tableName); // sorted by ID
//  while (query.next()) {
//    nodeParams.clear();
////    unsigned int nodeId = query.value(0).toUInt();
//
//    for( int ii=0; ii<nCols; ++ii) {
//      // nodeParams << query.value(ii).toString();
//      nodeParams.push_back( query.value(ii).toString().toStdString() );
//    }
//    // records[nodeId] = nodeParams;
//    records.push_back(nodeParams);
//  }
//return records;
}


void GMDBManager::showError(const QSqlError &err) const
{
	qWarning() << "Unable to initialize Database" << "Error initializing database: " + err.text();
}

//QVariant GMDBManager::addPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume)
//{
//  if (m_debug) std::cout << "GMDBManager::addPhysVol() - is root?" << isRootVolume << std::endl;
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into PhysVols(logvol, parent) values(?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(logVolId);
//  q.addBindValue(parentPhysVolId);
//  q.exec();
//
//  QVariant lastInserted = q.lastInsertId();
//
//  if (isRootVolume) {
//    storeRootVolume(lastInserted.toUInt(), "GeoPhysVol");
//  }
//
//  return lastInserted;
//
//}

//QVariant GMDBManager::addFullPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume)
//{
//  if (m_debug) std::cout << "GMDBManager::addFullPhysVol() - is root?" << isRootVolume << std::endl;
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into FullPhysVols(logvol, parent) values(?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(logVolId);
//  q.addBindValue(parentPhysVolId);
//  q.exec();
//
//  QVariant lastInserted = q.lastInsertId();
//
//  if (isRootVolume) {
//    storeRootVolume(lastInserted.toUInt(), "GeoFullPhysVol");
//  }
//
//  return lastInserted;
//
//}



//QVariant GMDBManager::addLogVol(const QString &name, const QVariant &shapeId, const QVariant &materialId)
//{
////  if (m_debug) qDebug() << "GMDBManager::addLogVol()";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into LogVols(name, shape, material) values(?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(name);
//  q.addBindValue(shapeId);
//  q.addBindValue(materialId);
//  q.exec();
//  return q.lastInsertId();
//}

//QVariant GMDBManager::addMaterial(const QString &name, const QString &density, const QString &elements)
//{
////  if (m_debug) qDebug() << "GMDBManager::addMaterial()";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into Materials(name, density, elements) values(?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//  q.addBindValue(name);
//  q.addBindValue(density);
//  q.addBindValue(elements);
//  q.exec();
//  return q.lastInsertId();
//}


//QVariant GMDBManager::addElement(const QString &name, const QString &symbol, const QString &elZ, const QString &elA)
//{
////  if (m_debug) qDebug() << "GMDBManager::addElement()";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into Elements(name, symbol, Z, A) values(?, ?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//  q.addBindValue(name);
//  q.addBindValue(symbol);
//  q.addBindValue(elZ);
//  q.addBindValue(elA);
//  q.exec();
//  return q.lastInsertId();
//}


bool GMDBManager::addListOfChildrenPositions(const std::vector<std::vector<std::string>> &records)
{
    // NOTE: Choose the right function for your version of SQLite!!
	return addListOfRecordsToTable("ChildrenPositions", records); // needs SQLite >= 3.7.11
	//return addListOfRecordsToTableOld("ChildrenPositions", records); // old SQLite versions
}



bool GMDBManager::addListOfRecords(const std::string geoType, const std::vector<std::vector<std::string>> records)
{
//  if (m_debug) qDebug() << "GMDBManager::addListOfRecords():" << geoType;

  std::string tableName = m_childType_tableName[geoType];

  if (tableName.size() == 0) {
    //qWarning() << "m_childType_tableName:" << m_childType_tableName;
    std::cout << "ERROR!! could not retrieve tableName for node type '" << geoType << "'!! Aborting..." << std::endl;
    exit(EXIT_FAILURE);
  }

  if (records.size() > 0 ) {
    // NOTE: Choose the right function!!
    return addListOfRecordsToTable(tableName, records); // better, but needs SQLite >= 3.7.11
    //return addListOfRecordsToTableOld(tableName, records); // old SQLite versions
  }
  else
  std::cout << "Info: no records to save for geoType '" << geoType << "'. Skipping..." << std::endl;
  return true;
}



// ***Note***
// the syntax built here below is more convenient
// but it is supported only in SQLite >= 3.7.11
// (Note: old SLC6-based Athena releases uses SQLite 3.7.9 from LCG!
// here we build a query like this:
// queryStr = QString("INSERT INTO Materials (id, name) VALUES  (1,'Air'), (2,'Silicon'), (368,'ShieldSteel');");
//
/*
bool GMDBManager::addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::string>> records)
//bool addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::string>> records)
{
  // get table columns and format them for query
  QString tableColString = "(" + QString::fromStdString(joinVectorStrings(m_tableNames.at(tableName), ", ")) + ")";

  qWarning() << "tableColString:" << tableColString;

  unsigned int nRecords = records.size();
  std::cout << "number of " << tableName << " records to insert into the DB:" << nRecords << std::endl;

  // preparing the SQL query
  QString queryStr("INSERT INTO %1 %2 VALUES ");
  queryStr = queryStr.arg(QString::fromStdString(tableName)); // insert table name
  queryStr = queryStr.arg(tableColString); // insert table columns

  unsigned int nMat = nRecords;
  unsigned int id = 0;
//  foreach(QStringList rec, records) {
  for( const std::vector<std::string>& rec : records) {
    //qDebug() << "rec:" << rec;

    ++id;
//    QStringList items;
    std::vector<std::string> items;

    for ( const std::string& item : rec) {
      items.push_back("'" + item + "'");
    }
//    QString values = items.join(",");
    std::string values = joinVectorStrings(items, ",");
    queryStr += " (" + QString::number(id) + "," + QString::fromStdString(values) + ")";
    if (id != nMat)
    queryStr += ",";
    else
    queryStr += ";";

  }

  qWarning() << "Query string:" << queryStr;

  // executing the SQL query
  QSqlQuery q;
  if (!q.exec(queryStr)) {
    qWarning() << "ERROR!!! SQL error:";
    showError(q.lastError());
    return false;
  }

  return true;
}
*/



// ***Note***
// The syntax built here below is more convenient
// but it is supported only in SQLite >= 3.7.11
// (Note: old SLC6-based Athena releases uses SQLite 3.7.9 from LCG)
//
// Here we build a query like this:
// queryStr = QString("INSERT INTO Materials (id, name) VALUES  (1,'Air'), (2,'Silicon'), (368,'ShieldSteel');");
//
bool GMDBManager::addListOfRecordsToTable(const std::string tableName, const std::vector<std::vector<std::string>> records)
{
  // get table columns and format them for query
  std::string tableColString = "(" + joinVectorStrings(m_tableNames.at(tableName), ", ") + ")";
  //std::cout << "tableColString:" << tableColString << std::endl; // debug

  unsigned int nRecords = records.size();
  std::cout << "Info: number of " << tableName << " records to dump into the DB:" << nRecords << std::endl;

  // preparing the SQL query
  std::string sql = fmt::format("INSERT INTO {0} {1} VALUES ", tableName, tableColString);
  unsigned int id = 0;
  for( const std::vector<std::string>& rec : records) {
    ++id;
    std::vector<std::string> items;
    for ( const std::string& item : rec) {
      items.push_back("'" + item + "'");
    }
    std::string values = joinVectorStrings(items, ",");
    sql += " (" + std::to_string(id) + "," + values + ")";
    if (id != nRecords) {
      sql += ",";
    } else {
      sql += ";";
    }
    
  }
  if(m_debug) std::cout << "Query string:" << sql << std::endl; // debug
  
  // executing the SQL query
  if ( ! (execQuery(sql)) ) {
    return false;
  }
  return true;
}


// ***Note***
// old syntax, for SQLite in SLC6
// here below we build the syntax for multiple INSERT
// compatible with old SQLite versions
// see: stackoverflow.com/questions/1609637
//
// we want to build a query like this:
//  queryStr = QString("INSERT INTO Materials (id, name) SELECT 1 as id, 'Air' as name UNION ALL SELECT 2,'Silicon' UNION ALL SELECT 368,'ShieldSteel' ")
//
/*
bool GMDBManager::addListOfRecordsToTableOld(const QString tableName, const std::vector<QStringList> records)
{
	// get table columns and format them for query
  std::string tabColNames = joinVectorStrings(m_tableNames.at(tableName.toStdString()), ", ");
  QString tableColString = "(" + QString::fromStdString(tabColNames) + ")";



    QStringList colNames = m_tableNames.at(tableName);
    unsigned int nRecords = records.size();
    qInfo() << "number of " << tableName << "records to insert into the DB:" << nRecords;


     // SQLite has a limit on 'union' items, set at 500. So we have to split the items if we have more.
     // See: stackoverflow.com/questions/9527851/

    unsigned int bunchSize = 500;
    if ( nRecords > bunchSize ) {
        qWarning() << "WARNING! " << nRecords << " records of type " << tableName << "to store in the DB in one call! Call limit is " << bunchSize << " --> We split them in bunches...";

        std::vector<QStringList> recordsCopy( records ); // TODO: maybe we should use a deque or queue, which have fast pop of first element?

        std::vector<QStringList>::const_iterator first;
        std::vector<QStringList>::const_iterator last;

        unsigned int start = 0;

        while ( recordsCopy.size() > 0 ) {

        // preparing the SQL query
        QString queryStr("INSERT INTO %1 %2 SELECT ");
	    queryStr = queryStr.arg(tableName); // insert table name
	    queryStr = queryStr.arg(tableColString); // insert table columns

        first = recordsCopy.begin();
        last  = recordsCopy.size() > bunchSize ? recordsCopy.begin() + bunchSize : recordsCopy.end();

        std::vector<QStringList> recordsBunch( first, last ); // we take the first 500 records
        recordsCopy.erase( first, last ); // we delete the first 500 records
        if (m_debug) qDebug() << "start:" << start << "recordsBunch size:" << recordsBunch.size() << "- recordsCopy size after removal:" << recordsCopy.size();

	    // --- first record
        // outcome should be like: " 1 as id, 'Air' as name "
        unsigned int id = start+1; // set the first ID as 'start'
        QStringList recFirst = recordsBunch.front(); // access first record from vector
        recordsBunch.erase( recordsBunch.begin() ); // delete first record
        if (m_debug) qDebug() << "after taking first record - recordsBunch size:" << recordsBunch.size();

        // first item in the first record (it is the ID)
        QString firstCol = colNames[0];
        queryStr += QString::number(id) + " as " + firstCol + ", ";

        // the other items in the first record
        unsigned int ii = 1;
        unsigned int nRecs = recFirst.size();
        foreach (QString rec, recFirst) {
            queryStr += "'" + rec + "'" + " as " + colNames[ii];
            if (ii != nRecs)
                queryStr += ","; // add coma, but not on latest
            queryStr += " "; // add space
            //qDebug() << "first element:" << ii << nRecs << queryStr;
            ++ii;
        }
        if (m_debug) qDebug() << "first element query:" << queryStr;
        // --- other records
        // outcome should be: " UNION ALL SELECT 2,'Silicon' "


            foreach(QStringList recs, recordsBunch) {

							// DEBUG
							if (tableName == "Functions") {
								if (recs[1].length() > 65000) {
									std::cout << "LONG STRING! size: " << recs[1].length() << std::endl;
									std::cout << "LONG STRING! string: " << recs[1].toStdString() << std::endl << std::endl;
								}
							}
							//------

								++id;
                // put single quotes around items
                QStringList items;
                foreach (QString item, recs) {
			        		items << "'" + item + "'";
		        		}
                // join items into a string and add the SQL commands
                QString itemsStr = items.join(", ");
                queryStr += " UNION ALL SELECT " + QString::number(id) + ", " + itemsStr;
            }

        //qDebug() << "queryStr:" << queryStr;

	    // executing the SQL query
	    QSqlQuery q;
	    if (!q.exec(queryStr)) {
            qWarning() << "ERROR!!! SQL error:";
		    showError(q.lastError());
		    return false;
	    }
        // JFB commented: qDebug() << bunchSize << "elements have been saved into the DB, starting at:" << start;

        start += bunchSize; // for the next iteration

      } // end of while

    } // end of if(>500)
    else {
    // preparing the SQL query
    QString queryStr("INSERT INTO %1 %2 SELECT ");
	queryStr = queryStr.arg(tableName); // insert table name
	queryStr = queryStr.arg(tableColString); // insert table columns

    unsigned int id = 0;
    // loop on all records
	foreach(QStringList recs, records) {

		++id;

        // first record
        // outcome should be like: " 1 as id, 'Air' as name "
        if (id == 1) {

            // first item if the record (it is the ID)
            QString firstCol = colNames.takeFirst();
            queryStr += QString::number(id) + " as " + firstCol + ", ";

            // the other items in the record
            unsigned int ii = 0;
            unsigned int nRecs = recs.size();
            foreach (QString rec, recs) {
                queryStr += "'" + rec + "'" + " as " + colNames[ii];
                if (ii != nRecs-1)
                    queryStr += ","; // add coma, but not on latest
                queryStr += " "; // add space
                //qDebug() << ii << nRecs << queryStr;
                ++ii;
            }

         }
        // other records
        // outcome should be: " UNION ALL SELECT 2,'Silicon' "
        else {

            // put single quotes around items
            QStringList items;
            foreach (QString item, recs) {
			    items << "'" + item + "'";
		    }
            // join items into a string and add the SQL commands
            QString itemsStr = items.join(", ");
            queryStr += " UNION ALL SELECT " + QString::number(id) + ", " + itemsStr;
        }

	} // end of: foreach(QStringList recs, records)

    //qDebug() << "queryStr:" << queryStr;

	// executing the SQL query
	QSqlQuery q;
	if (!q.exec(queryStr)) {
        qWarning() << "ERROR!!! SQL error:";
		showError(q.lastError());
		return false;
	}
} // end of (else > 500)

	// JFB commented: qDebug() << "DONE. The list of " << nRecords << "records have been inserted into the DB.";
	return true;

}
*/





//QVariant GMDBManager::addShape(const QString &type, const QString &parameters)
//{
////  if (m_debug) qDebug() << "GMDBManager::addShape()";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into Shapes(type, parameters) values(?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(type);
//  q.addBindValue(parameters);
//  q.exec();
//  return q.lastInsertId();
//}
//
//QVariant GMDBManager::addSerialDenominator(const QString &baseName)
//{
////  if (m_debug) qDebug() << "GMDBManager::addSerialDenominator("+baseName+")";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into SerialDenominators(baseName) values(?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(baseName);
//  q.exec();
//  return q.lastInsertId();
//}

//QVariant GMDBManager::addNameTag(const QString &name)
//{
////  if (m_debug) qDebug() << "GMDBManager::addNameTag("+name+")";
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into NameTags(name) values(?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//  q.addBindValue(name);
//  q.exec();
//  return q.lastInsertId();
//}
//
//QVariant GMDBManager::addFunction(const QString expression)
//{
////  if (m_debug) qDebug() << "GMDBManager::addFunction()";
//
//  // TEST
//  std::cout << "Function - expression string len: " << expression.length();
//  std::cout << "Function - expression: " << expression.toStdString() << std::endl << std::endl;
//  //-----
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into Functions(expression) values(?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(expression);
//  q.exec();
//  return q.lastInsertId();
//
//
//}

//QVariant GMDBManager::addSerialTransformer(const unsigned int &funcId, const unsigned int &physvolId, const std::string &physvolType, const unsigned int &copies)
//{
////  if (m_debug) qDebug() << "GMDBManager::addSerialTransformer()" << funcId << physvolId << copies;
//
//  const unsigned int volTableId = getTableIdFromNodeType(physvolType);
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into SerialTransformers(funcId, volId, volTable, copies) values(?, ?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(funcId);
//  q.addBindValue(physvolId);
//  q.addBindValue(volTableId);
//  q.addBindValue(copies);
//  q.exec();
//  return q.lastInsertId();
//}

//QVariant GMDBManager::addTransform(QVector<double> params)
//{
////  if (m_debug) qDebug() << "GMDBManager::addTransform()";
//
//  // get the 12 matrix elements
//  double xx = params[0];
//  double xy = params[1];
//  double xz = params[2];
//
//  double yx = params[3];
//  double yy = params[4];
//  double yz = params[5];
//
//  double zx = params[6];
//  double zy = params[7];
//  double zz = params[8];
//
//  double dx = params[9];
//  double dy = params[10];
//  double dz = params[11];
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into Transforms(xx, xy, xz, yx, yy, yz, zx, zy, zz, dx, dy, dz) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(xx);
//  q.addBindValue(xy);
//  q.addBindValue(xz);
//  q.addBindValue(yx);
//  q.addBindValue(yy);
//  q.addBindValue(yz);
//  q.addBindValue(zx);
//  q.addBindValue(zy);
//  q.addBindValue(zz);
//  q.addBindValue(dx);
//  q.addBindValue(dy);
//  q.addBindValue(dz);
//  q.exec();
//  return q.lastInsertId();
//}
//
//
//QVariant GMDBManager::addAlignableTransform(QVector<double> params)
//{
////  if (m_debug) qDebug() << "GMDBManager::addAlignableTransform()";
//
//  // get the 12 matrix elements
//  double xx = params[0];
//  double xy = params[1];
//  double xz = params[2];
//
//  double yx = params[3];
//  double yy = params[4];
//  double yz = params[5];
//
//  double zx = params[6];
//  double zy = params[7];
//  double zz = params[8];
//
//  double dx = params[9];
//  double dy = params[10];
//  double dz = params[11];
//
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into AlignableTransforms(xx, xy, xz, yx, yy, yz, zx, zy, zz, dx, dy, dz) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"))) {
//    showError(q.lastError());
//    return QVariant();
//  }
//
//  q.addBindValue(xx);
//  q.addBindValue(xy);
//  q.addBindValue(xz);
//  q.addBindValue(yx);
//  q.addBindValue(yy);
//  q.addBindValue(yz);
//  q.addBindValue(zx);
//  q.addBindValue(zy);
//  q.addBindValue(zz);
//  q.addBindValue(dx);
//  q.addBindValue(dy);
//  q.addBindValue(dz);
//  q.exec();
//  return q.lastInsertId();
//}


bool GMDBManager::addRootVolume(const std::vector<std::string> &values)
{
	if (values.size() > 0 ) {
    const unsigned int volId = std::stoi(values[0]);
    const std::string nodeType = values[1];
		return storeRootVolume( volId, nodeType );
	}
	else
  std::cout << "WARNING! No records to save for RootVolume!" << std::endl;
	return false;
}

/*
// insert a single parent-child relationship for a given parent volume
void GMDBManager::addChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &childId, const unsigned int &parentCopyNumber, const unsigned int &childPos, const std::string &childType, const unsigned int &childCopyN)
{
//  if (m_debug) qDebug() << "GMDBManager::addChildPosition(" << parentId << "," << parentType << "," << QString::number(parentCopyNumber) << "," <<childPos << "," << childType << ", " << childId << ", " << QString::number(childCopyN) << ")";

	// get table name -- which the child ID refers to -- based on the child type
	const unsigned int parentTableId = getTableIdFromNodeType(parentType);
	const unsigned int childTableId = getTableIdFromNodeType(childType);

  std::string tableName = "ChildrenPositions";
  std::vector<std::string> cols = getTableColumnNames(tableName);

	QString queryStr = QString("insert into %1(%2, %3, %4, %5, %6, %7, %8) values(?, ?, ?, ?, ?, ?, ?)").arg( QString::fromStdString(tableName) );
	for (int i=0; i < cols.size(); ++i) {
		if (i != 0) // skip the "id" column
			queryStr = queryStr.arg( QString::fromStdString( cols.at(i)) );
	}
//  if (m_debug) qDebug() << "queryStr:" << queryStr;
	QSqlQuery q;
	if (!q.prepare(queryStr)) {
		showError(q.lastError());
		return;
	}
	q.addBindValue(parentId);
	q.addBindValue(parentTableId);
	q.addBindValue(parentCopyNumber);
	q.addBindValue(childPos);
	q.addBindValue(childTableId);
	q.addBindValue(childId);
	q.addBindValue(childCopyN);
	q.exec();
	return;
}
*/

//void GMDBManager::addDBversion(const QString version)
//{
//  QSqlQuery q;
//  if (!q.prepare(QLatin1String("insert into dbversion(version) values(?)"))) {
//    showError(q.lastError());
//    return;
//  }
//  q.addBindValue(version);
//  q.exec();
//  return;
//}
void GMDBManager::addDBversion(std::string version)
{
  checkIsDBOpen();
  sqlite3_stmt * st = nullptr;
  int rc = -1;
  std::string sql = "INSERT INTO dbversion(version) VALUES(?)";
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  rc = sqlite3_bind_text(st, 1, version.c_str(), version.length(), SQLITE_TRANSIENT);
  rc = sqlite3_step( st );
  if (rc != SQLITE_DONE) {
    printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  // finalize
  sqlite3_finalize(st);
  return;
}

bool GMDBManager::checkIsDBOpen() const
{
  if(m_dbSqlite != nullptr) {
    return true;
  } else {
    std::cout << "ERROR! The SQLite DB is not accessible! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
}




std::vector<std::string> GMDBManager::getItemAndType(unsigned int tableId, unsigned int id)
{
  std::vector<std::string> results;

  std::string tableName = getTableNameFromTableId(tableId);
  std::string nodeType  = getNodeTypeFromTableId(tableId);
  std::vector<std::string> item  =  getItemFromTableName(tableName, id);

  // add the type
  results.push_back( nodeType );
  // add the item
  results.insert(results.end(), item.begin(), item.end());

  return results;

}


std::vector<std::string> GMDBManager::getItemFromTableName(std::string tableName, unsigned int id)
{
  // FIXME: when you create caches for all tables, replace this method with a lookup action in the cache.
  /*
   * Get the object from DB.
   */
  std::vector<std::string> item;
  // set a SQL command string with the right table name
  std::string sql = fmt::format("SELECT * FROM {0} WHERE id = (?)", tableName);
  // prepare the query
  sqlite3_stmt * stmt = nullptr;
  int rc = -1;
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] (%s) : Error msg: %s\n", __func__, sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  // bind the parameters
  rc = sqlite3_bind_int(stmt, 1, id);
  // execute the query and loop over all rows and all columuns
  if ( stmt )
  {
    int ctotal = sqlite3_column_count(stmt); // Count the Number of Columns in the Table
    int res = 0;
    while ( 1 )
    {
      res = sqlite3_step(stmt); // Execute SQL Statement.
      if ( res == SQLITE_ROW )
      {
        for ( int i = 0; i < ctotal; i++ )  // Loop times the number of columns in the table
        {
          std::string s = (char*)sqlite3_column_text(stmt, i);  // Read each Column in the row as text FIXME: is there a method to get the right type, e.g. double, instead of text?
          item.push_back( s );
        }
      }
      if ( res == SQLITE_DONE || res==SQLITE_ERROR)
      {
        if (res == SQLITE_ERROR) {
          std::string errmsg(sqlite3_errmsg(m_dbSqlite));
          sqlite3_finalize(stmt);
          throw errmsg;
        }
        break;
      }
    }
  }
  if (rc != SQLITE_DONE) {
    printf( "[SQLite ERR] (%s) : Error msg: %s\n", __func__, sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  // finalize
  sqlite3_finalize(stmt);
    
  if (item.size()==0) {
    std::cout << "ERROR!!" << "Item with ID:'" << id << "' does not exist in table" << tableName << "! Exiting...";
    exit(EXIT_FAILURE);
  }
    
  return item;
  
    /*
  QString queryStr = QString("SELECT * FROM %1 WHERE id = (?)").arg(QString::fromStdString(tableName));
  // prepare the query
  QSqlQuery q;
  if (!q.prepare( queryStr )) {
    showError(q.lastError());
//    return QStringList();
    exit(EXIT_FAILURE);
  }
  q.addBindValue(id);
  q.exec();

  std::vector<std::string> params;

  // get the number of columns of the DB table
  int nCols = (m_tableNames.at(tableName)).size();

  while (q.next()) {

    for( int ii=0; ii<nCols; ++ii)
      params.push_back( q.value(ii).toString().toStdString() );
  }
  if (params.size()==0) {
    std::cout << "WARNING!!" << "Item" << id << "does not exist in table" << tableName << "!!";
  }
  return params;
    */
}

/*
// get the list of children for a single GeoVPhysVol (i.e., GeoPhysVol or GeoFullPhysVol)
QMap<unsigned int, QStringList> GMDBManager::getVPhysVolChildren(const unsigned int &id, const std::string &nodeType, const unsigned int &parentCopyNumber)
{
  if (m_debug) std::cout << "GMDBManager::getVPhysVolChildren() - id:" << id << "- type:" << nodeType << "- copyN:" << parentCopyNumber;

	const unsigned int tableId = getTableIdFromNodeType(nodeType);

	// get children and their positions
	QSqlQuery q;
	if (!q.prepare(QLatin1String("SELECT * FROM ChildrenPositions WHERE parentId = (?) AND parentTable = (?) AND parentCopyNumber = (?)"))) {
		showError(q.lastError());
		return QMap<unsigned int, QStringList>();
	}
	q.addBindValue(id);
	q.addBindValue(tableId);
	q.addBindValue(parentCopyNumber);
	q.exec();

	QMap<unsigned int, QStringList> children;
	QStringList childParams;

	// get the number of columns of the DB table
	int nCols = m_tableNames["ChildrenPositions"].size();

	while (q.next()) {

		childParams.clear();

		unsigned int childPos = q.value(3).toUInt();

		for( int ii=0; ii<nCols; ++ii)
			childParams << q.value(ii).toString();

		children[childPos] = childParams;
	}

	return children;
}
 */


/*
// Get all parent-children data from the database in one go
QHash<QString, QMap<unsigned int, QStringList>> GMDBManager::getChildrenTable()
{
	QSqlQuery q = selectAllFromTable("ChildrenPositions");

	QHash< QString, QMap<unsigned int, QStringList> > all_children; // to store all children
	// QMap<unsigned int, QStringList> children; // to temporarily store the children of one parent
	QStringList childParams; // to temporarily store the children parameters

	// get the number of columns of the DB table
	int nCols = m_tableNames["ChildrenPositions"].size();

    // loop over all children's positions stored in the DB
	while (q.next()) {

		childParams.clear();

		QString parentId = q.value(1).toString();
		QString parentTable = q.value(2).toString();
		QString parentCopyNumber = q.value(3).toString();
		unsigned int childPos = q.value(4).toUInt();

		QString key = parentId + ":" + parentTable + ":" + parentCopyNumber;

    for( int ii=0; ii<nCols; ++ii) {
			childParams << q.value(ii).toString();
    }

		all_children[key][childPos] = childParams;
	}
	return all_children;
}
*/

// Get all parent-children data from the database in one go
std::vector<std::vector<std::string>> GMDBManager::getChildrenTable()
{
  return getTableRecords("ChildrenPositions");
  
  /*
  QSqlQuery q;
  QString queryStr = QString("SELECT * FROM ChildrenPositions ORDER BY parentTable, parentId, parentCopyNumber, position");
  if (!q.prepare(queryStr)) {
    showError(q.lastError());
  }
  q.exec();

  std::vector<std::vector<std::string>> all_children; // to store all children
  std::vector<std::string> childParams; // to temporarily store the children parameters

  // get the number of columns of the DB table
  int nCols = m_tableNames["ChildrenPositions"].size();
  //  std::cout << "num of columns in childrenPos table" << nCols << std::endl; // debug

  // loop over all children's positions stored in the DB
  while (q.next()) {

    childParams.clear();

    for( int ii=0; ii<nCols; ++ii) {
      childParams.push_back( q.value(ii).toString().toStdString() );
    }

//    // debug
//    if(childParams[2]=="8920")
//      std::cout << "CoolingTube parent:"; print childParams;

    all_children.push_back(childParams);
  }
  return all_children;
  */
}



unsigned int GMDBManager::getTableIdFromNodeType(const std::string &nodeType)
{
  return m_cache_nodeType_tableID[nodeType];
  
  /*
	QSqlQuery q;
	if (!q.prepare(QLatin1String("SELECT id FROM GeoNodesTypes WHERE nodeType = (?)"))) {
		showError(q.lastError());
    exit(EXIT_FAILURE);
	}
  q.addBindValue(QString::fromStdString(nodeType));
	q.exec();

	QVariant id;
	while (q.next()) {
		id = q.value(0);
	}

	return id.toUInt();
  */
}


std::string GMDBManager::getTableNameFromNodeType(const std::string &nodeType)
{
  return m_cache_nodeType_tableName.at(nodeType);
}



/*
QString GMDBManager::getTableNameFromTableId(unsigned int tabId)
{
	QSqlQuery q;
	if (!q.prepare(QLatin1String("SELECT tableName FROM GeoNodesTypes WHERE id = (?)"))) {
		showError(q.lastError());
		return QString();
	}
	q.addBindValue(tabId);
	q.exec();

	QString tableName;
	while (q.next()) {
		tableName = q.value(0).toString();
	}

	return tableName;
}
 */

// build the GeoNodeTypes cache
int GMDBManager::loadGeoNodeTypesAndBuildCache()
{
    checkIsDBOpen();
    std::string queryStr;
    sqlite3_stmt * st = nullptr;
    int rc = -1;
    //set the SQL query string
    std::string sql = "SELECT * FROM GeoNodesTypes";
    // declare the data we want to fetch
    unsigned int id = 0;
    std::string nodeType = "";
    std::string tableName = "";
    // prepare the query
    rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
    if (rc != SQLITE_OK) {
      printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
      exit(EXIT_FAILURE);
    }
    // execute the statement until all selected records are processed
    while ((rc = sqlite3_step(st)) == SQLITE_ROW) {
      // get the data
      id = sqlite3_column_int(st, 0);
      nodeType = std::string(reinterpret_cast<const char*>(sqlite3_column_text(st, 1)));
      tableName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(st, 2)));
//      std::cout << "row: " << id << "," << nodeType << "," << tableName << std::endl; // debug
      // fill the caches
      m_cache_tableId_tableName.insert( std::pair<unsigned int, std::string>(id, tableName));
      m_cache_tableId_nodeType.insert( std::pair<unsigned int, std::string>(id, nodeType));
      m_cache_nodeType_tableName.insert( std::pair<std::string, std::string>(nodeType, tableName));
      m_cache_nodeType_tableID.insert( std::pair<std::string, unsigned int>(nodeType, id));
    }
    if (rc != SQLITE_DONE) {
      std::string errmsg(sqlite3_errmsg(m_dbSqlite));
      sqlite3_finalize(st);
      throw errmsg;
    }
    // finalize
    sqlite3_finalize(st);
  return rc;
}

std::string GMDBManager::getTableNameFromTableId(unsigned int tabId)
{
  return m_cache_tableId_tableName[tabId];
}

std::string GMDBManager::getNodeTypeFromTableId(unsigned int tabId)
{
  return m_cache_tableId_nodeType[tabId];
}

std::unordered_map<unsigned int, std::string> GMDBManager::getAll_TableIDsNodeTypes()
{
  return m_cache_tableId_nodeType;
}

std::unordered_map<std::string, unsigned int> GMDBManager::getAll_NodeTypesTableIDs()
{
  return m_cache_nodeType_tableID;
}



sqlite3_stmt* GMDBManager::selectAllFromTable(std::string tableName) const
{
  return selectAllFromTableSortBy(tableName, "id");
}


sqlite3_stmt* GMDBManager::selectAllFromTableSortBy(std::string tableName, std::string sortColumn) const
{
  checkIsDBOpen();
  
  if ("" == sortColumn || 0 == sortColumn.size()) {
    sortColumn = "id";
  }
  
  sqlite3_stmt * st = nullptr;
  int rc = -1;
  //set the SQL query string
  std::string sql = fmt::format("SELECT * FROM {0} ORDER BY {1}", tableName, sortColumn);
  // prepare the query
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  return st;
  
//  QSqlQuery q;
//
//  if ("" == sortColumn || 0 == sortColumn.size()) {
//    sortColumn = "id";
//  }
//  QString qsortColumn = QString::fromStdString(sortColumn);
//
//  QStringList args;
//  args << QString::fromStdString(tableName) << qsortColumn;
//
//  QString queryStr = QString("SELECT * FROM %1 ORDER BY %2");
////  queryStr = queryStr.arg(tableName);
//  for (int i=0; i < args.size(); ++i) {
//    queryStr = queryStr.arg( args.at(i) );
//  }
//
//  if (!q.prepare(queryStr)) {
//    showError(q.lastError());
//    return QSqlQuery();
//  }
//  q.exec();
//  return q;
}


sqlite3_stmt* GMDBManager::selectAllFromTableChildrenPositions() const
{
  checkIsDBOpen();
  sqlite3_stmt * st = nullptr;
  int rc = -1;
  //set the SQL query string
  std::string sql = "SELECT * FROM ChildrenPositions ORDER BY parentTable, parentId, parentCopyNumber, position";
  // prepare the query
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  return st;
}


bool GMDBManager::initDB()
{
  // create tables
  bool tablesOK = createTables();
	// store DB version
  addDBversion(dbversion);
  // build caches
  loadGeoNodeTypesAndBuildCache();
	return tablesOK;

}



void GMDBManager::printAllDBTables()
{
  if(m_cache_tables.size() == 0)
    getAllDBTables();
  
  for( auto& str : m_cache_tables )
    std::cout << str << std::endl;
}


void GMDBManager::getAllDBTables()
{
  std::string tableName;
  std::vector<std::string> tables;
  // define a query string containing the necessary SQL instructions
  std::string queryStr = "SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
  // prepare the query with the query string
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(m_dbSqlite, queryStr.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw std::string(sqlite3_errmsg(m_dbSqlite));
  }
  // execute the statement until all selected records are processed
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    tableName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    //std::cout << "tableName: " << tableName << std::endl; // debug
    tables.push_back(tableName);
  }
  if (rc != SQLITE_DONE) {
    std::string errmsg(sqlite3_errmsg(m_dbSqlite));
    sqlite3_finalize(stmt);
    throw errmsg;
  }
  // finalize
  sqlite3_finalize(stmt);
  
  m_cache_tables = tables;
}

bool GMDBManager::createTables()
{
  checkIsDBOpen();
  
  int rc = -1; // sqlite return code
  std::string queryStr;

  std::string geoNode;
  std::string tableName;
  std::vector<std::string> tab;

  // create a table to store the relation between the types of GeoNodes and the name of the table
  tableName = "dbversion";
  tab.insert(tab.begin(), {tableName, "id", "version"});
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0} ({1} integer primary key, {2} integer)", tab[0], tab[1], tab[2]);
  rc = execQuery(queryStr);
  tab.clear();
  
  // create a table to store the relation between the types of GeoNodes and the name of the table
  tableName = "GeoNodesTypes";
  tab.insert(tab.begin(), {tableName, "id", "nodeType", "tableName"});
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar, {3} varchar)", tab[0], tab[1], tab[2], tab[3]);
  rc = execQuery(queryStr);
  tab.clear();
  
  // create a table to store the mother-daughter relationships between nodes (notably, between the [Full]PhysVols as the parents and their children)
  tableName = "ChildrenPositions";
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("parentId");
  tab.push_back("parentTable");
  tab.push_back("parentCopyNumber");
  tab.push_back("position");
  tab.push_back("childTable");
  tab.push_back("childId");
  tab.push_back("childCopyNumber");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} integer, {3} integer not null REFERENCES GeoNodesTypes(id), {4} integer, {5} integer, {6} integer not null REFERENCES GeoNodesTypes(id), {7} integer not null, {8} integer not null)", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7], tab[8]);
  rc = execQuery(queryStr);
  tab.clear();

  // create a table to store information about the 'root' volume (also known as the 'world' volume)
  tableName = "RootVolume";
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("volId");
  tab.push_back("volTable");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} integer not null, {3} integer not null REFERENCES GeoNodesTypes(id))", tab[0], tab[1], tab[2], tab[3]);
  rc = execQuery(queryStr);
  tab.clear();
  
  // PhysVols table
  geoNode = "GeoPhysVol";
  tableName = "PhysVols";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("logvol");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} integer not null)", tab[0], tab[1], tab[2]);
  if ( 0==(rc = execQuery(queryStr)) ) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // FullPhysVols table
  geoNode = "GeoFullPhysVol";
  tableName = "FullPhysVols";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("logvol");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} integer not null)", tab[0], tab[1], tab[2]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // LogVols table
  geoNode = "GeoLogVol";
  tableName = "LogVols";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("name");
  tab.push_back("shape");
  tab.push_back("material");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar, {3} integer not null, {4} integer not null)", tab[0], tab[1], tab[2], tab[3], tab[4]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // Materials table
  geoNode = "GeoMaterial";
  tableName = "Materials";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("name");
  tab.push_back("density");
  tab.push_back("elements");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar, {3} varchar, {4} varchar)", tab[0], tab[1], tab[2], tab[3], tab[4]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // Elements table
  geoNode = "GeoElement";
  tableName = "Elements";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("name");
  tab.push_back("symbol");
  tab.push_back("Z");
  tab.push_back("A");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar, {3} varchar, {4} varchar, {5} varchar)", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // Shapes table
  geoNode = "GeoShape";
  tableName = "Shapes";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("type");
  tab.push_back("parameters");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar, {3} varchar)", tab[0], tab[1], tab[2], tab[3]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // SerialDenominators table
  geoNode = "GeoSerialDenominator";
  tableName = "SerialDenominators";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("baseName");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar)", tab[0], tab[1], tab[2]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();

  // Functions table
  geoNode = "Function";
  tableName = "Functions";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("expression");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar)", tab[0], tab[1], tab[2]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // SerialDenominators table
  geoNode = "GeoSerialTransformer";
  tableName = "SerialTransformers";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("funcId");
  tab.push_back("volId");
  tab.push_back("volTable");
  tab.push_back("copies");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} integer not null REFERENCES Functions(id), {3} integer not null, {4} integer not null REFERENCES GeoNodesTypes(id), {5} integer)", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // Transforms table
  geoNode = "GeoTransform";
  tableName = "Transforms";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("xx");
  tab.push_back("xy");
  tab.push_back("xz");
  tab.push_back("yx");
  tab.push_back("yy");
  tab.push_back("yz");
  tab.push_back("zx");
  tab.push_back("zy");
  tab.push_back("zz");
  tab.push_back("dx");
  tab.push_back("dy");
  tab.push_back("dz");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} real, {3} real, {4} real, {5} real, {6} real, {7} real, {8} real, {9} real, {10} real, {11} real, {12} real, {13} real)", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7], tab[8], tab[9], tab[10], tab[11], tab[12], tab[13]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // Transforms table
  geoNode = "GeoAlignableTransform";
  tableName = "AlignableTransforms";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("xx");
  tab.push_back("xy");
  tab.push_back("xz");
  tab.push_back("yx");
  tab.push_back("yy");
  tab.push_back("yz");
  tab.push_back("zx");
  tab.push_back("zy");
  tab.push_back("zz");
  tab.push_back("dx");
  tab.push_back("dy");
  tab.push_back("dz");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} real, {3} real, {4} real, {5} real, {6} real, {7} real, {8} real, {9} real, {10} real, {11} real, {12} real, {13} real)", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7], tab[8], tab[9], tab[10], tab[11], tab[12], tab[13]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  // NameTags table
  geoNode = "GeoNameTag";
  tableName = "NameTags";
  m_childType_tableName[geoNode] = tableName;
  tab.push_back(tableName);
  tab.push_back("id");
  tab.push_back("name");
  storeTableColumnNames(tab);
  queryStr = fmt::format("create table {0}({1} integer primary key, {2} varchar)", tab[0], tab[1], tab[2]);
  if ( 0==(rc = execQuery(queryStr))) {
    storeNodeType(geoNode, tableName); }
  tab.clear();
  
  if(m_debug) {
    std::cout << "All these tables have been successfully created:" << std::endl; // debug
    printAllDBTables();
  }

  if(rc==0) {
    return true;
  }
  return false;
}


int GMDBManager::execQuery(std::string queryStr)
{
  if(m_debug) std::cout << "queryStr to execute: " << queryStr << std::endl; // debug
  checkIsDBOpen();
  int result = -1;
  if( (result = sqlite3_exec(m_dbSqlite, queryStr.c_str(), NULL, 0, &m_SQLiteErrMsg)) )
  {
    printf( "[ERR] : \t> CMD: %s , Error: %d\n" , queryStr.c_str() , result );
    if ( m_SQLiteErrMsg )
    {
      printf( "[ERR] : Error msg: %s\n", m_SQLiteErrMsg );
      sqlite3_free(m_SQLiteErrMsg);
    }
  }
  return result;
}




void GMDBManager::storeTableColumnNames(std::vector<std::string> input)
{
  if (! (input.size() == 0) ) {
    std::string tabName = input.at(0); // get the first element storing the name of the table
    input.erase(input.begin()); // remove the first element storing the name of the table
    m_tableNames[tabName] = input;
  }
}


std::vector<std::string> GMDBManager::getTableColumnNames(const std::string &tableName)
{
	//	qDebug() << "GMDBManager::getTableColumnNames()";
	return m_tableNames.at(tableName);
}

void GMDBManager::storeNodeType(std::string nodeType, std::string tableName)
{
  checkIsDBOpen();
  std::string queryStr;
  sqlite3_stmt * st = nullptr;
  int rc = -1;
  // preparing the SQL query
  std::string sql = "INSERT INTO GeoNodesTypes(nodeType, tableName) VALUES(?, ?)";
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] (%s) : Error msg: %s\n", __func__, sqlite3_errmsg(m_dbSqlite) ); // TODO: add __func__ to all error messages, as I did here
    exit(EXIT_FAILURE);
  }
  if(m_debug) std::cout << "storeNodeType - Query string:" << sql << std::endl; // debug
  // bind the parameters
  rc = sqlite3_bind_text(st, 1, nodeType.c_str(), nodeType.length(), SQLITE_TRANSIENT);
  rc = sqlite3_bind_text(st, 2, tableName.c_str(), tableName.length(), SQLITE_TRANSIENT);
  // execute the query
  rc = sqlite3_step( st );
  if (rc != SQLITE_DONE) {
    printf( "[SQLite ERR] : Error msg: %s\n", sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  // finalize
  sqlite3_finalize(st);
  return;
  
  /*
	QSqlQuery q;
	if (!q.prepare(QLatin1String("insert into GeoNodesTypes(nodeType, tableName) values(?, ?)"))) {
		showError(q.lastError());
		return;
	}

  q.addBindValue(QString::fromStdString(nodeType));
  q.addBindValue(QString::fromStdString(tableName));
	q.exec();
	return;
  */
}



//void GMDBManager::loadTestData()
//{
//  // create test data
//  QSqlQuery q;
//  //
//  //  qDebug() << "Loading Shapes...";
//  //  if (!q.prepare(QLatin1String("insert into Shapes(name) values(?)"))) {
//  //    showError(q.lastError());
//  //    return;
//  //  }
//  QVariant boxId    = addShape(QLatin1String("Box"), "");
//  QVariant coneId   = addShape(QLatin1String("Cone"), "");
//  QVariant sphereId = addShape(QLatin1String("Sphere"), "");
//
//  qWarning() << "Sample GeoElement data --> to be implemented!";
//
//  QVariant airId  = addMaterial(QLatin1String("Air"),QLatin1String("density"),QLatin1String("elements"));
//  QVariant ironId = addMaterial(QLatin1String("Iron"),QLatin1String("density"),QLatin1String("elements"));
//  QVariant leadId = addMaterial(QLatin1String("Lead"),QLatin1String("density"),QLatin1String("elements"));
//
//  QVariant worldLogId = addLogVol(QLatin1String("WorldLog"), boxId, airId);
//  QVariant toyLogId  = addLogVol(QLatin1String("ToyLog"), coneId, leadId);
//
//  QVariant rootPhysId = addPhysVol(worldLogId, QVariant()); // no parent
//  QVariant childPhysId = addPhysVol(toyLogId, rootPhysId);
//}


//std::vector<std::string> GMDBManager::getTableColNamesFromDB(std::string tableName) const
//{
//  std::vector<std::string> colNames;
//  colNames. push_back( tableName );
//
//  // fetch the driver
//  QSqlDriver* driver = m_db.driver();
//  QSqlRecord record = driver->record(QString::fromStdString(tableName));
//  unsigned int colN = record.count();
//  for (unsigned int i = 0; i < colN; ++i)
//    colNames.push_back( record.fieldName(i).toStdString() );
//  return colNames;
//}





//void GMDBManager::printTableColNamesFromDB(const std::string &tableName) const
//{
//  // JFB commented: qDebug() << "GMDBManager::printTableColNames:" << tableName;
//
//  // fetch the driver
//  // JFB commented: qDebug() << getTableColNamesFromDB(tableName);
//}

bool GMDBManager::storeRootVolume(const unsigned int &id, const std::string &nodeType)
{
  checkIsDBOpen();
  
  std::string tableName = "RootVolume";
	const unsigned int typeId = getTableIdFromNodeType(nodeType);
  std::vector<std::string> cols = getTableColumnNames(tableName);

  // set the SQL query
  std::string queryStr;
  std::string sql = fmt::format("INSERT INTO {0} ({1}, {2}) VALUES (?, ?)", tableName, cols[1], cols[2]);
  // preparing the SQL query
  sqlite3_stmt * st = nullptr;
  int rc = -1;
  rc = sqlite3_prepare_v2( m_dbSqlite, sql.c_str(), -1, &st, NULL);
  if (rc != SQLITE_OK) {
    printf( "[SQLite ERR] (%s) : Error msg: %s\n", __func__, sqlite3_errmsg(m_dbSqlite) ); // TODO: add __func__ to all error messages, as I did here
    exit(EXIT_FAILURE);
  }
  if(m_debug) std::cout << "Query string:" << sql << std::endl; // debug
  // bind the parameters
  rc = sqlite3_bind_int(st, 1, id);
  rc = sqlite3_bind_int(st, 2, typeId);
  // execute the query
  rc = sqlite3_step( st );
  if (rc != SQLITE_DONE) {
    printf( "[SQLite ERR] (%s) : Error msg: %s\n", __func__, sqlite3_errmsg(m_dbSqlite) );
    exit(EXIT_FAILURE);
  }
  // finalize
  sqlite3_finalize(st);
  
  /*
	// prepare the query string
	QString queryStr = QString("insert into %1(%2, %3) values(?, ?)").arg( QString::fromStdString(tableName) );
	for (int i=0; i < cols.size(); ++i) {
		if (i != 0) // skip the "id" column
    queryStr = queryStr.arg( QString::fromStdString(cols.at(i)) );
	}
	//qDebug() << "queryStr:" << queryStr;
	// prepare the query
	QSqlQuery q;
	if (!q.prepare(queryStr)) {
		showError(q.lastError());
		return false;
	}
	q.addBindValue(id);
	q.addBindValue(typeId);
	// run the query
	q.exec();
  */
  
	return true;
}

/*
std::vector<std::string> GMDBManager::getRootPhysVol()
{
	// get the ID of the ROOT vol from the table "RootVolume"
	// QSqlQuery query("SELECT * FROM RootVolume");
	QSqlQuery query = selectAllFromTable("RootVolume");

	QVariant id;
	QVariant typeId;
	while (query.next()) {
		id = query.value(1);
		typeId = query.value(2);
	}
	return getItemAndType(typeId.toUInt(), id.toUInt());
}
*/

std::vector<std::string> GMDBManager::getRootPhysVol()
{
  // get the ID of the ROOT vol from the table "RootVolume"
  sqlite3_stmt* stmt = selectAllFromTable("RootVolume");
  // declare the data we want to fetch
  unsigned int id;
  unsigned int typeId;
  // execute the statement on all rows
  int rc = -1;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    // get the data
    id = sqlite3_column_int(stmt, 0);
    typeId = sqlite3_column_int(stmt, 1);
    // TODO: fill a cache
  }
  if (rc != SQLITE_DONE) {
    std::string errmsg(sqlite3_errmsg(m_dbSqlite));
    sqlite3_finalize(stmt);
    throw errmsg;
  }
  // finalize
  sqlite3_finalize(stmt);
  
  return getItemAndType(typeId, id);
}


std::string GMDBManager::getDBFilePath()
{
    return m_dbpath;
}

// TODO: move to an utility class
int lastIndexOf(std::vector<std::string> v, std::string str, int pos = 0) {
  auto it = std::find(std::next(v.rbegin(), v.size() - pos), v.rend(), str);
  if (it != v.rend())
  {
    auto idx = std::distance(v.begin(), it.base() - 1);
//    std::cout << idx << std::endl;
    return idx;
  }
  return -1; // no item matched
}

int GMDBManager::getTableColIndex(const std::string &tableName, const std::string &colName)
{
  std::vector<std::string> colFields = m_tableNames.at(tableName);
	return lastIndexOf(colFields, colName);
}

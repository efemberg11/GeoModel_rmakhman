/*
 * GeoModelReadIn.cpp
 *
 *  Created on: May 20, 2016
 *      Author: riccardo.maria.bianchi@cern.ch
 *
 * major updates:
 *  - Feb 2019, R.M.Bianchi
 *  - Mar 2020, R.M.Bianchi
 *  - Mar 2020, boudreau
 */

// local includes
#include "GeoModelRead/ReadGeoModel.h"

// TFPersistification includes
#include "TFPersistification/TransFunctionInterpreter.h"


// GeoModelKernel
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoGraphNode.h"

// GeoModel shapes
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoUnidentifiedShape.h"


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

//VP1Base
// TODO: we should get rid of VP1Base::VP1Msg dependency, since GeoModelRead should not depend on VP1 packages. Maybe we can move VP1Msg to a standalone package.
//#include "VP1Base/VP1Msg.h"

// Qt includes
#include <QDebug>
#include <QString>

// C++ includes
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <vector>     /* exit, EXIT_FAILURE */
#include <stdexcept>
#include <future>
#include <mutex>
#include <chrono>
#include <cstdlib> /* std::getenv */


std::mutex muxStore;
std::mutex muxGet;
std::mutex muxCout;



using namespace GeoGenfun;
using namespace GeoXF;




namespace GeoModelIO {

ReadGeoModel::ReadGeoModel(GMDBManager* db, unsigned long* progress) : m_progress(nullptr), m_deepDebug(false), m_debug(false), m_runMultithreaded(true)
{
	qDebug() << "===> DumpGeoModelAction: constructor";

	#ifdef GEOMODELREAD_DEEP_DEBUG
	  m_deepDebug = true;
	  std::cout << "You defined the GEOMODELREAD_DEEP_DEBUG variable, so you will see a verbose output." << std::endl;
 	#endif
	#ifdef GEOMODELREAD_DEBUG
	  m_debug = true;
	  std::cout << "You defined the GEOMODELREAD_DEBUG variable, so you will see a verbose output." << std::endl;
 	#endif
  // m_deepDebug = true;

	if ( progress != nullptr) {
	m_progress = progress;
	}

	// set the geometry file
	m_dbManager = db;
	if (m_dbManager->isOpen()) {
	  if (m_deepDebug) qDebug() << "OK! Database is open!";
	}
	else {
		qWarning() << "ERROR!! Database is NOT open!";
		return;
	}
    
    // Check if the user asked for running in multi-threading mode
//    const char* env_option_multithreaded = std::getenv("GEOMODEL_GEOMODELIO_MULTITHREADED");
    if( "1" == getEnvVar("GEOMODEL_GEOMODELIO_NOT_MULTITHREADED") ) {
        std::cout << "\nYou set the GEOMODEL_GEOMODELIO_NOT_MULTITHREADED to '1', so we will run GeoModelIO in serial mode, without using worker threads. If you experience problems, please ask to the GeoModel developers ( --> geomodel-developers<at>cern.ch )\n\n";
        m_runMultithreaded = false;
    }
}

ReadGeoModel::~ReadGeoModel() {
	// TODO Auto-generated destructor stub
}

std::string ReadGeoModel::getEnvVar( std::string const & key ) const
{
    char * val = std::getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}
    
GeoPhysVol* ReadGeoModel::buildGeoModel()
{
  if (m_deepDebug) qDebug() << "ReadGeoModel::buildGeoModel()";

	// return buildGeoModelByCalls();
	GeoPhysVol* rootVolume = buildGeoModelOneGo();

  // warn the user if there are unknown/unhalded shapes
	if (m_unknown_shapes.size() > 0) {
		qWarning() << "\tWARNING!! There were unknwon shapes:";
		for ( auto it = m_unknown_shapes.begin(); it != m_unknown_shapes.end(); it++ ) {
			std::cout << "\t\t---> " << *it << std::endl;
		}
		std::cout << "\tRemember: unknown shapes are rendered with a dummy cube of 30cm side length.\n\n" << std::endl;
		}

	return rootVolume;
}


GeoPhysVol* ReadGeoModel::buildGeoModelOneGo()
{
  if (m_deepDebug) qDebug() << "ReadGeoModel::buildGeoModelOneGo()";

	// get all objects from the DB
	m_physVols = m_dbManager->getTableFromNodeType("GeoPhysVol");
	m_fullPhysVols = m_dbManager->getTableFromNodeType("GeoFullPhysVol");
	m_logVols = m_dbManager->getTableFromNodeType("GeoLogVol");
	m_shapes = m_dbManager->getTableFromNodeType("GeoShape");
	m_materials = m_dbManager->getTableFromNodeType("GeoMaterial");
	m_elements = m_dbManager->getTableFromNodeType("GeoElement");
	m_functions = m_dbManager->getTableFromNodeType("Function");
	m_serialDenominators = m_dbManager->getTableFromNodeType("GeoSerialDenominator");
	m_serialTransformers = m_dbManager->getTableFromNodeType("GeoSerialTransformer");
	m_alignableTransforms = m_dbManager->getTableFromNodeType("GeoAlignableTransform");
	m_transforms = m_dbManager->getTableFromNodeType("GeoTransform");
	m_nameTags = m_dbManager->getTableFromNodeType("GeoNameTag");
	// qDebug() << "physVols: " << _physVols;
	// qDebug() << "fullPhysVols: " << _fullPhysVols;

	// get DB metadata
	m_tableid_tableName = m_dbManager->getAll_TableIDsNodeTypes();

	// get the children table from DB
	m_allchildren = m_dbManager->getChildrenTable();
	// qDebug() << "all children from DB:" << _allchildren;

	// get the root volume data
	m_root_vol_data = m_dbManager->getRootPhysVol();

	// return loopOverAllChildren();

	loopOverAllChildrenInBunches();
	return getRootVolume();

}


//----------------------------------------
// Serial loop over parents' children
void ReadGeoModel::loopOverAllChildren(QStringList keys)
{

  int nChildrenRecords = keys.size();

  if (m_debug || m_deepDebug) {
    muxCout.lock();
  	std::cout << "Thread " << std::this_thread::get_id() << " - processing " << nChildrenRecords << " keys..." << std::endl;
  	muxCout.unlock();
  }

	// loop over parents' keys.
	// It returns a list of children with positions (sorted by position)

	// Get Start Time
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	foreach (const QString &parentKey, keys ) {
		// qWarning() << "parentKey" << parentKey;
		processParentChildren(parentKey);
	} // end loop over parent's childrenPositions records

	// Get End Time
	auto end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();

  if (m_debug || m_deepDebug) {
    muxCout.lock();
  	std::cout << "Thread " << std::this_thread::get_id() << " -- Time Taken to process " << nChildrenRecords << " children = " << diff << " Seconds" << std::endl;
  	muxCout.unlock();
  }
}

//----------------------------------------
// Parallel loop over parents' children
void ReadGeoModel::loopOverAllChildrenInBunches()
{
	int nChildrenRecords = m_allchildren.size();
	if (m_debug) std::cout << "number of children to process: " << nChildrenRecords << std::endl;

	// If we have a few children, then process them serially
    	if ( !(m_runMultithreaded) || nChildrenRecords <= 500)
    	{
		loopOverAllChildren(m_allchildren.keys());
	}
	// ...otherwise, lets spawn some threads to process them in bunches, parallelly!
	else {
		// Get Start Time
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

		unsigned int nThreadsPlatform = std::thread::hardware_concurrency();
        unsigned int nThreads = nThreadsPlatform * 2;
		if (m_debug) std::cout << "INFO - You are running with multiple threads. On this platform, " << nThreadsPlatform << " concurrent threads are supported. Using " << nThreads << " threads.\n";

		unsigned int nBunches = nChildrenRecords / nThreads;
		if (m_debug) std::cout << "Processing " << nThreads << " bunches, with " << nBunches << " children each, plus the remainder." << std::endl;

		// a vector to store the "futures" of async calls
		std::vector<std::future<void>> futures;

		for (unsigned int bb=0; bb<nThreads; ++bb ) {

			unsigned int start = nBunches * bb;
			int len = nBunches;
			if ( bb == (nThreads - 1) ) len = -1; // '-1' means for mid() all the elements starting from 'start'


      if (m_debug || m_deepDebug) {
  			muxCout.lock();
  			std::cout << "Thread " << bb+1 << " - Start: " << start << ", len: " << len << "   ['len=-1' = all remaining items]" << std::endl;
  			muxCout.unlock();
      }

			QStringList bunch = QStringList((m_allchildren.keys()).mid(start,len));
      if (m_debug || m_deepDebug) {
        muxCout.lock();
			   std::cout << "'bunch' size: " << bunch.size() << std::endl;
        muxCout.unlock();
      }

			// loopOverAllChildren(bunch);
			futures.push_back( std::async(std::launch::async, &ReadGeoModel::loopOverAllChildren, this, bunch) );
		}

		// wait for all async calls to complete
		//retrieve and print the value stored in the future
		muxCout.lock();
		if (m_debug) std::cout << "Waiting for the threads to finish..." << std::flush;
		muxCout.unlock();
	  	for(auto &e : futures) {
	    	e.wait();
	   	}
		muxCout.lock();
    	if (m_debug) std::cout << "Done!\n";
		muxCout.unlock();

		// Get End Time
		auto end = std::chrono::system_clock::now();
		auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
		std::cout << "(Total time taken to recreate all " << nChildrenRecords << " mother-children relationships: " << diff << " seconds)" << std::endl;
	}
	return;
}

void ReadGeoModel::processParentChildren(const QString &parentKey)
{
	if (m_deepDebug) qDebug() << "\n" << "parent: " << parentKey << ':' << m_allchildren.value(parentKey) << "[parentId, parentType, parentCopyNumber, childPos, childType, childId, childCopyN]";

	// get the parent's details
	QStringList parentKeyItems = parentKey.split(":");
	QString parentId = parentKeyItems[0];
	QString parentTableId = parentKeyItems[1];
	QString parentCopyN = parentKeyItems[2];
	if (m_deepDebug) qDebug() << "parent ID:" << parentId << ", parent table ID:" << parentTableId << ", parent copy number:" << parentCopyN;

	bool isRootVolume = false;
	if (parentId == "NULL") {
		isRootVolume = true;
	}

	GeoVPhysVol* parentVol = nullptr;

	// build or get parent volume.
	// Using the parentCopyNumber here, to get a given instance of the parent volume
	if (!isRootVolume) {
		if (m_deepDebug) qDebug() << "get the parent volume...";
		parentVol = buildVPhysVol( parentId, parentTableId, parentCopyN);
	}

	// get the parent's children
	QMap<unsigned int, QStringList> children = m_allchildren.value(parentKey);
	// qWarning() << "children" << children;

	// loop over children, sorted by child position automatically
	// "id", "parentId", "parentTable", "parentCopyNumber", "position", "childTable", "childId", "childCopyNumber"
	if (m_deepDebug) qDebug() << "parent volume has " << children.size() << "children. Looping over them...";
	foreach(QStringList child, children) {
		processChild(parentVol, isRootVolume, child);
	} // loop over all children
}


// void ReadGeoModel::processChild(GeoVPhysVol* parentVol, bool& isRootVolume, const QStringList &child, std::mutex &mux)
void ReadGeoModel::processChild(GeoVPhysVol* parentVol, bool& isRootVolume, const QStringList &child)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::processChild() - child:" << child;
    muxCout.unlock();
  }

	if (child.length() < 8) {
		std::cout <<  "ERROR!!! Probably you are using an old geometry file..." << std::endl;
		exit(EXIT_FAILURE);
	}

	// build or get child node
	QString childTableId = child[5];
	QString childId = child[6];
	QString childCopyN = child[7];

	QString childNodeType = m_tableid_tableName[childTableId.toUInt()];

	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "childTableId:" << childTableId << ", type:" << childNodeType << ", childId:" << childId;
    muxCout.unlock();
  }

	if (childNodeType.isEmpty()) {
		qWarning("ERROR!!! childNodeType is empty!!! Aborting...");
		exit(EXIT_FAILURE);
	}

	if (childNodeType == "GeoPhysVol") {
		GeoVPhysVol* childNode = dynamic_cast<GeoPhysVol*>(buildVPhysVol(childId, childTableId, childCopyN));
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoFullPhysVol") {
		GeoVPhysVol* childNode = dynamic_cast<GeoFullPhysVol*>(buildVPhysVol(childId, childTableId, childCopyN));
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoSerialDenominator") {
		GeoSerialDenominator* childNode = buildSerialDenominator(childId);
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoAlignableTransform") {
		GeoAlignableTransform* childNode = buildAlignableTransform(childId);
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoTransform") {
		GeoTransform* childNode = buildTransform(childId);
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoSerialTransformer") {
		GeoSerialTransformer* childNode = buildSerialTransformer(childId);
		if (!isRootVolume) volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoNameTag") {
		GeoNameTag* childNode = buildNameTag(childId);
		if (!isRootVolume) volAddHelper(parentVol, childNode);
			}
	else {
		QString msg = "[" + childNodeType + "]" + QString(" ==> ERROR!!! - The conversion for this type of child node needs to be implemented, still!!!");
		qFatal("%s", msg.toLatin1().constData());
	}
}

void ReadGeoModel::volAddHelper(GeoVPhysVol* vol, GeoGraphNode* volChild)
{
	if (dynamic_cast<GeoPhysVol*>(vol)) {
		GeoPhysVol* volume = dynamic_cast<GeoPhysVol*>(vol);
		volume->add(volChild);
	} else if (dynamic_cast<GeoFullPhysVol*>(vol)) {
		GeoFullPhysVol* volume = dynamic_cast<GeoFullPhysVol*>(vol);
		volume->add(volChild);
	}
}


void ReadGeoModel::checkInputString(QString input)
{
	if (input.isEmpty() || input.isNull() || input == "NULL") {
		qWarning() << "ERROR!!! Input QString is empty or equal to 'NULL'!!! Aborting...";
		exit(1);
	}
}


// Instantiate a PhysVol and get its children
GeoVPhysVol* ReadGeoModel::buildVPhysVol(QString id, QString tableId, QString copyN)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildVPhysVol()" << id << tableId << copyN;
    muxCout.unlock();
  }

	checkInputString(id);
	checkInputString(tableId);

	// if previously built, return that // TODO: apparently this is only used for VPhysVol... check that, because it should be used for all nodes, otherwise we create new nodes in memory even if we have a copy of them already...
	if (isNodeBuilt(id, tableId, copyN)) {
		if (m_deepDebug) {
      muxCout.lock();
      qDebug() << "getting the volume from memory...";
      muxCout.unlock();
    }
		return dynamic_cast<GeoVPhysVol*>(getNode(id, tableId, copyN));
	}

	GeoVPhysVol* vol = nullptr;
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "building a new volume...";
    muxCout.unlock();
  }
	vol = buildNewVPhysVol(id, tableId, copyN);
	// if (m_deepDebug) std::cout << "--> built a new volume: " << vol << std::endl;
	return vol;
}


// Build a new VPhysVol volume and store it
GeoVPhysVol* ReadGeoModel::buildNewVPhysVol(QString id, QString tableId, QString copyN)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildNewVPhysVol() - building a new volume...";
    muxCout.unlock();
  }

	QString nodeType = m_tableid_tableName[tableId.toUInt()];

	// get the parent volume parameters
	// here we do not need to use copyN, since the actual volume is the same for all instances
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tget the parent...";
    muxCout.unlock();
  }
	QStringList values;
	if (nodeType == "GeoPhysVol")
		values = m_physVols[id.toUInt()];
	else if (nodeType == "GeoFullPhysVol")
		values = m_fullPhysVols[id.toUInt()];


	QString volId = values[0];
	QString logVolId = values[1];

	if (m_deepDebug) {
    muxCout.lock();
		// std::cout << "Waiting..." << std::flush;
  	qDebug() << "\tPhysVol-ID:" << volId;
  	qDebug() << "\tPhysVol-LogVol:" << logVolId;
  	qDebug() << "\tnodeType:" << nodeType;
    muxCout.unlock();
	}

	// GET LOGVOL
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tget LogVol...";
    muxCout.unlock();
  }
	GeoLogVol* logVol = buildLogVol(logVolId); // TODO: For PhysVols we first check if we created the vol in memory already; but I'm not sure about other nodes. Check it...

	// a pointer to the VPhysVol
	GeoVPhysVol* vol = nullptr;

	// BUILD THE PHYSVOL OR THE FULLPHYSVOL
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tbuild the VPhysVol...";
    muxCout.unlock();
  }
	if (nodeType == "GeoPhysVol")
		vol = new GeoPhysVol(logVol);
	else if (nodeType == "GeoFullPhysVol")
		vol = new GeoFullPhysVol(logVol);
	else {
    muxCout.lock();
		qWarning() << "ERROR!!! Unkonwn node type!! : " << nodeType;
    muxCout.unlock();
  }

	// storing the address of the newly built node
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tstoring the VPhysVol...";
    muxCout.unlock();
  }
	storeNode(id, tableId, copyN, vol);

	return vol;
}

// Get the root volume
GeoPhysVol* ReadGeoModel::getRootVolume()
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::getRootVolume()";
    muxCout.unlock();
  }
	QString id = m_root_vol_data[1];
	QString tableId = m_root_vol_data[2];
	QString copyNumber = "1"; // the Root volume has only one copy by definition
	return dynamic_cast<GeoPhysVol*>(buildVPhysVol(id, tableId, copyNumber));
}


GeoMaterial* ReadGeoModel::buildMaterial(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildMaterial()";
    muxCout.unlock();
  }
	QStringList values = m_materials[id.toUInt()];

	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "mat values=" << values;
    muxCout.unlock();
  }
	QString matId = values[0];
	QString matName = values[1];
	double matDensity = values[2].toDouble();
	QString matElements = values[3];

	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tMaterial - ID:" << matId
		  << ", name:" << matName
		  << ", density:" << matDensity
		  << " ( " << matDensity / (SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3) << "[g/cm3] )"
		  << ", elements:" << matElements;
    muxCout.unlock();
  }

	GeoMaterial* mat = new GeoMaterial(matName.toStdString(), matDensity);

	if (matElements.size() > 0) {
		// get parameters from DB string
		QStringList elements = matElements.split(";");
		foreach( QString par, elements) {

		  if (m_deepDebug) {
        muxCout.lock();
        qDebug() << "par:" << par;
        muxCout.unlock();
      }
			QStringList vars = par.split(":");
			QString elId = vars[0];
			double elFraction = vars[1].toDouble();

			GeoElement* el = buildElement(elId);

			mat->add(el, elFraction);
		}
		mat->lock();
	}
	return mat;
}

GeoElement* ReadGeoModel::buildElement(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildElement()";
    muxCout.unlock();
  }

	if (m_elements.size() == 0)
		qFatal("ERROR! 'm_elements' is empty! Did you load the 'Elements' table? \n\t ==> Aborting...");

	QStringList values = m_elements[id.toUInt()];

	QString elId = values[0];
	QString elName = values[1];
	QString elSymbol = values[2];
	double elZ = values[3].toDouble();
	double elA = values[4].toDouble();

	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tElement - ID:" << elId
			<< ", name:" << elName
			<< ", symbol:" << elSymbol
			<< ", Z:" << elZ
			<< ", A:" << elA
			<< " ( " << elA / (SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole) << "[g/mole] )";
    muxCout.unlock();
  }

	return new GeoElement(elName.toStdString(), elSymbol.toStdString(), elZ, elA);
}


GeoShape* ReadGeoModel::buildShape(QString shapeId)
{
	if (m_deepDebug) {
     muxCout.lock();
    std::cout << "ReadGeoModel::buildShape()" << std::endl;
     muxCout.unlock();
  }

//   try // TODO: implement try/catch
//   {
	QStringList paramsShape = m_shapes[ shapeId.toUInt() ];

	QString id = paramsShape[0];
	QString type = paramsShape[1];
	QString parameters = paramsShape[2];

	if (m_deepDebug) {
     muxCout.lock();
    std::cout << "\tShape-ID:" << id.toStdString() << ", Shape-type:" << type.toStdString();
     muxCout.unlock();
  }


	if (type == "Box") {
			// shape parameters
			double XHalfLength = 0.;
			double YHalfLength = 0.;
			double ZHalfLength = 0.;
			// get parameters from DB string
			QStringList shapePars = parameters.split(";");
			foreach( QString par, shapePars) {
					QStringList vars = par.split("=");
					QString varName = vars[0];
					QString varValue = vars[1];
					if (varName == "XHalfLength") XHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "YHalfLength") YHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			}
		return new GeoBox(XHalfLength, YHalfLength, ZHalfLength);
	}
	else if (type == "Cons") {
		// shape parameters
		double RMin1 = 0.;
		double RMin2 = 0.;
		double RMax1 = 0.;
		double RMax2 = 0.;
		double DZ = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
      // qWarning() << "varValue Cons:" << varValue;
			if (varName == "RMin1") RMin1 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMin2") RMin2 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax1") RMax1 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax2") RMax2 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "DZ") DZ = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi") SPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi") DPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
		}
		return new GeoCons (RMin1, RMin2, RMax1, RMax2, DZ, SPhi, DPhi);
	}
	else if (type == "Torus") {
		// Member Data:
		// * Rmax - outside radius of the torus tube
		// * Rmin - inside radius  of the torus tube (Rmin=0 if not hollow)
		// * Rtor - radius of the torus itself
		// *
		// * SPhi - starting angle of the segment in radians
		// * DPhi - delta angle of the segment in radians
		//
		// shape parameters
		double Rmin = 0.;
		double Rmax = 0.;
		double Rtor = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "Rmin") Rmin = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "Rmax") Rmax = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "Rtor") Rtor = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi") SPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi") DPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
		}
		return new GeoTorus (Rmin, Rmax, Rtor, SPhi, DPhi);
	}
	else if (type == "Para") {
		// shape parameters
		double XHalfLength = 0.;
		double YHalfLength = 0.;
		double ZHalfLength = 0.;
		double Alpha = 0.;
		double Theta = 0.;
		double Phi = 0.;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "XHalfLength") XHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "YHalfLength") YHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "Alpha") Alpha = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "Theta") Theta = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "Phi") Phi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
		}
		return new GeoPara (XHalfLength, YHalfLength, ZHalfLength, Alpha, Theta, Phi);
	}
	else if (type == "Pcon") {
		// shape parameters
		double SPhi = 0.;
		double DPhi = 0.;
		unsigned int NZPlanes = 0;

		// get parameters from DB string
		QStringList shapePars = parameters.split(";");

		bool error = 0;
		QString par;
		QStringList vars;
		QString varName;
		QString varValue;

		GeoPcon* pcon = nullptr;

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the three first GeoPcon parameters: the SPhi and DPhi angles, plus the number of Z planes
			for( int it=0; it < 3; it++) {
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				if (varName == "SPhi") SPhi = varValue.toDouble();
				if (varName == "DPhi") DPhi = varValue.toDouble();
				if (varName == "NZPlanes") NZPlanes = varValue.toDouble();
			}
			// build the basic GeoPcon shape
			pcon = new GeoPcon(SPhi, DPhi);

			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=3; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "it:" << it << "par:" << par << "varName:" << varName << "varValue:" << varValue;

				if (varName == "ZPos") {

					double zpos = varValue.toDouble();
					double rmin=0., rmax=0.;

					it++; // go to next variable

					par = shapePars[it];
					vars = par.split("=");
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmin") rmin = varValue.toDouble();
					else error = 1;
					it++; // go to next variable

					par = shapePars[it];
					vars = par.split("=");
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmax") rmax = varValue.toDouble();
					else error = 1;

					if(error) {
            muxCout.lock();
            qWarning() << "ERROR! GeoPcon 'ZRmin' and 'ZRmax' values are not at the right place! --> " << shapePars;
            muxCout.unlock();
          }

					// add a Z plane to the GeoPcon
					pcon->addPlane(zpos, rmin, rmax);
				} else {
					error = 1;
          muxCout.lock();
					qWarning() << "ERROR! GeoPcon 'ZPos' value is not at the right place! --> " << shapePars;
          muxCout.unlock();
				}
			}

			// sanity check on the resulting Pcon shape
			if( pcon->getNPlanes() != NZPlanes) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoPcon number of planes: " << QString::number(pcon->getNPlanes()) << " is not equal to the original size! --> " << shapePars;
        muxCout.unlock();
			}
			if(!pcon->isValid()) {
				error = 1;
        muxCout.lock();
        qWarning() << "ERROR! GeoPcon shape is not valid!! -- input: " << shapePars;
        muxCout.unlock();
			}
	  } // end if (size>3)
		else {
      muxCout.lock();
      qWarning() << "ERROR!! GeoPcon has no Z planes!! --> shape input parameters: " << shapePars;
      muxCout.unlock();
			error = 1;
		}

		if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoPcon shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }

		return pcon;
	}
	else if (type == "Pgon") {
		// shape parameters
		double SPhi = 0.;
		double DPhi = 0.;
		unsigned int NSides = 0;
		unsigned int NZPlanes = 0;

		bool error = false;
		GeoPgon* pgon = nullptr;
		QString par;
		QStringList vars;
		QString varName;
		QString varValue;

		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		// qInfo() << "shapePars: " << shapePars; // debug

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the first four GeoPgon parameters: the SPhi and DPhi angles, plus the number of Z planes
			for( int it=0; it < 4; it++) {
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "vars: " << vars; // for debug only
				if (varName == "SPhi") SPhi = varValue.toDouble();
				if (varName == "DPhi") DPhi = varValue.toDouble();
				if (varName == "NSides") NSides = varValue.toUInt();// * SYSTEM_OF_UNITS::mm;
				if (varName == "NZPlanes") NZPlanes = varValue.toDouble();

			}
			// build the basic GeoPgon shape
			pgon = new GeoPgon(SPhi, DPhi, NSides);

			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=4; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "it:" << it << "par:" << par << "varName:" << varName << "varValue:" << varValue;

				if (varName == "ZPos") {

					double zpos = varValue.toDouble();
					double rmin=0., rmax=0.;

					it++; // go to next variable

					par = shapePars[it];
					vars = par.split("=");
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmin") rmin = varValue.toDouble();
					else error = 1;
					it++; // go to next variable

					par = shapePars[it];
					vars = par.split("=");
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmax") rmax = varValue.toDouble();
					else error = 1;

					if(error) {
            muxCout.lock();
            qWarning() << "ERROR! GeoPgon 'ZRmin' and 'ZRmax' values are not at the right place! --> " << shapePars;
            muxCout.unlock();
          }

					// add a Z plane to the GeoPgon
					pgon->addPlane(zpos, rmin, rmax);
				} else {
					error = 1;
          muxCout.lock();
					qWarning() << "ERROR! GeoPgon 'ZPos' value is not at the right place! --> " << shapePars;
          muxCout.unlock();
				}
			}

			// sanity check on the resulting Pgon shape
			if( pgon->getNPlanes() != NZPlanes) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoPgon number of planes: " << QString::number(pgon->getNPlanes()) << " is not equal to the original size! --> " << shapePars;
        muxCout.unlock();
			}
			if(!pgon->isValid()) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoPgon shape is not valid!! -- input: " << shapePars;
        muxCout.unlock();
			}
		} // end if (size>3)
		else {
      muxCout.lock();
			qWarning() << "ERROR!! GeoPgon has no Z planes!! --> shape input parameters: " << shapePars;
      muxCout.unlock();
			error = 1;
		}
		if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoPgon shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		return pgon;
	}
	else if (type == "GenericTrap") {
		// shape parameters
		double ZHalfLength = 0.;
		unsigned int NVertices = 0;
		GeoGenericTrapVertices Vertices;
		bool error = false;
		GeoGenericTrap* gTrap = nullptr;
		QString par;
		QStringList vars;
		QString varName;
		QString varValue;

		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		// qInfo() << "shapePars: " << shapePars; // debug

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the two GeoGenericTrap parameters: the ZHalfLength plus the number of vertices
			for( int it=0; it < 2; it++) {
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "vars: " << vars; // for debug only
				if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();
				if (varName == "NVertices") NVertices = varValue.toUInt();
			}



			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=2; it < NVertices; it++)
			{
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "it:" << it << "par:" << par << "varName:" << varName << "varValue:" << varValue;

				if (varName == "X") {

					double x = varValue.toDouble();

					it++; // go to next variable

					par = shapePars[it];
					vars = par.split("=");
					varName = vars[0];
					varValue = vars[1];
					if (varName == "Y") {
					  double y = varValue.toDouble();
					  Vertices.push_back(GeoTwoVector(x,y));
					}
					else {
					  error = 1;
					}
					if(error) {
            muxCout.lock();
            qWarning() << "ERROR! GeoGenericTrap 'X' and 'Y' values are not at the right place! --> " << shapePars;
            muxCout.unlock();
          }
				} else {
					error = 1;
          muxCout.lock();
					qWarning() << "ERROR! GeoGenericTrap 'ZPos' value is not at the right place! --> " << shapePars;
          muxCout.unlock();
				}
			}
      if(error) {
        muxCout.lock();
        std::cout << "FATAL ERROR!!! - GeoGenericTrap shape error!!! Aborting..." << std::endl;
        muxCout.unlock();
        exit(EXIT_FAILURE);
      }
			// build the basic GenericTrap shape
			gTrap = new GeoGenericTrap(ZHalfLength,Vertices);
		} // end if (size>3)
		else {
			qWarning() << "ERROR!! GeoGenericTrap has no Z vertices!! --> shape input parameters: " << shapePars;
			error = 1;
		}
    if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoGenericTrap shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		return gTrap;
	}
	else if (type == "SimplePolygonBrep") {
		//qInfo() << "Reading-in: SimplePolygonBrep: "; // debug
		// shape parameters
		double DZ = 0.;
		unsigned int NVertices = 0;
		double xV = 0.;
		double yV = 0.;

		bool error = 0;
		GeoSimplePolygonBrep* sh = nullptr;
		QString par;
		QStringList vars;
		QString varName;
		QString varValue;

		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		//qInfo() << "shapePars: " << shapePars; // debug

		int sizePars = shapePars.size();
		// check if we have more than 2 parameters
		if (sizePars > 2) {

			// get the first two GeoSimplePolygonBrep parameters: DZ and the number of vertices.
			for( int it=0; it < 2; it++) {
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "vars: " << vars; // for debug only
				if (varName == "DZ") DZ = varValue.toDouble();
				if (varName == "NVertices") NVertices = varValue.toDouble();
				//else if (varName == "NVertices") NVertices = varValue.toDouble();
				//else error = 1;
				//if(error) qWarning() << "ERROR! GeoSimplePolygonBrep parameters are not correctly stored! -->" << vars;

			}
			// build the basic GeoSimplePolygonBrep shape
			sh = new GeoSimplePolygonBrep(DZ);

			// and now loop over the rest of the list, to get the parameters of all vertices
			for (int it=2; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				if (varName == "xV") xV = varValue.toDouble();
				else error = 1;

				it++; // go to next variable (they come in pairs)

				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				if (varName == "yV") yV = varValue.toDouble();
				else error = 1;

				if(error) {
          muxCout.lock();
          qWarning() << "ERROR! GeoSimplePolygonBrep 'xVertex' and 'yVertex' values are not at the right place! --> " << shapePars;
          muxCout.unlock();
        }

				// add a Z plane to the GeoSimplePolygonBrep
				sh->addVertex(xV, yV);
			}
			// sanity check on the resulting shape
			if( sh->getNVertices() != NVertices) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoSimplePolygonBrep number of planes: " << QString::number(sh->getNVertices()) << " is not equal to the original size! --> " << shapePars;
        muxCout.unlock();
			}
			if(!sh->isValid()) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoSimplePolygonBrep shape is not valid!! -- input: " << shapePars;
        muxCout.unlock();
			}
		} // end if (size>3)
		else {
      muxCout.lock();
			qWarning() << "ERROR!! GeoSimplePolygonBrep has no vertices!! --> shape input parameters: " << shapePars;
      muxCout.unlock();
			error = 1;
		}
    if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoSimplePolygonBrep shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		return sh;

	}
	else if (type == "TessellatedSolid") {
		// Tessellated pars example: "nFacets=1;TRI;vT=ABSOLUTE;nV=3;xV=0;yV=0;zV=1;xV=0;yV=1;zV=0;xV=1;yV=0;zV=0"
		qInfo() << "Reading-in: TessellatedSolid: "; // debug
		// Facet type
		QString facetType = "";
		// shape parameters
		unsigned int nFacets = 0;

		bool error = 0;
		GeoTessellatedSolid* sh = nullptr;
		QString par;
		QStringList vars;
		QString varName;
		QString varValue;

		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
    if (m_deepDebug) {
      muxCout.lock();
      qInfo() << "shapePars: " << shapePars; // debug
      muxCout.unlock();
    }

		int sizePars = shapePars.size();
		// check if we have at least 13 parameters,
		// which is the minimum for a shape
		// with a single triangular facet
		if (sizePars >= 13) {

			// get the first parameter
			par = shapePars[0];
			vars = par.split("=");
			varName = vars[0];
			varValue = vars[1];
			if (varName == "nFacets") nFacets = varValue.toInt();
			else {
        muxCout.lock();
        qWarning("ERROR!! - GeoTessellatedSolid - nFacets is not defined!!");
        muxCout.unlock();
				error = true; // TODO: check "error.h" functionalities and replace with that if useful
			}

			// build the basic GeoTessellatedSolid shape
			sh = new GeoTessellatedSolid();

			// and now loop over the rest of the list,
			// to get the parameters of the vertices of each facet
			// and to build the full GeoTessellatedSolid with all the facets
			for (int it=1; it < sizePars; it++)
			{
				// get facet type
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				if (varName == "QUAD") {
					facetType = "QUAD";
				}
				else if (varName == "TRI") {
					facetType = "TRI";
				}
				else {
          muxCout.lock();
					qWarning() << "ERROR!! - GeoTessellatedSolid - Facet type is not defined! [got: '" << varName << "']";
          muxCout.unlock();
					error = true;
				}

				it++; // advance to the next parameter

				// get the type of the vertexes composing the facet
				bool isAbsolute = true;
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				if (varName == "vT") {
					if (varValue == "ABSOLUTE") isAbsolute = true;
					else if (varValue == "RELATIVE") isAbsolute = false;
					else {
            muxCout.lock();
						qWarning() << "ERROR! - GeoTessellatedSolid - Vertex type not defined!";
            muxCout.unlock();
						error=true;
					}
				}
				else error = 1;

				it++; // advance to the next parameter

				unsigned int nVertexes = 0;
				par = shapePars[it];
				vars = par.split("=");
				varName = vars[0];
				varValue = vars[1];
				if (varName == "nV") nVertexes = varValue.toUInt();
				else {
          muxCout.lock();
					qWarning() << "ERROR! - GeoTessellatedSolid - nVertices not defined!";
          muxCout.unlock();
					error=true;
				}


				// qDebug() << "it:" << it;
				// if we get a QUAD ==> GeoQuadrangularFacet
				if (facetType=="QUAD") {

          muxCout.lock();
					qDebug() << "Handling a QUAD facet...";
          muxCout.unlock();
					// to store the 4 vertices of the GeoQuadrangularFacet
					auto vV = std::vector<std::unique_ptr<GeoFacetVertex>>{};

					// we look for 4 vertices for QUAD;
					// for each of them, we get 3 coordinates
					//					vertStart = it;
					for (unsigned int iV=0; iV<4; ++iV) {

						it++; // advance to the first of the facet's vertices' coordinates

						double xV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "xV") xV = varValue.toDouble();
						else {
              muxCout.lock();
							qWarning() << "ERROR! Got '" << varName << "' instead of 'xV'!";
              muxCout.unlock();
							error = 1;
						}

						it++; // go to the next coordinate

						double yV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "yV") yV = varValue.toDouble();
						else {
              muxCout.lock();
							qWarning() << "ERROR! Got '" << varName << "' instead of 'yV'!";
              muxCout.unlock();
							error = 1;
						}

						it++; // go to the next coordinate

						double zV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "zV") zV = varValue.toDouble();
						else {
              muxCout.lock();
							qWarning() << "ERROR! Got '" << varName << "' instead of 'zV'!";
              muxCout.unlock();
							error = 1;
						}

						if(error) {
              muxCout.lock();
              qWarning() << "ERROR! GeoTessellatedSolid 'xV', 'yV', and 'zV' values are not at the right place! --> " << shapePars;
              muxCout.unlock();
            }

						// build the facet's vertex and store it
						vV.push_back(std::make_unique<GeoFacetVertex>( GeoFacetVertex(xV,yV,zV)) );
					}

					// build the facet and add it to the GeoTessellatedSolid
					GeoQuadrangularFacet* quadFacet = new GeoQuadrangularFacet(*vV[0], *vV[1], *vV[2], *vV[3], (isAbsolute ? GeoFacet::ABSOLUTE : GeoFacet::RELATIVE));
					sh->addFacet(quadFacet);
				}
				// if we get a TRI ==> GeoTriangularFacet
				else if (facetType=="TRI") {

          muxCout.lock();
					qDebug() << "Handling a TRI facet...";
          muxCout.unlock();

//					std::vector<GeoFacetVertex*> vV(3, 0); // to store the 3 vertices of the GeoTriangularFacet
					auto vV = std::vector<std::unique_ptr<GeoFacetVertex>>{};

					// we look for 3 vertices for GeoTriangularFacet;
					// for each of them, we get 3 coordinates
//					vertStart = it;
					for (unsigned int iV=0; iV<3; ++iV) {

						it++; // advance to the first of the facet's vertices' coordinates

						double xV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "xV") xV = varValue.toDouble();
						else error = 1;

						it++; // go to the next coordinate

						double yV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "yV") yV = varValue.toDouble();
						else error = 1;

						it++; // go to the next coordinate

						double zV=0.;
						par = shapePars[it];
						vars = par.split("=");
						varName = vars[0];
						varValue = vars[1];
						if (varName == "zV") zV = varValue.toDouble();
						else error = 1;

						if(error) {
              muxCout.lock();
              qWarning() << "ERROR! GeoTessellatedSolid 'xV', 'yV', and 'zV' values are not at the right place! --> " << shapePars;
              muxCout.unlock();
            }

						// build the facet's vertex and store it
						vV.push_back(std::make_unique<GeoFacetVertex>( GeoFacetVertex(xV,yV,zV)) );
					}

					// build the facet and add it to the GeoTessellatedSolid
					GeoTriangularFacet* triFacet = new GeoTriangularFacet(*vV[0], *vV[1], *vV[2], (isAbsolute ? GeoFacet::ABSOLUTE : GeoFacet::RELATIVE));
					sh->addFacet(triFacet);
				}


			}

			// sanity check on the resulting shape
			if( sh->getNumberOfFacets() != nFacets) {
				error = 1;
        muxCout.lock();
				qWarning() << "ERROR! GeoTessellatedSolid number of facets: " << QString::number(sh->getNumberOfFacets()) << " is not equal to the original size! --> " << shapePars;
        muxCout.unlock();
			}
			/*
			 * TODO: uncomment it, when the isValid() method will be implemented for GeoTessellatedSolid
			if(!sh->isValid()) {
				error = 1;
				qWarning() << "ERROR! GeoTessellatedSolid shape is not valid!! -- input: " << shapePars;
			}
			*/
		} // END OF if (size>13)
		else {
      muxCout.lock();
			qWarning() << "ERROR!! GeoTessellatedSolid has no facets!! --> shape input parameters: " << shapePars;
      muxCout.unlock();
			error = 1;
		}
		if(error) {
      muxCout.lock();
      std::cout << "GeoTessellatedSolid shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		return sh;

	}
	else if (type == "Trap") {
			// shape constructor parameters
			double ZHalfLength = 0.;
			double Theta = 0.;
			double Phi = 0.;
			double Dydzn = 0.;
			double Dxdyndzn = 0.;
			double Dxdypdzn = 0.;
			double Angleydzn = 0.;
			double Dydzp = 0.;
			double Dxdyndzp = 0.;
			double Dxdypdzp = 0.;
			double Angleydzp = 0.;
			// get parameters from DB string
			QStringList shapePars = parameters.split(";");
			foreach( QString par, shapePars) {
					QStringList vars = par.split("=");
					QString varName = vars[0];
					QString varValue = vars[1];
					if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Theta") Theta = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Phi") Phi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dydzn") Dydzn = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdyndzn") Dxdyndzn = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdypdzn") Dxdypdzn = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Angleydzn") Angleydzn = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dydzp") Dydzp = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdyndzp") Dxdyndzp = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdypdzp") Dxdypdzp = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "Angleydzp") Angleydzp = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			}
		return new GeoTrap (ZHalfLength, Theta, Phi, Dydzn, Dxdyndzn, Dxdypdzn, Angleydzn, Dydzp, Dxdyndzp, Dxdypdzp, Angleydzp);
	}
	else if (type == "Trd") {
			// shape constructor parameters
			double XHalfLength1 = 0.;
			double XHalfLength2 = 0.;
			double YHalfLength1 = 0.;
			double YHalfLength2 = 0.;
			double ZHalfLength = 0.;
			// get parameters from DB string
			QStringList shapePars = parameters.split(";");
			foreach( QString par, shapePars) {
					QStringList vars = par.split("=");
					QString varName = vars[0];
					QString varValue = vars[1];
          // qWarning() << "varValue:" << varValue;
					if (varName == "XHalfLength1") XHalfLength1 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "XHalfLength2") XHalfLength2 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "YHalfLength1") YHalfLength1 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "YHalfLength2") YHalfLength2 = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
					if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			}
		return new GeoTrd (XHalfLength1, XHalfLength2, YHalfLength1, YHalfLength2, ZHalfLength);
	}
	else if (type == "Tube") {
		// shape parameters
		double RMin = 0.;
		double RMax = 0.;
		double ZHalfLength = 0.;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "RMin") RMin = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax") RMax = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
		}
		return new GeoTube(RMin, RMax, ZHalfLength);
	}
	else if (type == "Tubs") {
		// shape parameters
		double RMin = 0.;
		double RMax = 0.;
		double ZHalfLength = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "RMin") RMin = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax") RMax = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi") SPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi") DPhi = varValue.toDouble();// * SYSTEM_OF_UNITS::mm;
		}
		return new GeoTubs (RMin, RMax, ZHalfLength, SPhi, DPhi);
	}
	else if (type == "Intersection") {
		// shape parameters
		unsigned int opA = 0;
		unsigned int opB = 0;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "opA") opA = varValue.toUInt();
			if (varName == "opB") opB = varValue.toUInt();
		}
		// get the referenced shape
		const GeoShape* shapeA = buildShape( QString::number(opA) );
		const GeoShape* shapeB = buildShape( QString::number(opB) );
		// build and return the GeoShapeShift instance
		return new GeoShapeIntersection(shapeA, shapeB);
	}
	else if (type == "Shift") {
		// shape parameters
		unsigned int shapeId = 0;
		unsigned int transfId = 0;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "A") shapeId = varValue.toUInt();
			if (varName == "X") transfId = varValue.toUInt();
		}
		// get the referenced shape
		const GeoShape* shapeA = buildShape( QString::number(shapeId) );
		// get the referenced Transform
		QStringList transPars = m_transforms[transfId];
		if (m_deepDebug) qDebug() << "child:" << transPars;
		GeoTransform* transf = parseTransform(transPars);
		const GeoTrf::Transform3D transfX = transf->getTransform();
        transf->unref(); // delete the transf from the heap

		// qWarning() << "GeoShift:";
		// printTrf(transfX);

		// build and return the GeoShapeShift instance
		return new GeoShapeShift(shapeA, transfX);
	}
	else if (type == "Subtraction") {
		// shape parameters
		unsigned int opA = 0;
		unsigned int opB = 0;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "opA") opA = varValue.toUInt();
			if (varName == "opB") opB = varValue.toUInt();
		}

        if (opA == 0 || opB == 0) {
          std::cout << "ERROR! Subtraction shape - input shape IDs are empty! (opA: " << opA << ", opB:" << opB << ")" << std::endl;
          exit(EXIT_FAILURE);
        }

        // get the referenced shapes
        QString shAId = QString::number(opA);
        const GeoShape* shapeA = buildShape( shAId );

        QString shBId = QString::number(opB);
        const GeoShape* shapeB = buildShape( shBId );

       if ( shapeA == NULL) {
           std::cout << "ERROR!!! shapeA is NULL!" << std::endl;
           exit(EXIT_FAILURE);
       }

       //        std::cout << "Subtraction -- " << "opA: " << opA << ", opB: " << opB << ", shapeA: " << shapeA << ", shapeB: " << shapeB << std::endl;

		   // build and return the GeoShapeSubtraction instance
      return new GeoShapeSubtraction(shapeA, shapeB);
	}
	else if (type == "Union") {
		// shape parameters
		unsigned int opA = 0;
		unsigned int opB = 0;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "opA") opA = varValue.toUInt();
			if (varName == "opB") opB = varValue.toUInt();
		}
		if (opA == 0 || opB == 0) {
          muxCout.lock();
          std::cout << "ERROR! 'GeoUnion' shape - opA or opB have not been properly intialized!" << std::endl;
          muxCout.unlock();
          exit(EXIT_FAILURE);
        }
		// get the referenced shape
		const GeoShape* shapeA = buildShape( QString::number(opA) );
		const GeoShape* shapeB = buildShape( QString::number(opB) );
		// build and return the GeoShapeShift instance

//        std::cout << "Union -- " << "opA: " << opA << ", opB: " << opB << ", shapeA: " << shapeA << ", shapeB: " << shapeB << std::endl;
		return new GeoShapeUnion(shapeA, shapeB);
	}
  //LAr custom shape
  else if(type == "CustomShape") {
    std::string name = "";
    // get parameters from DB string
  	QStringList shapePars = parameters.split(";");
  	// qWarning() << "shapePars: " << shapePars << shapePars.size() << shapePars.isEmpty();
  	if ( shapePars.size() > 0 && ((shapePars.filter("=")).size() > 0) )  // this complex test is needed to handle null strings
  	{
  		foreach( QString par, shapePars) {
  				QStringList vars = par.split("=");
  				QString varName = vars[0];
  				QString varValue = vars[1];
  				if (varName == "name") name = varValue.toStdString();
  			}
  	} else {
      muxCout.lock();
      // throw std::invalid_argument("CustomShape parameters' list is empty!!");
      std::cout << "ERROR!!! --> CustomShape parameters' list is empty!! It seems the geometry file you are running on is corrupted." << std::endl;
      muxCout.unlock();
  		exit(EXIT_FAILURE);
  	}
      return new GeoUnidentifiedShape("LArCustomShape",name);
  }
  else if (type=="UnidentifiedShape") {
    std::string name = "";
    std::string asciiData = "";
    // get parameters from DB string
    QStringList shapePars = parameters.split(";");
    if ( shapePars.size() > 0 && ((shapePars.filter("=")).size() > 0) )  // this complex test is needed to handle null strings
  	{
      foreach( QString par, shapePars) {
        QStringList vars = par.split("=");
        QString varName = vars[0];
        QString varValue = vars[1];
        if (varName == "name") name = varValue.toStdString();
        if (varName == "asciiData") asciiData=varValue.toStdString();
      }
    } else {
      // throw std::invalid_argument("UnidentifiedShape parameters' list is empty!!");
      muxCout.lock();
      std::cout << "ERROR!!! --> UnidentifiedShape parameters' list is empty!! It seems the geometry file you are running on is corrupted." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
    return new GeoUnidentifiedShape(name,asciiData);

  }
  else {
    // QString msg = "WARNING!! - Shape '" + type + "' not implemented yet!!! Returning a dummy cube.";
    // qWarning(msg.toStdString().c_str());
    m_unknown_shapes.insert(type.toStdString()); // save unknwon shapes for later warning message
    return new GeoBox(30.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm); // FIXME: bogus shape. Use actual shape!
  }

// }

// catch (std::invalid_argument& e)
// {
//     std::cerr << e.what() << std::endl;
//     exit(EXIT_FAILURE);
// }


}


GeoLogVol* ReadGeoModel::buildLogVol(QString logVolId)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildLogVol()";
    muxCout.unlock();
  }

	// get logVol properties from the DB
	QStringList values = m_logVols[logVolId.toUInt()];
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "buildLogVol() - params:" << values;
    muxCout.unlock();
  }

	// build the LogVol
	QString logVolName = values[1];

	// GET LOGVOL SHAPE
	QString shapeId = values[2];
	GeoShape* shape = buildShape(shapeId);

	// GET LOGVOL MATERIAL
	QString matId = values[3];
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "buildLogVol() - material Id:" << matId;
    muxCout.unlock();
  }
	GeoMaterial* mat = buildMaterial(matId);

	return new GeoLogVol(logVolName.toStdString(), shape, mat);

}


GeoSerialDenominator* ReadGeoModel::buildSerialDenominator(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildSerialDenominator()";
    muxCout.unlock();
  }
	return parseSerialDenominator( m_serialDenominators[id.toUInt()] );
}

GeoSerialDenominator* ReadGeoModel::parseSerialDenominator(QStringList values)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::parseSerialDenominator()";
    muxCout.unlock();
  }
	QString id = values[0];
	QString baseName = values[1];
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "\tparseSerialDenominator() - ID:" << id << ", base-name:" << baseName;
    muxCout.unlock();
  }
	return new GeoSerialDenominator(baseName.toStdString());
}

// TODO: this should be moved to an Utilities class!
void ReadGeoModel::printTrf(GeoTrf::Transform3D t) {
  muxCout.lock();
	std::cout << "transformation: " << std::endl;
	std::cout << "[[" << t(0, 0) << " , ";
	std::cout <<         t(0, 1) << " , ";
	std::cout <<         t(0, 2) << " , ";
	std::cout <<         t(0, 3) << " ]";
	std::cout << "["  << t(1, 0) << " , ";
	std::cout <<         t(1, 1) << " , ";
	std::cout <<         t(1, 2) << " , ";
	std::cout <<         t(1, 3) << " ]";
	std::cout << "["  << t(2, 0) << " , ";
	std::cout <<         t(2, 1) << " , ";
	std::cout <<         t(2, 2) << " , ";
	std::cout <<         t(2, 3) << " ]";
	std::cout << "["  << t(3, 0) << " , ";
	std::cout <<         t(3, 1) << " , ";
	std::cout <<         t(3, 2) << " , ";
	std::cout <<         t(3, 3) << " ]]" << std::endl;
  muxCout.unlock();
}

// TODO: should go in a QtUtils header-only class, to be used in other packages
QList<double> ReadGeoModel::convertQstringListToDouble(QStringList listin) {
	QList<double> listout;
  foreach (const QString &s, listin) {
      listout.append(s.toDouble());
  }
	return listout;
}

void ReadGeoModel::printTransformationValues(QStringList values) {
	QList<double> t = convertQstringListToDouble(values);
  muxCout.lock();
  std::cout << "transformation input values: " << std::endl;
	qWarning() << "[[" << t[0] << "," << t[1] << "," << t[2] << "]["
	                   << t[3] << "," << t[4] << "," << t[5] << "]["
	                   << t[6] << "," << t[7] << "," << t[8] << "]["
	                   << t[9] << "," << t[10] << "," << t[11] << "]]";
  muxCout.unlock();
}


GeoAlignableTransform* ReadGeoModel::buildAlignableTransform(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildAlignableTransform()";
    muxCout.unlock();
  }
	return parseAlignableTransform( m_alignableTransforms[id.toUInt()] );
}

GeoAlignableTransform* ReadGeoModel::parseAlignableTransform(QStringList values)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::parseAlignableTransform()";
    muxCout.unlock();
  }

	QString id = values.takeFirst(); // it pops out the first element, leaving the other items in the list
	// printTransformationValues(values); // DEBUG

	// get the 12 matrix elements
	double xx = values[0].toDouble();
	double xy = values[1].toDouble();
	double xz = values[2].toDouble();

	double yx = values[3].toDouble();
	double yy = values[4].toDouble();
	double yz = values[5].toDouble();

	double zx = values[6].toDouble();
	double zy = values[7].toDouble();
	double zz = values[8].toDouble();

	double dx = values[9].toDouble();
	double dy = values[10].toDouble();
	double dz = values[11].toDouble();

	GeoTrf::Transform3D txf;
	// build the rotation matrix with the first 9 elements
	txf(0,0)=xx;
	txf(0,1)=xy;
	txf(0,2)=xz;

	txf(1,0)=yx;
	txf(1,1)=yy;
	txf(1,2)=yz;

	txf(2,0)=zx;
	txf(2,1)=zy;
	txf(2,2)=zz;

	// build the translation matrix with the last 3 elements
	txf(0,3)=dx;
	txf(1,3)=dy;
	txf(2,3)=dz;

	// printTrf(txf); // DEBUG
	return new GeoAlignableTransform(txf);
}

GeoTransform* ReadGeoModel::buildTransform(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildTransform()";
    muxCout.unlock();
  }
	return parseTransform( m_transforms[id.toUInt()] );
}


GeoTransform* ReadGeoModel::parseTransform(QStringList values)
{
  muxCout.lock();
	if (m_deepDebug) qDebug() << "ReadGeoModel::parseTransform()";
	if (m_deepDebug) qDebug() << "values:" << values;
  muxCout.unlock();

	QString id = values.takeFirst(); // it pops out the first element, the 'id', leaving the other items in the list
  	// printTransformationValues(values); // DEBUG

	// get the 12 matrix elements
	double xx = values[0].toDouble();
	double xy = values[1].toDouble();
	double xz = values[2].toDouble();

	double yx = values[3].toDouble();
	double yy = values[4].toDouble();
	double yz = values[5].toDouble();

	double zx = values[6].toDouble();
	double zy = values[7].toDouble();
	double zz = values[8].toDouble();

	double dx = values[9].toDouble();
	double dy = values[10].toDouble();
	double dz = values[11].toDouble();

	GeoTrf::Transform3D txf;
	// build the rotation matrix with the first 9 elements
	txf(0,0)=xx;
	txf(0,1)=xy;
	txf(0,2)=xz;

	txf(1,0)=yx;
	txf(1,1)=yy;
	txf(1,2)=yz;

	txf(2,0)=zx;
	txf(2,1)=zy;
	txf(2,2)=zz;

	// build the translation matrix with the last 3 elements
	txf(0,3) = dx;
	txf(1,3) = dy;
	txf(2,3) = dz;

	// printTrf(txf); // DEBUG
	return new GeoTransform(txf);
}


GeoSerialTransformer* ReadGeoModel::buildSerialTransformer(QString nodeId)
{
  muxCout.lock();
	if (m_deepDebug) qDebug() << "ReadGeoModel::buildSerialTransformer()";
  muxCout.unlock();

	QStringList values = m_serialTransformers[nodeId.toUInt()];
  muxCout.lock();
	if (m_deepDebug) qDebug() << "values:" << values;
  muxCout.unlock();

	// std::cout <<"ST * " << values[0].toStdString() << " " << values[1].toStdString() << " " << values[2].toStdString() << std::endl;
  muxCout.lock();
  if (m_deepDebug) qDebug() << "buildSerialTransformer() - ST * " << values[0] << ", " << values[1] << ", " << values[2] << ", " << values[3] << ", " << values[4];
  muxCout.unlock();

	QString id = values[0];
	QString functionId = values[1];
	QString physVolId = values[2];
	QString physVolTableIdStr = values[3];
	QString copies = values[4];

	unsigned int physVolTableId = physVolTableIdStr.toUInt();

	QString physVolType = m_tableid_tableName[physVolTableId];

  muxCout.lock();
	if (m_deepDebug) qDebug() << "\tbuildSerialTransformer() - ID:" << id << ", functionId:" << functionId << ", physVolId:" << physVolId << ", physVolTableId:" << physVolTableId << ", copies:" << copies;
  muxCout.unlock();

	// GET FUNCTION
  muxCout.lock();
	if (m_deepDebug) qDebug() << "\tbuildSerialTransformer() - function Id:" << functionId;
  muxCout.unlock();
	TRANSFUNCTION func = buildFunction(functionId);

	// GET PHYSVOL
  muxCout.lock();
	if (m_deepDebug) qDebug() << "\tbuildSerialTransformer() - referenced physVol - Id:" << physVolId << ", type:" << physVolType << "tableId:" << physVolTableIdStr;
  muxCout.unlock();
	const GeoVPhysVol* physVol = buildVPhysVol(physVolId, physVolTableIdStr, "1"); // we use "1" as default copyNumber: taking the first copy of the VPhysVol as the referenced volume

  muxCout.lock();
  if (m_deepDebug) qDebug() << "\tbuildSerialTransformer() - physVol:" << physVol << ", function:" << &func;
  muxCout.unlock();

	// get PhysVol or FullPhysVol pointer and return the SerialTransformer
	if (dynamic_cast<const GeoFullPhysVol*>(physVol)) {
		const GeoFullPhysVol* vol = dynamic_cast<const GeoFullPhysVol*>(physVol);
		return new GeoSerialTransformer(vol, &func, copies.toUInt() );
	}
	const GeoPhysVol* vol = dynamic_cast<const GeoPhysVol*>(physVol);
	return new GeoSerialTransformer(vol, &func, copies.toUInt() );

}

TRANSFUNCTION ReadGeoModel::buildFunction(QString id)
{
  muxCout.lock();
	if (m_deepDebug) qDebug() << "ReadGeoModel::buildFunction()";
  muxCout.unlock();
	QStringList values = m_functions[id.toUInt()];
	return parseFunction( values[1].toStdString() );
}

TRANSFUNCTION ReadGeoModel::parseFunction(const std::string& expr)
{
  muxCout.lock();
	if (m_deepDebug) qDebug() << "ReadGeoModel::parseFunction(const std::string& expr)";
	if (m_deepDebug) qDebug() << "expression: " << QString::fromStdString(expr);
  muxCout.unlock();

	if (expr.empty()) {
			qFatal("FATAL ERROR!! Function expression is empty!! Aborting...");
			abort();
	}

  muxCout.lock();
	if (m_deepDebug) qDebug() << "expression:" << QString::fromStdString(expr);
  muxCout.unlock();
  TransFunctionInterpreter interpreter;
	TFPTR func=interpreter.interpret( expr );
	// if (m_deepDebug) qDebug() << "expression interpreted";
	return *(func.release()); // make func returns a pointer to the managed object and releases the ownership, then get the object dereferencing the pointer
}

GeoNameTag* ReadGeoModel::buildNameTag(QString id) // TODO: Remove the "parseABC" methods, put everything into "buildABC". The "parseABC" are there for historical reasons.
{

	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::buildNameTag()";
    muxCout.unlock();
  }
	return parseNameTag( m_nameTags[id.toUInt()] );
}

GeoNameTag* ReadGeoModel::parseNameTag(QStringList values)
{
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "ReadGeoModel::parseNameTag()";
    muxCout.unlock();
  }
	QString id = values[0];
	QString name = values[1];
	if (m_deepDebug) {
    muxCout.lock();
    qDebug() << "nameTag:" << name;
    muxCout.unlock();
  }
	return new GeoNameTag(name.toStdString());
}


bool ReadGeoModel::isNodeBuilt(const QString id, const QString tableId, const QString copyNumber)
{
	std::lock_guard<std::mutex> lk(muxStore);
	// qDebug() << "ReadGeoModel::isNodeBuilt(): " << id << tableId << copyNumber;
	QString key = id + ":" + tableId + ":" + copyNumber;
	return m_memMap.contains(key);
}


GeoGraphNode* ReadGeoModel::getNode(const QString id, const QString tableId, const QString copyN)
{
	std::lock_guard<std::mutex> lk(muxGet);
	if (m_deepDebug) qDebug() << "ReadGeoModel::getNode(): " << id << tableId << copyN;
	QString key = id + ":" + tableId + ":" + copyN;
	return m_memMap[key];
}

void ReadGeoModel::storeNode(const QString id, const QString tableId, const QString copyN, GeoGraphNode* node)
{
	std::lock_guard<std::mutex> lk(muxStore);
	if (m_deepDebug) qDebug() << "ReadGeoModel::storeNode(): " << id << tableId << copyN << node;
	QString key = id + ":" + tableId + ":" + copyN;
	m_memMap[key] = node;
	if (m_deepDebug) qDebug() << "Store done.";
}

} /* namespace GeoModelIO */

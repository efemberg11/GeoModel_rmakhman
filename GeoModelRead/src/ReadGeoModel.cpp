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
 *  - May 2020, R.M.Bianchi
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
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include <stdexcept>
#include <future>
#include <mutex>
#include <chrono>   /* system_clock */
#include <ctime>    /* std::time */
#include <cstdlib>  /* std::getenv */
#include <vector>
#include <unordered_map>
#include <unordered_set>

// mutexes for synchronized access to containers and output streams in multi-threading mode
std::mutex muxVPhysVol;
std::mutex muxShape;
std::mutex muxTransf;
std::mutex muxCout;


using namespace GeoGenfun;
using namespace GeoXF;


namespace GeoModelIO {

ReadGeoModel::ReadGeoModel(GMDBManager* db, unsigned long* progress) : m_progress(nullptr), m_deepDebug(false), m_debug(false), m_timing(false), m_runMultithreaded(false), m_runMultithreaded_nThreads(0)
{
	qDebug() << "===> DumpGeoModelAction: constructor";

	#ifdef GEOMODEL_IO_DEEP_DEBUG
	  m_deepDebug = true;
	  std::cout << "You defined the GEOMODEL_IO_DEEP_DEBUG variable, so you will see a verbose output." << std::endl;
 	#endif
	#ifdef GEOMODEL_IO_DEBUG
	  m_debug = true;
	  std::cout << "You defined the GEOMODEL_IO_DEBUG variable, so you will see a verbose output." << std::endl;
 	#endif
	#ifdef GEOMODEL_IO_TIMING
	  m_debug = true;
	  std::cout << "You defined the GEOMODEL_IO_TIMING variable, so you will see a timing measurement in the output." << std::endl;
 	#endif

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

  // Check if the user asked for running in serial or multi-threading mode
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_NTHREADS"))
  {
    int nThreads = std::stoi(getEnvVar("GEOMODEL_ENV_IO_NTHREADS"));
    if (nThreads == 0) {
      std::cout << "\nYou set the GEOMODEL_ENV_IO_NTHREADS to " << nThreads << "; thus, GeoModelIO will be run in serial mode." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = false;
    } else if( nThreads > 0 ) {
      std::cout << "\nYou set the GEOMODEL_ENV_IO_NTHREADS to " << nThreads << "; thus, GeoModelIO will use that number of worker threads." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = true;
    } else if (nThreads == -1) {
      std::cout << "\nYou set the GEOMODEL_ENV_IO_NTHREADS to " << nThreads << "; thus, GeoModelIO will use the number of threads supported by the platform." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = true;
    }
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

  if (m_timing || m_debug || m_deepDebug) {
    muxCout.lock();
  	std::cout << "Time Taken to process " << nChildrenRecords << " children = " << diff << " Seconds" << std::endl;
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
	// ...otherwise, let's spawn some threads to process them in bunches, parallelly!
	else {

    std::chrono::system_clock::time_point start, end;
    if (m_timing || m_debug || m_deepDebug) {
  		// Get Start Time
  		start = std::chrono::system_clock::now();
    }

    // set number of worker threads
    unsigned int nThreads = 0;
    if(m_runMultithreaded_nThreads > 0) {
      nThreads = m_runMultithreaded_nThreads;
    }
    else if (m_runMultithreaded_nThreads == -1) {
      unsigned int nThreadsPlatform = std::thread::hardware_concurrency();
      nThreads = nThreadsPlatform;
		  if (m_debug || m_deepDebug) std::cout << "INFO - You have asked for hardware native parellelism. On this platform, " << nThreadsPlatform << " concurrent threads are supported. Thus, using " << nThreads << " threads.\n";
    }

		unsigned int nBunches = nChildrenRecords / nThreads;
		if (m_debug || m_deepDebug) std::cout << "Processing " << nThreads << " bunches, with " << nBunches << " children each, plus the remainder." << std::endl;

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

			futures.push_back( std::async(std::launch::async, &ReadGeoModel::loopOverAllChildren, this, bunch) );
		}

		// wait for all async calls to complete
		//retrieve and print the value stored in the 'std::future'
		muxCout.lock();
		if (m_debug || m_deepDebug) std::cout << "Waiting for the threads to finish...\n" << std::flush;
		muxCout.unlock();
	  	for(auto &e : futures) {
	    	e.wait();
	   	}
  	if (m_debug || m_deepDebug) std::cout << "Done!\n";

    // createBooleanShapeOperands(); // TODO: move to threads somehow, but the shared container needs to be handled... maybe a dispatcher thread that gives items to the worker threads to process? A FIFO should work here...

    if (m_timing || m_debug || m_deepDebug) {
  		// Get End Time
  		end = std::chrono::system_clock::now();
  		auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
  		std::cout << "(Total time taken to recreate all " << nChildrenRecords << " mother-children relationships: " << diff << " seconds)" << std::endl;
    }
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
		std::cout <<  "ERROR!!! Probably you are using an old geometry file. Exiting..." << std::endl;
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

	// if previously built, return that // FIXME: TODO: apparently this is only used for VPhysVol... check that, because it should be used for all nodes, otherwise we create new nodes in memory even if we have a copy of them already...
	if (isNodeBuilt(id, tableId, copyN)) {
		if (m_deepDebug) {
      muxCout.lock();
      qDebug() << "getting the volume from memory...";
      muxCout.unlock();
    }
		return dynamic_cast<GeoVPhysVol*>(getVPhysVol(id, tableId, copyN));
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
	storeVPhysVol(id, tableId, copyN, vol);

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


std::string ReadGeoModel::getShapeType(const unsigned int shapeId)
{
    if (shapeId > m_shapes.size()) {
      std::cout << "ERROR!! Shape ID is larger than the container size. Exiting..." << std::endl;
      exit(EXIT_FAILURE);
    }
  	QStringList paramsShape = m_shapes[ shapeId ];
    // std::cout << "shapeID: " <<  shapeId << ", shapeType:" << paramsShape[1].toStdString() << std::endl; // debug
  	QString qtype = paramsShape[1];
    std::string type = qtype.toStdString();
    return type;
}


/// Recursive function, to build GeoShape nodes
GeoShape* ReadGeoModel::buildShape(const unsigned int shapeId, type_shapes_boolean_info* shapes_info_sub)
{
	if (m_deepDebug) {
     muxCout.lock();
     std::cout << "ReadGeoModel::buildShape()" << std::endl;
     muxCout.unlock();
  }

//   try // TODO: implement try/catch
//   {
	// QStringList paramsShape = m_shapes[ shapeId.toUInt() ];
	QStringList paramsShape = m_shapes[ shapeId ];

	QString id = paramsShape[0];
	QString qtype = paramsShape[1];
	QString parameters = paramsShape[2];

  std::string type = qtype.toStdString();



	if (m_deepDebug) {
     muxCout.lock();
     std::cout << "\tShape-ID:" << id.toStdString() << ", Shape-type:" << qtype.toStdString();
     muxCout.unlock();
  }

  GeoShape* shape = nullptr;

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
		shape = new GeoBox(XHalfLength, YHalfLength, ZHalfLength);
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
		shape = new GeoCons (RMin1, RMin2, RMax1, RMax2, DZ, SPhi, DPhi);
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
		shape = new GeoTorus (Rmin, Rmax, Rtor, SPhi, DPhi);
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
		shape = new GeoPara (XHalfLength, YHalfLength, ZHalfLength, Alpha, Theta, Phi);
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

		shape = pcon;
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
		shape = pgon;
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
		shape = gTrap;
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
		shape = sh;

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

					// std::vector<GeoFacetVertex*> vV(3, 0); // to store the 3 vertices of the GeoTriangularFacet
					auto vV = std::vector<std::unique_ptr<GeoFacetVertex>>{};

					// we look for 3 vertices for GeoTriangularFacet;
					// for each of them, we get 3 coordinates
					// vertStart = it;
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
		shape = sh;

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
		shape = new GeoTrap (ZHalfLength, Theta, Phi, Dydzn, Dxdyndzn, Dxdypdzn, Angleydzn, Dydzp, Dxdyndzp, Dxdypdzp, Angleydzp);
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
		shape = new GeoTrd (XHalfLength1, XHalfLength2, YHalfLength1, YHalfLength2, ZHalfLength);
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
		shape = new GeoTube(RMin, RMax, ZHalfLength);
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
		shape = new GeoTubs (RMin, RMax, ZHalfLength, SPhi, DPhi);
	}
	else if (type == "Shift") {
		// shape parameters
		unsigned int shapeOpId = 0;
		unsigned int transfId = 0;
		// get parameters from DB string
		QStringList shapePars = parameters.split(";");
		foreach( QString par, shapePars) {
			QStringList vars = par.split("=");
			QString varName = vars[0];
			QString varValue = vars[1];
			if (varName == "A") shapeOpId = varValue.toUInt();
			if (varName == "X") transfId = varValue.toUInt();
		}
    if (shapeOpId == 0 || transfId == 0) {
      std::cout << "ERROR! Shift shape - input operand shapes' IDs are empty! (shapeId: " << shapeOpId << ", transfId:" << transfId << ")" << std::endl;
      exit(EXIT_FAILURE);
    }

    // if both operands are built already,
    // then get them from cache,
    // and build the operator shape with them,
    if ( isBuiltShape(shapeOpId) && isBuiltTransform(transfId) ) {
        const GeoShape* shapeOp = getBuiltShape(shapeOpId);
        const GeoTransform* transf = getBuiltTransform(transfId);
        // TODO: here we create a fake GeoTransform to get a GeoTrf::Transform3D.
        // TODO: ==> Perhaps we could keep a table for bare GeoTrf::Transform3D transforms used in GeoShift nodes.
        GeoTrf::Transform3D transfX = transf->getTransform();
        transf->unref(); // delete the transf from the heap, because we don't need the node, only the bare transformation matrix
        GeoShapeShift* shapeNew = new GeoShapeShift(shapeOp, transfX);
        storeBuiltShape(shapeId, shapeNew);
        shape = shapeNew;
    }
    // otherwise, build the operands
    else {

      // TODO: IMPORTANT!!! --> check how the transf used in shift are saved into the DB, because they are bare transf and not GeoTransform nodes... I fear thay are wrongly stored...

      // first, check if the transform is built;
      // if so, use that;
      // if not, build the transform

      // get the referenced Transform, then get the bare transform matrix from it
      GeoTransform* transf = nullptr;
      GeoTrf::Transform3D transfX;
      if (isBuiltTransform(transfId)) {
        transf = getBuiltTransform(transfId);
      } else {
        transf = buildTransform(transfId);
      }
      // TODO: here we create a fake GeoTransform to get a GeoTrf::Transform3D.
      // TODO: ==> Perhaps we could keep a table for bare GeoTrf::Transform3D transforms used in GeoShift nodes.
      transfX = transf->getTransform();
      transf->unref(); // delete the transf from the heap, because we don't need the node, only the bare transformation matrix



      // then, check the type of the operand shape
      bool isAOperator = isShapeOperator( shapeOpId );

      // if operand shape is simple/actual shape (i.e., not boolean/operator),
      // then build it,
      // then build the boolean shape with that
      if ( !isAOperator ) {
        const GeoShape* shapeOp = buildShape( shapeOpId, shapes_info_sub );

        if ( shapeOp == nullptr || transf == nullptr ) {
          std::cout << "ERROR!!! Shift - shapeOp or transfX are NULL! Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
        GeoShapeShift* shapeNew = new GeoShapeShift(shapeOp, transfX);
        storeBuiltShape(shapeId, shapeNew);
        shape = shapeNew;
      }
      // ...otherwise, build the Shift operator shape without operands
      // and save the needed pieces of information to build the actual
      // operands and shape later.
      else {
        GeoShapeShift* shapeNew = new GeoShapeShift();

        // debug
        // muxCout.lock();
        // std::cout << "adding 'empty' shape (1): " << shapeId << ", " << shapeNew << ", " << shapeOpId << ", " << transfId << std::endl;
        // muxCout.unlock();

        tuple_shapes_boolean_info tt (shapeId, shapeNew, shapeOpId, transfId);
        shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over

        shape = shapeNew;
      }
    }
	}
	else if (type == "Subtraction" || type == "Union" || type == "Intersection") {

    // Check what shapes are subtracted/united/intersected:
    // - If they are actual shapes, build them and return
    // - If they are boolean/operator shapes, then store the shape for later and continue

		// shape's operands
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
      std::cout << "ERROR! Subtraction/Union/Intersection shape - input operand shapes' IDs are empty! (opA: " << opA << ", opB:" << opB << ")" << std::endl;
      exit(EXIT_FAILURE);
    }

    // if both operands are built already,
    // then get them from cache,
    // and build the operator with them
    if ( isBuiltShape(opA) && isBuiltShape(opB) ) {
        // std::cout << "both operand shapes are built, build the operator shape..." << std::endl;
        GeoShape* shapeNew = nullptr;
        const GeoShape* shapeA = getBuiltShape(opA);
        const GeoShape* shapeB = getBuiltShape(opB);
        if ("Subtraction" == type) {
          shapeNew = new GeoShapeSubtraction(shapeA, shapeB);
        }
        if ("Union" == type) {
          shapeNew = new GeoShapeUnion(shapeA, shapeB);
        }
        if ("Intersection" == type) {
          shapeNew = new GeoShapeIntersection(shapeA, shapeB);
        }
        storeBuiltShape(shapeId, shapeNew);
        shape = shapeNew;
    }
    // otherwise, build the operand shapes
    else {

      // first check the operands' type
      bool isAOperator = isShapeOperator( opA );
      bool isBOperator = isShapeOperator( opB );

      // if both are simple/actual shapes (i.e., not booleans),
      // then build them, then build the boolean shape with them, and returns...
      if ( !isAOperator && !isBOperator) {
        const GeoShape* shapeA = buildShape( opA, shapes_info_sub );
        const GeoShape* shapeB = buildShape( opB, shapes_info_sub );
        if ( shapeA == NULL || shapeB == NULL ) {
          std::cout << "ERROR!!! shapeA or shapeB are NULL!" << std::endl;
          exit(EXIT_FAILURE);
        }
        GeoShapeSubtraction* shapeNew = new GeoShapeSubtraction(shapeA, shapeB);
        storeBuiltShape(shapeId, shapeNew);
        shape = shapeNew;
      }
      // ...otherwise, build the Subtraction operator shape without operands
      // and save the needed pieces of information to build the actual
      // operands and shape later.
      else {
        GeoShape* shapeNew = nullptr;
        if ( "Subtraction" == type ) {
          shapeNew = new GeoShapeSubtraction;
        } else if ( "Union" == type ) {
          shapeNew = new GeoShapeUnion;
        } else if ( "Intersection" == type ) {
          shapeNew = new GeoShapeIntersection;
        }

        // debug
        // muxCout.lock();
        // std::cout << "adding 'empty' shape (2): " << shapeId << ", " << shapeNew << ", " << opA << ", " << opB << std::endl;
        // muxCout.unlock();

        tuple_shapes_boolean_info tt (shapeId, shapeNew, opA, opB);
        shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over

        shape = shapeNew;
      }
    }
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
      shape = new GeoUnidentifiedShape("LArCustomShape",name);
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
    shape = new GeoUnidentifiedShape(name,asciiData);

  }
  else {
    // QString msg = "WARNING!! - Shape '" + type + "' not implemented yet!!! Returning a dummy cube.";
    // qWarning(msg.toStdString().c_str());
    // m_unknown_shapes.insert(type.toStdString()); // save unknwon shapes for later warning message
    m_unknown_shapes.insert(type); // save unknwon shapes for later warning message
    shape = buildDummyShape();
  }

  //! store the shape we built into the cache,
  //! for later use if referenced again by another node
  storeBuiltShape(shapeId, shape);




  return shape;

// }

// catch (std::invalid_argument& e)
// {
//     std::cerr << e.what() << std::endl;
//     exit(EXIT_FAILURE);
// }


}



// TODO: move to an untilities file/class
void printTuple(tuple_shapes_boolean_info tuple)
{
  std::apply([](auto&&... args) { ( (std::cout << args << ", "), ...); }, tuple); // needs C++17
  std::cout << std::endl;
}
// TODO: move to an untilities file/class
void inspectListShapesToBuild(type_shapes_boolean_info list)
{
  for (auto tuple : list) {
    printTuple(tuple);
    std::cout << std::endl;
  }
}


void ReadGeoModel::createBooleanShapeOperands(type_shapes_boolean_info* shapes_info_sub)
{
  if (shapes_info_sub->size() == 0) return;

  // debug
  // std::cout << "\ncreateBooleanShapeOperands() - start..." << std::endl;
  // inspectListShapesToBuild(shapes_info_sub);

	// Iterate over the list. The size may be incremented while iterating (therefore, we cannot use iterators)
  for (type_shapes_boolean_info::size_type ii = 0; ii < shapes_info_sub->size(); ++ii)
  {
    // get the tuple containing the data about the operand shapes to build
    tuple_shapes_boolean_info tuple = (*shapes_info_sub)[ii];
    // std::cout << "tuple: "; printTuple(tuple); // debug

      // Initializing variables for unpacking
      unsigned int shapeID = 0;       //std::get<0>(tuple);
      GeoShape* boolShPtr  = nullptr; //std::get<1>(tuple);
      unsigned int idA     = 0;       //std::get<2>(tuple);
      unsigned int idB     = 0;       //std::get<3>(tuple);

      // use 'tie' to unpack the tuple values into separate variables
      std::tie(shapeID, boolShPtr, idA, idB) = tuple;

      // debug
      // muxCout.lock();
      // std::cout << "operands' map's item: " << shapeID << ", " << boolShPtr << "," << idA << "," << idB << std::endl; // debug only!
      // muxCout.unlock();

      if (shapeID == 0 || boolShPtr == nullptr || idA == 0 || idB == 0) {
        muxCout.lock();
        std::cout << "ERROR! Boolean/Operator shape - shape is NULL or operands' IDs are not defined! (shapeID: " << shapeID << ", idA: " << idA << ", idB:" << idB << ")" << std::endl;
        muxCout.unlock();
        exit(EXIT_FAILURE);
      }

      if (isShapeBoolean(shapeID)) {

        GeoShape* shapeA = nullptr;
        GeoShape* shapeB = nullptr;

        // if both operands are built already...
        if ( isBuiltShape(idA) && isBuiltShape(idB) ) {
          // then build the operator shape...
          shapeA = getBuiltShape(idA);
          shapeB = getBuiltShape(idB); //TODO: customize for Shift as well
        } else {
          // otherwise, build the operand shapes
          shapeA = getBooleanReferencedShape(idA, shapes_info_sub);
          shapeB = getBooleanReferencedShape(idB, shapes_info_sub);
        }
        // Now, assign the new shapes to the boolean shape we're building
        if (dynamic_cast<GeoShapeIntersection*>(boolShPtr)) {
          GeoShapeIntersection* ptr = dynamic_cast<GeoShapeIntersection*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else if (dynamic_cast<GeoShapeSubtraction*>(boolShPtr)) {
          GeoShapeSubtraction* ptr = dynamic_cast<GeoShapeSubtraction*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else if (dynamic_cast<GeoShapeUnion*>(boolShPtr)) {
          GeoShapeUnion* ptr = dynamic_cast<GeoShapeUnion*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else{
          // TODO: move to standard error message for all instances
          std::cout << "ERROR!!! shape is not boolean/operator! Write to 'geomodel-developers@cern.ch'. Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
      } else if ("Shift" == getShapeType(shapeID)) {

        GeoShape* opShape = nullptr;
        GeoTrf::Transform3D shiftX;
        GeoTransform* shiftTransf = nullptr; // TODO: remove the need for a temp GeoTransform, store the bare transforms as well...

        // if both operands are built already...
        if ( isBuiltShape(idA) && isBuiltTransform(idB) ) {
          // then build the operator shape...
          opShape = getBuiltShape(idA);
          shiftTransf = getBuiltTransform(idB);
        } else {
          // otherwise, build the operand shapes
          opShape = getBooleanReferencedShape(idA, shapes_info_sub);
          shiftTransf = buildTransform(idB);
        }
        shiftX = shiftTransf->getTransform();
        shiftTransf->unref(); // delete from heap, we only needed to get the bare transform // TODO: remove that need, store the bare transforms as well...

        if (dynamic_cast<GeoShapeShift*>(boolShPtr)) {
        GeoShapeShift* ptr = dynamic_cast<GeoShapeShift*>(boolShPtr);
        ptr->m_op = opShape;
        ptr->m_shift = shiftX;
        ptr->m_op->ref();
        }
        else {
          std::cout << "ERROR!!! shape is not a Shift operator! Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
    }
    // then, store the now completed shape and continue to the next item
    storeBuiltShape(shapeID, boolShPtr);
	}
}



GeoShape* ReadGeoModel::getBooleanReferencedShape(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub)
{
  if (0 == shapeID) {
    std::cout << "ERROR!! ShapeID = 0!" << std::endl;
    exit(EXIT_FAILURE);
  }

  GeoShape* shape;
  // if A is built already, then take it from cache
  if (isBuiltShape(shapeID)) {
    if (m_deepDebug) std::cout << "operandA is built, taking it from cache..." << std::endl; // debug
    shape = getBuiltShape(shapeID);
  } else {
    // if not built and not a boolean shape, then build it
    if (!isShapeOperator(shapeID)) {
      if (m_deepDebug) std::cout << "operandA is not built and not an operator, build it..." << std::endl; // debug
      shape = buildShape( shapeID, shapes_info_sub );
      if ( shape == NULL ) {
        std::cout << "ERROR!!! shape is NULL!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    // if A is a boolean shape, then create an empty shape,
    // store it for later completion, and use that
    else {
      if (m_deepDebug) std::cout << "operandA is not built and it is an operator, add it to build it later..." << std::endl; // debug
      shape = addEmptyBooleanShapeForCompletion(shapeID, shapes_info_sub);
      }
  }

//   inspectListShapesToBuild(m_shapes_info_sub); // debug
  return shape;
}



GeoShape* ReadGeoModel::addEmptyBooleanShapeForCompletion(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub)
{
  // get the operands' IDs,
  // build an empty instance of the appropriate boolean/operator shape,
  // and store all of that together, by appending to this same container,
  // so it will be visited at a later time during this very same loop
  std::pair<unsigned int, unsigned int> ops = getBooleanShapeOperands(shapeID);
  unsigned int opA = ops.first;
  unsigned int opB = ops.second;

  // get the type and create an appropriate empty shape
  std::string shType = getShapeType(shapeID);
  GeoShape* shape = nullptr;
  if (shType == "Intersection"){
    shape = new GeoShapeIntersection();
  } else if (shType == "Shift") {
    shape = new GeoShapeShift();
  } else if (shType == "Subtraction") {
    shape = new GeoShapeSubtraction();
  } else if (shType == "Union") {
    shape = new GeoShapeUnion();
  }

  // debug
  // muxCout.lock();
  // std::cout << "adding 'empty' shape (3): " << shapeID << ", " << shape << ", " << opA << ", " << opB << std::endl;
  // muxCout.unlock();

  tuple_shapes_boolean_info tt (shapeID, shape, opA, opB);
  shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over

  return shape;
}


// TODO: move this to utility class/file
std::vector<std::string> ReadGeoModel::splitString(const std::string& s, const char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

// TODO: move this to utility class/file
std::vector<std::string> ReadGeoModel::toStdVectorStrings(QStringList qlist)
{
  std::vector<std::string> vec;
  foreach(QString qstr, qlist) {
    vec.push_back(qstr.toStdString());
  }
  return vec;
}

std::pair<unsigned int, unsigned int> ReadGeoModel::getBooleanShapeOperands(const unsigned int shapeID)
{
  std::pair<unsigned int, unsigned int> pair;

  std::vector<std::string> paramsShape = toStdVectorStrings(m_shapes[ shapeID ]);

	unsigned int id = std::stoi(paramsShape[0]); //! the ID of the boolean/operator shape
	std::string type = paramsShape[1]; //! the GeoModel type of the shape
	std::string parameters = paramsShape[2];  //! the parameters defining the shape, coming from the DB

  //! The Subtraction boolean shape has two operands, here we store their IDs
  unsigned int opA = 0;
  unsigned int opB = 0;
  // get parameters from DB string
  std::vector<std::string> shapePars = splitString( parameters, ';' );
  // std::cout << "shapePars size: " << shapePars.size() << std::endl; // debug only

  for (auto par : shapePars) {
    std::vector<std::string> vars = splitString(par, '=');
    std::string varName = vars[0];
    std::string varValue = vars[1];
    // std::cout << "vars ==> " << vars[0] << ", " << vars[1] << std::endl; // debug
    if ( isShapeBoolean(type) ) {
      if (varName == "opA") opA = std::stoi(varValue);
      if (varName == "opB") opB = std::stoi(varValue);
    } else if ("Shift" == type) {
      if (varName == "A") opA = std::stoi(varValue);
      if (varName == "X") opB = std::stoi(varValue);
    } else {
      std::cout << "ERROR!!! This type of shape cannot be handled here: " << type << ", --> Exiting..." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  if (opA == 0 || opB == 0) {
    std::cout << "ERROR! Intersection/Subtraction/Union/Shift shape - operands' IDs are empty! (opA: " << opA << ", opB:" << opB << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
  pair.first = opA;
  pair.second = opB;

  return pair;
}


bool ReadGeoModel::isShapeOperator(const unsigned int shapeId)
{
  return isShapeOperator( getShapeType(shapeId) );
}
bool ReadGeoModel::isShapeOperator(const std::string type)
{
  std::unordered_set<std::string> opShapes = {"Intersection", "Shift", "Subtraction", "Union"};
  return (opShapes.find(type) != opShapes.end());
}

bool ReadGeoModel::isShapeBoolean(const unsigned int shapeId)
{
  return isShapeBoolean( getShapeType(shapeId) );
}
bool ReadGeoModel::isShapeBoolean(const std::string type)
{
  std::unordered_set<std::string> opShapes = {"Intersection", "Subtraction", "Union"};
  return (opShapes.find(type) != opShapes.end());
}




GeoBox* ReadGeoModel::buildDummyShape() {
  return new GeoBox(30.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm); // FIXME: bogus shape. Use actual shape!
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

	// get the parameters to build the GeoLogVol node
	QString logVolName = values[1];

	// build the referenced GeoShape node
	unsigned int shapeId = values[2].toUInt();
  type_shapes_boolean_info shapes_info_sub; // tuple to store the boolean shapes to complete at a second stage
	GeoShape* shape = buildShape(shapeId, &shapes_info_sub);
  // now, create the missing operand shapes for boolean/operator shapes
  createBooleanShapeOperands(&shapes_info_sub);

	// build the referenced GeoMaterial node
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



// TODO: to be removed when dropping Qt
GeoTransform* ReadGeoModel::buildTransform(QString id)
{
	if (m_deepDebug) {
    muxCout.lock();
    if(m_deepDebug) qDebug() << "ReadGeoModel::buildTransform(QString)";
    muxCout.unlock();
  }
	return buildTransform( id.toUInt() );
}


// GeoTransform* ReadGeoModel::parseTransform(QStringList values)
GeoTransform* ReadGeoModel::buildTransform(unsigned int id)
{
  QStringList values = m_transforms[id];

  muxCout.lock();
  if (m_deepDebug) qDebug() << "ReadGeoModel::buildTransform(unsigned int)";
	if (m_deepDebug) qDebug() << "values:" << values;
  muxCout.unlock();

	values.takeFirst(); // it pops out the first element, the 'id', leaving the other items in the list
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
	GeoTransform* tr = new GeoTransform(txf);
  storeBuiltTransform(id, tr);
  return tr;
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


// --- methods for caching GeoShape nodes ---
bool ReadGeoModel::isBuiltShape(const unsigned int id)
{
  std::lock_guard<std::mutex> lk(muxShape);
  // std::cout << "ReadGeoModel::isBuiltShape(): " << id << std::endl;
  return m_memMapShapes.count(id);
}
void ReadGeoModel::storeBuiltShape(const unsigned int id, GeoShape* nodePtr)
{
  std::lock_guard<std::mutex> lk(muxShape);
  // std::cout << "ReadGeoModel::storeBuiltShape(): " << id << ", " << nodePtr << std::endl;
  m_memMapShapes[id] = nodePtr;
}
GeoShape* ReadGeoModel::getBuiltShape(const unsigned int id)
{
	std::lock_guard<std::mutex> lk(muxShape); // TODO: is this lock needed at all?? I guess STD containers are thread safe for read-only operations
	// std::cout << "ReadGeoModel::getBuiltShape(): " << id << std::endl;
	return m_memMapShapes[id];
}
// --- methods for caching GeoTransform nodes ---
bool ReadGeoModel::isBuiltTransform(const unsigned int id)
{
  std::lock_guard<std::mutex> lk(muxTransf);
  // std::cout << "ReadGeoModel::isBuiltTransform(): " << id << std::endl;
  return m_memMapTransforms.count(id);
}
void ReadGeoModel::storeBuiltTransform(const unsigned int id, GeoTransform* nodePtr)
{
  std::lock_guard<std::mutex> lk(muxTransf);
  // std::cout << "ReadGeoModel::storeBuiltTransform(): " << id << ", " << nodePtr << std::endl;
  m_memMapTransforms[id] = nodePtr;
}
GeoTransform* ReadGeoModel::getBuiltTransform(const unsigned int id)
{
	std::lock_guard<std::mutex> lk(muxTransf); // TODO: is this lock needed at all?? I guess STD containers are thread safe for read-only operations
	// std::cout << "ReadGeoModel::getBuiltTransform(): " << id << std::endl;
	return m_memMapTransforms[id];
}
// --- methods for caching GeoPhysVol/GeoFullPhysVol nodes ---
bool ReadGeoModel::isNodeBuilt(const QString id, const QString tableId, const QString copyNumber)
{
	std::lock_guard<std::mutex> lk(muxVPhysVol);
	// qDebug() << "ReadGeoModel::isNodeBuilt(): " << id << tableId << copyNumber;
	QString key = id + ":" + tableId + ":" + copyNumber;
	return m_memMap.contains(key);
}
void ReadGeoModel::storeVPhysVol(const QString id, const QString tableId, const QString copyN, GeoGraphNode* node)
{
  std::lock_guard<std::mutex> lk(muxVPhysVol);
  // if (m_deepDebug) qDebug() << "ReadGeoModel::storeVPhysVol(): " << id << ", " << tableId << ", " << copyN << node;
  QString key = id + ":" + tableId + ":" + copyN;
  m_memMap[key] = node;
}
GeoGraphNode* ReadGeoModel::getVPhysVol(const QString id, const QString tableId, const QString copyN)
{
	std::lock_guard<std::mutex> lk(muxVPhysVol);
	// if (m_deepDebug) qDebug() << "ReadGeoModel::getVPhysVol(): " << id << ", " << tableId << ", " << copyN;
	QString key = id + ":" + tableId + ":" + copyN;
	return m_memMap[key];
}


} /* namespace GeoModelIO */

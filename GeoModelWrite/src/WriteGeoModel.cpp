// author: Riccardo.Maria.Bianchi@cern.ch, 2017
// major updates: Aug 2018, Feb 2019

// local includes
#include "GeoModelWrite/WriteGeoModel.h"

// TFPersistification includes
#include "TFPersistification/TransFunctionPersistifier.h"

// GeoSpecialShapes
// #include "GeoSpecialShapes/LArCustomShape.h"

// GeoModelKernel includes
#include "GeoModelKernel/GeoNodePath.h"
#include "GeoModelKernel/GeoCountVolAction.h"
// GeoModelKernel shapes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeUnion.h"

#include "GeoModelKernel/GeoUnidentifiedShape.h"

// Qt includes
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QVector>
#include <QDebug>

// C++ includes
#include <sstream>


namespace GeoModelIO {


/// Get next child position available, given the parent type, id and copy number
unsigned int WriteGeoModel::getChildPosition(const QString parentId, const QString parentType, const unsigned int copyN)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getChildPosition()";
	QString tableId = getIdFromNodeType(parentType);
	QString key = tableId + ":" + parentId + ":" + QString::number(copyN);

	if ( ! (m_parentChildrenMap2.contains(key)) ) {
		m_parentChildrenMap2[key] = 1;
	} else {
	    ++m_parentChildrenMap2[key];
    }

    //JFB Commented out: qDebug() << "parent key:" << key << " [tableId:parentId:copyN] - pos: " <<  m_parentChildrenMap2[key];
	return m_parentChildrenMap2[key];
}

unsigned int WriteGeoModel::setVolumeCopyNumber(QString volId, QString volType)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::setVolumeCopyNumber()";
	QString tableId = getIdFromNodeType(volType);
	QString key = tableId + ":" + volId;

	if ( ! (m_volumeCopiesMap.contains(key)) ) {
		m_volumeCopiesMap[key] = 1;
	} else {
    	++m_volumeCopiesMap[key];
    }

    //JFB Commented out: qDebug() << "volume key:" << key << " [tableId:volumeId] - copy number: " <<  m_volumeCopiesMap[key];
	return m_volumeCopiesMap[key];
}


unsigned int WriteGeoModel::getLatestParentCopyNumber(QString parentId, QString parentType)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getLatestParentCopyNumber()";
	QString tableId = getIdFromNodeType(parentType);
	QString key = tableId + ":" + parentId;
    //JFB Commented out: qDebug() << "key:" << key;

	if ( ! (m_volumeCopiesMap.contains(key)) ) {
		qFatal("ERROR!!! Something's wrong in storing the number of copies!");
	}

    //JFB Commented out: qDebug() << "get latest parent copy number:" << key << " [tableId:parentId] - copy number: " <<  m_volumeCopiesMap[key];
	return m_volumeCopiesMap[key];
}


void WriteGeoModel::handlePhysVol (const GeoPhysVol *vol)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handlePhysVol(GeoPhysVol*)";
	handleVPhysVolObjects( vol );
}


void WriteGeoModel::handleFullPhysVol (const GeoFullPhysVol *vol)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleFullPhysVol( GeoFullPhysVol* )";
	handleVPhysVolObjects( vol );
}


void WriteGeoModel::handleVPhysVolObjects(const GeoVPhysVol* vol)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::handleVPhysVolObjects( GeoVPhysVol* )";


	// get the address string for the current volume
	QString address = getAddressStringFromPointer( vol );

	// variables used to persistify the object
	QVariant physId;

	// check the volume position in the geometry tree
	GeoNodePath* path = getPath();
	unsigned int len = path->getLength();
//    const GeoVPhysVol* tailVol = path->getTail();
//    const GeoVPhysVol* headVol = path->getHead();
    const GeoVPhysVol* upperVol = nullptr;
    if (len > 1)
        upperVol = path->getItem(len-2); // item(len-1) is THIS volume ==> the length is updated when visiting a VPhysVol node
    else
        upperVol = path->getItem(len-1); // ROOT volume
	//JFB Commented out: qDebug() << "path: " << path << " - length: " << len << " - upper vol: " << upperVol << " - tail vol: " << tailVol << " - head vol: " << headVol;
	//	std::cout << "path: " << path << std::endl;

	// this below is performed until the root volume is found, then "length" is not used anymore,
	// because not updated when recursively visiting children of PhysVols: it is updated only when a PhysVol node is visited

	bool doGetParentNode = true;
	bool storeRootVolume = false;
	bool isRootVolume = false;


	if ( ! m_rootVolumeFound) {
		if (len > 1) {
			doGetParentNode = true; // TODO: is that needed????
		} else{
			//JFB Commented out: qDebug() << "This is the Root volume!";
            isRootVolume = true;
			m_rootVolumeFound = true;
			storeRootVolume = true;
			doGetParentNode = false;
		}
	} else {
		doGetParentNode = true;
	}

	// When dumping an unconnected sub-tree, e.g. a PhysVol used only in a SerialTransformer,
	// we don't want to get the parent ID.
	// If we do, the ID of the parent of the SerialTransformer is returned, which is wrong.
	if (m_unconnectedTree) {
		doGetParentNode = false;
		//JFB Commented out: qDebug() << "Handling an unconnected tree: we skip the parent-finding step...";
		// now, we reset the status,
		// otherwise all the children of the first referenced, unconnected volume
		// will be treated as unconnected as well
		//JFB Commented out: qDebug() << "setting 'unconnected' to: false";
		m_unconnectedTree = false;
	}


	// // get the parent volume
	// QStringList parentList = getParentNode(); // you can use that, but you have to modify it to get len-2, instead of len-1, for VPhysVol nodes
	// QString parentId = parentList[0];
	// QString parentType = parentList[1];

	// get the parent volume, if this is not the Root volume or an unconnected sub-tree
	const GeoVPhysVol* parentNode = nullptr;
	QVariant parentId = "NULL";

    QString volTypeStr = "NNN";

	if (doGetParentNode) {

//        bool isShared = vol->isShared();
        //JFB Commented out: qDebug() << "is this node shared?" << isShared;

        //if (isShared)
            parentNode = upperVol;
        //else
		//    parentNode = dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() ))); // this cannot be trust for shared nodes
        //JFB Commented out: qDebug() << "parentNode address" << parentNode;

		if (parentNode) {
			QString parentAddress = getAddressStringFromPointer(parentNode);
			//JFB Commented out: qDebug() << "==> parent's address:" << parentNode;

			if (isAddressStored(parentAddress))
				parentId = getStoredIdFromAddress(parentAddress);
			//		else
			//			qFatal("FATAL ERROR!!! - The parent node of this child should has been stored in the DB already, but it was not found!!");

			//JFB Commented out: qDebug() << "==> parent's LogVol name:" << QString::fromStdString(parentNode->getLogVol()->getName());
		}
		else {
			//JFB Commented out: qDebug() << "NULL parent node!! --> this node appeared unconnected.";
		}
	}

	// counting children
//  unsigned int nChildren = vol->getNChildVols();
	//JFB Commented out: qDebug() << "number of child physical volumes:" << nChildren;
	//JFB Commented out: qDebug() << "[number of PhysVol and SerialTransformer child nodes:" << vol->getNChildVolAndST() << "]";

	//// for debug
	// GeoCountVolAction cv;
	// cv.setDepthLimit(1);
	// vol->exec(&cv);
	// int nChildCount = cv.getCount();
	// //JFB Commented out: qDebug() << "number of child volumes:" << nChildCount;



	// check if this object has been stored already
	if (! isAddressStored(address)) {

		/*
		 * VPhysVol features:
		 * - 1 parent VPhysVol (if any)
		 * - 1 LogVol
		 * - 1 Material
		 * - 1 Shape
		 */

		// Note: PhysVol has no name. Its LogVol has a name.
		//	  const std::string name = vol->getLogVol()->getName();

		// LOGVOL
		const GeoLogVol* logVol = vol->getLogVol();
		const QString logName = QString::fromStdString(logVol->getName());
		//JFB Commented out: qDebug() << "LogVol name:"  << logName;


		// MATERIAL
		const GeoMaterial * mat = vol->getLogVol()->getMaterial();
		// const QString matName = QString::fromStdString(mat->getName());
		// qDebug() << "material name:" << matName << ", address:" << mat;


		// SHAPE
		const GeoShape * shape = vol->getLogVol()->getShape();
		// const QString shapeType = QString::fromStdString(shape->type());
		// qDebug() << "shape name:" << shapeType  << ", address:" << shape;
		// // get shape parameters
		// QString shapePars = getShapeParameters(shape);


		/*
		 * STORE THE OBJECTS IN THE DB
		 */

		// store/get the Material object into/from the DB
		QVariant matId;
		// matId = storeObj(mat, matName);
		matId = storeMaterial(mat);

		// store/get the Shape object into/from the DB
		QVariant shapeId;
		shapeId = storeShape(shape);
		// shapeId = storeObj(shape, shapeType, shapePars);

		// store/get the LogVol object into/from the DB
		QVariant logvolId;
		logvolId = storeObj(logVol, logName, shapeId, matId);

		if (dynamic_cast<const GeoPhysVol*>(vol)) {
			//JFB Commented out: qDebug() << "New PhysVol, storing it...";
			const GeoPhysVol* physVol = dynamic_cast<const GeoPhysVol*>(vol);
			// store the PhysVol volume into the DB
			physId = storeObj(physVol, logvolId, parentId, storeRootVolume); // with parent info
            volTypeStr = "GeoPhysVol";
			//JFB Commented out: qDebug() << "PhysVol stored. Id:" << physId.toString();
		}
		else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
			//JFB Commented out: qDebug() << "New FullPhysVol, storing it...";
			const GeoFullPhysVol* fullVol = dynamic_cast<const GeoFullPhysVol*>(vol);
			// store the FullPhysVol volume into the DB
			physId = storeObj(fullVol, logvolId, parentId, storeRootVolume); // with parent info
            volTypeStr = "GeoFullPhysVol";
			//JFB Commented out: qDebug() << "FullPhysVol stored. Id:" << physId.toString();
		} else {
			qWarning() << "WARNING!! Unknown GeoVPhysVol type!!";
		}

	} else {
		//	qDebug() << "Volume stored already. It is a shared volume. Taking ID from memory map and moving to its physical children...";
		//JFB Commented out: qDebug() << "Volume stored already. It is a shared volume. Now, we are handling a 'copy' of it. We take the ID from memory map...";
		physId = getStoredIdFromAddress(address);
        volTypeStr = getGeoTypeFromVPhysVol(vol);
	}

    // Now we get the 'copy number' for this volume,
    // to distinguish this volume from the other volumes created from the same shared node (if any)
    if (volTypeStr == "NULL") qFatal("ERROR!! volTypeStr is 'NULL'!!!");
    const unsigned int volCopyN = setVolumeCopyNumber(physId.toString(), volTypeStr);
    //JFB Commented out: qDebug() << "physId: " << physId << "- volume copy number: " << volCopyN;

    if ( isRootVolume || parentId == "NULL") {
        //JFB Commented out: qDebug() << "This is the RootVolume or the volume has 'NULL' parent (unconnected subtree?) - So, we do not store the child position for this volume!";
    } else {
	    // store the parent-child relationship in the DB
	    QString parentType = getGeoTypeFromVPhysVol(parentNode);
        // get the copy number of the parent
        const unsigned int parentCopyN = getLatestParentCopyNumber(parentId.toString(), parentType);
	    //JFB Commented out: qDebug() << "PhysVol Id:" << physId << " - copyNumber:" << QString::number(parentCopyN);

	    QString childType  = getGeoTypeFromVPhysVol(vol);

	    storeChildPosition(parentId, parentType, physId, parentCopyN, getChildPosition( parentId.toString(), parentType, parentCopyN), childType, volCopyN);
    }
}


QString WriteGeoModel::getGeoTypeFromVPhysVol(const GeoVPhysVol* vol)
{
	if (!vol)
		return QString("NULL");

	QString geoType;
	if (dynamic_cast<const GeoPhysVol*>(vol)) {
		//JFB Commented out: qDebug() << "GeoType: GeoPhysVol";
		geoType = "GeoPhysVol";
	}
	else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
		//JFB Commented out: qDebug() << "GeoType: GeoFullPhysVol";
		geoType = "GeoFullPhysVol";
	} else {
		qWarning() << "WARNING!! Unknown GeoVPhysVol type!!";
	}
	return geoType;
}


void WriteGeoModel::handleSerialDenominator (const GeoSerialDenominator *node)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleSerialDenominator(GeoSerialDenominator*)";

	QString address = getAddressStringFromPointer( node );

	std::string baseNameStr = node->getBaseName();
	QString baseName = QString::fromStdString(baseNameStr);
	//JFB Commented out: qDebug() << "base name:" << baseName << "address:" << address;

	// variables used to persistify the object
	QVariant sdId;

	// get the parent volume
	QStringList parentList = getParentNode();
	QString parentId = parentList[0];
	QString parentType = parentList[1];
	unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

	// check if this object has been stored already
	if (! isAddressStored(address)) {

		//JFB Commented out: qDebug() << "New SerialDenominator, storing it...";

		/* STORE THE OBJECT IN THE DB */
		sdId = storeObj(node, baseName);
		//JFB Commented out: qDebug() << "SerialDenominator stored. Id:" << sdId.toString();

	} else {
		sdId = getStoredIdFromAddress(address);
		//JFB Commented out: qDebug() << "SerialDenominator already stored in the DB. Id:" << sdId.toString();
	}

	storeChildPosition(parentId, parentType, sdId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoSerialDenominator", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
}



void WriteGeoModel::handleSerialTransformer (const GeoSerialTransformer *node)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleSerialTransformer(GeoSerialTransformer*)";

	QString address = getAddressStringFromPointer( node );

	// variables used to persistify the object
	QVariant functionId;
	QVariant physvolId;
	QVariant physvolTable;
	unsigned int nCopies;
	QVariant stId;

	// get the parent volume
	QStringList parentList = getParentNode();
	QString parentId = parentList[0];
	QString parentType = parentList[1];
	unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);


	// check if this object has been stored already
	if (! isAddressStored(address)) {

		//JFB Commented out: qDebug() << "New SerialTransformer, storing it...";

		/*
		 * Get Node characteristics
		 *
		 * SerialTransformer features:
		 * - 1 parent PhysVol (if any)
		 * - 1 GENFUNCTION
		 * - 1 PhysVolId
		 * - 1 PhysVolType
		 * - 1 number of copies
		 */
		// get linked function and number of copies
		const GeoXF::Function * func = node->getFunction();
		nCopies =  node->getNCopies();
		//JFB Commented out: qDebug() << "n. of copies:" << QString::number(nCopies);

		// get linked VPhysVol volume
		const GeoVPhysVol *vol = &( *( node->getVolume() ) );
		// const GeoPhysVol* vol = dynamic_cast<const GeoPhysVol*>(volV);
		QString volType;
		if (dynamic_cast<const GeoPhysVol*>(vol)) {
			volType = "GeoPhysVol";
		} else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
			volType = "GeoFullPhysVol";
		} else {
			qWarning() << "ERROR!!! Unknown VPhysVol type!!";
		}

		/*
		 * Persistify the Function
		 */
	 	TransFunctionPersistifier persistifier;
        try {
            persistifier.persistify(*func);
        } catch (const std::runtime_error & error) {
            std::cout << "SEVERE WARNING!! Handling std::runtime_error! -->" << error.what() << std::endl;
        }
		QString expression = QString::fromStdString( persistifier.getCodedString() );
		//JFB Commented out: qDebug() << "FUNCTION:" << expression;

		if (expression.size() == 0) {
				qFatal("FATAL ERROR!! Function expression is empty!! Aborting...");
				abort();
		}


		/*
		 * STORE/GET THE INNER OBJECTS IN THE DB
		 */

		// store/get the Function object in the DB
		functionId = storeObj(func, expression);

		// store/get the PhysVol volume in the DB

		/* Concerning the physVol referenced by the SerialTransformer:
		 *
		 * - if physvol is not stored in the DB already;
		 * - if physVol has no parent (==NULL)
		 * ==> it means that its children will never visited by the NodeAction executed on the root tree
		 *
		 * ==> So, we need to persitify it as a new tree, to get all its children visited and persistified
		 *
		 */
		//JFB Commented out: qDebug() << "Handling the referenced VPhysVol, going into the sub-tree, if any...";
		handleReferencedVPhysVol(vol);

		//JFB Commented out: qDebug() << "Storing the referenced VPhysVol...";
		QString physvolAddress = getAddressStringFromPointer(vol);
		physvolId = getStoredIdFromAddress(physvolAddress);


		/*
		 * STORE THE NODE INTO THE DB AND ITS POSITION WITHIN THE TREE
		 */
		// store the SerialTransformer volume in the DB
		stId = storeObj(node, functionId, physvolId, volType, nCopies);
		//JFB Commented out: qDebug() << "SerialTransformer stored. Id:" << stId.toString();

	} else {
		stId = getStoredIdFromAddress(address);
		//JFB Commented out: qDebug() << "SerialTransformer already stored in the DB. Id:" << stId.toString();
	}

	//JFB Commented out: qDebug() << "Storing:" << parentId << parentType << stId;
	storeChildPosition(parentId, parentType, stId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoSerialTransformer", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
}




void WriteGeoModel::handleTransform(const GeoTransform* node)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleTransform(GeoTransform*)";

	QString address = getAddressStringFromPointer( node );

	// get the parent volume
	QStringList parentList = getParentNode();
	QString parentId = parentList[0];
	QString parentType = parentList[1];

	unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

	/*
	 * STORE THE OBJECT IN THE DB AND ITS PLACE WITHIN THE TREE
	 */

	 // store the transformation in the DB
	 QVariant trId = storeTranform(node);

	// Store the child-parent relationship

	if ( dynamic_cast<const GeoAlignableTransform*>(node) ) {
		//JFB Commented out: qDebug() << "Storing a GeoAlignableTransform...";
		storeChildPosition(parentId, parentType, trId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoAlignableTransform", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
	}
	else if ( dynamic_cast<const GeoTransform*>(node) ) {
		//JFB Commented out: qDebug() << "Storing a GeoTransform...";
		storeChildPosition(parentId, parentType, trId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoTransform", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
	}
	else {
		qWarning("Not implemented yet!!!! ");
	}

}


void WriteGeoModel::handleNameTag(const GeoNameTag* node)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleNameTag(GeoNameTag*)";

	std::string nameStr = node->getName();
	QString name = QString::fromStdString(nameStr);
	//JFB Commented out: qDebug() << "name:" << name;

	QString address = getAddressStringFromPointer( node );

	// get the parent volume
	QStringList parentList = getParentNode();
	QString parentId = parentList[0];
	QString parentType = parentList[1];
    unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

    // TODO: add "if stored"...

	/*
	 * STORE THE OBJECT IN THE DB AND ITS POSITION WITHIN THE TREE
	 */

	 // store the name tag in the DB
	 QVariant nameId = storeObj(node, name);

	 // Store the child-parent relationship
	 storeChildPosition(parentId, parentType, nameId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoNameTag", 0);
}



//__________________________________________________
QStringList WriteGeoModel::getParentNode()
{
    //JFB Commented out: qDebug() << "WriteGeoModel::getParentNode()";

	// check the current volume position in the geometry tree
	GeoNodePath* path = getPath();
	unsigned int len = path->getLength();
	//JFB Commented out: qDebug() << "length: " << len;

	// reset the number of visited node, if len is different than before
	if (len > m_len) {
		m_len_nChild = 0;
		m_len = len;
	}
	// update number of visited nodes if len is the same as before
	else {
		++m_len_nChild;
	}

	// get the parent volume, if this is not the Root volume
	const GeoVPhysVol* parentNode = nullptr;
	QVariant parentId = "NULL";
	QString parentType = "NULL";

		if (len >= 1)
		{
			// get the parent node
			parentNode = path->getItem(len-1); // i=(len-1) gives you the parent VPhysVol

			if (parentNode) {

				// get the parent type
				parentType = getGeoTypeFromVPhysVol(parentNode);

				// get the parent memory address
				QString parentAddress = getAddressStringFromPointer(parentNode);
				//JFB Commented out: qDebug() << "-- parent's address:" << parentNode;

				// get the id of the parent node, which should be stored already in the DB
				if (isAddressStored(parentAddress)) {
					parentId = getStoredIdFromAddress(parentAddress);
				}
				else {
					// qFatal("FATAL ERROR!!! - The parent node of this child should has been stored in the DB already, but it was not found!!");
					qWarning() << "The parent node of this child node seems to not having be stored in the DB yet! [It is normal if it is the root volume or a transformation node used for example only in the definition of a 'GeoShapeShift' instance]";
				}
				//JFB Commented out: qDebug() << "-- parent's LogVol name:" << QString::fromStdString(parentNode->getLogVol()->getName());
			}
		}
		else{
			// qWarning() << "ERROR!! Len == 1, but this cannot be the Root volume!";
			qWarning() << "WARNING!! Len == 0, but this cannot be the Root volume!";
		}

		QStringList parentList;
		parentList << parentId.toString() << parentType;

		return  parentList;
}


//__________________________________________________________________
QVariant WriteGeoModel::storeShape(const GeoShape* shape)
{
	QString shapeType = QString::fromStdString(shape->type());
	//JFB Commented out: qDebug() << "storeShape() - shape name:" << shapeType  << ", address:" << shape;
	
    // LArCustomShape is deprecated.  Write it out as a GeoUnidentifiedShape;
    if (shapeType=="CustomShape") shapeType="UnidentifiedShape";
    
    // get shape parameters
	QString shapePars = getShapeParameters(shape);
	
	// store the shape in the DB and returns the ID
	return storeObj(shape, shapeType, shapePars);
}


//______________________________________________________________________
QVariant WriteGeoModel::storeMaterial(const GeoMaterial* mat)
{
	const QString matName = QString::fromStdString(mat->getName());   //The name of the material.
	const QString matID = QString::number(mat->getID());              //Gives an integral identifier for the material.For convenience.
	const QString matDensity = QString::number(mat->getDensity());	  //The density of the material.
	const unsigned int numElements = mat->getNumElements();

	const QString matNumElements = QString::number(numElements);
	//JFB Commented out: qDebug() << "storeMaterial() - material name:" << matName
	//		<< ", address:" << mat
	//		<< ", matID:" << matID
	//		<< ", matDensity:" << matDensity
	//		<< ", matNumElements:" << matNumElements;

	// loop over the elements composing the material
	QString matElements;
	QStringList matElementsList;
	for (unsigned int i=0; i < numElements; i++) {

		//Gets the i-th element.
		const GeoElement* element = mat->getElement(i);
		std::string elName = element->getName();
		/*
		  std::cout << "\t element n. " << i << std::endl;
		  std::cout << "\t element address: " << element << std::endl;
		  std::cout << "\t element name: " << elName << std::endl;
		 */

		// Store the element and get its DataBase ID
		QVariant elementId = storeElement(element);

		//Gets the fraction by weight of the i-th element
		const QString elementFraction = QString::number( mat->getFraction(i) );

		//JFB Commented out: qDebug() << "\t--> element ID: " << elementId.toString() << " - getFraction:" << elementFraction;

		matElementsList << elementId.toString() + ":" + elementFraction;
	}
	matElements = matElementsList.join(";");

	//JFB Commented out: qDebug() << "\t==> material's elements:" << matElements;

	// store the material in the DB and returns the ID
	return storeObj(mat, matName, matDensity, matElements);
}


//_______________________________________________________________________
QVariant WriteGeoModel::storeElement(const GeoElement* el)
{
	//	The name of the element, e.g. "Carbon".
	const QString elName = QString::fromStdString(el->getName());

	//	The chemical symbol for the element, e.g. C, O, S, Na....
	const QString elSymbol = QString::fromStdString(el->getSymbol());

	//	The atomic number Z for the material.
	const QString elZ = QString::number(el->getZ());

	//	The average atomic mass for the element.
	const QString elA = QString::number(el->getA());

	// store the material in the DB and returns the ID
	return storeObj(el, elName, elSymbol, elZ, elA);
}


//_______________________________________________________________________
QVariant WriteGeoModel::storeTranform(const GeoTransform* node)
{
	/*
	 * STORE THE OBJECT IN THE DB
	 */

	 QString address = getAddressStringFromPointer( node );

	 QVariant trId;

	// check if this object has been stored already
	if (! isAddressStored(address)) {

		//JFB Commented out: qDebug() << "New Transform, storing it...";


				// TODO: simplify and put common code in a separate class

				/*
				 * get the 12 matrix elements
				 */
				// Get the 9 rotation coefficients
				double xx = node->getTransform()(0, 0);
				double xy = node->getTransform()(0, 1);
				double xz = node->getTransform()(0, 2);

				double yx = node->getTransform()(1, 0);
				double yy = node->getTransform()(1, 1);
				double yz = node->getTransform()(1, 2);

				double zx = node->getTransform()(2, 0);
				double zy = node->getTransform()(2, 1);
				double zz = node->getTransform()(2, 2);

				// Get the 3 translation coefficients
				double dx = node->getTransform()(0, 3);
				double dy = node->getTransform()(1, 3);
				double dz = node->getTransform()(2, 3);


				// Instanciate an Eigen's 3D Transformation
				GeoTrf::Transform3D tr;

				// set rotation
				tr(0,0)=xx;
				tr(0,1)=xy;
				tr(0,2)=xz;

				tr(1,0)=yx;
				tr(1,1)=yy;
				tr(1,2)=yz;

				tr(2,0)=zx;
				tr(2,1)=zy;
				tr(2,2)=zz;

			 	// set translation vector/column
				tr(0,3)=dx;
				tr(1,3)=dy;
				tr(2,3)=dz;

				// explicitely set the last row: (0,0,0,1)
				tr(3,0)=0;
				tr(3,1)=0;
				tr(3,2)=0;
				tr(3,3)=1;

		std::vector<double> params = getTransformParameters(tr);
		//JFB Commented out: qDebug() << "Transform parameters:" << QVector<double>::fromStdVector(params);

		/*
		 * STORE THE OBJECT IN THE DB
		 */

		// store the object in the DB
		if ( dynamic_cast<const GeoAlignableTransform*>(node) ) {
			trId = storeObj(dynamic_cast<const GeoAlignableTransform*>(node), params);
			//JFB Commented out: qDebug() << "AlignableTransform stored. Id:" << trId.toString();
		}
		else if ( dynamic_cast<const GeoTransform*>(node) ) {
			trId = storeObj(dynamic_cast<const GeoTransform*>(node), params);
			//JFB Commented out: qDebug() << "Transform stored. Id:" << trId.toString();
		}
		else {
			std::cout << "WARNING!!! - This type of transformation still needs to be persistified!!" << std::endl;
		}

	} else {
		trId = getStoredIdFromAddress(address);
		//JFB Commented out: qDebug() << "Transform already stored in the DB. Id:" << trId.toString();
	}

	return trId;
}


void WriteGeoModel::handleReferencedVPhysVol (const GeoVPhysVol *vol)
{
	//JFB Commented out: qDebug() << "\nWriteGeoModel::handleReferencedVPhysVol(GeoVPhysVol*)";

	// qDebug() << "PhysVol's LogVol name:" << QString::fromStdString(vol->getLogVol()->getName());

	// get the address string for the current volume
	QString address = getAddressStringFromPointer( vol );

	QVariant parentId = "NULL";

	//JFB Commented out: qDebug() << "setting 'unconnected' to: false";
	m_unconnectedTree = false;

	// get the parent volume, if this is not the Root volume
	// JFB The following construct does not work:  what if the parent is null?  
	// const GeoVPhysVol* parentNode = dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() )));
	/// JFB So I replaced it by the next two lines:
	auto p=vol->getParent();
	const GeoVPhysVol* parentNode = p ? dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() ))) : nullptr;
	
	if (parentNode) {
		QString parentAddress = getAddressStringFromPointer(parentNode);
		//JFB Commented out: qDebug() << "--> parent's address:" << parentNode;

		if (isAddressStored(parentAddress))
			parentId = getStoredIdFromAddress(parentAddress);
		//		else
		//			qFatal("FATAL ERROR!!! - The parent node of this child should has been stored in the DB already, but it was not found!!");

		//JFB Commented out: qDebug() << "--> parent's LogVol name:" << QString::fromStdString(parentNode->getLogVol()->getName());
	}
	else {
		//JFB Commented out: qDebug() << "NULL parent node!! --> it seems to be an unconnected subtree.";
		//JFB Commented out: qDebug() << "setting 'unconnected' to: true";
		m_unconnectedTree = true;
	}

  // for Debug
	// // counting children
	// unsigned int nChildren = vol->getNChildVols();
	// qDebug() << "number of child physical volumes:" << nChildren;
	// qDebug() << "[number of PhysVol and SerialTransformer child nodes:" << vol->getNChildVolAndST() << "]";
	//
	// GeoCountVolAction cv;
	// cv.setDepthLimit(1);
	// vol->exec(&cv);
	// int nChildCount = cv.getCount();
	// qDebug() << "number of child volumes:" << nChildCount;



	// check if this object has been stored already
	if (! isAddressStored(address)) {

		//JFB Commented out: qDebug() << "This is a new root PhysVol node of an 'unconnected' tree, so we start another action on it to dump it into the DB...";

		// Dump the tree volumes into the DB
		vol->exec(this); // TODO: check if the new action overwrites the id of the volumes already in the DB...!!!

	} else {
		//JFB Commented out: qDebug() << "The referenced volume has been stored already. Skipping...";
	}
	// at the end, we make sure we reset the status
	//JFB Commented out: qDebug() << "setting 'unconnected' to: false";
	m_unconnectedTree = false;
}


// Get shape parameters
QString WriteGeoModel::getShapeParameters(const GeoShape* shape)
{
	const QString shapeType = QString::fromStdString(shape->type());
	//JFB Commented out: qDebug() << "shapeType:" << shapeType;

	QString shapePars = "";

	if (shapeType == "Box") {
		//JFB Commented out: qDebug() << "get GeoBox parameters";
		QStringList pars;
		const GeoBox* box = dynamic_cast<const GeoBox*>(shape);
		pars << "XHalfLength=" + QString::number(box->getXHalfLength()) ;
		pars << "YHalfLength=" + QString::number(box->getYHalfLength()) ;
		pars << "ZHalfLength=" + QString::number(box->getZHalfLength()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Cons") {
		QStringList pars;
		const GeoCons* shapeIn = dynamic_cast<const GeoCons*>(shape);
		pars << "RMin1=" + QString::number(shapeIn->getRMin1()) ;
		pars << "RMin2=" + QString::number(shapeIn->getRMin2()) ;
		pars << "RMax1=" + QString::number(shapeIn->getRMax1()) ;
		pars << "RMax2=" + QString::number(shapeIn->getRMax2()) ;
		pars << "DZ=" + QString::number(shapeIn->getDZ()) ;
		pars << "SPhi=" + QString::number(shapeIn->getSPhi()) ;
		pars << "DPhi=" + QString::number(shapeIn->getDPhi()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Torus") {
		// Member Data:
		// * Rmax - outside radius of the torus tube
		// * Rmin - inside radius  of the torus tube (Rmin=0 if not hollow)
		// * Rtor - radius of the torus itself
		// *
		// * SPhi - starting angle of the segment in radians
		// * DPhi - delta angle of the segment in radians
		//
		QStringList pars;
		const GeoTorus* shapeIn = dynamic_cast<const GeoTorus*>(shape);
		pars << "Rmin=" + QString::number(shapeIn->getRMin()) ;
		pars << "Rmax=" + QString::number(shapeIn->getRMax()) ;
		pars << "Rtor=" + QString::number(shapeIn->getRTor()) ;
		pars << "SPhi=" + QString::number(shapeIn->getSPhi()) ;
		pars << "DPhi=" + QString::number(shapeIn->getDPhi()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Para") {
		QStringList pars;
		const GeoPara* shapeIn = dynamic_cast<const GeoPara*>(shape);
		pars << "XHalfLength=" + QString::number(shapeIn->getXHalfLength()) ;
		pars << "YHalfLength=" + QString::number(shapeIn->getYHalfLength()) ;
		pars << "ZHalfLength=" + QString::number(shapeIn->getZHalfLength()) ;
		pars << "Alpha=" + QString::number(shapeIn->getAlpha()) ;
		pars << "Theta=" + QString::number(shapeIn->getTheta()) ;
		pars << "Phi=" + QString::number(shapeIn->getPhi()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Pcon") {
		QStringList pars;
		const GeoPcon* shapeIn = dynamic_cast<const GeoPcon*>(shape);
		pars << "SPhi=" + QString::number(shapeIn->getSPhi());
		pars << "DPhi=" + QString::number(shapeIn->getDPhi());
		// get number of Z planes and loop over them
		const int nZplanes = shapeIn->getNPlanes();
		pars << "NZPlanes=" + QString::number(nZplanes);
		for (int i=0; i<nZplanes; ++i) {
			pars << "ZPos=" + QString::number(shapeIn->getZPlane(i));
			pars << "ZRmin=" + QString::number(shapeIn->getRMinPlane(i));
			pars << "ZRmax=" + QString::number(shapeIn->getRMaxPlane(i));
		}
		shapePars = pars.join(";");
	} else if (shapeType == "Pgon") {
		QStringList pars;
		const GeoPgon* shapeIn = dynamic_cast<const GeoPgon*>(shape);
		pars << "SPhi=" + QString::number(shapeIn->getSPhi()) ;
		pars << "DPhi=" + QString::number(shapeIn->getDPhi()) ;
		pars << "NSides=" + QString::number(shapeIn->getNSides()) ;
		// get number of Z planes and loop over them
		const int nZplanes = shapeIn->getNPlanes();
		pars << "NZPlanes=" + QString::number(nZplanes);
		for (int i=0; i<nZplanes; ++i) {
			pars << "ZPos=" + QString::number(shapeIn->getZPlane(i));
			pars << "ZRmin=" + QString::number(shapeIn->getRMinPlane(i));
			pars << "ZRmax=" + QString::number(shapeIn->getRMaxPlane(i));
		}
		shapePars = pars.join(";");
	} else if (shapeType == "SimplePolygonBrep") {
		QStringList pars;
		const GeoSimplePolygonBrep* shapeIn = dynamic_cast<const GeoSimplePolygonBrep*>(shape);
		pars << "DZ=" + QString::number(shapeIn->getDZ()) ;
		// get number of vertices and loop over them
		const int nVertices = shapeIn->getNVertices();
		pars << "NVertices=" + QString::number(nVertices);
		for (int i=0; i<nVertices; ++i) {
			pars << "xV=" + QString::number(shapeIn->getXVertex(i));
			pars << "yV=" + QString::number(shapeIn->getYVertex(i));
		}
		shapePars = pars.join(";");
	} else if (shapeType == "Trap") {
		QStringList pars;
		const GeoTrap* shapeIn = dynamic_cast<const GeoTrap*>(shape);
		pars << "ZHalfLength=" + QString::number(shapeIn->getZHalfLength()) ;
		pars << "Theta=" + QString::number(shapeIn->getTheta()) ;
		pars << "Phi=" + QString::number(shapeIn->getPhi()) ;
		pars << "Dydzn=" + QString::number(shapeIn->getDydzn()) ;
		pars << "Dxdyndzn=" + QString::number(shapeIn->getDxdyndzn()) ;
		pars << "Dxdypdzn=" + QString::number(shapeIn->getDxdypdzn()) ;
		pars << "Angleydzn=" + QString::number(shapeIn->getAngleydzn()) ;
		pars << "Dydzp=" + QString::number(shapeIn->getDydzp()) ;
		pars << "Dxdyndzp=" + QString::number(shapeIn->getDxdyndzp()) ;
		pars << "Dxdypdzp=" + QString::number(shapeIn->getDxdypdzp()) ;
		pars << "Angleydzp=" + QString::number(shapeIn->getAngleydzp()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Trd") {
		QStringList pars;
		const GeoTrd* shapeIn = dynamic_cast<const GeoTrd*>(shape);
		pars << "XHalfLength1=" + QString::number(shapeIn->getXHalfLength1()) ;
		pars << "XHalfLength2=" + QString::number(shapeIn->getXHalfLength2()) ;
		pars << "YHalfLength1=" + QString::number(shapeIn->getYHalfLength1()) ;
		pars << "YHalfLength2=" + QString::number(shapeIn->getYHalfLength2()) ;
		pars << "ZHalfLength=" + QString::number(shapeIn->getZHalfLength()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Tube") {
		QStringList pars;
		const GeoTube* tube = dynamic_cast<const GeoTube*>(shape);
		pars << "RMin=" + QString::number(tube->getRMin()) ;
		pars << "RMax=" + QString::number(tube->getRMax()) ;
		pars << "ZHalfLength=" + QString::number(tube->getZHalfLength()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "Tubs") {
		QStringList pars;
		const GeoTubs* shapeIn = dynamic_cast<const GeoTubs*>(shape);
		pars << "RMin=" + QString::number(shapeIn->getRMin()) ;
		pars << "RMax=" + QString::number(shapeIn->getRMax()) ;
		pars << "ZHalfLength=" + QString::number(shapeIn->getZHalfLength()) ;
		pars << "SPhi=" + QString::number(shapeIn->getSPhi()) ;
		pars << "DPhi=" + QString::number(shapeIn->getDPhi()) ;
		shapePars = pars.join(";");
	} else if (shapeType == "TessellatedSolid") {
		QStringList pars;
		const GeoTessellatedSolid* shapeIn = dynamic_cast<const GeoTessellatedSolid*>(shape);
		// get number of facets
		const size_t nFacets = shapeIn->getNumberOfFacets();
		pars << "nFacets=" + QString::number(nFacets);
		// loop over the facets
		for (size_t i=0; i<nFacets; ++i) {
			GeoFacet* facet = shapeIn->getFacet(i);
			// get GeoFacet actual implementation
			if (dynamic_cast<GeoTriangularFacet*>(facet))        pars << "TRI";
			else if (dynamic_cast<GeoQuadrangularFacet*>(facet)) pars << "QUAD";
			// get vertex type (ABSOLUTE/RELATIVE)
			GeoFacet::GeoFacetVertexType facetVertexType = facet->getVertexType();
			if (facetVertexType == GeoFacet::ABSOLUTE) pars << "vT=ABSOLUTE";
			if (facetVertexType == GeoFacet::RELATIVE) pars << "vT=RELATIVE";
			// get number of vertices and loop over them
			const size_t nVertices = facet->getNumberOfVertices();
			pars << "nV=" + QString::number(nVertices);
			for (size_t i=0; i<nVertices; ++i) {
				GeoFacetVertex facetVertex = facet->getVertex(i);
				pars << "xV=" + QString::number( facetVertex[0] );
				pars << "yV=" + QString::number( facetVertex[1] );
				pars << "zV=" + QString::number( facetVertex[2] );
			}
		}
		shapePars = pars.join(";");
		//qDebug() << "Tessellated pars:" << shapePars; // debug
	}
	else if (shapeType == "Intersection") {
		//JFB Commented out: qDebug() << "get GeoShapeIntersection parameters";
		QStringList pars;
		const GeoShapeIntersection* shapeIn = dynamic_cast<const GeoShapeIntersection*>(shape);
		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		QVariant shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		QVariant shapeIdB = storeShape(shapeOpB);
		pars << "opA=" + QString::number( shapeIdA.toUInt() ) ;
		pars << "opB=" + QString::number( shapeIdB.toUInt() ) ;
		shapePars = pars.join(";");
	}
	else if (shapeType == "Shift") {
		//JFB Commented out: qDebug() << "get GeoShapeShift parameters";
		QStringList pars;
		const GeoShapeShift* shapeIn = dynamic_cast<const GeoShapeShift*>(shape);

		// get the referenced Shape used in the 'shift' operation, store it in the DB
		const GeoShape* shapeOp = shapeIn->getOp();
		QVariant shapeId = storeShape(shapeOp);

		// get the Transformation, store it in the DB
		GeoTransform* transf = new GeoTransform( shapeIn->getX() );
		QVariant trId = storeTranform(transf);

		pars << "A=" + QString::number( shapeId.toUInt() ) ;
		pars << "X=" + QString::number( trId.toUInt() ) ;
		shapePars = pars.join(";");
	}
	else if (shapeType == "Subtraction") {
		//JFB Commented out: qDebug() << "get GeoShapeSubtraction parameters";
		QStringList pars;
		const GeoShapeSubtraction* shapeIn = dynamic_cast<const GeoShapeSubtraction*>(shape);
		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		QVariant shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		QVariant shapeIdB = storeShape(shapeOpB);
		pars << "opA=" + QString::number( shapeIdA.toUInt() ) ;
		pars << "opB=" + QString::number( shapeIdB.toUInt() ) ;
		shapePars = pars.join(";");
	}
	else if (shapeType == "Union") {
		//JFB Commented out: qDebug() << "get GeoShapeUnion parameters";
		QStringList pars;
		const GeoShapeUnion* shapeIn = dynamic_cast<const GeoShapeUnion*>(shape);

		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		QVariant shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		QVariant shapeIdB = storeShape(shapeOpB);

		pars << "opA=" + QString::number( shapeIdA.toUInt() ) ;
		pars << "opB=" + QString::number( shapeIdB.toUInt() ) ;
		shapePars = pars.join(";");
	}
	else if (shapeType=="GenericTrap") {
	  QStringList pars;
	  const GeoGenericTrap * shapeIn = dynamic_cast<const GeoGenericTrap*>(shape);
	  pars << "ZHalfLength=" + QString::number(shapeIn->getZHalfLength());
	  pars << "NVertices="   + QString::number(shapeIn->getVertices().size());
	  for (int i=0; i<shapeIn->getVertices().size(); ++i) {
	    pars << "X=" + QString::number(shapeIn->getVertices()[i](0));
	    pars << "Y=" + QString::number(shapeIn->getVertices()[i](1));
	  }
	  shapePars = pars.join(";");
	}
	else if (shapeType=="UnidentifiedShape") {
	  const GeoUnidentifiedShape *shapeIn=dynamic_cast<const GeoUnidentifiedShape *> (shape);
	  QStringList pars;
	  pars << "name="+QString::fromStdString(shapeIn->name());
	  pars << "asciiData="+QString::fromStdString(shapeIn->asciiData());
	  shapePars=pars.join(";");
  }
  //LAr custom shape
  // else if(shapeType == "CustomShape") {
  //   std::cout << "\n\tWARNING!!! - Use of LArCustomShape is deprecated, translating to GeoUnidentifiedShape representation!!\n\n";
  //   const LArCustomShape* shapeIn = dynamic_cast<const LArCustomShape*> (shape);
  //   QStringList pars;
  //   pars << "name=LArCustomShape";
  //   pars << "asciiData="+QString::fromStdString(shapeIn->name());
  //   shapePars=pars.join(";");
  // }
  else {
    std::cout << "\n\tWARNING!!! - Shape '" << shapeType.toStdString() << "' needs to be persistified!!\n\n";
    m_objectsNotPersistified << shapeType;
  }

  return shapePars;

}


std::vector<double> WriteGeoModel::getTransformParameters(GeoTrf::Transform3D tr)
{
	std::vector<double> vec;

    // get the rotation matrix, the first 3x3 matrix
	vec.push_back(tr(0,0)); // xx
	vec.push_back(tr(0,1)); // xy
	vec.push_back(tr(0,2)); // xz

	vec.push_back(tr(1,0)); // yx
	vec.push_back(tr(1,1)); // yy
	vec.push_back(tr(1,2)); // yz

	vec.push_back(tr(2,0)); // zx
	vec.push_back(tr(2,1)); // zy
	vec.push_back(tr(2,2)); // zz

    // get the translation vector, i.e. the last column/3vector
	vec.push_back(tr(0,3)); // dx
	vec.push_back(tr(1,3)); // dy
	vec.push_back(tr(2,3)); // dz

	return vec;
}


WriteGeoModel::WriteGeoModel(GMDBManager &db)
{
	//JFB Commented out: qDebug() << "WriteGeoModel: constructor";

	// init variables
	m_len = 0;
	m_len_nChild = 0;
	m_rootVolumeFound = false;
	m_unconnectedTree = false;

	// init anche check the database handle
	m_dbManager = &db;
	if (m_dbManager->isOpen()) {
		//JFB Commented out: qDebug() << "OK! Database is open!";
	}
	else {
		//JFB Commented out: qDebug() << "Database is not open!";
	}

    m_dbpath = m_dbManager->getDBFilePath();

	// get DB metadata
	m_memMap_Tables = m_dbManager->getAll_NodeTypesTableIDs();
	// qDebug() << "m_memMap_Tables:" << m_memMap_Tables;

}

WriteGeoModel::~WriteGeoModel()
{
	// nothing to do so far...
}


void WriteGeoModel::showMemoryMap()
{
	//JFB Commented out: qDebug() << "WriteGeoModel::showMemoryMap()";

	QMap<QString, QVariant>::const_iterator it = m_memMap.constBegin();
	while (it != m_memMap.constEnd()) {
		std::cout << it.key().toStdString() << ": " << it.value().toString().toStdString() << std::endl;
		++it;
	}
}



QVariant WriteGeoModel::storeObj(const GeoMaterial* pointer, const QString name, const QString density, const QString elements)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoMaterial*) - name:" << name << "- address:" << pointer << "- density:" << density << "- elements:" << elements;

	QString address = getAddressStringFromPointer( pointer );
	QVariant materialId;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New Material! Storing it...";

		materialId = addMaterial(name, density, elements);

		storeAddress( address, materialId );
	}
	else {
		//JFB Commented out: qDebug() << "Material node stored already. Getting ID from the memory map...";
		materialId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "materialId:" << materialId;
	return materialId;
}


QVariant WriteGeoModel::storeObj(const GeoElement* pointer, const QString name, const QString symbol, const QString elZ, const QString elA)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoElement*) - name:" << name << "address:" << pointer << " - symbol: " << symbol << " - elZ: " << elZ << " - elA: " << elA;

	QString address = getAddressStringFromPointer( pointer );
	QVariant elementId;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New Element! Storing it...";

		elementId = addElement(name, symbol, elZ, elA);

		storeAddress( address, elementId );
	}
	else {
		//JFB Commented out: qDebug() << "Element node stored already. Getting ID from the memory map...";
		elementId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "elementId:" << elementId;
	return elementId;
}


QVariant WriteGeoModel::storeObj(const GeoShape* pointer, const QString name, const QString parameters)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoShape*) - name:" << name << "address:" << pointer;

	QString address = getAddressStringFromPointer( pointer );

	QVariant shapeId;
	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New Shape! Storing it...";

		// shapeId = m_dbManager->addShape(name, parameters);
		shapeId = addShape(name, parameters);

		storeAddress( address, shapeId);
	}
	else {
		//JFB Commented out: qDebug() << "Shape node stored already. Getting ID from the memory map...";
		shapeId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "shapeId:" << shapeId;
	return shapeId;
}

QVariant WriteGeoModel::storeObj(const GeoLogVol* pointer, const QString name, const QVariant shapeId, const QVariant materialId)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoLogVol*) - name:" << name << "address:" << pointer;

	QString address = getAddressStringFromPointer( pointer );

	QVariant logvolId;
	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New LogVol! Storing it...";

		// logvolId = m_dbManager->addLogVol(name, shapeId, materialId);
		logvolId = addLogVol(name, shapeId, materialId);

		storeAddress( address, logvolId );
	}
	else {
		//JFB Commented out: qDebug() << "LogVol node stored already. Getting ID from the memory map...";
		logvolId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "logvolId:" << logvolId;
	return logvolId;
}


QVariant WriteGeoModel::storeObj(const GeoPhysVol* pointer, const QVariant logvolId, const QVariant parentId, bool isRootVolume)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoPhysVol*) - address:" << pointer << "- is root volume?" << isRootVolume;

	QString address = getAddressStringFromPointer( pointer );

	QVariant physvolId;
	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New PhysVol! Storing it...";

		// physvolId = m_dbManager->addPhysVol(logvolId, parentId, isRootVolume);
		physvolId = addPhysVol(logvolId, parentId, isRootVolume);

		storeAddress( address, physvolId );
	}
	else {
		//JFB Commented out: qDebug() << "PhysVol node stored already. Getting ID from the memory map...";
		physvolId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "physvolId:" << physvolId;
	return physvolId;
}

QVariant WriteGeoModel::storeObj(const GeoFullPhysVol* pointer, const QVariant logvolId, const QVariant parentId, bool isRootVolume)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoFullPhysVol*) - address:" << pointer << "- is root volume?" << isRootVolume;

	QString address = getAddressStringFromPointer( pointer );

	QVariant physvolId;
	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New FullPhysVol! Storing it...";

		// physvolId = m_dbManager->addFullPhysVol(logvolId, parentId, isRootVolume);
		physvolId = addFullPhysVol(logvolId, parentId, isRootVolume);

		storeAddress( address, physvolId );
	}
	else {
		//JFB Commented out: qDebug() << "FullPhysVol node stored already. Getting ID from the memory map...";
		physvolId = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "fullphysvolId:" << physvolId;
	return physvolId;
}

QVariant WriteGeoModel::storeObj(const GeoSerialDenominator* pointer, const QString baseName)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoSerialDenominator*) - baseName:" << baseName << "address:" << pointer;

	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New SerialDenominator! Storing it...";

		// id = m_dbManager->addSerialDenominator(baseName);
		id = addSerialDenominator(baseName);

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "SerialDenominator node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}


QVariant WriteGeoModel::storeObj(const GeoSerialTransformer* pointer, const QVariant functionId, const QVariant volId, const QString volType, const unsigned int copies)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoSerialTransformer*):" << volId.toUInt() << volType << "- n. of copies: " << copies;

	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New SerialTransformer! Storing it...";

		// id = m_dbManager->addSerialTransformer(functionId, volId, volType, copies);
		id = addSerialTransformer(functionId, volId, volType, copies);

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "SerialTransformer node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}

QVariant WriteGeoModel::storeObj(const GeoXF::Function* pointer, const QString expression)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoXF::Function*)";

	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New GeoXF::Function! Storing it...";

		// id = m_dbManager->addFunction(expression);
		id = addFunction(expression);

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "GeoXF::Function node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}

QVariant WriteGeoModel::storeObj(const GeoTransform* pointer, std::vector<double> parameters)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoTransform*)";


	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New GeoXF::Function! Storing it...";

		// id = m_dbManager->addTransform( QVector<double>::fromStdVector(parameters) );
		id = addTransform( parameters );

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "GeoTransform node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}

QVariant WriteGeoModel::storeObj(const GeoAlignableTransform* pointer, std::vector<double> parameters)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoAlignableTransform*)";


	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New GeoXF::Function! Storing it...";

		// id = m_dbManager->addAlignableTransform( QVector<double>::fromStdVector(parameters) );
		id = addAlignableTransform( parameters );

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "GeoAlignableTransform node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}


QVariant WriteGeoModel::storeObj(const GeoNameTag* pointer, const QString name)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeObj(GeoNameTag*) - name:" << name << "address:" << pointer;

	QString address = getAddressStringFromPointer( pointer );
	QVariant id;

	if (! isAddressStored(address)) {
		//JFB Commented out: qDebug() << "New SerialDenominator! Storing it...";

		// id = m_dbManager->addNameTag(name);
		id = addNameTag(name);

		storeAddress( address, id );
	}
	else {
		//JFB Commented out: qDebug() << "SerialDenominator node stored already. Getting ID from the memory map...";
		id = getStoredIdFromAddress(address);
	}
	//JFB Commented out: qDebug() << "ID:" << id;
	return id;
}

void WriteGeoModel::storeChildPosition(const QVariant parentId, const QString parentType, const QVariant childId, const unsigned int parentCopyN, const unsigned int childPos, const QString childType, const unsigned int childCopyN)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeChildPosition()";
	addChildPosition(parentId, parentType, childId, parentCopyN, childPos, childType, childCopyN); // FIXME: change the positions of the parameters to a more logical order, like: parentID, parentType, parentCopyN, childPos, ChildType, childId, childCopyN
	return;
}


unsigned int WriteGeoModel::addRecord(std::vector<QStringList>* container, const QStringList values) const
{
	container->push_back(values);
	unsigned int idx = container->size(); // index of pushed element = size after pushing, to match ID starting at 1 in the DB
	return idx;
}

QVariant WriteGeoModel::addMaterial(const QString name, const QString density, const QString elements)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addMaterial(QString*) - name:" << name << "- density:" << density << "- elements:" << elements;
	std::vector<QStringList>* container = &m_materials;
	QStringList values;
	values << name << density << elements;
	return QVariant( addRecord(container, values) );
}


QVariant WriteGeoModel::addElement(const QString name, const QString symbol, const QString elZ, const QString elA)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addElement(QString*) - name:" << name << "- symbol: " << symbol << "- elZ:" << elZ << "- elA:" << elA;
	std::vector<QStringList>* container = &m_elements;
	QStringList values;
	values << name << symbol << elZ << elA;
	return QVariant( addRecord(container, values) );
}


QVariant WriteGeoModel::addNameTag(const QString name)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addNameTag(QString*) - name:" << name;
	std::vector<QStringList>* container = &m_nameTags;
	QStringList values;
	values << name;
	return QVariant( addRecord(container, values) );
}

QVariant WriteGeoModel::addSerialDenominator(const QString &baseName)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addSerialDenominator(QString*) - basename:" << baseName;
	std::vector<QStringList>* container = &m_serialDenominators;
	QStringList values;
	values << baseName;
	return QVariant( addRecord(container, values) );
}


QVariant WriteGeoModel::addFunction(const QString expression)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addFunction(QString*) - expression:" << expression;
	std::vector<QStringList>* container = &m_functions;
	QStringList values;
	values << expression;
	return QVariant( addRecord(container, values) );
}


QVariant WriteGeoModel::addAlignableTransform(const std::vector<double> params)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addAlignableTransform(QString*)";
	std::vector<QStringList>* container = &m_alignableTransforms;
	QStringList values;
	foreach(double par, params) {
		values << QString::number(par);
	}
	return QVariant( addRecord(container, values) );
}



QVariant WriteGeoModel::addTransform(const std::vector<double> params)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addTransform(QString*)";
	std::vector<QStringList>* container = &m_transforms;
	QStringList values;
	foreach(double par, params) {
		values << QString::number(par);
	}
	return QVariant( addRecord(container, values) );
}

QString WriteGeoModel::getIdFromNodeType( QString nodeType )
{
        //JFB Commented out: qDebug() << "getIdFromNodeType(" << nodeType <<")";
		if (m_memMap_Tables.contains(nodeType))
			return QString::number(m_memMap_Tables.value(nodeType));
		else
			return QString("NULL");
}

QVariant WriteGeoModel::addSerialTransformer(const QVariant &funcId, const QVariant &physvolId, const QString volType, const unsigned int &copies)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::addSerialTransformer()";
	std::vector<QStringList>* container = &m_serialTransformers;
	QString volTypeID = getIdFromNodeType(volType);

	QStringList values;
	values << funcId.toString() << physvolId.toString() << volTypeID << QString::number(copies);

	return QVariant( addRecord(container, values) );
}

QVariant WriteGeoModel::addShape(const QString &type, const QString &parameters)
{
	std::vector<QStringList>* container = &m_shapes;
	QStringList values;
	values << type << parameters;
	return QVariant( addRecord(container, values) );
}


QVariant WriteGeoModel::addPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume)
{
	std::vector<QStringList>* container = &m_physVols;

	QStringList values;
	values << logVolId.toString() << parentPhysVolId.toString(); // TODO: we should remove the parent info: it's not complete because the type is missing (PhysVol or FullPhysVol) and it's redundant, because we store the childrenPositions. It's only useful for quick visual debug, by dumping the PhysVol DB table

	unsigned int idx = addRecord(container, values);

	if (isRootVolume) {
		QStringList rootValues;
		rootValues << QString::number(idx) << "GeoPhysVol";
		m_rootVolume = rootValues;
	}
	return QVariant(idx);
}


QVariant WriteGeoModel::addFullPhysVol(const QVariant &logVolId, const QVariant &parentPhysVolId, bool isRootVolume)
{
	std::vector<QStringList>* container = &m_fullPhysVols;

	QStringList values;
	values << logVolId.toString() << parentPhysVolId.toString(); // TODO: we should remove the parent info: it's not complete because the type is missing (PhysVol or FullPhysVol) and it's redundant, because we store the childrenPositions. It's only useful for quick visual debug, by dumping the PhysVol DB table

	unsigned int idx = addRecord(container, values);

	if (isRootVolume) {
		QStringList rootValues;
		rootValues << QString::number(idx) << "GeoFullPhysVol";
		m_rootVolume = rootValues;
	}
	return QVariant(idx);
}

QVariant WriteGeoModel::addLogVol(const QString &name, const QVariant &shapeId, const QVariant &materialId)
{
	std::vector<QStringList>* container = &m_logVols;
	QStringList values;
	values << name << shapeId.toString() << materialId.toString();
	return QVariant( addRecord(container, values) );
}


void WriteGeoModel::addChildPosition(const QVariant parentId, const QString parentType, const QVariant childId, const unsigned int parentCopyN, const unsigned int childPos, const QString childType, const unsigned int childCopyN)
{
	std::vector<QStringList>* container = &m_childrenPositions;

	QString parentTableID = getIdFromNodeType(parentType);
	QString childTableID = getIdFromNodeType(childType);

	QStringList values;
	values << parentId.toString() << parentTableID <<  QString::number(parentCopyN) << QString::number(childPos) << childTableID << childId.toString() << QString::number(childCopyN);
	addRecord(container, values);
	return;
}


void WriteGeoModel::saveToDB()
{
	//JFB Commented out: qDebug() << "WriteGeoModel::savetoDB()";
    std::cout << "saving to file: " << m_dbpath.toStdString() << std::endl;

	m_dbManager->addListOfRecords("GeoMaterial", m_materials);
	m_dbManager->addListOfRecords("GeoElement", m_elements);
	m_dbManager->addListOfRecords("GeoNameTag", m_nameTags);
	m_dbManager->addListOfRecords("GeoAlignableTransform", m_alignableTransforms);
	m_dbManager->addListOfRecords("GeoTransform", m_transforms);
	m_dbManager->addListOfRecords("Function", m_functions);
	m_dbManager->addListOfRecords("GeoSerialTransformer", m_serialTransformers);
	m_dbManager->addListOfRecords("GeoShape", m_shapes);
	m_dbManager->addListOfRecords("GeoSerialDenominator", m_serialDenominators);
	m_dbManager->addListOfRecords("GeoPhysVol", m_physVols);
	m_dbManager->addListOfRecords("GeoFullPhysVol", m_fullPhysVols);
	m_dbManager->addListOfRecords("GeoLogVol", m_logVols);

	m_dbManager->addListOfChildrenPositions(m_childrenPositions);
	m_dbManager->addRootVolume(m_rootVolume);

	if ( !m_objectsNotPersistified.empty() ) {
		qWarning() << "\n\tWARNING!! There are objects which need to be persistified! --> " << m_objectsNotPersistified << "\n\n";
	}

	return;
}


void WriteGeoModel::storeAddress(const QString address, QVariant id)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::storeAddress(" << address << "," << id << ")";
	m_memMap.insert(address, id);
}

bool WriteGeoModel::isAddressStored(const QString address)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::isAddressStored(): " << address;
	//showMemoryMap(); // only for Debug
	return m_memMap.contains(address);
}


QVariant WriteGeoModel::getStoredIdFromAddress(QString address)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getStoredIdFromAddress(): " << address;
	return m_memMap.value(address);
}

// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoMaterial* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoMaterial*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoElement* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoElement*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoShape* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoShape*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoLogVol* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoLogVol*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoPhysVol* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoPhysVol*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
QString WriteGeoModel::getAddressStringFromPointer(const GeoVPhysVol* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoVPhysVol*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
QString WriteGeoModel::getAddressStringFromPointer(const GeoSerialDenominator* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoSerialDenominator*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
QString WriteGeoModel::getAddressStringFromPointer(const GeoSerialTransformer* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoSerialTransformer*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
QString WriteGeoModel::getAddressStringFromPointer(const GeoXF::Function* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoXF::Function*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

QString WriteGeoModel::getAddressStringFromPointer(const GeoTransform* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoTransform*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

QString WriteGeoModel::getAddressStringFromPointer(const GeoNameTag* pointer)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getAddressStringFromPointer(GeoNameTag*)";
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

QString WriteGeoModel::getQStringFromOss(std::ostringstream &oss)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::getQStringFromOss()";
	std::string addr = oss.str();
	QString address = QString::fromStdString(addr);
	//JFB Commented out: qDebug() << "address string:" << address;
	return address;
}


} /* namespace GeoModelIO */

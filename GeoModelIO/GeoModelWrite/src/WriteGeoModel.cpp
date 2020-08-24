/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

// author: Riccardo.Maria.Bianchi@cern.ch, 2017
// major updates:
// - Aug 2018 - Riccardo Maria Bianchi
// - Feb 2019 - Riccardo Maria Bianchi
// - May 2020 - Riccardo Maria Bianchi
// - Aug 2020 - Riccardo Maria Bianchi - Added support to publish lists of FPV and AXF nodes
// - Aug 2020 - Marilena Bandieramonte (e-mail: marilena.bandieramonte@cern.ch)


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
#include "GeoModelKernel/GeoPublisher.h"

#include "GeoModelKernel/GeoUnidentifiedShape.h"

// C++ includes
#include <sstream>


namespace GeoModelIO {

  // TODO: should go to an utility class
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
  
  // TODO: move this to utility class/file
  void printStdVectorStrings(std::vector<std::string> vec)
  {
    for ( const auto& str : vec) {
      std::cout << str << " ";
    }
    std::cout << std::endl;
    return;
  }

// Function to increase the precision of the conversion from double to string - used in the write operation
std::string to_string_with_precision(const double a_value, const int n = 16){
    
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
  
/// Get next child position available, given the parent type, id and copy number
  unsigned int WriteGeoModel::getChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &copyN)
{
  unsigned int tableId = getIdFromNodeType(parentType);
  //INT
    std::string key = std::to_string(tableId) + ":" + std::to_string(parentId) + ":" + std::to_string(copyN); //INT
  
  std::unordered_map<std::string, unsigned int>::iterator it = m_parentChildrenMap.find(key);
	if ( it == m_parentChildrenMap.end() ) {
		m_parentChildrenMap[key] = 1; // if item is not present, create an entry
	} else {
	    ++m_parentChildrenMap[key]; // if present already, then increment its entry
    }
	return m_parentChildrenMap[key];
}

  unsigned int WriteGeoModel::setVolumeCopyNumber(const unsigned int& volId, const std::string& volType)
{
	//JFB Commented out: qDebug() << "WriteGeoModel::setVolumeCopyNumber()";
	const unsigned int tableId = getIdFromNodeType(volType);
  std::string key = std::to_string(tableId) + ":" + std::to_string(volId); //INT

  std::unordered_map<std::string, unsigned int>::iterator it = m_volumeCopiesMap.find(key);
  if ( it == m_volumeCopiesMap.end() ) {
		m_volumeCopiesMap[key] = 1;
	} else {
    	++m_volumeCopiesMap[key];
    }
	return m_volumeCopiesMap[key];
}


  unsigned int WriteGeoModel::getLatestParentCopyNumber(const unsigned int &parentId, const std::string &parentType)
{
  const unsigned int tableId = getIdFromNodeType(parentType);
  std::string key = std::to_string(tableId) + ":" + std::to_string(parentId); //INT

  std::unordered_map<std::string, unsigned int>::iterator it = m_volumeCopiesMap.find(key);
  if ( it == m_volumeCopiesMap.end() ) {
    std::cout << "ERROR!!! Something's wrong in storing the number of copies!" << std::endl;
	}
	return m_volumeCopiesMap[key];
}


void WriteGeoModel::handlePhysVol (const GeoPhysVol *vol)
{
	handleVPhysVolObjects( vol );
}


void WriteGeoModel::handleFullPhysVol (const GeoFullPhysVol *vol)
{
	handleVPhysVolObjects( vol );
}


void WriteGeoModel::handleVPhysVolObjects(const GeoVPhysVol* vol)
{
	// get the address string for the current volume
  std::string address = getAddressStringFromPointer( vol );

	// variables used to persistify the object
	unsigned int physId;

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
		//qDebug() << "Handling an unconnected tree: we skip the parent-finding step...";
		// now, we reset the status,
		// otherwise all the children of the first referenced, unconnected volume
		// will be treated as unconnected as well
		//qDebug() << "setting 'unconnected' to: false";
		m_unconnectedTree = false;
	}


	// // get the parent volume
	// QStringList parentList = getParentNode(); // you can use that, but you have to modify it to get len-2, instead of len-1, for VPhysVol nodes
	// QString parentId = parentList[0];
	// QString parentType = parentList[1];

	// get the parent volume, if this is not the Root volume or an unconnected sub-tree
	const GeoVPhysVol* parentNode = nullptr;
	unsigned int parentId = 0;

  std::string volTypeStr = "0";

	if (doGetParentNode) {

//        bool isShared = vol->isShared();
        //qDebug() << "is this node shared?" << isShared;

        //if (isShared)
            parentNode = upperVol;
        //else
		//    parentNode = dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() ))); // this cannot be trust for shared nodes
        //qDebug() << "parentNode address" << parentNode;

		if (parentNode) {
      std::string parentAddress = getAddressStringFromPointer(parentNode);
			//JFB Commented out: qDebug() << "==> parent's address:" << parentNode;

			if (isAddressStored(parentAddress))
				parentId = getStoredIdFromAddress(parentAddress);
			//		else
			//			qFatal("FATAL ERROR!!! - The parent node of this child should has been stored in the DB already, but it was not found!!");

			//qDebug() << "==> parent's LogVol name:" << QString::fromStdString(parentNode->getLogVol()->getName());
		}
		else {
			//qDebug() << "NULL parent node!! --> this node appeared unconnected.";
		}
	}

	// counting children
//  unsigned int nChildren = vol->getNChildVols();
	//qDebug() << "number of child physical volumes:" << nChildren;
	//qDebug() << "[number of PhysVol and SerialTransformer child nodes:" << vol->getNChildVolAndST() << "]";

	//// for debug
	// GeoCountVolAction cv;
	// cv.setDepthLimit(1);
	// vol->exec(&cv);
	// int nChildCount = cv.getCount();
	// qDebug() << "number of child volumes:" << nChildCount;



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
    const std::string logName = logVol->getName();

		// MATERIAL
		const GeoMaterial * mat = vol->getLogVol()->getMaterial();

		// SHAPE
		const GeoShape * shape = vol->getLogVol()->getShape();

		/*
		 * STORE THE OBJECTS IN THE DB
		 */

		// store/get the Material object into/from the DB
		unsigned int matId;
		// matId = storeObj(mat, matName);
		matId = storeMaterial(mat);

		// store/get the Shape object into/from the DB
		unsigned int shapeId;
		shapeId = storeShape(shape);
		// shapeId = storeObj(shape, shapeType, shapePars);

		// store/get the LogVol object into/from the DB
		unsigned int logvolId;
		logvolId = storeObj(logVol, logName, shapeId, matId);

		if (dynamic_cast<const GeoPhysVol*>(vol)) {
			const GeoPhysVol* physVol = dynamic_cast<const GeoPhysVol*>(vol);
			// store the PhysVol volume into the DB
			physId = storeObj(physVol, logvolId, parentId, storeRootVolume); // with parent info
      volTypeStr = "GeoPhysVol";
		}
		else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
			const GeoFullPhysVol* fullVol = dynamic_cast<const GeoFullPhysVol*>(vol);
			physId = storeObj(fullVol, logvolId, parentId, storeRootVolume); // with parent info
      volTypeStr = "GeoFullPhysVol";
		} else {
      std::cout << "WARNING!! Unknown GeoVPhysVol type!! Exiting..." << std::endl;
      exit(EXIT_FAILURE);
		}

	} else {
		physId = getStoredIdFromAddress(address);
    volTypeStr = getGeoTypeFromVPhysVol(vol);
	}

    // Now we get the 'copy number' for this volume,
    // to distinguish this volume from the other volumes created from the same shared node (if any)
  if (volTypeStr == "NULL") std::cout << "ERROR!! volTypeStr is 'NULL'!!!\n";
    const unsigned int volCopyN = setVolumeCopyNumber(physId, volTypeStr);
    //JFB Commented out: qDebug() << "physId: " << physId << "- volume copy number: " << volCopyN;

    if ( isRootVolume || parentId == 0) {
        //qDebug() << "This is the RootVolume or the volume has 'NULL' parent (unconnected subtree?) - So, we do not store the child position for this volume!";
    } else {
	    // store the parent-child relationship in the DB
      std::string parentType = getGeoTypeFromVPhysVol(parentNode);
      // get the copy number of the parent
      const unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);
      std::string childType  = getGeoTypeFromVPhysVol(vol);
	    storeChildPosition(parentId, parentType, physId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN), childType, volCopyN);
    }
}


  std::string WriteGeoModel::getGeoTypeFromVPhysVol(const GeoVPhysVol* vol)
{
	if (!vol)
		return "NULL";

  std::string geoType;
	if (dynamic_cast<const GeoPhysVol*>(vol)) {
		geoType = "GeoPhysVol";
	}
	else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
		geoType = "GeoFullPhysVol";
	} else {
    std::cout << "WARNING!! Unknown GeoVPhysVol type!!" << std::endl;
	}
	return geoType;
}


void WriteGeoModel::handleSerialDenominator (const GeoSerialDenominator *node)
{
  std::string address = getAddressStringFromPointer( node );
  std::string baseName = node->getBaseName();

	// variables used to persistify the object
	unsigned int sdId;

	// get the parent volume
  const std::vector<std::string> parentList = getParentNode();
  const unsigned int parentId = std::stoi(parentList[0]);
  const std::string parentType = parentList[1];
	const unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

	// check if this object has been stored already
	if (! isAddressStored(address)) {
		/* STORE THE OBJECT IN THE DB */
		sdId = storeObj(node, baseName);
	} else {
    /* GET THE OBJECT FROM THE DB */
		sdId = getStoredIdFromAddress(address);
	}

	storeChildPosition(parentId, parentType, sdId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoSerialDenominator", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
}



void WriteGeoModel::handleSerialTransformer (const GeoSerialTransformer *node)
{
  std::string address = getAddressStringFromPointer( node );

	// variables used to persistify the object
	unsigned int functionId;
	unsigned int physvolId;
//  unsigned int physvolTable;
	unsigned int nCopies;
	unsigned int stId;

	// get the parent volume
  const std::vector<std::string> parentList = getParentNode();
  const unsigned int parentId = std::stoi(parentList[0]);
  const std::string parentType = parentList[1];
	unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);


	// check if this object has been stored already
	if (! isAddressStored(address)) {
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

		// get linked VPhysVol volume
		const GeoVPhysVol *vol = &( *( node->getVolume() ) );
		// const GeoPhysVol* vol = dynamic_cast<const GeoPhysVol*>(volV);
    std::string volType;
		if (dynamic_cast<const GeoPhysVol*>(vol)) {
			volType = "GeoPhysVol";
		} else if (dynamic_cast<const GeoFullPhysVol*>(vol)) {
			volType = "GeoFullPhysVol";
		} else {
      std::cout << "ERROR!!! Unknown VPhysVol type!!" << std::endl;
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
    std::string expression = persistifier.getCodedString();

		if (expression.size() == 0) {
      std::cout << "FATAL ERROR!! Function expression is empty!! Aborting...\n";
      exit(EXIT_FAILURE);
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
		handleReferencedVPhysVol(vol);

    std::string physvolAddress = getAddressStringFromPointer(vol);
		physvolId = getStoredIdFromAddress(physvolAddress);


		/*
		 * STORE THE NODE INTO THE DB AND ITS POSITION WITHIN THE TREE
		 */
		// store the SerialTransformer volume in the DB
		stId = storeObj(node, functionId, physvolId, volType, nCopies);
	}
  else {
		stId = getStoredIdFromAddress(address);
	}

	storeChildPosition(parentId, parentType, stId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoSerialTransformer", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
}




void WriteGeoModel::handleTransform(const GeoTransform* node)
{
  std::string address = getAddressStringFromPointer( node );

	// get the parent volume
  const std::vector<std::string> parentList = getParentNode();
  const unsigned int parentId = std::stoi(parentList[0]);
  const std::string parentType = parentList[1];

	unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

	/*
	 * STORE THE OBJECT IN THE DB AND ITS PLACE WITHIN THE TREE
	 */

	 // store the transformation in the DB
	 unsigned int trId = storeTranform(node);

	// Store the child-parent relationship

	if ( dynamic_cast<const GeoAlignableTransform*>(node) ) {
		storeChildPosition(parentId, parentType, trId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoAlignableTransform", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
	}
	else if ( dynamic_cast<const GeoTransform*>(node) ) {
		storeChildPosition(parentId, parentType, trId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoTransform", 0); // TODO: Check if the copyN=0 at the end is OK for nodes as Transforms, which cannot be used as parents, only as children!
	}
	else {
    std::cout << "ERROR!! Unknwon Transform type! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
	}

}


void WriteGeoModel::handleNameTag(const GeoNameTag* node)
{
  std::string name = node->getName();
  const std::string address = getAddressStringFromPointer( node );
	// get the parent volume
  const std::vector<std::string> parentList = getParentNode();
  const unsigned int parentId = std::stoi(parentList[0]);
  const std::string parentType = parentList[1];
  unsigned int parentCopyN = getLatestParentCopyNumber(parentId, parentType);

  // FIXME: TODO: add "if stored"...

	/*
	 * STORE THE OBJECT IN THE DB AND ITS POSITION WITHIN THE TREE
	 */

	 // store the name tag in the DB
	 unsigned int nameId = storeObj(node, name);

	 // Store the child-parent relationship
	 storeChildPosition(parentId, parentType, nameId, parentCopyN, getChildPosition( parentId, parentType, parentCopyN ), "GeoNameTag", 0);
}



//__________________________________________________
  std::vector<std::string> WriteGeoModel::getParentNode()
{
	// check the current volume position in the geometry tree
	GeoNodePath* path = getPath();
	unsigned int len = path->getLength();

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
	unsigned int parentId = 0;
  std::string parentType = "NULL";

		if (len >= 1)
		{
			// get the parent node
			parentNode = path->getItem(len-1); // i=(len-1) gives you the parent VPhysVol

			if (parentNode) {

				// get the parent type
				parentType = getGeoTypeFromVPhysVol(parentNode);

				// get the parent memory address
        std::string parentAddress = getAddressStringFromPointer(parentNode);

				// get the id of the parent node, which should be stored already in the DB
				if (isAddressStored(parentAddress)) {
					parentId = getStoredIdFromAddress(parentAddress);
				}
				else {
          std::cout << "The parent node of this child node seems to not having be stored in the DB yet! [It is normal if it is the root volume or a transformation node used for example only in the definition of a 'GeoShapeShift' instance]" << std::endl;
				}
			}
		}
		else{
      std::cout << "WARNING!! Len == 0, but this cannot be the Root volume!" << std::endl;
		}

  std::vector<std::string> parentList;
  parentList.insert(parentList.begin(), {std::to_string(parentId), parentType}); //INT

		return  parentList;
}


//__________________________________________________________________
unsigned int WriteGeoModel::storeShape(const GeoShape* shape)
{
//  QString shapeType = QString::fromStdString(shape->type());
  std::string shapeType = shape->type();
  
  // LArCustomShape is deprecated.  Write it out as a GeoUnidentifiedShape;
  if (shapeType=="CustomShape") shapeType="UnidentifiedShape";
    
  // get shape parameters
  std::string shapePars = getShapeParameters(shape);
	
	// store the shape in the DB and returns the ID
	return storeObj(shape, shapeType, shapePars);
}


//______________________________________________________________________
unsigned int WriteGeoModel::storeMaterial(const GeoMaterial* mat)
{
  const std::string matName = mat->getName();   //The name of the material.
	const double matDensity = mat->getDensity();	  //The density of the material.
	const unsigned int numElements = mat->getNumElements();

	// loop over the elements composing the material
  std::string matElements;
  std::vector<std::string> matElementsList;
	for (unsigned int i=0; i < numElements; i++) {

		//Gets the i-th element.
		const GeoElement* element = mat->getElement(i);
		std::string elName = element->getName();

		// Store the element and get its DataBase ID
		unsigned int elementId = storeElement(element);

		//Gets the fraction by weight of the i-th element
    const std::string elementFraction = to_string_with_precision( mat->getFraction(i) ); //double

    matElementsList.push_back( std::to_string(elementId) + ":" + elementFraction ); //INT+string
	}
	matElements = joinVectorStrings(matElementsList, ";");

	// store the material in the DB and returns the ID
	return storeObj(mat, matName, matDensity, matElements);
}


//_______________________________________________________________________
unsigned int WriteGeoModel::storeElement(const GeoElement* el)
{
	//	The name of the element, e.g. "Carbon".
  const std::string elName = el->getName();

	//	The chemical symbol for the element, e.g. C, O, S, Na....
  const std::string elSymbol = el->getSymbol();

	//	The atomic number Z for the material.
  const double elZ = el->getZ();

	//	The average atomic mass for the element.
  const double elA = el->getA();

	// store the material in the DB and returns the ID
	return storeObj(el, elName, elSymbol, elZ, elA);
}


//_______________________________________________________________________
unsigned int WriteGeoModel::storeTranform(const GeoTransform* node)
{
	/*
	 * STORE THE OBJECT IN THE DB
	 */

  std::string address = getAddressStringFromPointer( node );

	 unsigned int trId = 0;

	// check if this object has been stored already
	if (! isAddressStored(address)) {

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

		/*
		 * STORE THE OBJECT IN THE DB
		 */

		// store the object in the DB
		if ( dynamic_cast<const GeoAlignableTransform*>(node) ) {
			trId = storeObj(dynamic_cast<const GeoAlignableTransform*>(node), params);
		}
		else if ( dynamic_cast<const GeoTransform*>(node) ) {
			trId = storeObj(dynamic_cast<const GeoTransform*>(node), params);
		}
		else {
			std::cout << "ERROR!!! - Unknown type of transformation! Exiting..." << std::endl;
      exit(EXIT_FAILURE);
		}
	} else {
		trId = getStoredIdFromAddress(address);
	}
  
  if(!trId) {
    std::cout << "ERROR!!! - Error while handling a Transform node! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return trId;
}


void WriteGeoModel::handleReferencedVPhysVol (const GeoVPhysVol *vol)
{
	// qDebug() << "PhysVol's LogVol name:" << QString::fromStdString(vol->getLogVol()->getName());

	// get the address string for the current volume
  std::string address = getAddressStringFromPointer( vol );

	unsigned int parentId = 0;

	m_unconnectedTree = false;

	// get the parent volume, if this is not the Root volume
	// JFB The following construct does not work:  what if the parent is null?  
	// const GeoVPhysVol* parentNode = dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() )));
	/// JFB So I replaced it by the next two lines:
	auto p=vol->getParent();
	const GeoVPhysVol* parentNode = p ? dynamic_cast<const GeoVPhysVol*>( &(*(vol->getParent() ))) : nullptr;
	
	if (parentNode) {
    std::string parentAddress = getAddressStringFromPointer(parentNode);

		if (isAddressStored(parentAddress))
			parentId = getStoredIdFromAddress(parentAddress);
		//		else
		//			qFatal("FATAL ERROR!!! - The parent node of this child should has been stored in the DB already, but it was not found!!");

		//qDebug() << "--> parent's LogVol name:" << QString::fromStdString(parentNode->getLogVol()->getName());
	}
	else {
		//qDebug() << "NULL parent node!! --> it seems to be an unconnected subtree.";
		//qDebug() << "setting 'unconnected' to: true";
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

		//qDebug() << "This is a new root PhysVol node of an 'unconnected' tree, so we start another action on it to dump it into the DB...";

		// Dump the tree volumes into the DB
		vol->exec(this); // TODO: check if the new action overwrites the id of the volumes already in the DB...!!!

	} else {
		//qDebug() << "The referenced volume has been stored already. Skipping...";
	}
	// at the end, we make sure we reset the status
	//qDebug() << "setting 'unconnected' to: false";
	m_unconnectedTree = false;
}


// Get shape parameters
std::string WriteGeoModel::getShapeParameters(const GeoShape* shape)
{
  const std::string shapeType = shape->type();

  std::string shapePars = "";
  std::vector<std::string> pars;

	if (shapeType == "Box") {
		const GeoBox* box = dynamic_cast<const GeoBox*>(shape);
		pars.push_back("XHalfLength=" + to_string_with_precision(box->getXHalfLength())) ;
		pars.push_back("YHalfLength=" + to_string_with_precision(box->getYHalfLength())) ;
		pars.push_back("ZHalfLength=" + to_string_with_precision(box->getZHalfLength())) ;
		shapePars = joinVectorStrings(pars,";");
	} else if (shapeType == "Cons") {
		const GeoCons* shapeIn = dynamic_cast<const GeoCons*>(shape);
		pars.push_back("RMin1=" + to_string_with_precision(shapeIn->getRMin1())) ;
		pars.push_back("RMin2=" + to_string_with_precision(shapeIn->getRMin2())) ;
		pars.push_back("RMax1=" + to_string_with_precision(shapeIn->getRMax1())) ;
		pars.push_back("RMax2=" + to_string_with_precision(shapeIn->getRMax2())) ;
		pars.push_back("DZ=" + to_string_with_precision(shapeIn->getDZ())) ;
		pars.push_back("SPhi=" + to_string_with_precision(shapeIn->getSPhi())) ;
		pars.push_back("DPhi=" + to_string_with_precision(shapeIn->getDPhi())) ;
	} else if (shapeType == "Torus") {
		// Member Data:
		// * Rmax - outside radius of the torus tube
		// * Rmin - inside radius  of the torus tube (Rmin=0 if not hollow)
		// * Rtor - radius of the torus itself
		// *
		// * SPhi - starting angle of the segment in radians
		// * DPhi - delta angle of the segment in radians
		//
		const GeoTorus* shapeIn = dynamic_cast<const GeoTorus*>(shape);
		pars.push_back("Rmin=" + to_string_with_precision(shapeIn->getRMin())) ;
		pars.push_back("Rmax=" + to_string_with_precision(shapeIn->getRMax())) ;
		pars.push_back("Rtor=" + to_string_with_precision(shapeIn->getRTor())) ;
		pars.push_back("SPhi=" + to_string_with_precision(shapeIn->getSPhi())) ;
		pars.push_back("DPhi=" + to_string_with_precision(shapeIn->getDPhi())) ;
	}
  else if (shapeType == "Para") {
		const GeoPara* shapeIn = dynamic_cast<const GeoPara*>(shape);
		pars.push_back("XHalfLength=" + to_string_with_precision(shapeIn->getXHalfLength())) ;
		pars.push_back("YHalfLength=" + to_string_with_precision(shapeIn->getYHalfLength())) ;
		pars.push_back("ZHalfLength=" + to_string_with_precision(shapeIn->getZHalfLength())) ;
		pars.push_back("Alpha=" + to_string_with_precision(shapeIn->getAlpha())) ;
		pars.push_back("Theta=" + to_string_with_precision(shapeIn->getTheta())) ;
		pars.push_back("Phi=" + to_string_with_precision(shapeIn->getPhi())) ;
	}
  else if (shapeType == "Pcon") {
		const GeoPcon* shapeIn = dynamic_cast<const GeoPcon*>(shape);
		pars.push_back("SPhi=" + to_string_with_precision(shapeIn->getSPhi()));
		pars.push_back("DPhi=" + to_string_with_precision(shapeIn->getDPhi()));
		// get number of Z planes and loop over them
		const int nZplanes = shapeIn->getNPlanes();
		pars.push_back("NZPlanes=" + std::to_string(nZplanes)); //INT
		for (int i=0; i<nZplanes; ++i) {
			pars.push_back("ZPos=" + to_string_with_precision(shapeIn->getZPlane(i)));
			pars.push_back("ZRmin=" + to_string_with_precision(shapeIn->getRMinPlane(i)));
			pars.push_back("ZRmax=" + to_string_with_precision(shapeIn->getRMaxPlane(i)));
		}
	}
  else if (shapeType == "Pgon") {
		const GeoPgon* shapeIn = dynamic_cast<const GeoPgon*>(shape);
		pars.push_back("SPhi=" + to_string_with_precision(shapeIn->getSPhi())) ;
		pars.push_back("DPhi=" + to_string_with_precision(shapeIn->getDPhi())) ;
		pars.push_back("NSides=" + std::to_string(shapeIn->getNSides())) ; //INT
		// get number of Z planes and loop over them
		const int nZplanes = shapeIn->getNPlanes();
		pars.push_back("NZPlanes=" + std::to_string(nZplanes)); //INT
		for (int i=0; i<nZplanes; ++i) {
			pars.push_back("ZPos=" + to_string_with_precision(shapeIn->getZPlane(i)));
			pars.push_back("ZRmin=" + to_string_with_precision(shapeIn->getRMinPlane(i)));
			pars.push_back("ZRmax=" + to_string_with_precision(shapeIn->getRMaxPlane(i)));
		}
	}
  else if (shapeType == "SimplePolygonBrep") {
		const GeoSimplePolygonBrep* shapeIn = dynamic_cast<const GeoSimplePolygonBrep*>(shape);
		pars.push_back("DZ=" + to_string_with_precision(shapeIn->getDZ())) ;
		// get number of vertices and loop over them
		const int nVertices = shapeIn->getNVertices();
		pars.push_back("NVertices=" + std::to_string(nVertices)); //INT
		for (int i=0; i<nVertices; ++i) {
			pars.push_back("xV=" + to_string_with_precision(shapeIn->getXVertex(i)));
			pars.push_back("yV=" + to_string_with_precision(shapeIn->getYVertex(i)));
		}
	}
  else if (shapeType == "Trap") {
		const GeoTrap* shapeIn = dynamic_cast<const GeoTrap*>(shape);
		pars.push_back("ZHalfLength=" + to_string_with_precision(shapeIn->getZHalfLength())) ;
		pars.push_back("Theta=" + to_string_with_precision(shapeIn->getTheta())) ;
		pars.push_back("Phi=" + to_string_with_precision(shapeIn->getPhi())) ;
		pars.push_back("Dydzn=" + to_string_with_precision(shapeIn->getDydzn())) ;
		pars.push_back("Dxdyndzn=" + to_string_with_precision(shapeIn->getDxdyndzn())) ;
		pars.push_back("Dxdypdzn=" + to_string_with_precision(shapeIn->getDxdypdzn())) ;
		pars.push_back("Angleydzn=" + to_string_with_precision(shapeIn->getAngleydzn())) ;
		pars.push_back("Dydzp=" + to_string_with_precision(shapeIn->getDydzp())) ;
		pars.push_back("Dxdyndzp=" + to_string_with_precision(shapeIn->getDxdyndzp())) ;
		pars.push_back("Dxdypdzp=" + to_string_with_precision(shapeIn->getDxdypdzp())) ;
		pars.push_back("Angleydzp=" + to_string_with_precision(shapeIn->getAngleydzp())) ;
	}
  else if (shapeType == "Trd") {
		const GeoTrd* shapeIn = dynamic_cast<const GeoTrd*>(shape);
		pars.push_back("XHalfLength1=" + to_string_with_precision(shapeIn->getXHalfLength1())) ;
		pars.push_back("XHalfLength2=" + to_string_with_precision(shapeIn->getXHalfLength2())) ;
		pars.push_back("YHalfLength1=" + to_string_with_precision(shapeIn->getYHalfLength1())) ;
		pars.push_back("YHalfLength2=" + to_string_with_precision(shapeIn->getYHalfLength2())) ;
		pars.push_back("ZHalfLength=" + to_string_with_precision(shapeIn->getZHalfLength())) ;
	}
  else if (shapeType == "Tube") {
		const GeoTube* tube = dynamic_cast<const GeoTube*>(shape);
		pars.push_back("RMin=" + to_string_with_precision(tube->getRMin())) ;
		pars.push_back("RMax=" + to_string_with_precision(tube->getRMax())) ;
		pars.push_back("ZHalfLength=" + to_string_with_precision(tube->getZHalfLength())) ;
	}
  else if (shapeType == "Tubs") {
		const GeoTubs* shapeIn = dynamic_cast<const GeoTubs*>(shape);
		pars.push_back("RMin=" + to_string_with_precision(shapeIn->getRMin())) ;
		pars.push_back("RMax=" + to_string_with_precision(shapeIn->getRMax())) ;
		pars.push_back("ZHalfLength=" + to_string_with_precision(shapeIn->getZHalfLength())) ;
		pars.push_back("SPhi=" + to_string_with_precision(shapeIn->getSPhi())) ;
		pars.push_back("DPhi=" + to_string_with_precision(shapeIn->getDPhi())) ;
	}
  else if (shapeType == "TessellatedSolid") {
		const GeoTessellatedSolid* shapeIn = dynamic_cast<const GeoTessellatedSolid*>(shape);
		// get number of facets
		const size_t nFacets = shapeIn->getNumberOfFacets();
		pars.push_back("nFacets=" + std::to_string(nFacets));//size_t INT
		// loop over the facets
		for (size_t i=0; i<nFacets; ++i) {
			GeoFacet* facet = shapeIn->getFacet(i);
			// get GeoFacet actual implementation
			if (dynamic_cast<GeoTriangularFacet*>(facet))        pars.push_back("TRI");
			else if (dynamic_cast<GeoQuadrangularFacet*>(facet)) pars.push_back("QUAD");
			// get vertex type (ABSOLUTE/RELATIVE)
			GeoFacet::GeoFacetVertexType facetVertexType = facet->getVertexType();
			if (facetVertexType == GeoFacet::ABSOLUTE) pars.push_back("vT=ABSOLUTE");
			if (facetVertexType == GeoFacet::RELATIVE) pars.push_back("vT=RELATIVE");
			// get number of vertices and loop over them
			const size_t nVertices = facet->getNumberOfVertices();
			pars.push_back("nV=" + std::to_string(nVertices));//size_t INT
			for (size_t i=0; i<nVertices; ++i) {
				GeoFacetVertex facetVertex = facet->getVertex(i);
				pars.push_back("xV=" + to_string_with_precision( facetVertex[0] ));
				pars.push_back("yV=" + to_string_with_precision( facetVertex[1] ));
				pars.push_back("zV=" + to_string_with_precision( facetVertex[2] ));
			}
		}
	}
	else if (shapeType == "Intersection") {
		const GeoShapeIntersection* shapeIn = dynamic_cast<const GeoShapeIntersection*>(shape);
		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		const unsigned int shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		const unsigned int shapeIdB = storeShape(shapeOpB);
		pars.push_back("opA=" + std::to_string( shapeIdA )) ; //INT
		pars.push_back("opB=" + std::to_string( shapeIdB )) ; //INT
	}
	else if (shapeType == "Shift") {
		const GeoShapeShift* shapeIn = dynamic_cast<const GeoShapeShift*>(shape);

		// get the referenced Shape used in the 'shift' operation, store it in the DB
		const GeoShape* shapeOp = shapeIn->getOp();
		const unsigned int shapeId = storeShape(shapeOp);

		// get the Transformation, store it in the DB
		GeoTransform* transf = new GeoTransform( shapeIn->getX() );
		const unsigned int trId = storeTranform(transf);

		pars.push_back("A=" + std::to_string( shapeId )); //INT
		pars.push_back("X=" + std::to_string( trId ));    //INT
	}
	else if (shapeType == "Subtraction") {
		const GeoShapeSubtraction* shapeIn = dynamic_cast<const GeoShapeSubtraction*>(shape);
		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		const unsigned int shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		const unsigned int shapeIdB = storeShape(shapeOpB);
		pars.push_back("opA=" + std::to_string( shapeIdA )); //INT
		pars.push_back("opB=" + std::to_string( shapeIdB )); //INT
	}
	else if (shapeType == "Union") {
		const GeoShapeUnion* shapeIn = dynamic_cast<const GeoShapeUnion*>(shape);

		// get the referenced Shape used in the 'union' operation, store it in the DB
		const GeoShape* shapeOpA = shapeIn->getOpA();
		unsigned int shapeIdA = storeShape(shapeOpA);
		const GeoShape* shapeOpB = shapeIn->getOpB();
		unsigned int shapeIdB = storeShape(shapeOpB);

		pars.push_back("opA=" + std::to_string( shapeIdA )) ; //INT
        pars.push_back("opB=" + std::to_string( shapeIdB )) ; //INT
	}
	else if (shapeType=="GenericTrap") {
	  const GeoGenericTrap * shapeIn = dynamic_cast<const GeoGenericTrap*>(shape);
	  pars.push_back("ZHalfLength=" + to_string_with_precision(shapeIn->getZHalfLength()));
	  pars.push_back("NVertices="   + to_string_with_precision(shapeIn->getVertices().size()));
	  for (int i=0; i<shapeIn->getVertices().size(); ++i) {
	    pars.push_back("X=" + to_string_with_precision(shapeIn->getVertices()[i](0)));
	    pars.push_back("Y=" + to_string_with_precision(shapeIn->getVertices()[i](1)));
	  }
	}
	else if (shapeType=="UnidentifiedShape") {
	  const GeoUnidentifiedShape *shapeIn=dynamic_cast<const GeoUnidentifiedShape *> (shape);
	  pars.push_back("name="+shapeIn->name());
	  pars.push_back("asciiData="+shapeIn->asciiData());
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
    std::cout << "\n\tWARNING!!! - Shape '" << shapeType << "' needs to be persistified!!\n\n";
    printStdVectorStrings(m_objectsNotPersistified);
  }
                     

  shapePars = joinVectorStrings(pars,";");

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
	// init variables
	m_len = 0;
	m_len_nChild = 0;
	m_rootVolumeFound = false;
	m_unconnectedTree = false;

	// get a handle to the database and check it
	m_dbManager = &db;
	if (m_dbManager->checkIsDBOpen()) { 
//    if(m_debug) std::cout << "OK! Database is open!" << std::endl;
	}
	else {
		//qDebug() << "Database is not open!";
	}
  // initialize the database (create tables, ...)
  m_dbManager->initDB();
  // build caches
  m_dbManager->loadGeoNodeTypesAndBuildCache();
  // get the database file path
  m_dbpath = m_dbManager->getDBFilePath();

	// get DB metadata
	m_memMap_Tables = m_dbManager->getAll_NodeTypesTableIDs();
}

WriteGeoModel::~WriteGeoModel()
{
	// nothing to do so far...
}


void WriteGeoModel::showMemoryMap()
{
  std::unordered_map<std::string, unsigned int>::const_iterator it = m_memMap.begin();
	while (it != m_memMap.end()) {
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}
}



  unsigned int WriteGeoModel::storeObj(const GeoMaterial* pointer, const std::string &name, const double &density, const std::string &elements)
{
  std::string address = getAddressStringFromPointer( pointer );
	unsigned int materialId;

	if (! isAddressStored(address)) {
		materialId = addMaterial(name, density, elements);
		storeAddress( address, materialId );
	}
	else {
		materialId = getStoredIdFromAddress(address);
	}
	return materialId;
}


  unsigned int WriteGeoModel::storeObj(const GeoElement* pointer, const std::string &name, const std::string &symbol, const double &elZ, const double &elA)
{
  std::string address = getAddressStringFromPointer( pointer );
	unsigned int elementId;

	if (! isAddressStored(address)) {
    elementId = addElement(name, symbol, elZ, elA);
		storeAddress( address, elementId );
	}
	else {
		elementId = getStoredIdFromAddress(address);
	}
	return elementId;
}


  unsigned int WriteGeoModel::storeObj(const GeoShape* pointer, const std::string &name, const std::string &parameters)
{
  std::string address = getAddressStringFromPointer( pointer );

	unsigned int shapeId;
	if (! isAddressStored(address)) {
		shapeId = addShape(name, parameters);
		storeAddress( address, shapeId);
	}
	else {
		shapeId = getStoredIdFromAddress(address);
	}
	return shapeId;
}

unsigned int WriteGeoModel::storeObj(const GeoLogVol* pointer, const std::string &name, const unsigned int &shapeId, const unsigned int &materialId)
{
  std::string address = getAddressStringFromPointer( pointer );

	unsigned int logvolId;
	if (! isAddressStored(address)) {
		logvolId = addLogVol(name, shapeId, materialId);
		storeAddress( address, logvolId );
	}
	else {
		logvolId = getStoredIdFromAddress(address);
	}
	return logvolId;
}


unsigned int WriteGeoModel::storeObj(const GeoPhysVol* pointer, const unsigned int &logvolId, const unsigned int parentId, const bool isRootVolume)
{
  std::string address = getAddressStringFromPointer( pointer );

	unsigned int physvolId;
	if (! isAddressStored(address)) {
    physvolId = addPhysVol(logvolId, parentId, isRootVolume); // FIXME: remove parentInfo
    
		storeAddress( address, physvolId );
	}
	else {
		physvolId = getStoredIdFromAddress(address);
	}
	return physvolId;
}

unsigned int WriteGeoModel::storeObj(const GeoFullPhysVol* pointer, const unsigned int &logvolId, const unsigned int parentId, const bool isRootVolume)
{
  std::string address = getAddressStringFromPointer( pointer );

	unsigned int physvolId;
	if (! isAddressStored(address)) {
    physvolId = addFullPhysVol(logvolId, parentId, isRootVolume); // FIXME: remove parent info!

		storeAddress( address, physvolId );
	}
	else {
		physvolId = getStoredIdFromAddress(address);
	}
	return physvolId;
}

  unsigned int WriteGeoModel::storeObj(const GeoSerialDenominator* pointer, const std::string &baseName)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id;

	if (! isAddressStored(address)) {
		id = addSerialDenominator(baseName);
		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
	return id;
}


  unsigned int WriteGeoModel::storeObj(const GeoSerialTransformer* pointer, const unsigned int &functionId, const unsigned int &volId, const std::string &volType, const unsigned int &copies)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id = 0;

	if (! isAddressStored(address)) {
		id = addSerialTransformer(functionId, volId, volType, copies);

		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
  
  if (!id) {
    std::cout << "ERROR while storing a GeoSerialTransformer! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return id;
}

  unsigned int WriteGeoModel::storeObj(const GeoXF::Function* pointer, const std::string &expression)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id = 0;

	if (! isAddressStored(address)) {
		id = addFunction(expression);

		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
  
  if (!id) {
    std::cout << "ERROR while storing a Function! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return id;
}

unsigned int WriteGeoModel::storeObj(const GeoTransform* pointer, const std::vector<double> &parameters)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id = 0;

	if (! isAddressStored(address)) {
		id = addTransform( parameters );
		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
  if (!id) {
    std::cout << "ERROR while storing a GeoTransform! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return id;
}

unsigned int WriteGeoModel::storeObj(const GeoAlignableTransform* pointer, const std::vector<double> &parameters)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id = 0;

	if (! isAddressStored(address)) {
		id = addAlignableTransform( parameters );
		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
  if (!id) {
    std::cout << "ERROR while storing a GeoAlignableTransform! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return id;
}


  unsigned int WriteGeoModel::storeObj(const GeoNameTag* pointer, const std::string &name)
{
  const std::string address = getAddressStringFromPointer( pointer );
	unsigned int id = 0;

	if (! isAddressStored(address)) {
		id = addNameTag(name);
		storeAddress( address, id );
	}
	else {
		id = getStoredIdFromAddress(address);
	}
  if (!id) {
    std::cout << "ERROR while storing a GeoNameTag! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
	return id;
}



  void WriteGeoModel::storeChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &childId, const unsigned int &parentCopyN, const unsigned int &childPos, const std::string &childType, const unsigned int &childCopyN)
{
	addChildPosition(parentId, parentType, childId, parentCopyN, childPos, childType, childCopyN); // FIXME: change the positions of the parameters to a more logical order, like: parentID, parentType, parentCopyN, childPos, ChildType, childId, childCopyN
	return;
}


  unsigned int WriteGeoModel::addRecord(std::vector<std::vector<std::string>>* container, const std::vector<std::string> values) const
  {
	container->push_back(values);
	unsigned int idx = container->size(); // index of pushed element = size after pushing, to match ID starting at 1 in the DB
	return idx;
}


  unsigned int WriteGeoModel::addMaterial(const std::string &name, const double &density, const std::string &elements)
{
  std::vector<std::vector<std::string>>* container = &m_materials;
  std::vector<std::string> values;
  values.push_back( name );
  values.push_back( to_string_with_precision(density) );
  values.push_back( elements );
	return addRecord(container, values);
}




unsigned int WriteGeoModel::addElement(const std::string &name, const std::string &symbol, const double &elZ, const double &elA)
{
  std::vector<std::vector<std::string>>* container = &m_elements;
  std::vector<std::string> values;
  values.insert(values.begin(), { name, symbol, to_string_with_precision(elZ), to_string_with_precision(elA)} );
	return addRecord(container, values);
}


unsigned int WriteGeoModel::addNameTag(const std::string &name)
{
	std::vector<std::vector<std::string>>* container = &m_nameTags;
	std::vector<std::string> values;
	values.push_back(name);
	return addRecord(container, values);
}

unsigned int WriteGeoModel::addSerialDenominator(const std::string &baseName)
{
	std::vector<std::vector<std::string>>* container = &m_serialDenominators;
	std::vector<std::string> values;
	values.push_back(baseName);
	return addRecord(container, values);
}


unsigned int WriteGeoModel::addFunction(const std::string &expression)
{
	std::vector<std::vector<std::string>>* container = &m_functions;
	std::vector<std::string> values;
	values.push_back(expression);
	return addRecord(container, values);
}


unsigned int WriteGeoModel::addAlignableTransform(const std::vector<double> &params)
{
	std::vector<std::vector<std::string>>* container = &m_alignableTransforms;
	std::vector<std::string> values;
  for(const double& par : params) {
    values.push_back( to_string_with_precision(par) );
	}
	return addRecord(container, values);
}



unsigned int WriteGeoModel::addTransform(const std::vector<double> &params)
{
	std::vector<std::vector<std::string>>* container = &m_transforms;
	std::vector<std::string> values;
  for(const double& par : params) {
		values.push_back( to_string_with_precision(par) );
	}
	return addRecord(container, values);
}

  unsigned int WriteGeoModel::getIdFromNodeType( const std::string &nodeType )
{
  std::unordered_map<std::string, unsigned int>::iterator it = m_memMap_Tables.find(nodeType);
  if ( it != m_memMap_Tables.end() ) { // item found
      return m_memMap_Tables.at(nodeType);
  }
  return 0; // item not found
}

  unsigned int WriteGeoModel::addSerialTransformer(const unsigned int &funcId, const unsigned int &physvolId, const std::string volType, const unsigned int &copies)
{
	std::vector<std::vector<std::string>>* container = &m_serialTransformers;
  const unsigned int volTypeID = getIdFromNodeType(volType);

  std::vector<std::string> values;
  values.insert( values.begin(),  {std::to_string(funcId), std::to_string(physvolId), std::to_string(volTypeID), std::to_string(copies)} ); //INT

	return addRecord(container, values);
}

  unsigned int WriteGeoModel::addShape(const std::string &type, const std::string &parameters)
{
  std::vector<std::vector<std::string>>* container = &m_shapes;
  std::vector<std::string> values;
  values.push_back(type);
  values.push_back(parameters);
	return addRecord(container, values);
}


unsigned int WriteGeoModel::addPhysVol(const unsigned int &logVolId, const unsigned int &parentPhysVolId, const bool &isRootVolume)
{
	std::vector<std::vector<std::string>>* container = &m_physVols;
  std::vector<std::string> values;
    values.push_back( std::to_string(logVolId) ); //INT
	unsigned int idx = addRecord(container, values);
	if (isRootVolume) {
		std::vector<std::string> rootValues;
    rootValues.insert(rootValues.begin(), { std::to_string(idx), "GeoPhysVol"} ); //INT
		m_rootVolume = rootValues;
	}
	return idx;
}


unsigned int WriteGeoModel::addFullPhysVol(const unsigned int &logVolId, const unsigned int &parentPhysVolId, const bool &isRootVolume)
{
	std::vector<std::vector<std::string>>* container = &m_fullPhysVols;
	std::vector<std::string> values;
  values.push_back( std::to_string(logVolId) ); //INT
	unsigned int idx = addRecord(container, values);
	if (isRootVolume) {
		std::vector<std::string> rootValues;
//    rootValues << QString::number(idx) << "GeoFullPhysVol";
    rootValues.insert(rootValues.begin(), { std::to_string(idx), "GeoPhysVol"} ); //INT
		m_rootVolume = rootValues;
	}
	return idx;
}

unsigned int WriteGeoModel::addLogVol(const std::string &name, const unsigned int &shapeId, const unsigned int &materialId)
{
	std::vector<std::vector<std::string>>* container = &m_logVols;
  std::vector<std::string> values;
  values.insert( values.begin(), {name, std::to_string(shapeId), std::to_string(materialId)} ); //INT
	return addRecord(container, values);
}


  void WriteGeoModel::addChildPosition(const unsigned int &parentId, const std::string &parentType, const unsigned int &childId, const unsigned int &parentCopyN, const unsigned int &childPos, const std::string &childType, const unsigned int &childCopyN)
{
	std::vector<std::vector<std::string>>* container = &m_childrenPositions;
	const unsigned int parentTableID = getIdFromNodeType(parentType);
	const unsigned int childTableID = getIdFromNodeType(childType);

	std::vector<std::string> values;
//  values << parentId.toString() << parentTableID <<  QString::number(parentCopyN) << QString::number(childPos) << childTableID << childId.toString() << QString::number(childCopyN);
  values.insert(values.begin(), { std::to_string(parentId), std::to_string(parentTableID), std::to_string(parentCopyN), std::to_string(childPos), std::to_string(childTableID), std::to_string(childId), std::to_string(childCopyN)} ); //INT
	addRecord(container, values);
	return;
}


/*
 * The 'publisher' parameter is optional, by default it is set to 'nullptr' in the header.
 */
void WriteGeoModel::saveToDB( GeoPublisher* publisher ) 
{
    std::vector<GeoPublisher*> vec;
    if( publisher )
        vec.push_back(publisher);
    saveToDB(vec);
}
void WriteGeoModel::saveToDB( std::vector<GeoPublisher*>& publishers )
{
    std::cout << "Saving the GeoModel tree to file: '" << m_dbpath << "'" << std::endl;

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

    if(publishers.size()) {
            std::cout << "\nINFO: A pointer to a GeoPublisher instance has been provided, "
                << "so we dump the published list of FullPhysVol and AlignableTransforms\n" 
                << std::endl;
        for(GeoPublisher* publisher : publishers) {
            storePublishedNodes(publisher);
        }
	}

	if ( !m_objectsNotPersistified.empty() ) {
        std::cout << "\n\tWARNING!! There are shapes/nodes which need to be persistified! --> ";
        printStdVectorStrings(m_objectsNotPersistified);
        std::cout << "\n\n";
	}

	return;
}


void WriteGeoModel::storePublishedNodes(GeoPublisher* store)
{
    // loop over the published AlignableTransform nodes
    std::map<GeoAlignableTransform*, std::any> mapAXF = store->getPublishedAXF();
    storeRecordPublishedNodes<std::map<GeoAlignableTransform*, std::any>>(mapAXF, &m_publishedAlignableTransforms_String);   

    // loop over the published GeoVFullPhysVol nodes
    std::map<GeoVFullPhysVol*, std::any> mapFPV = store->getPublishedFPV();
    storeRecordPublishedNodes<std::map<GeoVFullPhysVol*, std::any>>(mapFPV, &m_publishedFullPhysVols_String);   

    // save the list of matching published nodes to the DB
    std::string storeName = store->getName();
    if (mapAXF.size() > 0) {
        m_dbManager->addListOfPublishedAlignableTransforms(m_publishedAlignableTransforms_String, storeName); 
    } else {
        std::cout << "\nWARNING! A pointer to a GeoPublisher was provided, but no GeoAlignableTransform nodes have been published. Please, check if that was intended. (If in doubt, please ask to 'geomodel-developers@cern.ch')\n" << std::endl;
    }
    if (mapFPV.size() > 0) {
        m_dbManager->addListOfPublishedFullPhysVols(m_publishedFullPhysVols_String, storeName);
    } else {
        std::cout << "\nWARNING! A pointer to a GeoPublisher was provided, but no GeoFullPhysVol nodes have been published. Please, check if that was intended. (If in doubt, please ask to 'geomodel-developers@cern.ch')\n" << std::endl;
    }

    // clear the caches
    m_publishedAlignableTransforms_String.clear();
    m_publishedFullPhysVols_String.clear();
}

template <typename TT> void WriteGeoModel::storeRecordPublishedNodes(const TT storeMap, std::vector<std::vector<std::string>>* cachePublishedNodes   )
{
    // NOTE: We store all keys as strings, independently of their original format. 
    //       However, we store the original format as well, 
    //       so we will be able to convert the keys to their original format when clients will read them back.
    //
    for( const auto& [vol, key ] : storeMap ) 
    {
        auto& keyType = key.type();

        // get key type and convert to std::string to store into the cache
        std::string keyStr;
        std::string keyTypeStr;
        if ( typeid(std::string) == keyType ) {
            keyTypeStr = "string";
            keyStr = std::any_cast<std::string>(key);
        }
        else if ( typeid(int) == keyType ) {
            keyTypeStr = "int";
            keyStr = std::to_string( std::any_cast<int>(key) );
        }
        else if ( typeid(unsigned) == keyType ) {
            keyTypeStr = "uint";
            keyStr = std::to_string( std::any_cast<unsigned>(key) );
        }
        else {
            std::cout << "ERROR! The type of the key used to publish FPV and AXF nodes is not 'std::string', nor 'int', nor 'unsigned int'. Format not supported, at the moment..\n"
                      << "If in doubt, please ask to 'geomodel-developers@cern.ch'. Exiting...\n\n";
            exit(EXIT_FAILURE);
        }

        // check if address is stored already; and get the ID associated with it. 
        // NOTE: All of the addresses should be stored already, at this stage. 
        //       If not, there's a serious bug!
        unsigned int volID = 0;
        std::string volStr = getAddressStringFromPointer( vol );
        if( isAddressStored(volStr) ) {
            volID = getStoredIdFromAddress(volStr);
        } else {
            std::cout << "ERROR!!! Address of node is not stored, but it should! Ask 'geomodel-developers@cern.ch'. Exiting...\n\n";
            exit(EXIT_FAILURE);
        }

        // debug msg
        //std::cout << vol << "::" << keyStr << " [" << keyTypeStr << "] --> " << volID << std::endl;

        // prepare the vector containing the pieces of information to be stored in the DB table
        std::vector<std::string> values;
        values.push_back(keyStr);
        values.push_back(std::to_string(volID));
        values.push_back(keyTypeStr); // TODO: store the key type in a metadata table, not in the records' table; so it can be stored once only.

        unsigned int recordID = addRecord(cachePublishedNodes, values);
        //std::cout << "Pushed record: " << recordID << std::endl; // debug msg
    }
}

  void WriteGeoModel::storeAddress(const std::string &address, const unsigned int &id)
{
  m_memMap.insert( std::pair<std::string,unsigned int>(address, id) );
}

  bool WriteGeoModel::isAddressStored(const std::string &address)
{
	//showMemoryMap(); // only for Debug
  std::unordered_map<std::string, unsigned int>::iterator it = m_memMap.find(address);
  return ( it != m_memMap.end() );
}


  unsigned int WriteGeoModel::getStoredIdFromAddress(const std::string &address)
{
	return m_memMap.at(address);
}

// get pointer string
  std::string WriteGeoModel::getAddressStringFromPointer(const GeoMaterial* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
std::string WriteGeoModel::getAddressStringFromPointer(const GeoElement* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
std::string WriteGeoModel::getAddressStringFromPointer(const GeoShape* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
std::string WriteGeoModel::getAddressStringFromPointer(const GeoLogVol* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
std::string WriteGeoModel::getAddressStringFromPointer(const GeoPhysVol* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
std::string WriteGeoModel::getAddressStringFromPointer(const GeoVPhysVol* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
// get pointer string
std::string WriteGeoModel::getAddressStringFromPointer(const GeoSerialDenominator* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
std::string WriteGeoModel::getAddressStringFromPointer(const GeoSerialTransformer* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}
std::string WriteGeoModel::getAddressStringFromPointer(const GeoXF::Function* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

std::string WriteGeoModel::getAddressStringFromPointer(const GeoTransform* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

std::string WriteGeoModel::getAddressStringFromPointer(const GeoNameTag* pointer)
{
	std::ostringstream oss;
	oss << pointer;
	return getQStringFromOss(oss);
}

std::string WriteGeoModel::getQStringFromOss(std::ostringstream &oss)
{
	std::string addr = oss.str();
	return addr;
}


} /* namespace GeoModelIO */

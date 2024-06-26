/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

/*
 * ReadGeoModel.h
 *
 * Created on: May 20, 2016
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch>
 *
 * major updates:
 * - 2019 Feb, R.M.Bianchi
 * - 2020 May, R.M.Bianchi - Added parallel read
 * - 2020 Aug, R.M.Bianchi - Added support for reading back published nodes
 * - 2021 Aug, R.M.Bianchi <riccardo.maria.bianchi@cern.ch> - Added support
 * GeoIdentifierTag and GeoSerialIdentifier nodes
 * - Jun 2022, R.M.Bianchi <riccardo.maria.bianchi@cern.ch>
 *              Fixed the duplication of VPhysVol instances due to a wrong key
 * used for caching volumes that were built already The copyNumber was wrongly
 * used together with tableID and volID For details, see:
 * https://gitlab.cern.ch/GeoModelDev/GeoModel/-/issues/39
 * - Jan 2023, R.M.Bianchi <riccardo.maria.bianchi@cern.ch>
 *             Added getters to get number of GeoModel nodes
 *             restored from the * .db file
 * - 2023 Jan, R.M.Bianchi <riccardo.maria.bianchi@cern.ch>
 *   Added method to get records out of custom tables from client code.
 * - Feb 2023, R.M.Bianchi <riccardo.maria.bianchi@cern.ch>
 *             Added 'setLoglevel' method, to steer output messages
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
namespace GeoModelIO {
class ReadGeoModel;
}
// the 'Persistifier' class name is used by GeoModel boolean shapes
// to grant access to friend classes
typedef GeoModelIO::ReadGeoModel Persistifier;
// ****************************************************************
// ****************************************************************

// GeoModel includes
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelDBManager/definitions.h"

#include "GeoModelKernel/GeoXF.h"

// C++ includes
#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
class GeoSerialIdentifier;
class GeoIdentifierTag;
class GeoAlignableTransform;
class GeoTransform;
class GeoSerialTransformer;
class GeoGraphNode;
class GeoShapeSubtraction;
class GeoBox;

class BuildGeoShapes_Box;
class BuildGeoShapes_Tube;
class BuildGeoShapes_Cons;
class BuildGeoShapes_Para;
class BuildGeoShapes_Trap;
class BuildGeoShapes_Trd;
class BuildGeoShapes_Tubs;
class BuildGeoShapes_Torus;
class BuildGeoShapes_TwistedTrap;
class BuildGeoShapes_Pcon;
class BuildGeoShapes_Pgon;
class BuildGeoShapes_SimplePolygonBrep;
class BuildGeoShapes_UnidentifiedShape;

// type definitions
typedef const GeoXF::Function& TRANSFUNCTION;
// containers for boolean shapes' information
typedef std::tuple<unsigned int /*shape ID*/, GeoShape*,
                   unsigned int /*opA ID*/, unsigned int /*opB ID*/>
    tuple_shapes_boolean_info;
typedef std::tuple<std::string /*shape Type*/, unsigned /*shape ID*/, GeoShape*,
                   std::string /*opA type*/, unsigned /*opA ID*/, std::string /*opB type*/, unsigned /*opB ID*/>
    tuple_boolean_shapes_operands;

typedef std::vector<tuple_shapes_boolean_info> type_shapes_boolean_info;
typedef std::vector<tuple_boolean_shapes_operands> boolean_shapes_operands_info;



namespace GeoModelIO {

class ReadGeoModel {
   public:
    ReadGeoModel(GMDBManager* db, unsigned long* progress = nullptr);
    virtual ~ReadGeoModel();

    const GeoVPhysVol* buildGeoModel();

    /// Set the 'loglevel', that is the level of output messages.
    /// The loglevel is set to 0 by default, but it can be set
    /// to a larger value.
    /// Loglevel:
    /// - 0 : Default
    /// - 1 : Verbose
    /// - 2 : Debug
    void setLogLevel(unsigned loglevel) { m_loglevel = loglevel; };

    // NB, this template method needs only the "publisher name" to be specified
    // (i.e. the last suffix), since the first part of the table name get added
    // automatically according to the data type it is templated on
    template <typename T, class N>
    std::map<T, N> getPublishedNodes(
        std::string publisherName = "" /*optional variable*/,
                                     bool doCheckTable = false);

    void printDBTable(const std::string& tableName) {
        m_dbManager->printAllRecords(tableName);
    }
    void printAllDBTables() { m_dbManager->printAllDBTables(); }

    void loadDB();
    
    unsigned long getNLogVols() { return m_logVols.size(); };
    unsigned long getNPhysVols() { return m_physVols.size(); };
    unsigned long getNFullPhysVols() { return m_fullPhysVols.size(); };
    unsigned long getNMaterials() { return m_materials.size(); };
    unsigned long getNElements() { return m_elements.size(); };
    unsigned long getNTransforms() { return m_transforms.size(); };
    unsigned long getNAlignableTransforms() {
        return m_alignableTransforms.size();
    };
    unsigned long getNSerialDenominators() {
        return m_serialDenominators.size();
    };
    unsigned long getNSerialIdentifiers() {
        return m_serialIdentifiers.size();
    };
    unsigned long getNIdentifierTags() { return m_identifierTags.size(); };
    unsigned long getNSerialTransformers() {
        return m_serialTransformers.size();
    };
    unsigned long getNFunctions() { return m_functions.size(); };
    unsigned long getNNameTags() { return m_nameTags.size(); };
    unsigned long getNChildrenConnections() { return m_allchildren.size(); };
    std::vector<std::vector<std::string>> getTableFromTableName_String(
        const std::string_view tableName) {
        return m_dbManager->getTableRecords_String(tableName);
    };
    unsigned long getNShapes() { return m_shapes.size(); }; // TODO: to be removed later
    unsigned long getNShapes_Box() { return m_shapes_Box.size(); };
    unsigned long getNShapes_Cons() { return m_shapes_Cons.size(); };
    unsigned long getNShapes_Para() { return m_shapes_Para.size(); };
    unsigned long getNShapes_Pcon() { return m_shapes_Pcon.size(); };
    unsigned long getNShapes_Pgon() { return m_shapes_Pgon.size(); };
    unsigned long getNShapes_SimplePolygonBrep() { return m_shapes_SimplePolygonBrep.size(); };
    unsigned long getNShapes_Trap() { return m_shapes_Trap.size(); };
    unsigned long getNShapes_Trd() { return m_shapes_Trd.size(); };
    unsigned long getNShapes_Tube() { return m_shapes_Tube.size(); };
    unsigned long getNShapes_Tubs() { return m_shapes_Tubs.size(); };
    unsigned long getNShapes_Torus() { return m_shapes_Torus.size(); };
    unsigned long getNShapes_TwistedTrap() { return m_shapes_TwistedTrap.size(); };
    unsigned long getNShapes_UnidentifiedShape() { return m_shapes_UnidentifiedShape.size(); };
    unsigned long getNShapes_Shift() { return m_shapes_Shift.size(); };
    unsigned long getNShapes_Union() { return m_shapes_Union.size(); };
    unsigned long getNShapes_Intersection() { return m_shapes_Intersection.size(); };
    unsigned long getNShapes_Subtraction() { return m_shapes_Subtraction.size(); };

   private:
    void buildAllShapes(); // TODO: OLD METHOD, TO BE REMOVED WHEN READY
    void buildAllShapes_Box();
    void buildAllShapes_Tube();
    void buildAllShapes_Cons();
    void buildAllShapes_Para();
    void buildAllShapes_Trap();
    void buildAllShapes_Trd();
    void buildAllShapes_Tubs();
    void buildAllShapes_Torus();
    void buildAllShapes_TwistedTrap();
    void buildAllShapes_Pcon();
    void buildAllShapes_Pgon();
    void buildAllShapes_SimplePolygonBrep();
    void buildAllShapes_UnidentifiedShape();
    void buildAllShapes_Operators();


    void buildAllElements();
    void buildAllMaterials();
    void buildAllLogVols();
    void buildAllPhysVols();
    void buildAllFullPhysVols();
    //  void buildAllFunctions(); // FIXME:
    void buildAllTransforms();
    void buildAllAlignableTransforms();
    void buildAllSerialDenominators();
    void buildAllSerialIdentifiers();
    void buildAllIdentifierTags();
    void buildAllSerialTransformers();
    void buildAllNameTags();

    GeoVPhysVol* buildGeoModelPrivate();

    GeoBox* buildDummyShape();

    // void loopOverAllChildrenInBunches_String(); // OLD
    void loopOverAllChildrenInBunches_VecVecData();
    // void loopOverAllChildrenRecords(
    //     std::vector<std::vector<std::string>> records);
    void loopOverAllChildrenRecords(DBRowsList records);
    void processParentChild(const std::vector<std::string>& parentchild);
    void processParentChild(const DBRowEntry& parentchild);

    GeoVPhysVol* getRootVolume();

    GeoVPhysVol* buildVPhysVolInstance(const unsigned id,
                                       const unsigned tableId,
                                       const unsigned copyNumber);
    GeoVPhysVol* buildVPhysVol(const unsigned id,
                               const unsigned tableId,
                               unsigned logVol_ID = 0);

    GeoLogVol* buildLogVol(const unsigned id);
    
    GeoShape* buildShape(const unsigned id,
                         type_shapes_boolean_info* shapes_info_sub);
    GeoShape *buildShapeOperator(const std::string_view shapeType, const DBRowEntry row,
                                 boolean_shapes_operands_info *shapes_info_sub);

    GeoMaterial* buildMaterial(const unsigned id);
    GeoElement* buildElement(const unsigned id);
    GeoAlignableTransform* buildAlignableTransform(const unsigned id);
    GeoTransform* buildTransform(const unsigned int id);
    GeoSerialTransformer* buildSerialTransformer(const unsigned id);
    TRANSFUNCTION buildFunction(const unsigned id);

    void checkNodePtr(GeoGraphNode* nodePtr, std::string varName = "",
                      std::string funcName = "",
                      std::string funcSignature =
                          "");  // TODO: to be moved to an utility class
    void volAddHelper(GeoVPhysVol* vol, GeoGraphNode* volChild);

    // methods for shapes
    std::string getShapeType(const unsigned shapeId);
    bool isShapeOperator(const unsigned shapeId);
    bool isShapeOperator(const std::string_view type);
    bool isShapeBoolean(const unsigned shapeId);
    bool isShapeBoolean(const std::string_view type);
    void createBooleanShapeOperands(type_shapes_boolean_info* shapes_info_sub);
    void createBooleanShapeOperands(boolean_shapes_operands_info* shapes_info_sub);
    std::pair<unsigned, unsigned> getBooleanShapeOperands(
        const unsigned shape);
    std::tuple<std::string, unsigned int, std::string, unsigned> getBooleanShapeOperands(
        const std::string_view shapeType, const unsigned shapeId);
    GeoShape* addEmptyBooleanShapeForCompletion(
        const unsigned shapeID, type_shapes_boolean_info* shapes_info_sub);
    GeoShape* getBooleanReferencedShape(
        const unsigned shapeID, type_shapes_boolean_info* shapes_info_sub);
    GeoShape* addEmptyBooleanShapeForCompletion(
        const std::string_view shapeType, const unsigned shapeID, boolean_shapes_operands_info* shapes_info_sub);
    GeoShape* getBooleanReferencedShape(
        const std::string_view shapeType, const unsigned shapeID, boolean_shapes_operands_info* shapes_info_sub);

    // caching methods
    // TODO: perhaps we could merge all those 'isBuiltYYY' methods in a single
    // one, with the GeoModel class as a second argument ? (RMB)
    bool isBuiltShape(const unsigned id);
    bool isBuiltShape_Operators_Shift(const unsigned id);
    bool isBuiltShape_Operators_Subtraction(const unsigned id);
    bool isBuiltShape_Operators_Intersection(const unsigned id);
    bool isBuiltShape_Operators_Union(const unsigned id);
    bool isBuiltShape(std::string_view shapeType, const unsigned id);
    void storeBuiltShape(const unsigned, GeoShape* node);
    void storeBuiltShape(const std::string_view type, const unsigned id, GeoShape *nodePtr);
    GeoShape* getBuiltShape(const unsigned shapeId, std::string_view shapeType = "");

    void storeBuiltShapeOperators_Shift(const unsigned, GeoShape* node);
    void storeBuiltShapeOperators_Subtraction(const unsigned, GeoShape* node);
    void storeBuiltShapeOperators_Union(const unsigned, GeoShape* node);
    void storeBuiltShapeOperators_Intersection(const unsigned, GeoShape* node);

    bool isBuiltTransform(const unsigned id);
    void storeBuiltTransform(GeoTransform* node);
    GeoTransform* getBuiltTransform(const unsigned id);

    bool isBuiltAlignableTransform(const unsigned id);
    void storeBuiltAlignableTransform(GeoAlignableTransform* node);
    GeoAlignableTransform* getBuiltAlignableTransform(const unsigned id);

    // void storeVPhysVol(const unsigned int id, const unsigned int tableId,
    // const unsigned int copyNumber, GeoGraphNode* node); GeoGraphNode*
    // getVPhysVol(const unsigned int id, const unsigned int tableId, const
    // unsigned int copyNumber);
    void storeVPhysVol(const unsigned int id, const unsigned int tableId,
                       GeoGraphNode* node);
    GeoGraphNode* getVPhysVol(const unsigned int id,
                              const unsigned int tableId);

    bool isBuiltLog(const unsigned int id);
    void storeBuiltLog(GeoLogVol* nodePtr);
    GeoLogVol* getBuiltLog(const unsigned int id);

    bool isBuiltMaterial(const unsigned int id);
    void storeBuiltMaterial(GeoMaterial* nodePtr);
    GeoMaterial* getBuiltMaterial(const unsigned int id);

    bool isBuiltElement(const unsigned int id);
    void storeBuiltElement(GeoElement* nodePtr);
    GeoElement* getBuiltElement(const unsigned int id);

    bool isBuiltFunction(const unsigned int id);
    // void storeBuiltFunction(const unsigned int id, GeoXF::Function* nodePtr);
    // // TODO: implement this GeoXF::Function* getBuiltFunction(const unsigned
    // int id); // TODO: implement this

    bool isBuiltPhysVol(const unsigned int id);
    void storeBuiltPhysVol(GeoPhysVol* nodePtr);
    GeoPhysVol* getBuiltPhysVol(const unsigned int id);

    bool isBuiltFullPhysVol(const unsigned int id);
    void storeBuiltFullPhysVol(GeoFullPhysVol* nodePtr);
    GeoFullPhysVol* getBuiltFullPhysVol(const unsigned int id);

    bool isBuiltSerialDenominator(const unsigned int id);
    void storeBuiltSerialDenominator(GeoSerialDenominator* nodePtr);
    GeoSerialDenominator* getBuiltSerialDenominator(const unsigned int id);

    bool isBuiltSerialIdentifier(const unsigned int id);
    void storeBuiltSerialIdentifier(GeoSerialIdentifier* nodePtr);
    GeoSerialIdentifier* getBuiltSerialIdentifier(const unsigned int id);

    bool isBuiltIdentifierTag(const unsigned int id);
    void storeBuiltIdentifierTag(GeoIdentifierTag* nodePtr);
    GeoIdentifierTag* getBuiltIdentifierTag(const unsigned int id);

    bool isBuiltNameTag(const unsigned int id);
    void storeBuiltNameTag(GeoNameTag* nodePtr);
    GeoNameTag* getBuiltNameTag(const unsigned int id);

    bool isBuiltSerialTransformer(const unsigned int id);
    void storeBuiltSerialTransformer(GeoSerialTransformer* nodePtr);
    GeoSerialTransformer* getBuiltSerialTransformer(const unsigned int id);

    // Utility functions
    std::vector<std::string> splitString(const std::string& s, char delimiter); // TODO: move to global helpers
    void printStdVectorStrings(
        std::vector<std::string> vec);  // TODO: move it to utility class or move to global helpers

    // void printTransformationValues(QStringList t); // TODO: move to a Qt
    // utility class QList<double> convertQstringListToDouble(QStringList
    // listin); // TODO: move to a Qt utility class std::vector<std::string>
    // toStdVectorStrings(QStringList qlist); // TODO: move to a Qt utility
    // class QStringList toQStringList(std::vector<std::string> vec); // TODO:
    // move to a Qt utility class void checkInputString(QString input); // TODO:
    // move to an utility class

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

    //! builders
    // std::unique_ptr<BuildGeoShapes_Box> m_builderShape_Box;
    BuildGeoShapes_Box* m_builderShape_Box;
    BuildGeoShapes_Tube* m_builderShape_Tube;
    BuildGeoShapes_Cons* m_builderShape_Cons;
    BuildGeoShapes_Para* m_builderShape_Para;
    BuildGeoShapes_Trap* m_builderShape_Trap;
    BuildGeoShapes_Trd* m_builderShape_Trd;
    BuildGeoShapes_Tubs* m_builderShape_Tubs;
    BuildGeoShapes_Torus* m_builderShape_Torus;
    BuildGeoShapes_TwistedTrap* m_builderShape_TwistedTrap;
    BuildGeoShapes_Pcon* m_builderShape_Pcon;
    BuildGeoShapes_Pgon* m_builderShape_Pgon;
    BuildGeoShapes_SimplePolygonBrep* m_builderShape_SimplePolygonBrep;
    BuildGeoShapes_UnidentifiedShape* m_builderShape_UnidentifiedShape;

    //! containers to store the list of GeoModel nodes coming from the DB
    std::vector<std::vector<std::string>> m_physVols;
    std::vector<std::vector<std::string>> m_fullPhysVols;
    std::vector<std::vector<std::string>> m_transforms;
    std::vector<std::vector<std::string>> m_alignableTransforms;
    std::vector<std::vector<std::string>> m_serialDenominators;
    std::vector<std::vector<std::string>> m_serialIdentifiers;
    std::vector<std::vector<std::string>> m_identifierTags;
    std::vector<std::vector<std::string>> m_serialTransformers;
    std::vector<std::vector<std::string>> m_nameTags;
    // std::vector<std::vector<std::string>> m_materials;
    // std::vector<std::vector<std::string>> m_elements;
    std::vector<std::vector<std::string>> m_shapes;

    DBRowsList m_elements;
    DBRowsList m_materials;
    DBRowsList m_materials_Data;
    DBRowsList m_logVols;
    DBRowsList m_allchildren;

    // containers to store shapes' parameters
    DBRowsList m_shapes_Box;
    DBRowsList m_shapes_Tube;
    DBRowsList m_shapes_Cons;
    DBRowsList m_shapes_Para;
    DBRowsList m_shapes_Trap;
    DBRowsList m_shapes_Trd;
    DBRowsList m_shapes_Tubs;
    DBRowsList m_shapes_Torus;
    DBRowsList m_shapes_TwistedTrap;
    DBRowsList m_shapes_Pcon;
    DBRowsList m_shapes_Pgon;
    DBRowsList m_shapes_SimplePolygonBrep;
    DBRowsList m_shapes_UnidentifiedShape;
    
    // containers to store shapes' data, 
    // for those shapes with a variable number of build parameters
    DBRowsList m_shapes_Pcon_data;
    DBRowsList m_shapes_Pgon_data;
    DBRowsList m_shapes_SimplePolygonBrep_data;

    // containers to store shape operators / boolean shapes
    DBRowsList m_shapes_Shift;
    DBRowsList m_shapes_Subtraction;
    DBRowsList m_shapes_Intersection;
    DBRowsList m_shapes_Union;

    // containers to store Functions and their related data
    DBRowsList m_functions;
    std::deque<double> m_funcExprData;


    std::unordered_map<unsigned int, std::string>
        m_tableID_toTableName;  // to look for node's type name starting from a
                                // table ID
    std::unordered_map<std::string, unsigned int>
        m_tableName_toTableID;  // to look for table ID starting from node's
                                // type name

    // std::vector<std::string> m_root_vol_data;
    std::pair<unsigned, unsigned> m_root_vol_data;

    //! memory chaches
    std::vector<GeoPhysVol*> m_memMapPhysVols;
    std::vector<GeoFullPhysVol*> m_memMapFullPhysVols;
    std::vector<GeoTransform*> m_memMapTransforms;
    std::vector<GeoAlignableTransform*> m_memMapAlignableTransforms;
    std::vector<GeoSerialDenominator*> m_memMapSerialDenominators;
    std::vector<GeoSerialIdentifier*> m_memMapSerialIdentifiers;
    std::vector<GeoIdentifierTag*> m_memMapIdentifierTags;
    std::vector<GeoSerialTransformer*> m_memMapSerialTransformers;
    std::vector<GeoNameTag*> m_memMapNameTags;
    std::vector<GeoLogVol*> m_memMapLogVols;
    std::vector<GeoMaterial*> m_memMapMaterials;
    std::vector<GeoElement*> m_memMapElements;
    //  std::vector<GeoXF::Function*> m_memMapFunctions; // FIXME:
    
    // we need keys, because shapes are not built following the ID order
    std::unordered_map<unsigned int, GeoShape*> m_memMapShapes; // OLD DB CACHE
    std::unordered_map<unsigned int, GeoShape*> m_memMapShapes_Shift;  
    std::unordered_map<unsigned int, GeoShape*> m_memMapShapes_Subtraction;  
    std::unordered_map<unsigned int, GeoShape*> m_memMapShapes_Union;  
    std::unordered_map<unsigned int, GeoShape*> m_memMapShapes_Intersection;  

    // we need keys, to keep track of the volume's copyNumber
    std::unordered_map<std::string, GeoGraphNode*> m_memMap;  

    //! container to store unknown shapes
    std::set<std::string> m_unknown_shapes;

    /// Stores the loglevel, the level of output messages
    unsigned m_loglevel;
};

} /* namespace GeoModelIO */

// include the implementation of the class' template functions
#include "ReadGeoModel.tpp"

#endif /* GeoModelRead_ReadGeoModel_H_ */

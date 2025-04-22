/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

//
//    Deal with positional indices (layer number etc.)
//
#ifndef GEO_MODEL_XML_POSITIONINDEX_H
#define GEO_MODEL_XML_POSITIONINDEX_H

/**
 * @brief Handle the position index names, formulae, and values
 *
 * Values are calculated strictly in the order that the index names are given in the xml file.
 * This allows the user freedom to choose the order of evaluation, which matters when he is using
 * the CNL_n values in her formulae.
 */

#include <vector>
#include <map>
#include <string>

class IEvaluator;

class PositionIndex {
public:
    PositionIndex(); 
    ~PositionIndex();

    int level();
    int incrementLevel();
    int decrementLevel();
    
    int copyNo(int level);
    int setCopyNo(int copyNo);

    void addIndex(const std::string& name);
    std::string name(int whichOne);
    std::string formula(const std::string& name);
    void setFormula(const std::string& name, const std::string& formula);

    void indices(std::map<std::string, int> &index, GeoModelTools::IEvaluator &eval);

    void addToLevelMap(std::string name, int level);
    void printLevelMap();
    void setLevelMapName(std::string filename);
    std::string getLevelMapName();

private:
    int m_level;
    std::vector <int> m_copyNo; // Size is always m_level+1
/** @brief We keep a separate vector of the names as a way of preserving the order of evaluation
 */
    std::vector <std::string> m_name;
    std::map <std::string, std::string> m_formula;
    std::map <std::string, int> m_levelMap;
    std::string m_mapName;
};
#endif // POSITIONINDEX_H

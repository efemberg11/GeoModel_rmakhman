/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/StringUtils.h"
#include "GeoModelHelpers/throwExcept.h"
#include <limits>
#include <array>
#include <functional>
#include <iostream>
#include <sstream>
#include <charconv>
#include <algorithm>

namespace GeoStrUtils {

    std::string whiteSpaces(const unsigned int n, const std::string_view space) {
        return chainUp<std::string_view>(n ,
                        [space](const unsigned int)->std::string_view{ return space; }, "");
    }
    std::string replaceExpInString(std::string str, 
                                   const std::string_view exp, 
                                   const std::string_view rep) {
        
        std::size_t exPos = str.find(exp);
        std::size_t expLen = exp.size();
        while(exPos != std::string::npos) {
            str = str.substr(0, exPos) + std::string{rep} + str.substr(exPos + expLen, std::string::npos);
            exPos = str.find(exp);
        }
        return str;
    }
    std::string_view longestCommonString(const std::string_view firstStr,
                                         const std::string_view secondStr){
        if (firstStr.size() < secondStr.size()) return longestCommonString(secondStr, firstStr);
        std::size_t commonPos{0};
        for ( ; commonPos <= secondStr.size(); ++commonPos) {
            std::string_view sub_str = secondStr.substr(0, commonPos);
            if (firstStr.find(sub_str) != 0) break;
        }
        return secondStr.substr(0, commonPos);    
    }


    std::string resolveEnviromentVariables(const std::string_view inStr) {
        std::string str{inStr};

        while (str.find("${") != std::string::npos) {
            std::string varName = str.substr(str.find("${") + 2, str.find("}") - str.find("${") - 2);
            std::string envVar{std::getenv(varName.data()) ? std::getenv(varName.data()) : ""};
            str = replaceExpInString(str, str.substr(str.find("${"), str.find("}") - str.find("${") + 1), envVar);
        }
        return str;
    }

    std::string getEnvVar(const std::string_view key)
    {
        std::string keyStr{key};
        char *val = std::getenv(keyStr.c_str());
        return val == NULL ? std::string{} : std::string(val);
    }

   std::vector<std::string> tokenize(const std::string& str,
                                      std::string_view delimiters) {
                                    
        std::vector<std::string> tokens{};
        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    
        while (std::string::npos != pos || std::string::npos != lastPos) {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }
    std::vector<double> tokenizeDouble(const std::string& the_str,
                                       std::string_view delimiter){
        const std::vector<std::string> strTokens = tokenize(the_str, delimiter);
        std::vector<double> toReturn{};
        std::transform(strTokens.begin(), strTokens.end(), std::back_inserter(toReturn), 
                    [](const std::string& token){
                        return atof(token);
                    });
        return toReturn;
    }
    std::string_view eraseWhiteSpaces(std::string_view str) {
        if (str.empty()) return str;
        size_t begin{0}, end{str.size() -1};
        while (std::isspace(str[begin])){
            ++begin;
        }
        while (end > 0 && std::isspace(str[end])){
            --end;
        }
        return str.substr(begin, end + 1);        
    }
    std::vector<int> tokenizeInt(const std::string& the_str,
                                 std::string_view delimiter) {
        const std::vector<std::string> strTokens = tokenize(the_str, delimiter);
        std::vector<int> toReturn{};
        std::transform(strTokens.begin(), strTokens.end(), std::back_inserter(toReturn), 
                        [](const std::string& token){
                            return atoi(token);
                        });
        return toReturn;
    }
    template <class dType> void convertToNumber(std::string_view str, dType& number) {
        /// Allow for trailing & leading white spaces
        if (str.empty()) {
            number = 0;
            return;   
        }
        if (std::find_if(str.begin(), str.end(), [](const char c){ return std::isspace(c);}) != str.end()) {
            std::string_view spaceFreeStr = eraseWhiteSpaces(str);
            /// To avoid infinite recursion because of string like '42 24' check that white spaces have been indeed removed
            if (spaceFreeStr.size() != str.size()) {
                convertToNumber(spaceFreeStr, number);
                return;
            }
        }
        if (str[0]=='+') {
            convertToNumber(str.substr(1), number);
            return;
        }
	std::istringstream stream(std::string(str.data(), str.data()+str.size()));
	stream >> number;
	if (!stream) THROW_EXCEPTION("convertToNumber() - The string '"<<str<<"'. Contains unallowed chars");
        
    }
    int atoi(std::string_view str) { 
        int result{std::numeric_limits<int>::max()};
       convertToNumber(str, result);       
        return result;
    }

    double atof(std::string_view str) {       
        double result{std::numeric_limits<double>::max()};
        convertToNumber(str, result);
        return result;
    }

    std::string to_string_with_precision(const double a_value, const unsigned n)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
    }

    void printStdVectorStrings(const std::vector<std::string> vec)
    {
        for (const auto &str : vec)
        {
            std::cout << str << " ";
        }
        std::cout << std::endl;
        return;
    }
}

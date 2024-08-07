/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelHelpers/FileUtils.h"
#include "GeoModelHelpers/StringUtils.h"


#include <filesystem>
#include <iostream>
#include <functional>
#include <algorithm>
using namespace GeoStrUtils;

namespace GeoFileUtils {
    bool doesFileExist(const std::string_view path) {
        const std::filesystem::path filePath{resolveEnviromentVariables(path)};
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    }

    bool doesDirectoryExist(const std::string_view path) {
        const std::filesystem::path dirPath{resolveEnviromentVariables(path)};
        return std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath);
    }

    bool mkdir(const std::string_view path) {
        std::filesystem::create_directories(resolveEnviromentVariables(path));
        return doesDirectoryExist(path);
    }

    std::vector<std::string> listDirectory(const std::string_view path,
                                           bool recursive) {
        const std::filesystem::path dirPath{resolveEnviromentVariables(path)};

        if (!doesDirectoryExist(path)) {
            std::cout<<"WARNING - No directory "<<dirPath<<std::endl;
            return {};
        }
        std::vector<std::string> contents{};
        if (recursive){
            for (const auto& entry: std::filesystem::recursive_directory_iterator{dirPath}){
                contents.emplace_back(entry.path());
            }
        } else {
            for (const auto& entry: std::filesystem::directory_iterator{dirPath}){
                contents.emplace_back(entry.path());
            }
        }
        return contents;
    }
    std::vector<std::string> findFile(const std::string_view path,
                                      const std::string_view file) {

        std::vector<std::string> foundFiles{};
        std::vector<std::string> contents{listDirectory(path, true)};
        std::copy_if(contents.begin(), contents.end(), 
                     std::back_inserter(foundFiles),
                     [file](const std::string& entry) {
                        return entry.find(file) != std::string::npos;
                     });
        return foundFiles;
    }
    
    bool copyFile(const std::string_view from,
                  const std::string_view to) {
        
        if (!doesFileExist(from)) {
            std::cerr<<"copyFile() -- File "<<from<<" does not exist"<<std::endl;
            return false;
        }
        if (to.find('/') != std::string::npos) {
            if (!mkdir(to.substr(0, to.rfind('/')))){
                std::cerr<<"copyFile() -- Cannot create directory "
                         <<to.substr(0, to.rfind('/'))<<".";
            }
        }
        std::filesystem::copy(from, to, std::filesystem::copy_options::overwrite_existing);    
        return doesFileExist(to);
    }

}

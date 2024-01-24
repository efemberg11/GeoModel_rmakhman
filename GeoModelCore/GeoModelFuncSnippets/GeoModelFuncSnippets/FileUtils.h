/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELFUNCSNIPPETS_FILEUTILS_H
#define GEOMODELFUNCSNIPPETS_FILEUTILS_H

#include <string>
#include <string_view>
#include <vector>

namespace GeoFileUtils {
    bool doesFileExist(const std::string_view path);
    bool doesDirectoryExist(const std::string_view path);
    bool mkdir(const std::string_view path);

    std::vector<std::string> listDirectory(const std::string_view path,
                                           bool recursive = false);

    std::vector<std::string> findFile(const std::string_view path,
                                      const std::string_view file);
    
    bool copyFile(const std::string_view from,
                  const std::string_view to);
                  

}
#endif
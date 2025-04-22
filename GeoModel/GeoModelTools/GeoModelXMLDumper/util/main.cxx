/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelHelpers/defineWorld.h"
#include "GeoModelKernel/GeoGeometryPluginLoader.h"
#include "GeoModelXMLDumper/GeoXMLDumper.h"
#include "GeoModelHelpers/FileUtils.h"
#include "GeoModelHelpers/StringUtils.h"


#include <vector>
#include <string>
#include <iostream>

using namespace GeoFileUtils;
using namespace GeoStrUtils;


int main(int argc, char ** argv) {
     const  std::string stdDtdFile{"geomodel_v0.dtd"};

    std::vector<std::string> plugins{};
    std::string outDir{};
    std::string detectorName{"ForkLiftGarage"};
    std::string xmlLayoutFile{};

    for (int k =1 ; k < argc; ++k) {
        const std::string the_arg{argv[k]};
        if (the_arg=="-p" || the_arg=="--plugin") {
           bool added{false};
           while (k+1 < argc) {
                const std::string toLoad{argv[k+1]};
                if (toLoad[0] == '-') break;
                plugins.push_back(toLoad);
                ++k;
                added = true;
           }
           if (!added) {
                std::cerr<<"Please give at least one follow-up argument to "<<the_arg<<std::endl;
                return EXIT_FAILURE;
           }
        } else if (the_arg == "-o" || the_arg=="--outDir"){
            if (k+1 < argc) {
                outDir = argv[k+1];
                ++k;
            } else {
                std::cerr<<"Please give one follow-up argument to "<<the_arg<<std::endl;
                return EXIT_FAILURE;
            }
        } else {
            std::cerr<<"Unknown argument "<<the_arg<<std::endl;
            return EXIT_FAILURE;
        }
    }
    if(plugins.empty()) {
        std::cerr<<"Please specify at least one plugin via -p / --plugin "<<std::endl;
        return EXIT_FAILURE;
    }

    if (outDir.empty()) {
        std::cerr<<"Please specify an output file via -o "<<std::endl;
        return EXIT_FAILURE;
    }
    if (!mkdir(outDir)) {
        std::cerr<<"Failed to create directory "<<outDir<<std::endl;
        return EXIT_FAILURE;
    }
    if (xmlLayoutFile.empty()) {
        const std::string ldEnvVar = resolveEnviromentVariables("${LD_LIBRARY_PATH}");
        const std::vector<std::string> ldLibPaths{tokenize(ldEnvVar, ":")};
        for (const std::string& ld_lib : ldLibPaths){
            const std::string sharePath{ld_lib+"/../share/"};
            if (!doesDirectoryExist(sharePath)) {
                continue;
            }            
            //  
            const std::vector<std::string> dtdPaths{findFile(sharePath, stdDtdFile)};
            
            if (dtdPaths.size() && !copyFile(dtdPaths[0], outDir + "/" + stdDtdFile)) {
                return EXIT_FAILURE;
            }
            xmlLayoutFile = stdDtdFile;
        }
        if (xmlLayoutFile.empty()) {
            return EXIT_FAILURE;
        }
    }
    GeoIntrusivePtr<GeoVPhysVol> world{createGeoWorld()};

    for (const std::string& load_me : plugins) {
        std::cout<<"Load plugin "<<load_me<<std::endl;
        GeoGeometryPluginLoader loader;
        std::unique_ptr<GeoVGeometryPlugin> factory{loader.load(load_me)};
        if (!factory) {
          std::cerr << "Could not load plugin " << load_me << std::endl;
          return EXIT_FAILURE;
        }
        factory->create(world);
    }
    GeoXMLDumper dumper{outDir, detectorName, xmlLayoutFile};
    dumper.publishWorld(world);

    return EXIT_SUCCESS;
}
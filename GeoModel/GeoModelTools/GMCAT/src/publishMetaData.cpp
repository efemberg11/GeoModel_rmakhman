#include "GeoModelDBManager/GMDBManager.h"
#include <GeoModelHelpers/StringUtils.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/utsname.h>
#include <array>
#define STR_VALUE(arg) #arg
#define STR_NAME(name) STR_VALUE(name)

namespace{
    static const std::string tableName{"AAHEADER"};
}

using tableVariant = std::variant<int,long,float,double,std::string>;

std::string getCommandOutput(const std::string & cmd, bool firstLineOnly=false)
{
  std::string response;
  // Ceci n'est pas une pipe:
  FILE *ceci=popen(cmd.c_str(),"r");
  if (ceci) {
    static const int MSGSIZE=1024;
    std::array<char, 1024> buff{};
    while (fgets(buff.data(),MSGSIZE,ceci)!=NULL) {
      response+=std::string(buff.data());
      if (firstLineOnly) break;
    }
    pclose(ceci);
  }
  return response;
}

void publishMetaData( GMDBManager & db,
                      const std::string& repoPath,
                      const std::vector<std::string> &inputFiles,
                      const std::vector<std::string> &inputPlugins,
                      const std::vector<std::string> &pluginNames,
                      const std::string              &outputFile) {

  struct Metadata {
    std::string dateString=getCommandOutput("date -Iminutes");
    std::string username{GeoStrUtils::resolveEnviromentVariables("${USER}")};
    std::string hostname{};
    std::string os{};
    std::string wd{};
    std::string gmversion=STR_NAME(GMVERSION);
    std::string outputFile{};
    std::string geoModelDataBranch="Undefined";      // or overwritten below
    std::string gmdataIsClean     ="Not applicable"; // or overwritten below
    std::string gmdataCommitHash  ="Undefined";      // or overwritten below
    std::string gmdataAssociatedTag = "Undefined";   // or overwritten below
  } metadata;

  struct XtraMetadata {
    std::string repo          ="Undefined";      // or overwritten below
    std::string branch        ="Undefined";      // or overwritten below
    std::string isClean       ="Not applicable"; // or overwritten below
    std::string commitHash    ="Undefined";      // or overwritten below
    std::string associatedTag ="Undefined";      // or overwritten below
  } xtraMetadata;
  
#ifdef __APPLE__
  char wdbuff[1024];
  metadata.wd=std::string(getcwd(wdbuff,1024));
#else
  metadata.wd= GeoStrUtils::resolveEnviromentVariables("${PWD}");
#endif

  metadata.outputFile=outputFile;
  utsname uts;
  uname (&uts);
  metadata.os=std::string(uts.sysname)+  "-" + std::string(uts.machine);

  // Sometimes "HOSTNAME" is empty as seen from gmcat.
  if (metadata.hostname.empty()) {
    char hn[1024];
    if (gethostname (hn,1024)==0) metadata.hostname=std::string(hn);
  }
  
  std::string geomodel_xml_dir=GeoStrUtils::resolveEnviromentVariables("${GEOMODEL_XML_DIR}");
  if (!geomodel_xml_dir.empty())  {
    {
      metadata.geoModelDataBranch=getCommandOutput("git -C "+ geomodel_xml_dir + " rev-parse --abbrev-ref HEAD");
      std::string shortGitStatus=getCommandOutput("git -C "+ geomodel_xml_dir + " status -s ");
      if (shortGitStatus!="") {
        metadata.gmdataIsClean="no";
      } else {
        std::string synchedToOrigin=getCommandOutput("git -C "+ std::string(geomodel_xml_dir) + " diff origin/"+metadata.geoModelDataBranch,true);
        if (synchedToOrigin.size()) {
            metadata.gmdataIsClean="no";
        }else {
            metadata.gmdataIsClean="yes";
            metadata.gmdataCommitHash=getCommandOutput("git -C " + std::string(geomodel_xml_dir) + " log -1 --format=format:\"%H\"");
            metadata.gmdataAssociatedTag=getCommandOutput("git -C " + std::string(geomodel_xml_dir) +  " describe --tag " + metadata.gmdataCommitHash+ "  2> /dev/null");
        }
      }
    }
  }
  std::string isGitManaged=getCommandOutput("git -C " + repoPath + " rev-parse --is-inside-work-tree");
  if (isGitManaged=="true\n") {
    xtraMetadata.repo=getCommandOutput("git -C " + repoPath + " remote get-url origin");
    xtraMetadata.branch=getCommandOutput("git -C " + repoPath + " rev-parse --abbrev-ref HEAD");
    std::string status=getCommandOutput("git -C " + repoPath + " status --porcelain");
    if (status.find(" M ")!=std::string::npos) {
      xtraMetadata.isClean="no";
    }
    else {
      std::string synchedToOrigin=getCommandOutput("git -C " + repoPath + " diff origin/"+xtraMetadata.branch,true);
      xtraMetadata.isClean = synchedToOrigin.empty() ? "yes" : "no";
      if (synchedToOrigin.empty()){
        xtraMetadata.commitHash=getCommandOutput("git -C " + repoPath + " log -1 --format=format:\"%H\"");
        xtraMetadata.associatedTag=getCommandOutput("git -C " + repoPath + " describe --tag " + xtraMetadata.commitHash+ "  2> /dev/null");
      }
    }
  } else {
    std::cerr << std::endl << "WARNING: no information on User repository will be written to metadata. " << std::endl;
  }

  //
  // Fill the header file with metadata
  //
  std::vector<std::string> gmcatColNames={"Date",
                                          "GeoModelDataBranch",
                                          "Username",
                                          "Hostname",
                                          "OS",
                                          "WorkingDirectory",
                                          "GeoModelVersion",
                                          "OutputFile",
                                          "PluginNames",
                                          "PluginLibs",
                                          "DataBaseFiles",
                                          "GeoModelDataIsClean",
                                          "GeoModelDataCommitHash",
                                          "GeoModelDataAssociatedTag",
                                          tableName + "_DATA_ID"
                                           };
  std::vector<std::string>  gmcatColTypes(gmcatColNames.size(), "STRING");
  gmcatColTypes[gmcatColTypes.size() - 1] = "INT";
  // Strip extraneous \n
  for (std::string * s : {&metadata.geoModelDataBranch,  &metadata.dateString, 
                          &metadata.gmdataAssociatedTag, &xtraMetadata.repo, &xtraMetadata.branch}) { 
       s->erase(std::remove(s->begin(), s->end(), '\n'), s->end());
  }
    
    
  
  std::vector<std::vector<tableVariant>>  gmcatData    ={{
      metadata.dateString,
      metadata.geoModelDataBranch,
      metadata.username,
      metadata.hostname,
      metadata.os,
      metadata.wd,
      metadata.gmversion,
      metadata.outputFile,
      GeoStrUtils::chainUp(pluginNames,";"),      
      GeoStrUtils::chainUp(inputPlugins,";"),
      GeoStrUtils::chainUp(inputFiles,";"),
      metadata.gmdataIsClean,
      metadata.gmdataCommitHash,
      metadata.gmdataAssociatedTag,
      0
    }};

  if (xtraMetadata.branch!="Undefined") {
    std::vector<std::string> xtraColNames={"UserCodeGitRepository",
                                           "UserCodeGitBranch",
                                           "UserCodeRepoIsClean",
                                           "UserCodeRepoCommitHash",
                                           "UserCodeAssociatedTag"};
    std::vector<std::string> xtraColTypes(xtraColNames.size(), "STRING");
    std::vector<std::vector<tableVariant>>  xtraData    ={{xtraMetadata.repo,
                                                           xtraMetadata.branch,
                                                           xtraMetadata.isClean,
                                                           xtraMetadata.commitHash,
                                                           xtraMetadata.associatedTag }};    
    gmcatColNames.insert(std::end(gmcatColNames), std::begin(xtraColNames), std::end(xtraColNames));
    gmcatColTypes.insert(std::end(gmcatColTypes), std::begin(xtraColTypes), std::end(xtraColTypes));
    gmcatData[0].insert(std::end(gmcatData[0]), std::begin(xtraData[0]), std::end(xtraData[0]));
   
  }
  db.createCustomTable(tableName, gmcatColNames,gmcatColTypes,gmcatData); 
  
}

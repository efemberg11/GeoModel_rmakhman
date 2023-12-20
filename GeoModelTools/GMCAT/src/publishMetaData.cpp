#include "GeoModelDBManager/GMDBManager.h"
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

std::string resolveVariable(const std::string& varName) {
   const char* var = std::getenv(varName.c_str());
   if (!var) return std::string{};
   return std::string(var);
}
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
		      std::vector<std::string> &inputFiles,
		      std::vector<std::string> &inputPlugins,
		      std::string              &outputFile) {

  struct Metadata {
    std::string dateString=getCommandOutput("date -Iminutes");
    std::string username{resolveVariable("USER")};
    std::string hostname{};
    std::string os;
    std::string wd;
    std::string gmversion=STR_NAME(GMVERSION);
    std::string outputFile;
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
  metadata.wd= resolveVariable("PWD");
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
  
  std::string geomodel_xml_dir=resolveVariable("GEOMODEL_XML_DIR");
  if (!geomodel_xml_dir.empty())  {
    {
      metadata.geoModelDataBranch=getCommandOutput("git -C "+ geomodel_xml_dir + " rev-parse --abbrev-ref HEAD");
      std::string shortGitStatus=getCommandOutput("git -C "+ geomodel_xml_dir + " status -s ");
      if (shortGitStatus!="") {
	metadata.gmdataIsClean="no";
      }
      else {
	std::string synchedToOrigin=getCommandOutput("git -C "+ std::string(geomodel_xml_dir) + " diff origin/"+metadata.geoModelDataBranch,true);
	if (synchedToOrigin!="") {
	  metadata.gmdataIsClean="no";
	}
	else {
	  metadata.gmdataIsClean="yes";
	  metadata.gmdataCommitHash=getCommandOutput("git -C " + std::string(geomodel_xml_dir) + " log -1 --format=format:\"%H\"");
	  metadata.gmdataAssociatedTag=getCommandOutput("git -C " + std::string(geomodel_xml_dir) +  " describe --tag " + metadata.gmdataCommitHash+ "  2> /dev/null");
	}
      }
    }
  }
  std::string isGitManaged=getCommandOutput("git rev-parse --is-inside-work-tree");
  if (isGitManaged=="true\n") {
    xtraMetadata.repo=getCommandOutput("git remote get-url origin");
    xtraMetadata.branch=getCommandOutput("git rev-parse --abbrev-ref HEAD");
    std::string status=getCommandOutput("git status --porcelain");
    if (status.find(" M ")!=std::string::npos) {
      xtraMetadata.isClean="no";
    }
    else {
      std::string synchedToOrigin=getCommandOutput("git diff origin/"+xtraMetadata.branch,true);
      if (synchedToOrigin!="") {
	xtraMetadata.isClean="no";
      }
      else {
	xtraMetadata.isClean="yes";
	xtraMetadata.commitHash=getCommandOutput("git log -1 --format=format:\"%H\"");
	xtraMetadata.associatedTag=getCommandOutput("git  describe --tag " + xtraMetadata.commitHash+ "  2> /dev/null");
      }
    }
  }

  //
  // Fill the header file with metadata
  //
  std::vector<std::string>                                                   gmcatColNames={"Date",
											    "GeoModelDataBranch",
											    "Username",
											    "Hostname",
											    "OS",
											    "WorkingDirectory",
											    "GeoModelVersion",
											    "OutputFile",
											    "GeoModelDataIsClean",
											    "GeoModelDataCommitHash",
											    "GeoModelDataAssociatedTag"
  };
  std::vector<std::string>                                                   gmcatColTypes={"STRING",
											    "STRING" ,
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING",
											    "STRING"
  };
  
  // Strip extraneous \n
  for (std::string * s : {
      &metadata.geoModelDataBranch,
      &metadata.dateString,
      &metadata.gmdataAssociatedTag}) { 
    s->erase(std::remove(s->begin(), s->end(), '\n'), s->end());
    std::cout << s << std::endl;
  }
	
	
  
  std::vector<std::vector<std::variant<int,long,float,double,std::string>>>  gmcatData    ={{
      metadata.dateString,
      metadata.geoModelDataBranch,
      metadata.username,
      metadata.hostname,
      metadata.os,
      metadata.wd,
      metadata.gmversion,
      metadata.outputFile,
      metadata.gmdataIsClean,
      metadata.gmdataCommitHash,
      metadata.gmdataAssociatedTag
    }};

  unsigned int pcounter(0);
  for (std::string plugin : inputPlugins) {
    gmcatColNames.push_back("P"+std::to_string(pcounter++));
    gmcatColTypes.push_back("STRING");
    gmcatData[0].push_back((plugin));
  }
  unsigned int fcounter(0);
  for (std::string file : inputFiles) {
    gmcatColNames.push_back("F"+std::to_string(fcounter++));
    gmcatColTypes.push_back("STRING");
    gmcatData[0].push_back(file);
  }
  
  // Strip extraneous \n
  for (std::string * s : {
      &xtraMetadata.repo,
      &xtraMetadata.branch }) { 
    s->erase(std::remove(s->begin(), s->end(), '\n'), s->end());
    std::cout << s << std::endl;
  }
	


  if (xtraMetadata.branch!="Undefined") {
    std::vector<std::string>                                                   xtraColNames={
      "UserCodeGitRepository",
      "UserCodeGitBranch",
      "UserCodeRepoIsClean",
      "UserCodeRepoCommitHash",
      "UserCodeAssociatedTag"};
    std::vector<std::string>                                                   xtraColTypes={"STRING",
											     "STRING",
											     "STRING",
											     "STRING",
											     "STRING"};
    std::vector<std::vector<std::variant<int,long,float,double,std::string>>>  xtraData    ={{
	xtraMetadata.repo,
	xtraMetadata.branch,
	xtraMetadata.isClean,
	xtraMetadata.commitHash,
	xtraMetadata.associatedTag
    }};    
    using std::begin, std::end;
    gmcatColNames.insert(end(gmcatColNames), begin(xtraColNames), end(xtraColNames));
    gmcatColTypes.insert(end(gmcatColTypes), begin(xtraColTypes), end(xtraColTypes));
    gmcatData[0].insert(end(gmcatData[0]), begin(xtraData[0]), end(xtraData[0]));
    db.createCustomTable("AAHEADER", gmcatColNames,gmcatColTypes,gmcatData);
  }
  else {
    db.createCustomTable("AAHEADER", gmcatColNames,gmcatColTypes,gmcatData); 
  }
}

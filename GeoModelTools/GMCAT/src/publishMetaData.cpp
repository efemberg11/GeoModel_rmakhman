#include "GeoModelDBManager/GMDBManager.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/utsname.h>
#define STR_VALUE(arg) #arg
#define STR_NAME(name) STR_VALUE(name)

std::string getCommandOutput(const std::string & cmd, bool firstLineOnly=false)
{
  std::string response;
  // Ceci n'est pas une pipe:
  FILE *ceci=popen(cmd.c_str(),"r");
  if (ceci) {
    static const int MSGSIZE=1024;
    char buff[MSGSIZE];
    while (fgets(buff,MSGSIZE,ceci)!=NULL) {
      response+=std::string(buff);
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
    std::string username=getlogin();
    std::string hostname;
    std::string os;
    std::string wd;
    std::string gmversion=STR_NAME(GMVERSION);
    std::string outputFile;
    std::string geoModelDataBranch="Undefined";      // or overwritten below
    std::string gmdataIsClean     ="Not applicable"; // or overwritten below
    std::string gmdataCommitHash  ="Undefined";      // or overwritten below
  } metadata;

  struct XtraMetadata {
    std::string repo        ="Undefined";      // or overwritten below
    std::string branch      ="Undefined";      // or overwritten below
    std::string isClean     ="Not applicable"; // or overwritten below
    std::string commitHash  ="Undefined";      // or overwritten below
  } xtraMetadata;
  
#ifdef __APPLE__
  char wdbuff[1024];
  metadata.wd=std::string(getcwd(wdbuff,1024));
#else
  metadata.wd=get_current_dir_name();
#endif

  metadata.outputFile=outputFile;
  
  char buff[1024];
  gethostname(buff,1024);
  metadata.hostname=buff;
 
  utsname uts;
  uname (&uts);
  metadata.os=std::string(uts.sysname)+  "-" + std::string(uts.machine);

  char *geomodel_xml_dir=getenv("GEOMODEL_XML_DIR");
  if (geomodel_xml_dir)  {
    {
      metadata.geoModelDataBranch=getCommandOutput("git -C "+ std::string(geomodel_xml_dir) + " rev-parse --abbrev-ref HEAD");
      std::string shortGitStatus=getCommandOutput("git -C "+ std::string(geomodel_xml_dir) + " status -s ");
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
											    "GeoModelDataCommitHash"
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
											    "STRING"
  };
  
  // Strip extraneous \n
  for (std::string * s : {
      &metadata.geoModelDataBranch,
      &metadata.dateString }) { 
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
      metadata.gmdataCommitHash
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
      "UserCodeRepoCommitHash" };
    std::vector<std::string>                                                   xtraColTypes={"STRING",
											     "STRING",
											     "STRING",
											     "STRING"};
    std::vector<std::vector<std::variant<int,long,float,double,std::string>>>  xtraData    ={{
	xtraMetadata.repo,
	xtraMetadata.branch,
	xtraMetadata.isClean,
	xtraMetadata.commitHash
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

#include <unistd.h>
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
int main(int argc, char ** argv) {

  // Prepend a fixed config file onto the command line args:
  std::string c="-c";
  std::string config=std::string(INSTALLPREFIX)+"/share/FullSimLight/ATLAS/atlas-conf.json";

  std::vector<char *>  Argv;
  Argv.push_back((char *) c.c_str());
  Argv.push_back((char *) config.c_str());
  for (int i=1;i<argc;i++) Argv.push_back(argv[i]);

  int status= execv((std::string(INSTALLPREFIX)+"/bin/fullSimLight").c_str(),&Argv[0]);
  if (status) {
    perror("FullSimLight Installation error: ");
  }

  return 0; 
}

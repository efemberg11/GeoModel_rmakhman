#ifndef __TrksHDF5Factory_hh__
#define __TrksHDF5Factory_hh__

#include "H5Cpp.h"

#include <mutex> 

class TrksHDF5Factory
{
public:

    static std::mutex gWriteFileMutex;

    static TrksHDF5Factory* GetInstance();
    static void OpenFile(const char*);

    static H5::H5File* GetFile() {return fgFile;}

private:

    TrksHDF5Factory();
    ~TrksHDF5Factory();

    static TrksHDF5Factory* fgInstance;
    static H5::H5File *fgFile;

};


#endif //__TrksHDF5Factory_hh__
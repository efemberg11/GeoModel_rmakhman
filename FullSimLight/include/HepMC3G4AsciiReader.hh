
#ifndef HEPMC3_G4_ASCII_READER_H
#define HEPMC3_G4_ASCII_READER_H

#include "HepMC3G4Interface.hh"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Print.h"
#include "HepMC3/Units.h"
#include "G4AutoLock.hh"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderAscii.h"


/*#if USE_ROOT
#include "HepMC3/ReaderRoot.h"
#endif*/

namespace {
G4Mutex mutino = G4MUTEX_INITIALIZER;

}
class HepMC3G4AsciiReader : public HepMC3G4Interface {
    
public:
    
    ~HepMC3G4AsciiReader();
    static HepMC3G4AsciiReader * GetInstance(G4String name,G4String reader_type)
    {
        if (m_pOnlyOneInstance == NULL)
        {
            G4cout<<"Going to lock in HepMC3G4AsciiReader::GetInstance"<<G4endl;
            G4AutoLock lock(mutino);
            G4cout<<"HepMC3G4AsciiReader::GetInstance MUTEX IS MINE"<<G4endl;
            if (m_pOnlyOneInstance == NULL)
                // Solution 1 and 2 gaps addressed by moving
                // critical section block and by re-doing this check!
            {
                m_pOnlyOneInstance = new HepMC3G4AsciiReader(name,reader_type);
            }
            lock.unlock();
            G4cout<<"HepMC3G4AsciiReader::GetInstance MUTEX unlocked"<<G4endl;
        }
        return m_pOnlyOneInstance;
    }
protected:
    HepMC3G4AsciiReader(G4String name,G4String reader_type );
    HepMC3::ReaderAsciiHepMC2* asciiInput;
    HepMC3::ReaderAscii* asciiv3Input;
    
/*#if USE_ROOT
    HepMC3::ReaderRoot* rootInput;
#endif*/
    
    std::shared_ptr<HepMC3::GenEvent>  evt;
    //HepMC3::ReaderMT<HepMC3::ReaderAsciiHepMC2,8> *asciiInput;
    virtual HepMC3::GenEvent* GenerateHepMC3Event() final override;
private:
    G4String reader;
    static HepMC3G4AsciiReader * m_pOnlyOneInstance;
    
};


#endif


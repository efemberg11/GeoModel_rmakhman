

#include "HepMC3G4AsciiReader.hh"
#include <iostream>
#include <fstream>
#include "G4AutoLock.hh"


HepMC3G4AsciiReader* HepMC3G4AsciiReader::m_pOnlyOneInstance=nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC3G4AsciiReader::HepMC3G4AsciiReader(G4String name,G4String reader_type )
{
  //  evt = HepMC3::GenEvent(HepMC3::Units::GEV ,HepMC3::Units::MM);
    
reader = reader_type;
evt= std::make_shared<HepMC3::GenEvent>();
    
    //
    if(reader=="Ascii")
    {
        asciiInput = new HepMC3::ReaderAsciiHepMC2(name.c_str());
    }
    else if (reader=="Asciiv3")
    {
        asciiv3Input = new HepMC3::ReaderAscii(name.c_str());
    }
/*#if USE_ROOT
    else if (reader=="Root")
    {
        rootInput = new HepMC3::ReaderRoot(name.c_str());
    }
#endif*/
    else
    {
        std::cout << "Unsupported Event File type: Exiting now" << std::endl;
        exit(-1);
    }
    
//std::cout << "Hey once!!!!!" << std::endl;

  //asciiInput = new HepMC3::ReaderMT<HepMC3::ReaderAsciiHepMC2,8>(filename);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC3G4AsciiReader::~HepMC3G4AsciiReader()
{
    if(reader=="Ascii")
    {
        delete asciiInput;
    }
    else if (reader=="Asciiv3")
    {
        delete asciiv3Input;
    }
/*#if USE_ROOT
    else if (reader=="Root")
    {
        delete rootInput;
    }
#endif*/
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC3::GenEvent* HepMC3G4AsciiReader::GenerateHepMC3Event()
{
    evt->clear();
   // evt = HepMC3::GenEvent(HepMC3::Units::GEV ,HepMC3::Units::MM);
   // evt= std::make_shared<HepMC3::GenEvent>();
    
    if(reader=="Ascii")
    {
        asciiInput->read_event(*evt);
        HepMC3::Print::listing(*evt);
        HepMC3::Print::content(*evt);
        if(asciiInput->failed()) return 0;
    }
    
    else if(reader=="Asciiv3")
    {
        asciiv3Input->read_event(*evt);
        HepMC3::Print::listing(*evt);
        HepMC3::Print::content(*evt);
        if(asciiv3Input->failed()) return 0;
    }
    
/*#if USE_ROOT

    else if(reader=="Root")
    {
        rootInput->read_event(*evt);
        if(rootInput->failed()) return 0;
    }
#endif*/
 // no more event
   
    return evt.get();
}


#include "GeoModelKernel/Units.h"
#include "GDMLInterface/GDMLController.h"

using namespace GeoModelKernelUnits;

void GDMLController::setConstant(const char* s, double value)
{
  std::string str(s);
  theEvaluator->RegisterConstant(str,value);
}

void GDMLController::setEvaluatorUnits()
{

setConstant(      "pi"  , pi);
setConstant(   "twopi"  , 2*pi);
setConstant(  "halfpi"  , pi/2);
setConstant(      "pi2" , pi*pi);

//
// Length [L]
//
setConstant(  "millimeter"  , millimeter);
setConstant(  "millimeter2" , millimeter*millimeter);
setConstant(  "millimeter3" , millimeter*millimeter*millimeter);

setConstant(  "centimeter"  , 10.*millimeter);
setConstant(  "centimeter2" , centimeter*centimeter);
setConstant(  "centimeter3" , centimeter*centimeter*centimeter);

setConstant(  "meter"  , 1000.*millimeter);
setConstant(  "meter2" , meter*meter);
setConstant(  "meter3" , meter*meter*meter);

setConstant(  "kilometer" , 1000.*meter);
setConstant(  "kilometer2" , kilometer*kilometer);
setConstant(  "kilometer3" , kilometer*kilometer*kilometer);

setConstant(  "parsec" , 3.0856775807e+16*meter);

setConstant(  "micrometer" , 1.e-6 *meter);
setConstant(   "nanometer" , 1.e-9 *meter);
setConstant(   "angstrom"  , 1.e-10*meter);
setConstant(   "fermi"     , 1.e-15*meter);

setConstant(       "barn" , 1.e-28*meter2);
setConstant(  "millibarn" , 1.e-3 *barn);
setConstant(  "microbarn" , 1.e-6 *barn);
setConstant(   "nanobarn" , 1.e-9 *barn);
setConstant(   "picobarn" , 1.e-12*barn);

// symbols
setConstant(  "nm"  , nanometer);
setConstant(  "um"  , micrometer);

setConstant(  "mm"  , millimeter);
setConstant(  "mm2" , millimeter2);
setConstant(  "mm3" , millimeter3);

setConstant(  "cm"  , centimeter);
setConstant(  "cm2" , centimeter2);
setConstant(  "cm3" , centimeter3);

setConstant(  "liter" , 1.e+3*cm3);
setConstant(   "L" , liter);
setConstant(  "dL" , 1.e-1*liter);
setConstant(  "cL" , 1.e-2*liter);
setConstant(  "mL" , 1.e-3*liter);

setConstant(  "m"  , meter);
setConstant(  "m2" , meter2);
setConstant(  "m3" , meter3);

setConstant(  "km"  , kilometer);
setConstant(  "km2" , kilometer2);
setConstant(  "km3" , kilometer3);

setConstant(  "pc" , parsec);

//
// Angle
//
setConstant(  "radian"      , radian);
setConstant(  "milliradian" , 1.e-3*radian);
setConstant(  "degree" , (pi/180.0)*radian);

setConstant(    "steradian" , steradian);

// symbols
setConstant(  "rad"  , radian);
setConstant(  "mrad" , milliradian);
setConstant(  "sr"   , steradian);
setConstant(  "deg"  , degree);

//
// Time [T]
//
setConstant(  "nanosecond"  , nanosecond);
setConstant(  "second"      , 1.e+9 *nanosecond);
setConstant(  "millisecond" , 1.e-3 *second);
setConstant(  "microsecond" , 1.e-6 *second);
setConstant(   "picosecond" , 1.e-12*second);

setConstant(  "hertz" , 1./second);
setConstant(  "kilohertz" , 1.e+3*hertz);
setConstant(  "megahertz" , 1.e+6*hertz);

// symbols
setConstant(  "ns" , nanosecond);
setConstant(   "s" , second);
setConstant(  "ms" , millisecond);
setConstant(  "us" , microsecond);
setConstant(  "ps" , picosecond);

//
// Electric charge [Q]
//
setConstant(  "eplus" , eplus );// positron charge
setConstant(  "e_SI"  , 1.602176487e-19);// positron charge in coulomb
setConstant(  "coulomb" , eplus/e_SI);// coulomb , 6.24150 e+18 * eplus

//
// Energy [E]
//
setConstant(  "megaelectronvolt" , megaelectronvolt );
setConstant(      "electronvolt" , 1.e-6*megaelectronvolt);
setConstant(  "kiloelectronvolt" , 1.e-3*megaelectronvolt);
setConstant(  "gigaelectronvolt" , 1.e+3*megaelectronvolt);
setConstant(  "teraelectronvolt" , 1.e+6*megaelectronvolt);
setConstant(  "petaelectronvolt" , 1.e+9*megaelectronvolt);

setConstant(  "joule" , electronvolt/e_SI);// joule , 6.24150 e+12 * MeV

// symbols
setConstant(  "MeV" , megaelectronvolt);
setConstant(   "eV" , electronvolt);
setConstant(  "keV" , kiloelectronvolt);
setConstant(  "GeV" , gigaelectronvolt);
setConstant(  "TeV" , teraelectronvolt);
setConstant(  "PeV" , petaelectronvolt);

//
// Mass [E][T^2][L^-2]
//
setConstant(   "kilogram" , joule*second*second/(meter*meter));
setConstant(       "gram" , 1.e-3*kilogram);
setConstant(  "milligram" , 1.e-3*gram);

// symbols
setConstant(   "kg" , kilogram);
setConstant(    "g" , gram);
setConstant(   "mg" , milligram);

//
// Power [E][T^-1]
//
setConstant(  "watt" , joule/second);// watt , 6.24150 e+3 * MeV/ns

//
// Force [E][L^-1]
//
setConstant(  "newton" , joule/meter);// newton , 6.24150 e+9 * MeV/mm

//
// Pressure [E][L^-3]
//
#define pascal hep_pascal                          // a trick to avoid warnings
setConstant(  "hep_pascal" , newton/m2);   // pascal , 6.24150 e+3 * MeV/mm3
setConstant(  "bar"        , 100000*pascal); // bar    , 6.24150 e+8 * MeV/mm3
setConstant(  "atmosphere" , 101325*pascal); // atm    , 6.32420 e+8 * MeV/mm3

//
// Electric current [Q][T^-1]
//
setConstant(       "ampere" , coulomb/second); // ampere , 6.24150 e+9 * eplus/ns
setConstant(  "milliampere" , 1.e-3*ampere);
setConstant(  "microampere" , 1.e-6*ampere);
setConstant(   "nanoampere" , 1.e-9*ampere);

//
// Electric potential [E][Q^-1]
//
setConstant(  "megavolt" , megaelectronvolt/eplus);
setConstant(  "kilovolt" , 1.e-3*megavolt);
setConstant(      "volt" , 1.e-6*megavolt);

//
// Electric resistance [E][T][Q^-2]
//
setConstant(  "ohm" , volt/ampere);// ohm , 1.60217e-16*(MeV/eplus)/(eplus/ns)

//
// Electric capacitance [Q^2][E^-1]
//
setConstant(  "farad" , coulomb/volt);// farad , 6.24150e+24 * eplus/Megavolt
setConstant(  "millifarad" , 1.e-3*farad);
setConstant(  "microfarad" , 1.e-6*farad);
setConstant(   "nanofarad" , 1.e-9*farad);
setConstant(   "picofarad" , 1.e-12*farad);

//
// Magnetic Flux [T][E][Q^-1]
//
setConstant(  "weber" , volt*second);// weber , 1000*megavolt*ns

//
// Magnetic Field [T][E][Q^-1][L^-2]
//
setConstant(  "tesla"     , volt*second/meter2);// tesla ,0.001*megavolt*ns/mm2

setConstant(  "gauss"     , 1.e-4*tesla);
setConstant(  "kilogauss" , 1.e-1*tesla);

//
// Inductance [T^2][E][Q^-2]
//
setConstant(  "henry" , weber/ampere);// henry , 1.60217e-7*MeV*(ns/eplus)**2

//
// Temperature
//
setConstant(  "kelvin" , kelvin);

//
// Amount of substance
//
setConstant(  "mole" , mole);

//
// Activity [T^-1]
//
setConstant(  "becquerel" , 1./second );
setConstant(  "curie" , 3.7e+10 * becquerel);
setConstant(  "kilobecquerel" , 1.e+3*becquerel);
setConstant(  "megabecquerel" , 1.e+6*becquerel);
setConstant(  "gigabecquerel" , 1.e+9*becquerel);
setConstant(  "millicurie" , 1.e-3*curie);
setConstant(  "microcurie" , 1.e-6*curie);
setConstant(  "Bq" , becquerel);
setConstant(  "kBq" , kilobecquerel);
setConstant(  "MBq" , megabecquerel);
setConstant(  "GBq" , gigabecquerel);
setConstant(  "Ci" , curie);
setConstant(  "mCi" , millicurie);
setConstant(  "uCi" , microcurie);

//
// Absorbed dose [L^2][T^-2]
//
setConstant(       "gray" , joule/kilogram );
setConstant(   "kilogray" , 1.e+3*gray);
setConstant(  "milligray" , 1.e-3*gray);
setConstant(  "microgray" , 1.e-6*gray);

//
// Luminous intensity [I]
//
setConstant(  "candela" , candela);

//
// Luminous flux [I]
//
setConstant(  "lumen" , candela*steradian);

//
// Illuminance [I][L^-2]
//
setConstant(  "lux" , lumen/meter2);

//
// Miscellaneous
//
setConstant(  "perCent"     , 0.01 );
setConstant(  "perThousand" , 0.001);
setConstant(  "perMillion"  , 0.000001);


setConstant(  "Avogadro" , 6.02214179e+23/mole);

//
// c   , 299.792458 mm/ns
// c^2 , 898.7404 (mm/ns)^2
//
setConstant(  "c_light"   , 2.99792458e+8 * m/s);
setConstant(  "c_squared" , c_light * c_light);

//
// h     , 4.13566e-12 MeV*ns
// hbar  , 6.58212e-13 MeV*ns
// hbarc , 197.32705e-12 MeV*mm
//
setConstant(  "h_Planck"      , 6.62606896e-34 * joule*s);
setConstant(  "hbar_Planck"   , h_Planck/twopi);
setConstant(  "hbarc"         , hbar_Planck * c_light);
setConstant(  "hbarc_squared" , hbarc * hbarc);

//
//
//
setConstant(  "electron_charge" , - eplus); // see SystemOfUnits.h
setConstant(  "e_squared" , eplus * eplus);

//
// amu_c2 - atomic equivalent mass unit
//        - AKA, unified atomic mass unit (u)
// amu    - atomic mass unit
//
setConstant(  "electron_mass_c2" , 0.510998910 * MeV);
setConstant(    "proton_mass_c2" , 938.272013 * MeV);
setConstant(   "neutron_mass_c2" , 939.56536 * MeV);
setConstant(            "amu_c2" , 931.494028 * MeV);
setConstant(               "amu" , amu_c2/c_squared);

//
// permeability of free space mu0    , 2.01334e-16 Mev*(ns*eplus)^2/mm
// permittivity of free space epsil0 , 5.52636e+10 eplus^2/(MeV*mm)
//
setConstant(  "mu0"      , 4*pi*1.e-7 * henry/m);
setConstant(  "epsilon0" , 1./(c_squared*mu0));

//
// electromagnetic coupling , 1.43996e-12 MeV*mm/(eplus^2)
//
setConstant(  "elm_coupling"           , e_squared/(4*pi*epsilon0));
setConstant(  "fine_structure_const"   , elm_coupling/hbarc);
setConstant(  "classic_electr_radius"  , elm_coupling/electron_mass_c2);
setConstant(  "electron_Compton_length" , hbarc/electron_mass_c2);
setConstant(  "Bohr_radius" , electron_Compton_length/fine_structure_const);

setConstant(  "alpha_rcl2" , fine_structure_const
*classic_electr_radius
*classic_electr_radius);

setConstant(  "twopi_mc2_rcl2" , twopi*electron_mass_c2
*classic_electr_radius
*classic_electr_radius);
//
//
//
setConstant(  "k_Boltzmann" , 8.617343e-11 * MeV/kelvin);

//
//
//
setConstant(  "STP_Temperature" , 273.15*kelvin);
setConstant(  "STP_Pressure"    , 1.*atmosphere);
setConstant(  "kGasThreshold"   , 10.*mg/cm3);

//
//
//
setConstant(  "universe_mean_density" , 1.e-25*g/cm3);

}

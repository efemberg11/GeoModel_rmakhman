#include <TString.h>
#include <TColor.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TTree.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <vector>
#include <TArc.h>
#include <TEllipse.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TProfile2D.h>
#include <TPaletteAxis.h>
#include <TString.h>
#include <string>
#include <iostream> 
#include <sstream>
#include <TMath.h>
#include <TGraph.h>
#include <TEfficiency.h>
#include <TColor.h>
#include <map>
#include <fstream>
#include <sstream>
#include "TApplication.h"
#include "TROOT.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"

#include <algorithm>
#include <TRandom.h>
#include <cctype>
#include <regex>

//namespace fs = std::filesystem;

 //Tree Branches
 int   m_genNPart;
 float m_genEta;
 float m_genPhi;
 float m_genZ;
 float m_genR;
      
 //X0 Branches
 float m_total_X0;
 float m_total_L0;

 std::vector<double> *m_collected_X0;
 std::vector<double> *m_collected_L0;

 std::vector<float> *m_collected_inhitr;
 std::vector<float> *m_collected_inhitz;

 std::vector<float> *m_collected_outhitr;
 std::vector<float> *m_collected_outhitz;

 std::vector<float> *m_collected_density;
 std::vector<std::string> *m_collected_material;
 std::vector<std::string> *m_collected_volume;
      
 std::vector<std::string> *m_collected_groupedmaterial;
 std::vector<std::string> *m_collected_volumetype;

std::map<std::string, std::pair<int, int>>  m_materialPlottingStyles;

TString atlasTEXT = "Internal";
float tolEta = 0.5;
bool doVERBOSEOUTPUT = false;
bool doMONITORING = false;
bool LengthIntegratorPlots(std::string filename, std::string outputdir, std::string geoname, std::string geoTag);
void defineMaterials();
TLegend* getLegend(float, bool);
void setPlotStyle();
void getLayer(float rHit, float zHit, int &be, int &layer);
std::string updateGrouping(std::string matname, std::string volname);


static void show_usage(std::string name) {

    std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Usage: " << name << " <option(s)> SOURCES" << std::endl;
    printf("Produce X0 Plots from LengthIntegrator output - Expecting TTree implemented in 21.9+\n");
    printf("--help\t\t\tShow this help message\n");
    printf("Necessary arguments\n");
    printf("--input\t\t\tfollowed by the output from LengthIntegrator\n");
    printf("--outdir\t\tfollowed by the name of the desirded output directory: [default : inputfile]\n");
    printf("--geoname\t\t\tfollowed by the name of the geometry: default will be [default :  ITk Layout]\n");
    printf("--geoTag\t\t\tfollowed by the geoTag: default will be [default :  ATLAS-P2-ITK-22-00-00]\n");
    printf("--domonitoring\t\twill reduce the output to plots to be used for monitoring\n");
    printf("Additional arguments\n");
    printf("--verbose\t\tgives verbose output for debugging\n");
    std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;

}

int main (int argc, char* argv[]) {

    std::cout << "Checking your command..." << std:: endl;
    for (int ar = 0; ar < argc; ar++) std::cout << std::string(argv[ar]) << " ";
    std::cout << std::endl;
    if(argc < 2) {
        show_usage(argv[0]);
        return 0;
    }
    std::cout << std::endl;

    std::string geoname = "ITk Layout";
    std::string geoTag = "ATLAS-P2-ITK-22-00-00";
    std::string outputdir = "./LengthIntegratorPlots"; 
    std::string filename = "";
   

    bool inputSet = false;
    bool outdirSet = false;
    bool nameSet = false;
    bool regexSet = false;
    for (int ar = 1; ar < argc; ++ar) {
        std::string arg = argv[ar];
        if(arg == "--help") {
            show_usage(argv[0]);
            return 0;
        } 
        else if(arg == "--input") { // getting input file
            if((ar+1 < argc) && (std::string(argv[ar+1]).find("--")==std::string::npos)) { // Make sure we aren't at the end of argv!
                filename = argv[ar+1];

                if(filename == "") {
                    std::cerr << "Define an inputfile..." << std::endl;
                    return 1;
                }
//                if(!fs::exists(filename)) {
//                    std::cerr << "File doesn't exist:  " << filename << std::endl;
//                    return 1;
//                }
//                fs::path p(filename);
//                outputdir = p.stem();
//                std::cout << "setting output to " << outputdir << std::endl;
                ar+=1; //Increment arg once to get next command
                inputSet = true;
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--input argument necessary to specify the input file..." << std::endl;
                return 1;
            }
        }
        else if(arg == "--outdir") { // getting output directory
            if((ar+1 < argc) && (std::string(argv[ar+1]).find("--")==std::string::npos)) { // Make sure we aren't at the end of argv!
                outputdir = (argv[ar+1] != "" && argv[ar+1] != " ") ? argv[ar+1] : outputdir;
                if(argv[ar+1] == "" || argv[ar+1] != " ") {
                    std::cout << "output directory command used but without an arguement, setting to default value: " << outputdir << std::endl;
                }
//                if(!fs::exists(outputdir)){
//                    if(fs::create_directory(outputdir)) std::cout << "Made directory for output: " << outputdir << std::endl;
//                    else {
//                        std::cerr << "Failed to make directory for output: " << outputdir << std::endl;
//                        return 1;
//                    }
//                }
                ar+=1; //Increment ar once to get next command
                outdirSet = true;
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--outdir option requires one argument." << std::endl;
                return 1;
            }
        }
        else if(arg == "--geoname") { // getting input directoris
            if((ar+1 < argc) && (std::string(argv[ar+1]).find("--")==std::string::npos)) { // Make sure we aren't at the end of argv!
                geoname = argv[ar+1] != "" ? argv[ar+1] : geoname;
                if(argv[ar+1] == "") {
                    std::cout << "geoname command used but without an argument, setting to default value:  " << geoname << std::endl;
                }
                ar+=1; //Increment arg once to get next command
                nameSet = true;
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--names option requires an argument." << std::endl;
                return 1;
            }
        }
        else if(arg == "--geoTag") { // getting input directoris
            if((ar+1 < argc) && (std::string(argv[ar+1]).find("--")==std::string::npos)) { // Make sure we aren't at the end of argv!
                geoTag = argv[ar+1] != "" ? argv[ar+1] : geoTag;
                if(argv[ar+1] == "") {
                    std::cout << "geoTag command used but without an argument, setting to default value:  " << geoTag << std::endl;
                }
                ar+=1; //Increment arg once to get next command
                nameSet = true;
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--names option requires an argument." << std::endl;
                return 1;
            }
        }
        else if(arg == "--verbose") { // getting output directory
            doVERBOSEOUTPUT = true;
        }
        else if(arg == "--domonitoring") { //reduce output for monitoring, save output in a rootfile
            doMONITORING = true;
        }
        else {
            std::cerr << argv[ar] << " undefined argument, check input: " << argv[0] << " --help for help :)" << std::endl;
            return 1;
        }
    }
    if(!inputSet) {
        std::cout << "Define --input path/to/dir1/file.root" << std::endl;
        return 1;
    }
    if(!outdirSet && !doMONITORING){
//        if(!fs::exists(outputdir)){
//            if(fs::create_directory(outputdir)) std::cout << "Made directory for output: " << outputdir << std::endl;
//            else {
//                std::cerr << "Failed to make directory for output: " << outputdir << std::endl;
//                return 1;
//            }
//         }

    }
    if(doVERBOSEOUTPUT){
        std::cout << std::endl;
        std::cout << "Running with configurations : " << std::endl;
        std::cout << "Input File : " << filename << std::endl;
        std::cout << "Path to output directory : " << outputdir << std::endl;
        std::cout << "For Geometry name : " << geoname << std::endl;
        std::cout << "For Geometry tag : " << geoTag << std::endl;
        std::cout << std::endl;
    }

    defineMaterials();
    
    TApplication theApp("Analysis", &argc, argv);
    if(!LengthIntegratorPlots(filename, outputdir, geoname, geoTag)) return 1;
  
    return 0;  
  
}

void defineMaterials(){

    int BeamPipeCol             = kGreen+3;
    int ActiveSensorsCol        = kCyan+1;
    int PixelChipsCol           = kOrange+2;
    int PixelHybridCol          = kViolet-3;
    int StripChipsCol           = kOrange+2;
    int StripHybridCol          = kViolet-3;
    int SupportStructureCol     = kGray+2;
    int TitaniumCoolingPipesCol = kRed+1;
    int ElectricalCablingCol    = kViolet-6;
    int ModeratorCol            = kGreen+1;
    int PP1Col                  = kAzure+1;
    int DryNitrogenCol          = 1;

    int BeamPipeHash             = 3013;//3544//3590
    int ActiveSensorsHash        = 3007;//3215
    int PixelChipsHash           = 3007;//3251
    int PixelHybridHash          = 3251;
    int StripChipsHash           = 3006;//3251
    int StripHybridHash          = 3544;
    int SupportStructureHash     = 3002;//3544
    int TitaniumCoolingPipesHash = 3425;
    int ElectricalCablingHash    = 3452;
    int ModeratorHash            = 3005;
    int PP1Hash                  = 3004;
    int DryNitrogenHash          = 3358;


    m_materialPlottingStyles[ "SupportStructure" ] = std::make_pair(SupportStructureCol, SupportStructureHash);
    m_materialPlottingStyles[ "Services" ]         = std::make_pair(ElectricalCablingCol, ElectricalCablingHash);
    m_materialPlottingStyles[ "Steel" ]            = std::make_pair(17, 17);
    m_materialPlottingStyles[ "FaradayCage" ]      = std::make_pair(15, 15);
    m_materialPlottingStyles[ "BeamPipe" ]         = std::make_pair(BeamPipeCol, BeamPipeHash);
    m_materialPlottingStyles[ "OtherSCT" ]         = std::make_pair(6, 6);
    m_materialPlottingStyles[ "OtherPixel"]        = std::make_pair(13, 13);
    m_materialPlottingStyles[ "Cooling" ]          = std::make_pair(TitaniumCoolingPipesCol, TitaniumCoolingPipesHash);
    m_materialPlottingStyles[ "Silicon" ]          = std::make_pair(9, 9);
    m_materialPlottingStyles[ "ActiveSensors" ]    = std::make_pair(ActiveSensorsCol, ActiveSensorsHash);
    m_materialPlottingStyles[ "Moderator" ]        = std::make_pair(ModeratorCol, ModeratorHash);
    m_materialPlottingStyles[ "PP0" ]              = std::make_pair(26, 26);
    m_materialPlottingStyles[ "PP1" ]              = std::make_pair(PP1Col, PP1Hash);
    m_materialPlottingStyles[ "Air" ]              = std::make_pair(DryNitrogenCol, DryNitrogenHash);
    m_materialPlottingStyles[ "PixelChips" ]       = std::make_pair(PixelChipsCol,  PixelChipsHash);
    m_materialPlottingStyles[ "PixelHybrids" ]     = std::make_pair(PixelHybridCol, PixelHybridHash);
    m_materialPlottingStyles[ "StripChips" ]       = std::make_pair(StripChipsCol,  StripChipsHash);
    m_materialPlottingStyles[ "StripHybrids" ]     = std::make_pair(StripHybridCol, StripHybridHash);
    m_materialPlottingStyles[ "NONE" ]             = std::make_pair(kCyan + 3, 3844);
    m_materialPlottingStyles[ "IST" ]              = std::make_pair(kSpring - 5, 3844);
    m_materialPlottingStyles[ "PST" ]              = std::make_pair(kSpring - 6, 3844);
    m_materialPlottingStyles[ "IPT" ]              = std::make_pair(kSpring + 4, 3844);
    m_materialPlottingStyles[ "HeatExchangers" ]   = std::make_pair(kBlack, 3854);
    
    m_materialPlottingStyles[ "AluminiumShell" ]   = std::make_pair(kOrange - 3, 3840);
    m_materialPlottingStyles[ "PP1Power" ]         = std::make_pair(kRed - 4, 3842);
    m_materialPlottingStyles[ "PP1Cooling" ]       = std::make_pair(kPink - 9, 3843);
    m_materialPlottingStyles[ "PP1Data" ]          = std::make_pair(kViolet - 3, 3845);
    m_materialPlottingStyles[ "SiliconModules" ]   = std::make_pair(PixelChipsCol,  PixelChipsHash);
    m_materialPlottingStyles[ "PixelModules" ]    = std::make_pair(PixelChipsCol,  PixelChipsHash);
    m_materialPlottingStyles[ "StripModules" ]    = std::make_pair(StripChipsCol,  StripChipsHash);
    m_materialPlottingStyles[ "PixelServicesCooling" ] = std::make_pair(ElectricalCablingCol, ElectricalCablingHash);
    m_materialPlottingStyles[ "StripServicesCooling" ] = std::make_pair(StripHybridCol, StripHybridHash);
    m_materialPlottingStyles[ "PixelSupports" ] = std::make_pair(SupportStructureCol, SupportStructureHash);
    m_materialPlottingStyles[ "StripSupports" ] = std::make_pair(TitaniumCoolingPipesCol, TitaniumCoolingPipesHash);
}

void setPlotStyle()
{
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000 };

  //kLightTemperature
  Double_t red[9]   = {  31./255.,  71./255., 123./255., 160./255., 210./255., 222./255., 214./255., 199./255., 183./255.};
  Double_t green[9] = {  40./255., 117./255., 171./255., 211./255., 231./255., 220./255., 190./255., 132./255.,  65./255.};
  Double_t blue[9]  = { 234./255., 214./255., 228./255., 222./255., 210./255., 160./255., 105./255.,  60./255.,  34./255.};

  TColor::CreateGradientColorTable(9, stops, red, green, blue, 255);
  gStyle->SetNumberContours(255);
  gStyle->SetOptTitle(0);
  gStyle->SetPaperSize(20,26);

  // set margin sizes
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

}



TLegend* getLegend(int nEntries = 2, float textSize=0.04, bool doBias = false){

  float xmin, xmax, ymin, ymax;

  ymax = 0.93; ymin = ymax - 0.05*nEntries;
  xmin = 0.19; xmax = 0.42;
  
  TLegend *l = new TLegend(xmin, ymin, xmax, ymax);
  l->SetFillColor(kWhite);
  l->SetBorderSize(0);
  l->SetTextSize(textSize);
  l->SetTextFont(42);
  if (doBias) l->SetTextSize(0.03);
  return l;
}


void atlasLABEL(Double_t x,Double_t y,Color_t color,Double_t tsize=0.03) 
{

  TLatex l; //l.SetTextAlign(12);
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");

}

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize=0.10, Float_t angle=0, int noNDC=0) {

 // Double_t tsize=0.05;
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(tsize); 
  if(!noNDC)l.SetNDC();
  l.SetTextColor(color);
  l.SetTextFont(42);
  if (angle != 0) l.SetTextAngle(angle);
  l.DrawLatex(x,y,text);

}
void drawATLAS(TString t, TString tc = "Simulation", TString format = "", TString smpl = ""){
  float x, y;
  if (t.Contains("Top"))      y = 0.87;
  if (t.Contains("Right"))    x = 0.59;
  if (t.Contains("Center"))   x = 0.45;
  if (t.Contains("Left"))     x = 0.18;
  
  atlasLABEL(x, y, kBlack, 0.06); 
  myText(x, y - 0.06, kBlack, tc, 0.06);
  myText(x + 0.15, y, kBlack, atlasTEXT, 0.06);
}


bool LengthIntegratorPlots(std::string filename, std::string outputdir, std::string geoname, std::string geoTag) {
  
    gStyle->SetOptStat(0);
    //uncomment if you want to use your faviour ATLAS style
    //    gROOT->LoadMacro("AtlasStyle.C");
    //    SetAtlasStyle(); 
    setPlotStyle();

    m_genNPart = 0;
    m_genEta = 0;
    m_genPhi = 0;
    m_genZ = 0;
    m_genR = 0;
      
    m_total_X0 = 0;
    m_total_L0 = 0;

    m_collected_X0 = 0;
    m_collected_L0 = 0;

    m_collected_inhitr = 0;
    m_collected_inhitz = 0;

    m_collected_outhitr = 0;
    m_collected_outhitz = 0;

    m_collected_density = 0;
    m_collected_material = 0;
    m_collected_volume = 0;
      
    m_collected_groupedmaterial = 0;
    m_collected_volumetype = 0;

    //Set up le tree
    TFile *file = new TFile(filename.c_str());
    TTree *larch =  (TTree*) file->Get("TheLarch");

    larch->SetBranchAddress("genEta",                     &m_genEta);
    larch->SetBranchAddress("genPhi",                     &m_genPhi);
    larch->SetBranchAddress("genZ",                       &m_genZ);
    larch->SetBranchAddress("genR",                       &m_genR);
    larch->SetBranchAddress("total_X0",                   &m_total_X0);
    larch->SetBranchAddress("total_L0",                   &m_total_L0);
    larch->SetBranchAddress("collected_X0",               &m_collected_X0);
    larch->SetBranchAddress("collected_L0",               &m_collected_L0);

    larch->SetBranchAddress("collected_inhitr",           &m_collected_inhitr);
    larch->SetBranchAddress("collected_inhitz",           &m_collected_inhitz);

    larch->SetBranchAddress("collected_outhitr",          &m_collected_outhitr);
    larch->SetBranchAddress("collected_outhitz",          &m_collected_outhitz);
    
    larch->SetBranchAddress("collected_density",          &m_collected_density);
    larch->SetBranchAddress("collected_material",         &m_collected_material);
    larch->SetBranchAddress("collected_volume",           &m_collected_volume);
    larch->SetBranchAddress("collected_groupedmaterial",  &m_collected_groupedmaterial);
    larch->SetBranchAddress("collected_volumetype",       &m_collected_volumetype);
    
    float m_zmax = 3750.0; float m_rmax = 1200.0;
    const int nHitMax = 9; //Max number of Si to count X0 for

    TFile *outfile;
    if(doMONITORING) outfile = new TFile("MaterialPlots.root","RECREATE");
    

    std::vector<std::string> materialType = { "Moderator", 
					      "NONE", 
					      "FaradayCage", 
					      "Steel", 
					      "OtherSCT", 
					      "OtherPixel", 
					      "PP0", 
					      "IST", 
					      "PST", 
					      "HeatExchangers",
					      "PP1", 
					      "AluminiumShell", 
					      "PP1Power", 
					      "PP1Cooling", 
					      "PP1Data", 
					      "Air", 
					      "Services", 
					      "Cooling", 
					      "SupportStructure", 
					      "StripServicesCooling",
					      "StripSupports",
					      "StripModules",
					      "StripChips", 
					      "StripHybrids",
					      "PixelServicesCooling",
					      "PixelSupports",
					      "PixelModules",
					      "PixelChips", 
					      "PixelHybrids",
					      "ActiveSensors",  
					      "IPT", 
					      "BeamPipe"};

    Color_t colors[11] = { kBlack, kBlue, kBlue - 9, kMagenta - 4, kRed - 3, kRed - 9, kSpring + 3,
                           kSpring - 5, kGreen - 7, kCyan + 2, kViolet - 6 };


    std::map<std::string, TProfile*> m_x0vseta_permaterial;
    std::map<std::string, TProfile*> m_x0vsphi_permaterial;

    std::map<std::string, TH2D*> m_x0vsrz_permaterial;

    std::map<std::string, TProfile*> m_l0vseta_permaterial;
    std::map<std::string, TProfile*> m_l0vsphi_permaterial;

    std::map<std::string, TH2D*> m_l0vsrz_permaterial;

    //Total X0 + L0
    TProfile *p_x0vseta = new TProfile("x0vseta","x0vseta; #eta; X_{0}",200,0.0,6.0);
    TProfile *p_l0vseta = new TProfile("l0vseta","l0vseta; #eta; X_{0}",200,0.0,6.0);
    TProfile *p_x0vsphi = new TProfile("x0vsphi","x0vsphi; #phi; X_{0}",200,-3.5,3.5);
    TProfile *p_l0vsphi = new TProfile("l0vsphi","l0vsphi; #phi; X_{0}",200,-3.5,3.5);

//     TProfile2D *p_x0vsetaphi = new TProfile2D("p_x0vsetaphi","x0vsetaphi; #eta; #phi [rad]; X_{0}",200,0.0,6.0,200,-3.5,3.5);
//     TProfile2D *p_l0vsetaphi = new TProfile2D("p_l0vsetaphi","l0vsetaphi; #eta; #phi [rad]; X_{0}",200,0.0,6.0,200,-3.5,3.5);
    
    TProfile2D *p_x0vsetaphi = new TProfile2D("p_x0vsetaphi","x0vsetaphi; #eta; #phi; Radiation Lengths [X_{0}]",320,-4.8,4.8,225,-3.5,5.5);
    TProfile2D *p_l0vsetaphi = new TProfile2D("p_l0vsetaphi","l0vsetaphi; #eta; #phi; Radiation Lengths [X_{0}]",320,-4.8,4.8,225,-3.5,5.5);
    
    TProfile2D *p_x0vsetaphi_pos = new TProfile2D("p_x0vsetaphi_pos","x0vsetaphi_pos; #eta; #phi; Radiation Lengths [X_{0}]",160,0.0,4.8,225,-3.5,5.5);
    TProfile2D *p_l0vsetaphi_pos = new TProfile2D("p_l0vsetaphi_pos","l0vsetaphi_pos; #eta; #phi; Radiation Lengths [X_{0}]",160,0.0,4.8,225,-3.5,5.5);
    
    TProfile2D *p_x0vsetaphi_neg = new TProfile2D("p_x0vsetaphi_neg","x0vsetaphi_neg; #eta; #phi; Radiation Lengths [X_{0}]",160,-4.8,0.0,225,-3.5,5.5);
    TProfile2D *p_l0vsetaphi_neg = new TProfile2D("p_l0vsetaphi_neg","l0vsetaphi_neg; #eta; #phi; Radiation Lengths [X_{0}]",160,-4.8,0.0,225,-3.5,5.5);


    const int nMat = materialType.size();

    TProfile* p_tmp[nMat]; TH2D* h_tmp[nMat];

    //X0 for layers and before x hits
    TProfile* p_x0vseta_layers[2][11]; 
    TProfile* p_x0vseta_countHits[nHitMax];
    
    //L0 for layers and before x hits
    TProfile* p_l0vseta_layers[2][11]; 
    TProfile* p_l0vseta_countHits[nHitMax];


    //Splitting these for now but might be best to re-write this a bit better    
    //Histograms for x0
    for(unsigned int ip = 0; ip < materialType.size(); ip++){ 
        p_tmp[ip] = new TProfile(("profiles_x0vseta_"+materialType[ip]).c_str(),("profiles_x0vseta_"+materialType[ip]+"; #eta; X_{0}").c_str(),500,0.0,6.0);
        p_tmp[ip]->SetFillColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetFillStyle(m_materialPlottingStyles[materialType[ip]].second);
        p_tmp[ip]->SetLineColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetMarkerStyle(0);
        m_x0vseta_permaterial[materialType.at(ip)] = p_tmp[ip];
        if(doVERBOSEOUTPUT) std::cout << "Created TProfile: " << ("profiles_x0"+materialType[ip]).c_str() << std::endl;

        p_tmp[ip] = new TProfile(("profiles_x0vsphi_"+materialType[ip]).c_str(),("profiles_x0vsphi_"+materialType[ip]+"; #phi; X_{0}").c_str(),500,-3.5,3.5);
        p_tmp[ip]->SetFillColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetFillStyle(m_materialPlottingStyles[materialType[ip]].second);
        p_tmp[ip]->SetLineColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetMarkerStyle(0);
        m_x0vsphi_permaterial[materialType.at(ip)] = p_tmp[ip];
        if(doVERBOSEOUTPUT) std::cout << "Created TProfile: " << ("profiles_x0"+materialType[ip]).c_str() << std::endl;

        h_tmp[ip] = new TH2D(("2d_"+materialType.at(ip)).c_str(),("2d_"+materialType.at(ip)+"; z [mm]; r [mm]; X_{0}").c_str(),4000,-m_zmax,m_zmax,1400,0.,m_rmax+200.0);
        m_x0vsrz_permaterial[materialType.at(ip)] = h_tmp[ip];   
    }

    for(unsigned int iec = 0; iec < 2; iec++){
        for(unsigned int il = 0; il < 11; il++){
            std::string hname = "profiles_x0vseta_barrelendcap" + std::to_string(iec) + "_layerdisk" + std::to_string(il);
            p_x0vseta_layers[iec][il] = new TProfile(hname.c_str(),(hname+"; #eta; X_{0}").c_str(),500,0.0,6.0);
            p_x0vseta_layers[iec][il]->SetLineColor(colors[il]); 
            p_x0vseta_layers[iec][il]->SetMarkerColor(colors[il]);
            p_x0vseta_layers[iec][il]->SetLineWidth(3);
            if(iec > 0) p_x0vseta_layers[iec][il]->SetLineStyle(7);

        }
    }

    for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
        std::string hname = "profiles_x0vseta_nhits" + std::to_string(ihit);
        p_x0vseta_countHits[ihit] = new TProfile(hname.c_str(),(hname+"; #eta; X_{0}").c_str(),500,0.0,6.0);
        p_x0vseta_countHits[ihit]->SetLineColor(ihit+29);
        p_x0vseta_countHits[ihit]->SetMarkerColor(ihit+29);
        p_x0vseta_countHits[ihit]->SetLineWidth(3);
    }

    //Histograms for l0
    for(unsigned int ip = 0; ip < materialType.size(); ip++){ 
        p_tmp[ip] = new TProfile(("profiles_l0vseta_"+materialType[ip]).c_str(),("profiles_l0vseta_"+materialType[ip]+"; #eta; #Lambda_{0}").c_str(),500,0.0,6.0);
        p_tmp[ip]->SetFillColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetFillStyle(m_materialPlottingStyles[materialType[ip]].second);
        p_tmp[ip]->SetLineColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetMarkerStyle(0);
        m_l0vseta_permaterial[materialType.at(ip)] = p_tmp[ip];
        if(doVERBOSEOUTPUT) std::cout << "Created TProfile: " << ("profiles_l0"+materialType[ip]).c_str() << std::endl;

        p_tmp[ip] = new TProfile(("profiles_l0vsphi_"+materialType[ip]).c_str(),("profiles_l0vsphi_"+materialType[ip]+"; #phi; #Lambda_{0}").c_str(),500,-3.5,3.5);
        p_tmp[ip]->SetFillColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetFillStyle(m_materialPlottingStyles[materialType[ip]].second);
        p_tmp[ip]->SetLineColor(m_materialPlottingStyles[materialType[ip]].first);
        p_tmp[ip]->SetMarkerStyle(0);
        m_l0vsphi_permaterial[materialType.at(ip)] = p_tmp[ip];
        if(doVERBOSEOUTPUT) std::cout << "Created TProfile: " << ("profiles_l0"+materialType[ip]).c_str() << std::endl;


    }

    for(unsigned int iec = 0; iec < 2; iec++){
        for(unsigned int il = 0; il < 11; il++){
            std::string hname = "profiles_l0vseta_barrelendcap" + std::to_string(iec) + "_layerdisk" + std::to_string(il);
            p_l0vseta_layers[iec][il] = new TProfile(hname.c_str(),(hname+"; #eta; #Lambda_{0}").c_str(),500,0.0,6.0);
            p_l0vseta_layers[iec][il]->SetLineColor(colors[il]); 
            p_l0vseta_layers[iec][il]->SetMarkerColor(colors[il]);
            p_l0vseta_layers[iec][il]->SetLineWidth(3);
            if(iec > 0) p_l0vseta_layers[iec][il]->SetLineStyle(7);

        }
    }

    for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
        std::string hname = "profiles_l0vseta_nhits" + std::to_string(ihit);
        p_l0vseta_countHits[ihit] = new TProfile(hname.c_str(),(hname+"; #eta; #Lambda_{0}").c_str(),500,0.0,6.0);
        p_l0vseta_countHits[ihit]->SetLineColor(ihit+29);
        p_l0vseta_countHits[ihit]->SetMarkerColor(ihit+29);
        p_l0vseta_countHits[ihit]->SetLineWidth(3);
    }

    ///////////////////////////////////////////
    std::map<std::string, double> summed_X0;
    std::map<std::string, double> summed_L0;
   

    //Loop over all the collected steps 
    Long64_t nentries = larch->GetEntries();
//     nentries = 20000;
    std::cout << "----Reading tree with " << nentries << " entries" << std::endl; 
    for (Long64_t i=0;i<nentries;i++) {
      larch->GetEntry(i);
      if(doVERBOSEOUTPUT && i%1000 == 0) std::cout << "Process entry:  " << i << std::endl;

      summed_X0.clear();
      double total_X0_ITk = 0;
      double m_x0vseta_layers[2][11] = { 0 };
      double m_x0vseta_countHits[nHitMax] = { 0 };

      summed_L0.clear();
      double total_L0_ITk = 0;
      double m_l0vseta_layers[2][11] = { 0 };
      double m_l0vseta_countHits[nHitMax] = { 0 };


      int countHits = 0;
      //Count X0 and L0
      for(unsigned int ci = 0; ci < m_collected_inhitr->size(); ci++)
      {
        if(fabs(m_collected_inhitz->at(ci)) < m_zmax && m_collected_inhitr->at(ci) < m_rmax)
        {
            total_X0_ITk+=m_collected_X0->at(ci);
            std::string fillname = m_collected_groupedmaterial->at(ci);
            //Temporary grouping for step 3.1...
           /*
            if(fillname == "Titanium"){
              if(fabs(m_collected_inhitz->at(ci)) > 3000. || fabs(m_collected_outhitz->at(ci)) > 3000.) fillname = "PP1";
              else { fillname = "Cooling"; }
            }
            if(fillname == "Copper"){
              if(fabs(m_collected_inhitz->at(ci)) > 3000. || fabs(m_collected_outhitz->at(ci)) > 3000.) fillname = "PP1";
              else { fillname = "Services"; }
            }
            if(fillname == "Aerogel") {
              fillname = "BeamPipe"; 
            }
            */
            std::string updatedname = updateGrouping(m_collected_material->at(ci), m_collected_volume->at(ci));
            if(updatedname != "NoUpdate") fillname = updatedname;


	    // Ugly hack for nicely formatted plots
	    if (fillname == "AluminiumShell" || fillname == "PP1Power" || fillname == "PP1Cooling" || fillname == "PP1Data" || fillname == "HeatExchangers") 
	      fillname = "PP1";
	    if (fillname == "StripChips" || fillname ==  "StripHybrids") 
	      fillname = "StripModules";

	    if (fillname == "PixelChips" || fillname == "PixelHybrids")
	      fillname = "PixelModules";

	    if (fillname == "ActiveSensors") {
	      if (m_collected_volume->at(ci).find("Pixel") != std::string::npos) 
		fillname = "PixelModules";
	      else if (m_collected_volume->at(ci).find("SCT") != std::string::npos) 
		fillname = "StripModules";
	    }
	     

	    if (fillname == "SupportStructure") {
	      if (m_collected_volume->at(ci).find("Pixel") != std::string::npos)
		fillname = "PixelSupports";
	      else if (m_collected_volume->at(ci).find("SCT") != std::string::npos)
		fillname = "StripSupports";
	    }

 
	    if (fillname == "Services") {
	      if (m_collected_volume->at(ci).find("Pixel") != std::string::npos) {
		// Do nothing
		//fillname = "PixelServices";
	      } else if (m_collected_volume->at(ci).find("SCT") != std::string::npos) {
		//fillname = "StripServices";
	      }
	      else
		fillname = "BeamPipe"; // what's left is apparently in the beam pipe
	    }
	    
	    if (fillname == "Cooling" || fillname == "Services") {
	      if (m_collected_volume->at(ci).find("Pixel") != std::string::npos) 
		fillname = "PixelServicesCooling";
	      else if (m_collected_volume->at(ci).find("SCT") != std::string::npos) 
		fillname = "StripServicesCooling";
	    }

	    if (fillname == "IPT") fillname = "BeamPipe";


	    // if((fabs(m_collected_inhitz->at(ci)) > 3000. || fabs(m_collected_outhitz->at(ci)) > 3000.) && fillname == "Cooling" ) fillname = "PP1";
            summed_X0[fillname]+=m_collected_X0->at(ci);
            m_x0vsrz_permaterial[fillname]->Fill(m_collected_inhitz->at(ci),m_collected_inhitr->at(ci),m_collected_X0->at(ci)/2.0); 
            m_x0vsrz_permaterial[fillname]->Fill(m_collected_outhitz->at(ci),m_collected_outhitr->at(ci),m_collected_X0->at(ci)/2.0);           

            total_L0_ITk+=m_collected_L0->at(ci);
            summed_L0[fillname]+=m_collected_L0->at(ci);
	    //  m_l0vsrz_permaterial[fillname]->Fill(m_collected_inhitz->at(ci),m_collected_inhitr->at(ci),m_collected_L0->at(ci)/2.0); 
	    //  m_l0vsrz_permaterial[fillname]->Fill(m_collected_outhitz->at(ci),m_collected_outhitr->at(ci),m_collected_L0->at(ci)/2.0);           

            //Count X0 belonging roughly to each layer
            int be = -2; int layer = -2;
            getLayer(m_collected_inhitr->at(ci), m_collected_inhitz->at(ci), be, layer);
            if(be >= 0 && layer >= 0) {
              m_x0vseta_layers[be][layer]+=m_collected_X0->at(ci);
              m_l0vseta_layers[be][layer]+=m_collected_L0->at(ci);
            }

            //Count X0 before x silicon hits
            if(countHits < nHitMax) {
              int ihit = nHitMax-1;
              while(ihit > countHits){
                m_x0vseta_countHits[ihit]+=m_collected_X0->at(ci);
                m_l0vseta_countHits[ihit]+=m_collected_L0->at(ci);
                ihit--;
              }
            }
            if(m_collected_groupedmaterial->at(ci) == "ActiveSensors") countHits++;
        }
      }

      //Fill TProfile of the x0
      p_x0vseta->Fill(fabs(m_genEta),total_X0_ITk);
      p_x0vsphi->Fill(m_genPhi,total_X0_ITk);

      p_x0vsetaphi->Fill(m_genEta,m_genPhi,total_X0_ITk);
      p_l0vsetaphi->Fill(m_genEta,m_genPhi,total_L0_ITk);
      
      if (m_genEta>=0.) {
        p_x0vsetaphi_pos->Fill(m_genEta,m_genPhi,total_X0_ITk);
        p_l0vsetaphi_pos->Fill(m_genEta,m_genPhi,total_L0_ITk);
      } else {
        p_x0vsetaphi_neg->Fill(m_genEta,m_genPhi,total_X0_ITk);
        p_l0vsetaphi_neg->Fill(m_genEta,m_genPhi,total_L0_ITk);        
      }
      
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
        //  if(summed_X0[materialType.at(imat)] > 1e-11){
            m_x0vseta_permaterial[materialType.at(imat)]->Fill(fabs(m_genEta), summed_X0[materialType.at(imat)]);
            if(fabs(m_genEta) > 2.4 && fabs(m_genEta) < 4.2)m_x0vsphi_permaterial[materialType.at(imat)]->Fill(m_genPhi, summed_X0[materialType.at(imat)]);
           
        //  }
      }

      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 0; il < 11; il++){
            p_x0vseta_layers[iec][il]->Fill(fabs(m_genEta),m_x0vseta_layers[iec][il]);
          }
      }

      //Limit this abit ...
      for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
          p_x0vseta_countHits[ihit]->Fill(fabs(m_genEta),m_x0vseta_countHits[ihit]);
      }

      //separate these for now to easily limit the output if necessary ...
      //Fill TProfile of the x0
      p_l0vseta->Fill(fabs(m_genEta),total_L0_ITk);
      p_l0vsphi->Fill(m_genPhi,total_L0_ITk);
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
            m_l0vseta_permaterial[materialType.at(imat)]->Fill(fabs(m_genEta), summed_L0[materialType.at(imat)]);
            if(fabs(m_genEta) > 2.4 && fabs(m_genEta) < 4.2)m_l0vsphi_permaterial[materialType.at(imat)]->Fill(m_genPhi, summed_L0[materialType.at(imat)]);     
      }

      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 0; il < 11; il++){
            p_l0vseta_layers[iec][il]->Fill(fabs(m_genEta),m_l0vseta_layers[iec][il]);
          }
      }

      //Limit this abit ...
      for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
          p_l0vseta_countHits[ihit]->Fill(fabs(m_genEta),m_l0vseta_countHits[ihit]);
      }



      if(doVERBOSEOUTPUT){
          for(auto x : summed_X0) {
              if(std::find(materialType.begin(), materialType.end(), x.first) == materialType.end()){
                  std::cout << "Missing material: " << x.first << std::endl;
              }
          }
      }

    }

    //Dump necessary histograms for monitoring, x0 before x hits and per layer
    if(doMONITORING){ 
       /*
        for(unsigned int ip = 0; ip < materialType.size(); ip++) {
          if(m_x0vsrz_permaterial[materialType.at(ip)]->Integral() < 1e-7) continue;
          _x0vsrz_permaterial[materialType.at(ip)]->Write();
        }
       */
        for(unsigned int iec = 0; iec < 2; iec++){
           for(unsigned int il = 0; il < 10; il++){
              if(il > 4 && iec == 0) continue;
              p_x0vseta_layers[iec][il]->Write();
              p_l0vseta_layers[iec][il]->Write();
           }
        }

        for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
          p_x0vseta_countHits[ihit]->Write();
          p_l0vseta_countHits[ihit]->Write();
        }
        outfile->Close();
        delete outfile;
        return 1;
    }
    else{

      TCanvas *ctmp = new TCanvas("ctmp","ctmp",0.,0.,800,600);
      ctmp->cd();
      p_x0vseta->SetAxisRange(0.,5.0,"Y");
      p_x0vseta->SetAxisRange(0.,6.0,"X");
      p_x0vseta->Draw();
      ctmp->Print((outputdir + "/x0vseta.png").c_str());
      ctmp->Print((outputdir + "/x0vseta.pdf").c_str());
      ctmp->Print((outputdir + "/x0vseta.eps").c_str());

      ctmp->cd();
      p_x0vsphi->SetAxisRange(0.,5.0,"Y");
      p_x0vsphi->SetAxisRange(0.,6.0,"X");
      p_x0vsphi->Draw();
      ctmp->Print((outputdir + "/x0vsphi.png").c_str());
      ctmp->Print((outputdir + "/x0vsphi.pdf").c_str());
      ctmp->Print((outputdir + "/x0vsphi.eps").c_str());

      TH1F* hdummyeta = new TH1F("dummyeta","dummy; #eta; Radiation Lengths [X_{0}]",200,0.0,4.8); // was 0.0 to 6.5
      hdummyeta->SetAxisRange(0.,3.2,"Y");
      hdummyeta->Draw();

      THStack *hseta = new THStack("hseta", "hseta; #eta; Radiation Lengths [X_{0}]");
      TLegend *leta = getLegend(9);
      TH1F* heta[nMat];
      std::vector<TH1F*> vhisteta; 
      std::vector<int> pieseta;
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
         heta[imat] = (TH1F*)m_x0vseta_permaterial[materialType.at(imat)]->ProjectionX();
         heta[imat]->SetFillColor(m_materialPlottingStyles[materialType[imat]].first);
         heta[imat]->SetFillStyle(m_materialPlottingStyles[materialType[imat]].second);
         heta[imat]->SetLineColor(m_materialPlottingStyles[materialType[imat]].first);
         heta[imat]->SetMarkerStyle(0);
         if(heta[imat]->Integral() > 1e-7 && materialType.at(imat) != "Moderator" /* && materialType.at(imat) != "NONE"*/){
          std::string legendname = materialType.at(imat);
          if(legendname == "Air") legendname = "Dry Nitrogen";
          if(legendname == "SiliconModules")   legendname = "Silicon modules";
	  if(legendname == "HeatExchangers")   legendname = "Heat exchangers";
	  if(legendname == "BeamPipe")         legendname = "Beam pipe and IPT";
	  if(legendname == "SupportStructure") legendname = "Support structure";
	  if(legendname == "PixelModules")     legendname = "Pixel modules";
	  if(legendname == "StripModules")     legendname = "Strip modules";
	  if(legendname == "PixelServicesCooling")    legendname = "Pixel services and cooling";
	  if(legendname == "StripServicesCooling")    legendname = "Strip services and cooling";
	  if(legendname == "PixelSupports") legendname = "Pixel supports";
	  if(legendname == "StripSupports") legendname = "Strip supports";
	  if(legendname == "PP1") legendname = "PP1 and enclosure";
	  
          leta->AddEntry(heta[imat],legendname.c_str(),"fl");
          pieseta.push_back(imat);
        }
      }


    for(int imat = pieseta.size()-1; imat >= 0; imat--){
      vhisteta.push_back(heta[pieseta.at(imat)]);
    }

      for( auto v : vhisteta) hseta->Add(v);
      

      hseta->Draw("histsame");
      leta->Draw();
      drawATLAS("TopRight","Simulation");
      myText(0.59,0.76,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.71,kBlack,geoTag.c_str(),0.03); 
      hdummyeta->Draw("axis same");
      ctmp->Print((outputdir + "/MaterialType_x0vseta.png").c_str());
      ctmp->Print((outputdir + "/MaterialType_x0vseta.pdf").c_str());
      ctmp->Print((outputdir + "/MaterialType_x0vseta.eps").c_str());
///////////////phi

      TH1F* hdummyphi = new TH1F("dummyphi","dummy; #phi; Radiation Lengths [X_{0}]",200,-3.5,3.5);
      hdummyphi->SetAxisRange(0.,6.0,"Y");
      hdummyphi->Draw();

      THStack *hsphi = new THStack("hsphi", "hsphi; #phi; Radiation Lengths [X_{0}]");
      TLegend *lphi = getLegend(9);
      TH1F* hphi[nMat];
      std::vector<TH1F*> vhistphi; 
      std::vector<int> piesphi;
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
         hphi[imat] = (TH1F*)m_x0vsphi_permaterial[materialType.at(imat)]->ProjectionX();
         hphi[imat]->SetFillColor(m_materialPlottingStyles[materialType[imat]].first);
         hphi[imat]->SetFillStyle(m_materialPlottingStyles[materialType[imat]].second);
         hphi[imat]->SetLineColor(m_materialPlottingStyles[materialType[imat]].first);
         hphi[imat]->SetMarkerStyle(0);
         if(hphi[imat]->Integral() > 1e-7 && materialType.at(imat) != "Moderator" /* && materialType.at(imat) != "NONE"*/){
          std::string legendname = materialType.at(imat);
          if(legendname == "Air") legendname = "Dry Nitrogen";
          if(legendname == "SiliconModules")   legendname = "Silicon modules";
	  if(legendname == "HeatExchangers")   legendname = "Heat exchangers";
	  if(legendname == "BeamPipe")         legendname = "Beam pipe and IPT";
	  if(legendname == "SupportStructure") legendname = "Support structure";
	  if(legendname == "PixelModules")     legendname = "Pixel modules";
	  if(legendname == "StripModules")     legendname = "Strip modules";
	  if(legendname == "PixelServicesCooling")    legendname = "Pixel services and cooling";
	  if(legendname == "StripServicesCooling")    legendname = "Strip services and cooling";
	  if(legendname == "PixelSupports") legendname = "Pixel supports";
	  if(legendname == "StripSupports") legendname = "Strip supports";
	  if(legendname == "PP1") legendname = "PP1 and enclosure";

          lphi->AddEntry(hphi[imat],legendname.c_str(),"fl");
          piesphi.push_back(imat);
        }
      }

    for(int imat = piesphi.size()-1; imat >= 0; imat--){
      vhistphi.push_back(hphi[piesphi.at(imat)]);
    }

    for( auto v : vhistphi) hsphi->Add(v);
      

      hsphi->Draw("histsame");
      lphi->Draw();
      drawATLAS("TopRight","Simulation");
      myText(0.59,0.76,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.71,kBlack,geoTag.c_str(),0.03); 
      ctmp->Print((outputdir + "/MaterialType_x0vsphi.png").c_str());
      ctmp->Print((outputdir + "/MaterialType_x0vsphi.pdf").c_str());
      ctmp->Print((outputdir + "/MaterialType_x0vsphi.eps").c_str());

//////////////phi    
      for(unsigned int ip = 0; ip < materialType.size(); ip++){
        if(!(m_x0vsrz_permaterial[materialType.at(ip)]->Integral() > 1e-7)) continue;

          m_x0vsrz_permaterial[materialType.at(ip)]->Draw("colz");
          drawATLAS("TopRight", "Simulation");
          myText(0.22,0.87,kBlack,(geoname).c_str(),0.04);
          myText(0.22,0.81,kBlack,(materialType.at(ip).c_str()),0.04);
          ctmp->Print((outputdir + "/MaterialType_x0vsrz_" + materialType.at(ip) + ".png").c_str());
	  ctmp->Print((outputdir + "/MaterialType_x0vsrz_" + materialType.at(ip) + ".pdf").c_str());
	  ctmp->Print((outputdir + "/MaterialType_x0vsrz_" + materialType.at(ip) + ".eps").c_str());
      }
    
      TLegend *l2 = getLegend(10);


      hdummyeta->SetAxisRange(0.,1.4,"Y"); 
      hdummyeta->Draw();
      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 0; il < 5; il++){ //Pixels
              if(iec == 0 && il > 8) continue;

              std::string det = il > 4 ? "Strip" : "Pixel";
              std::string part = iec == 0 ? "Barrel" : "Disk";
              std::string legendname = det + "  " + part;
              legendname += il > 4 ? std::to_string(il-5) : std::to_string(il);
              p_x0vseta_layers[iec][il]->Draw("histsame");
              l2->AddEntry(p_x0vseta_layers[iec][il],legendname.c_str(),"lp");
          }
      }
      l2->Draw();
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.45,0.86,kBlack,"Pixel",0.04);      
      ctmp->Print((outputdir +  "/x0vseta_pixels.png").c_str());
      ctmp->Print((outputdir +  "/x0vseta_pixels.pdf").c_str());
      ctmp->Print((outputdir +  "/x0vseta_pixels.eps").c_str());
    
      TLegend *l3 = getLegend(10);
      hdummyeta->SetAxisRange(0.,0.4,"Y");
      hdummyeta->SetAxisRange(0.,3.1,"X");
      hdummyeta->Draw();
      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 5; il < 11; il++){ //Pixels
              if(iec == 0 && il > 8) continue;
              std::string det = il > 4 ? "Strip" : "Pixel";
              std::string part = iec == 0 ? "Barrel" : "Disk";
              std::string legendname = det + "  " + part;
              legendname += il > 4 ? std::to_string(il-5) : std::to_string(il);
     
              p_x0vseta_layers[iec][il]->Draw("histsame");
              l3->AddEntry(p_x0vseta_layers[iec][il],legendname.c_str(),"lp");
         }

      }
      l3->Draw();
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.45,0.86,kBlack,"Strips",0.04);      

      ctmp->Print((outputdir +  "/x0vseta_strips.png").c_str());
      ctmp->Print((outputdir +  "/x0vseta_strips.pdf").c_str());
      ctmp->Print((outputdir +  "/x0vseta_strips.eps").c_str());

//    TLegend *l4 = getLegend(9);
      hdummyeta->SetAxisRange(0.,0.60,"Y");
      hdummyeta->SetAxisRange(0.,4.0,"X");
      hdummyeta->Draw();

      for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
        p_x0vseta_countHits[ihit]->Draw("same");
      }
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.22,0.86,kBlack,"X0 before X hits",0.04);      

      ctmp->Print((outputdir +  "/x0vseta_sihits.png").c_str());
      ctmp->Print((outputdir +  "/x0vseta_sihits.pdf").c_str());
      ctmp->Print((outputdir +  "/x0vseta_sihits.eps").c_str());

      //Split this to be able to easily comment out plots... perhaps loops over loops are necessary
      p_l0vseta->SetAxisRange(0.,1.0,"Y");
      p_l0vseta->SetAxisRange(0.,6.0,"X");
      p_l0vseta->Draw();
      ctmp->Print((outputdir + "/l0vseta.png").c_str());
      ctmp->Print((outputdir + "/l0vseta.pdf").c_str());
      ctmp->Print((outputdir + "/l0vseta.eps").c_str());

      p_l0vsphi->SetAxisRange(0.,1.0,"Y");
      p_l0vsphi->SetAxisRange(-3.5,3.5,"X");
      p_l0vsphi->Draw();
      ctmp->Print((outputdir + "/l0vsphi.png").c_str());
      ctmp->Print((outputdir + "/l0vsphi.pdf").c_str());
      ctmp->Print((outputdir + "/l0vsphi.eps").c_str());

      delete hdummyeta;

      hdummyeta = new TH1F("dummyeta","dummy; #eta; #Lambda_{0}",200,0.0,4.8);
      hdummyeta->SetAxisRange(0.,1.0,"Y");
      hdummyeta->Draw();
      
      delete hseta;

      THStack *hs2eta = new THStack("hs2eta", "hs2eta; #eta; #Lambda_{0}");
      
      delete leta;
      leta = getLegend(9);
      TH1F* h2eta[nMat];
      std::vector<TH1F*> vhist2eta; /// = { h2[10], h2[9], h2[7], h2[8], h2[5], h2[6], h2[4], h2[3], h2[2], h2[1], h2[0] }; 
      pieseta.clear();
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
	h2eta[imat] = (TH1F*)m_l0vseta_permaterial[materialType.at(imat)]->ProjectionX();
         h2eta[imat]->SetFillColor(m_materialPlottingStyles[materialType[imat]].first);
         h2eta[imat]->SetFillStyle(m_materialPlottingStyles[materialType[imat]].second);
         h2eta[imat]->SetLineColor(m_materialPlottingStyles[materialType[imat]].first);
         h2eta[imat]->SetMarkerStyle(0);
         if(h2eta[imat]->Integral() > 0.0001 && materialType.at(imat) != "Moderator" && materialType.at(imat) != "NONE"){
          std::string legendname = materialType.at(imat);
          if(legendname == "Air") legendname = "Dry Nitrogen";
          if(legendname == "SiliconModules")   legendname = "Silicon modules";
	  if(legendname == "HeatExchangers")   legendname = "Heat exchangers";
	  if(legendname == "BeamPipe")         legendname = "Beam pipe and IPT";
	  if(legendname == "SupportStructure") legendname = "Support structure";
	  if(legendname == "PixelModules")     legendname = "Pixel modules";
	  if(legendname == "StripModules")     legendname = "Strip modules";
	  if(legendname == "PixelServicesCooling")    legendname = "Pixel services and cooling";
	  if(legendname == "StripServicesCooling")    legendname = "Strip services and cooling";
	  if(legendname == "PixelSupports") legendname = "Pixel supports";
	  if(legendname == "StripSupports") legendname = "Strip supports";
	  if(legendname == "PP1") legendname = "PP1 and enclosure";

          leta->AddEntry(h2eta[imat],legendname.c_str(),"fl");
          pieseta.push_back(imat);
        }
      }

      for(int imat = pieseta.size()-1; imat >= 0; imat--){
	vhist2eta.push_back(h2eta[pieseta.at(imat)]);
      }

      
      //     std::vector<TH1F*> vhist2; /// = { h2[10], h2[9], h2[7], h2[8], h2[5], h2[6], h2[4], h2[3], h2[2], h2[1], h2[0] }; 
      for (auto v : vhist2eta) hs2eta->Add(v);
      

      hs2eta->Draw("histsame");
      leta->Draw();
      drawATLAS("TopRight","Simulation");
      myText(0.59,0.76,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.71,kBlack,geoTag.c_str(),0.03); 
      hdummyeta->Draw("axis same");
      ctmp->Print((outputdir + "/MaterialType_l0vseta.png").c_str());
      ctmp->Print((outputdir + "/MaterialType_l0vseta.pdf").c_str());
      ctmp->Print((outputdir + "/MaterialType_l0vseta.eps").c_str());
////////////phi

      delete hdummyphi;

      hdummyphi = new TH1F("dummyphi","dummy; #phi; #Lambda_{0}",200,-3.5,3.5);
      hdummyphi->SetAxisRange(0.,1.5,"Y");
      hdummyphi->Draw();
      
      delete hsphi;

      THStack *hs2phi = new THStack("hs2phi", "hs2phi; #phi; #Lambda_{0}");
      
      delete lphi;
      lphi = getLegend(9);
      TH1F* h2phi[nMat];
       std::vector<TH1F*> vhist2phi; /// = { h2[10], h2[9], h2[7], h2[8], h2[5], h2[6], h2[4], h2[3], h2[2], h2[1], h2[0] }; 
 piesphi.clear();
      for(unsigned int imat = 0; imat < materialType.size(); imat++){
         h2phi[imat] = (TH1F*)m_l0vsphi_permaterial[materialType.at(imat)]->ProjectionX();
         h2phi[imat]->SetFillColor(m_materialPlottingStyles[materialType[imat]].first);
         h2phi[imat]->SetFillStyle(m_materialPlottingStyles[materialType[imat]].second);
         h2phi[imat]->SetLineColor(m_materialPlottingStyles[materialType[imat]].first);
         h2phi[imat]->SetMarkerStyle(0);
         if(h2phi[imat]->Integral() > 0.0001 && materialType.at(imat) != "Moderator" && materialType.at(imat) != "NONE"){
          std::string legendname = materialType.at(imat);
          if(legendname == "Air") legendname = "Dry Nitrogen";
          if(legendname == "SiliconModules")   legendname = "Silicon modules";
	  if(legendname == "HeatExchangers")   legendname = "Heat exchangers";
	  if(legendname == "BeamPipe")         legendname = "Beam pipe and IPT";
	  if(legendname == "SupportStructure") legendname = "Support structure";
	  if(legendname == "PixelModules")     legendname = "Pixel modules";
	  if(legendname == "StripModules")     legendname = "Strip modules";
	  if(legendname == "PixelServicesCooling")    legendname = "Pixel services and cooling";
	  if(legendname == "StripServicesCooling")    legendname = "Strip services and cooling";
	  if(legendname == "PixelSupports") legendname = "Pixel supports";
	  if(legendname == "StripSupports") legendname = "Strip supports";
	  if(legendname == "PP1") legendname = "PP1 and enclosure";

          lphi->AddEntry(h2eta[imat],legendname.c_str(),"fl");
          piesphi.push_back(imat);
        }
      }

    for(int imat = piesphi.size()-1; imat >= 0; imat--){
      vhist2phi.push_back(h2phi[piesphi.at(imat)]);
    }


 //     std::vector<TH1F*> vhist2; /// = { h2[10], h2[9], h2[7], h2[8], h2[5], h2[6], h2[4], h2[3], h2[2], h2[1], h2[0] }; 
      for( auto v : vhist2phi) hs2phi->Add(v);


      hs2phi->Draw("histsame");
      lphi->Draw();
      drawATLAS("TopCenter","Simulation");
      myText(0.45,0.76,kBlack,geoname.c_str(),0.04);
      ctmp->Print((outputdir + "/MaterialType_l0vsphi.png").c_str());
      ctmp->Print((outputdir + "/MaterialType_l0vsphi.pdf").c_str());
      ctmp->Print((outputdir + "/MaterialType_l0vsphi.eps").c_str());
////////////phi

     
   /* 
      for(unsigned int ip = 0; ip < materialType.size(); ip++){
        if(!(m_x0vsrz_permaterial[materialType.at(ip)]->Integral() > 1e-7)) continue;

          m_x0vsrz_permaterial[materialType.at(ip)]->Draw("colz");
          drawATLAS("TopRight", "Simulation");
          myText(0.22,0.87,kBlack,(geoname).c_str(),0.04);
          myText(0.22,0.81,kBlack,(materialType.at(ip).c_str()),0.04);
          ctmp->Print((outputdir + "/MaterialType_x0vsrz_" + materialType.at(ip) + ".png").c_str());
      }
   */
      delete l2;
      l2 = getLegend(10);

      hdummyeta->SetAxisRange(0.,0.5,"Y"); 
      hdummyeta->Draw();
      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 0; il < 5; il++){ //Pixels
              if(iec == 0 && il > 8) continue;

              std::string det = il > 4 ? "Strip" : "Pixel";
              std::string part = iec == 0 ? "Barrel" : "Disk";
              std::string legendname = det + "  " + part;
              legendname += il > 4 ? std::to_string(il-5) : std::to_string(il);
              p_l0vseta_layers[iec][il]->Draw("histsame");
              l2->AddEntry(p_l0vseta_layers[iec][il],legendname.c_str(),"lp");
          }
      }
      l2->Draw();
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.45,0.86,kBlack,"Pixel",0.04);      
      ctmp->Print((outputdir +  "/l0vseta_pixels.png").c_str());
      ctmp->Print((outputdir +  "/l0vseta_pixels.pdf").c_str());
      ctmp->Print((outputdir +  "/l0vseta_pixels.eps").c_str());

      delete l3;
      l3 = getLegend(10);
      hdummyeta->SetAxisRange(0.,0.15,"Y");
      hdummyeta->SetAxisRange(0.,3.1,"X");
      hdummyeta->Draw();
      for(unsigned int iec = 0; iec < 2; iec++){
          for(unsigned int il = 5; il < 11; il++){ //Pixels
              if(iec == 0 && il > 8) continue;
              std::string det = il > 4 ? "Strip" : "Pixel";
              std::string part = iec == 0 ? "Barrel" : "Disk";
              std::string legendname = det + "  " + part;
              legendname += il > 4 ? std::to_string(il-5) : std::to_string(il);
     
              p_l0vseta_layers[iec][il]->Draw("histsame");
              l3->AddEntry(p_l0vseta_layers[iec][il],legendname.c_str(),"lp");
         }

      }
      l3->Draw();
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.45,0.86,kBlack,"Strips",0.04);      

      ctmp->Print((outputdir +  "/l0vseta_strips.png").c_str());
      ctmp->Print((outputdir +  "/l0vseta_strips.pdf").c_str());
      ctmp->Print((outputdir +  "/l0vseta_strips.eps").c_str());
    
//    TLegend *l4 = getLegend(9);
      hdummyeta->SetAxisRange(0.,0.25,"Y");
      hdummyeta->SetAxisRange(0.,4.0,"X");
      hdummyeta->Draw();

      for(unsigned int ihit = 0; ihit < nHitMax; ihit++){
        p_l0vseta_countHits[ihit]->Draw("same");
      }
      drawATLAS("TopRight", "Simulation");
      myText(0.59,0.76,kBlack,(geoname).c_str(),0.04); 
      myText(0.22,0.86,kBlack,"L0 before X hits",0.04);      

      ctmp->Print((outputdir +  "/l0vseta_sihits.png").c_str());
      ctmp->Print((outputdir +  "/l0vseta_sihits.pdf").c_str());
      ctmp->Print((outputdir +  "/l0vseta_sihits.eps").c_str());

      ctmp->cd();
      
      gPad->SetRightMargin(0.15);
      p_x0vsetaphi->SetAxisRange(0.,3.0,"Z");
      p_x0vsetaphi->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/x0vsetaphi.png").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi.pdf").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi.eps").c_str());
      ctmp->cd();
      
      gPad->SetRightMargin(0.15);
      p_x0vsetaphi_pos->SetAxisRange(0.,3.0,"Z");
      p_x0vsetaphi_pos->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/x0vsetaphi_poseta.png").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi_poseta.pdf").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi_poseta.eps").c_str());
      ctmp->cd();
      
      gPad->SetRightMargin(0.15);
      p_x0vsetaphi_neg->SetAxisRange(0.,3.0,"Z");
      p_x0vsetaphi_neg->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/x0vsetaphi_negeta.png").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi_negeta.pdf").c_str());
      ctmp->Print((outputdir + "/x0vsetaphi_negeta.eps").c_str());
      ctmp->cd();
      
      gPad->SetRightMargin(0.15);
      p_l0vsetaphi->SetAxisRange(0.,0.8,"Z");
      p_l0vsetaphi->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/l0vsetaphi.png").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi.pdf").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi.eps").c_str());
      ctmp->cd();

      gPad->SetRightMargin(0.15);
      p_l0vsetaphi_pos->SetAxisRange(0.,0.8,"Z");
      p_l0vsetaphi_pos->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/l0vsetaphi_poseta.png").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi_poseta.pdf").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi_poseta.eps").c_str());
      ctmp->cd();

      gPad->SetRightMargin(0.15);
      p_l0vsetaphi_neg->SetAxisRange(0.,0.8,"Z");
      p_l0vsetaphi_neg->Draw("colz");
      drawATLAS("TopLeft","Simulation");
      myText(0.59,0.86,kBlack,geoname.c_str(),0.04);
      myText(0.59,0.81,kBlack,geoTag.c_str(),0.03); 
      
      ctmp->Print((outputdir + "/l0vsetaphi_negeta.png").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi_negeta.pdf").c_str());
      ctmp->Print((outputdir + "/l0vsetaphi_negeta.eps").c_str());
      ctmp->cd();

      return 1;
    }
}


void getLayer(float rHit, float zHit, int &be, int &layer){

    //Only works for ITk STEP 3 layout    

    if(      rHit < 96   && rHit > 34    && fabs(zHit) < 257 ){
	      be = 0; layer = 0;
	  }else if(rHit < 128  && rHit > 96    && fabs(zHit) < 257 ){
	      be = 0; layer = 1;
	  }else if(rHit < 213  && rHit > 146.5 && fabs(zHit) < 1070 ){
	      be = 0; layer = 2;
	  }else if(rHit < 275  && rHit > 213   && fabs(zHit) < 1070 ){
	      be = 0; layer = 3;
	  }else if(rHit < 338  && rHit > 275   && fabs(zHit) < 1070 ){
	      be = 0; layer = 4;
	  }else if(rHit < 338  && rHit > 265   && fabs(zHit) < 3019 && fabs(zHit) > 1120 ){
	      be = 1; layer = 4;
  	}else if(rHit < 79.2 && rHit > 57.6  && fabs(zHit) < 3019 && fabs(zHit) > 257 ){
	      be = 1; layer = 0;
	  }else if(rHit < 57.6 && rHit > 34    && fabs(zHit) < 3019 && fabs(zHit) > 257 ){
        be = 1; layer = 0;
  	}else if(rHit < 128  && rHit > 79.2  && fabs(zHit) < 3019 && fabs(zHit) > 257 ){
	      be = 1; layer = 1; //Neal's rings
	  }else if(rHit < 205  && rHit > 146.5 && fabs(zHit) < 3019 && fabs(zHit) > 1120 ){
	      be = 1; layer = 2;
	  }else if(rHit < 265  && rHit > 205   && fabs(zHit) < 3019 && fabs(zHit) > 1120 ){
	      be = 1; layer = 3;
	  }else if(rHit < 338  && rHit > 265   && fabs(zHit) < 3019 && fabs(zHit) > 1120 ){
	      be = 1; layer = 4;
	  }


	  if(      rHit < 450   && rHit > 345 && fabs(zHit) < 1400 ){
        be = 0; layer = 5;
	  }else if(rHit < 610   && rHit > 500 && fabs(zHit) < 1400 ){
	      be = 0; layer = 6;
    }else if(rHit < 810   && rHit > 700 && fabs(zHit) < 1400 ){
	      be = 0; layer = 7;
	  }else if(rHit < 1035  && rHit > 920 && fabs(zHit) < 1400 ){
	      be = 0; layer = 8;
    }else if(rHit > 1050 ){
	      be = -1; layer = -1; 
	  }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 1540 && fabs(zHit) > 1490 ){
	      be = 1; layer = 5;
    }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 1730 && fabs(zHit) > 1680 ){
	      be = 1; layer = 6;
    }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 1980 && fabs(zHit) > 1930 ){
	      be = 1; layer = 7;
    }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 2260 && fabs(zHit) > 2210 ){
	      be = 1; layer = 8;
    }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 2560 && fabs(zHit) > 2500 ){
	      be = 1; layer = 9;
    }else if(rHit < 975.6 && rHit > 379 && fabs(zHit) < 3880 && fabs(zHit) > 2820 ){
	      be = 1; layer = 10;
    }
  
}

std::string updateGrouping(std::string matname, std::string volname){

    if(matname == "matHeatExchanger") return "HeatExchangers";

    if(volname.find("Svc") == std::string::npos && volname.find("PP1") == std::string::npos) return "NoUpdate";

    std::vector<std::string> pp1_pixel_aluminium = {
      "Pixel::Svc04A_1_PP1_Inner_Endplate",
      "Pixel::Svc04A_2_PP1_Inner_Endplate",
      "Pixel::Svc04C_1_PP1_Inner_Endplate",
      "Pixel::Svc04C_2_PP1_Inner_Endplate",
      "Pixel::Svc05A_PP1_Inner_Cylinder",
      "Pixel::Svc05C_PP1_Inner_Cylinder",
      "Pixel::Svc06A_PP1_Middle_Endplate",
      "Pixel::Svc06C_PP1_Middle_Endplate",
      "Pixel::Svc07A_PP1_InnerCone",
      "Pixel::Svc07C_PP1_InnerCone"
    };
    
    std::vector<std::string> pp1_strip_aluminium = {
      "SCT::pixPP1outer"
    };

    std::vector<std::string> pp1_pixel_services = {
      "Pixel::Svc08A_1_PP1_T1_OBService_Cyl",
      "Pixel::Svc08A_2_PP1_T1_OBService_Cyl",
      "Pixel::Svc08C_1_PP1_T1_OBService_Cyl",
      "Pixel::Svc08C_2_PP1_T1_OBService_Cyl",
      "Pixel::Svc09A_1_PP1_T1_OBService_Con",
      "Pixel::Svc09A_2_PP1_T1_OBService_Con",
      "Pixel::Svc09C_1_PP1_T1_OBService_Con",
      "Pixel::Svc09C_2_PP1_T1_OBService_Con",
      "Pixel::Svc10A_PP1_T1_InnerService_Con",
      "Pixel::Svc10C_PP1_T1_InnerService_Con",
      "Pixel::Svc11A_1_PP1_T1_InnerService_Cyl",
      "Pixel::Svc11A_2_PP1_T1_InnerService_Cyl",
      "Pixel::Svc11C_1_PP1_T1_InnerService_Cyl",
      "Pixel::Svc11C_2_PP1_T1_InnerService_Cyl",
      "Pixel::Svc12A_PP1_T1_T2_Connectors",
      "Pixel::Svc12C_PP1_T1_T2_Connectors",
      "Pixel::Svc13_PP1_T2_R235_R290_PZ_Power1",
      "Pixel::Svc14_PP1_T2_R290_R345_PZ_Power1",
      "Pixel::Svc15_PP1_T2_R345_R347_PZ_Power1",
      "Pixel::Svc16_PP1_T2_R235_R290_PZ_Power2",
      "Pixel::Svc17_PP1_T2_R290_R345_PZ_Power2",
      "Pixel::Svc18_PP1_T2_R345_R347_PZ_Power2",
      "Pixel::Svc19_PP1_T2_R235_R290_NZ_Power1",
      "Pixel::Svc20_PP1_T2_R290_R345_NZ_Power1",
      "Pixel::Svc21_PP1_T2_R345_R347_NZ_Power1",
      "Pixel::Svc22_PP1_T2_R235_R290_NZ_Power2",
      "Pixel::Svc23_PP1_T2_R290_R345_NZ_Power2",
      "Pixel::Svc24_PP1_T2_R345_R347_NZ_Power2",
      "Pixel::Svc25_PP1_T2_R180_R347_PZ_Cooling",
      "Pixel::Svc26_PP1_T2_R180_R347_NZ_Cooling"
    };

    std::vector<std::string> pp1_strip_services = {
      "SCT::Pix_PP1InnerServices",
      "SCT::Pix_PP1OuterServices",
      "SCT::OuterPP1Connectors",
      "SCT::InnerPP1Connectors",
      "SCT::pixSvc_PP1_T2_R347_R420_CoolingInner",
      "SCT::pixSvc_PP1_T2_R420_R1000_CoolingOuter",
      "SCT::pixSvc_PP1_T2_R420_R1000_CoolingSum",
      "SCT::pixSvc_PP1_T2_R347_R1000_Data",
      "SCT::pixSvc_PP1_T2_R700_R1000_Data",
      "SCT::pixSvc_PP1_T2_R347_R420_Power",
      "SCT::pixSvc_PP1_T2_R420_R474_Power",
      "SCT::pixSvc_PP1_T2_R474_R529_Power",
      "SCT::pixSvc_PP1_T2_R529_R584_Power",
      "SCT::pixSvc_PP1_T2_R584_R639_Power",
      "SCT::pixSvc_PP1_T2_R639_R700_Power",
      "SCT::pixSvc_PP1_T2_R700_R1000_Power"
    };

    if(std::find(pp1_pixel_aluminium.begin(),pp1_pixel_aluminium.end(),volname) != pp1_pixel_aluminium.end() || std::find(pp1_strip_aluminium.begin(),pp1_strip_aluminium.end(),volname) != pp1_strip_aluminium.end()) {
      return "AluminiumShell";
    }
    else if (std::find(pp1_pixel_services.begin(),pp1_pixel_services.end(),volname) != pp1_pixel_services.end() || std::find(pp1_strip_services.begin(),pp1_strip_services.end(),volname) != pp1_strip_services.end()){
        if(volname.find("Power") != std::string::npos) {
          return "PP1Power";
        }
        else if(volname.find("Cooling") != std::string::npos) {
          return "PP1Cooling";
        }
        else if(volname.find("Data") != std::string::npos) {
          return "PP1Data";
        }
        else{
          return "PP1";
        }
    }
    return "NoUpdate";

}




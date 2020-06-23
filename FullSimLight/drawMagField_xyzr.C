/* drawMagField_xyzr.C ROOT macro
 *
 * author: Marilena Bandieramonte
 * email:  marilena.bandieramonte@cern.ch
 *
 * How to run: root .x 'drawMagField_xyzr("ATLAS_BField.txt").C'
 *
 * This ROOT macro plot Magnetic Field heatmaps starting from
 * .ascii file that can be generated running testMagneticField.cc
 * The macro expects to find the following columns:
 *  x[m],y[m],z[m],r[m],BField[T]
 *
 * Output maps generated:
 *
 * - xy  heatmap
 * - xyz heatmap
 * - yz  heatmap
 * - xz  heatmap
 * - rz  heatmap
 *
 */


#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TCanvas.h"
#include <string>


void drawMagField_xyzr(std::string filename){
    gROOT->LoadMacro("AtlasStyle.C");
    Double_t w = 1200;
    Double_t h = 1000;
    
    TCanvas * c1 = new TCanvas("c", "c", w, h);
    c1->cd();
    c1->SetRightMargin(0.2);
    TTree *t = new TTree("t", "t");
    //t->ReadFile("ATLAS_BField_xyz.txt", "x:y:z:BField");
    t->ReadFile(filename.c_str(), "x:y:z:r:BField");
    gStyle->SetPalette(57);
    //gStyle->SetPalette(104);//kTemperature
    //gStyle->SetPalette(104);//Rainbow
    
    t->SetMarkerStyle(21);
    t->SetMarkerSize(0.5);
    
    std::string s = ".txt";
    std::string::size_type i = filename.find(s);
    if (i != std::string::npos)
        filename.erase(i, s.length());
    std::cout<<"Name base: "<<filename;
    
    
    t->Draw("y:x:BField", "", "COLZ");
    TH2F* histo = (TH2F*)gROOT->FindObject("htemp");
    histo->SetNameTitle("htemp", "Magnetic Field");
    histo->GetXaxis()->SetTitle("x[m]");
    histo->GetYaxis()->SetTitle("y[m]");
    histo->GetZaxis()->SetTitle("BField [T]");
    //gPad->SetLogz();
    //histo->GetZaxis()->SetTitleOffset(0.1);
    c1->Update();
    std::string name_yx = filename + "_yx.png";
    c1->SaveAs(name_yx.c_str());
    c1->Close();
    
    w = 1600;
    h = 1100;
    
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    c2->cd();
    c2->SetRightMargin(0.15);
    t->SetMarkerSize(0.5);
    
    t->Draw("x:y:z:BField", "", "COLZ");
    histo = (TH2F*)gROOT->FindObject("htemp");
    histo->SetNameTitle("htemp", "Magnetic Field");
    histo->GetXaxis()->SetTitle("z[m]");
    histo->GetYaxis()->SetTitle("y[m]");
    histo->GetZaxis()->SetTitle("x[m]");
    //histo->GetZaxis()->SetTitleOffset(0.1);
    c2->Update();
    std::string name_xyz = filename + "_xyz.png";
    c2->SaveAs(name_xyz.c_str());
    c2->Close();
    
    w = 1400;
    h = 800;
    
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    c3->SetRightMargin(0.2);
    
    t->SetMarkerSize(1.3);
    t->Draw("x:z:BField", "", "COLZ");
    histo = (TH2F*)gROOT->FindObject("htemp");
    histo->SetNameTitle("htemp", "Magnetic Field");
    histo->GetXaxis()->SetTitle("z[m]");
    histo->GetYaxis()->SetTitle("x[m]");
    histo->GetZaxis()->SetTitle("BField [T]");
    c3->Update();
    std::string name_xz = filename + "_xz.png";
    c3->SaveAs(name_xz.c_str());

    t->Draw("y:z:BField", "", "COLZ");
    histo = (TH2F*)gROOT->FindObject("htemp");
    histo->SetNameTitle("htemp", "Magnetic Field");
    histo->GetXaxis()->SetTitle("z[m]");
    histo->GetYaxis()->SetTitle("y[m]");
    histo->GetZaxis()->SetTitle("BField [T]");
    
    c3->Update();
    std::string name_zy = filename + "_yz.png";
    c3->SaveAs(name_zy.c_str());
    
    
    //std::string filename_rz = filename + "_rz.txt";
    //TTree *t2 = new TTree("t", "t");
    //t2->ReadFile(filename_rz.c_str(), "r:z:BField");
    //t2->SetMarkerStyle(21);
    t->SetMarkerSize(1.5);
    t->Draw("r:z:BField", "", "COLZ");
    histo = (TH2F*)gROOT->FindObject("htemp");
    histo->SetNameTitle("htemp", "Magnetic Field");
    histo->GetXaxis()->SetTitle("z[m]");
    histo->GetYaxis()->SetTitle("r[m]");
    histo->GetZaxis()->SetTitle("BField [T]");
    c3->Update();
    std::string name_rz = filename + "_rz.png";
    c3->SaveAs(name_rz.c_str());
    c3->Close();

}

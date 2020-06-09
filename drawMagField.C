/* drawMagField_xyzr.C ROOT macro
 *
 * author: Marilena Bandieramonte
 * email:  marilena.bandieramonte@cern.ch
 *
 * How to run: root .x 'drawMagField("ATLAS_BField.root").C'
 *  root .x 'drawMagField("ATLAS_BField.root").C'
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


void drawMagField(){
    
    TFile * f =  new TFile("ATLAS_BField_default.root", "READ");
    f->ls();
    gStyle->SetOptStat(0000);
    TCanvas * c1 = new TCanvas("c", "c", 1400, 700);
    c1->SetRightMargin(0.2);

    TH2F* BField_zy_x1 = (TH2F*) f->Get("BField_zy_x1");
    BField_zy_x1->GetXaxis()->SetTitle("z[m]");
    BField_zy_x1->GetYaxis()->SetTitle("y[m]");
    BField_zy_x1->GetZaxis()->SetTitle("BField [T]");
    BField_zy_x1->Draw("COLZ");
    c1->Update();
    std::string name_zy_x1 = "Bfield_zy_x1.png";
    c1->SaveAs(name_zy_x1.c_str());
    
    TH2F* BField_zy_x2= (TH2F*) f->Get("BField_zy_x2");
    BField_zy_x2->GetXaxis()->SetTitle("z[m]");
    BField_zy_x2->GetYaxis()->SetTitle("y[m]");
    BField_zy_x2->GetZaxis()->SetTitle("BField [T]");
    BField_zy_x2->Draw("COLZ");
    c1->Update();
    std::string name_zy_x2 = "Bfield_zy_x2.png";
    c1->SaveAs(name_zy_x2.c_str());
    
    TH2F* BField_zy_x3= (TH2F*) f->Get("BField_zy_x3");
    BField_zy_x3->GetXaxis()->SetTitle("z[m]");
    BField_zy_x3->GetYaxis()->SetTitle("y[m]");
    BField_zy_x3->GetZaxis()->SetTitle("BField [T]");
    BField_zy_x3->Draw("COLZ");
    c1->Update();
    std::string name_zy_x3 = "Bfield_zy_x3.png";
    c1->SaveAs(name_zy_x3.c_str());

    TH2F* BField_xy_z1= (TH2F*) f->Get("BField_xy_z1");
    BField_xy_z1->GetXaxis()->SetTitle("x[m]");
    BField_xy_z1->GetYaxis()->SetTitle("y[m]");
    BField_xy_z1->GetZaxis()->SetTitle("BField [T]");
    BField_xy_z1->Draw("COLZ");
    c1->Update();
    std::string name_xy_z1 = "Bfield_xy_z1.png";
    c1->SaveAs(name_xy_z1.c_str());
    
    TH2F* BField_xy_z2= (TH2F*) f->Get("BField_xy_z2");
    BField_xy_z2->GetXaxis()->SetTitle("x[m]");
    BField_xy_z2->GetYaxis()->SetTitle("y[m]");
    BField_xy_z2->GetZaxis()->SetTitle("BField [T]");
    BField_xy_z2->Draw("COLZ");
    c1->Update();
    std::string name_xy_z2 = "Bfield_xy_z2.png";
    c1->SaveAs(name_xy_z2.c_str());
    
    TH2F* BField_xy_z3= (TH2F*) f->Get("BField_xy_z3");
    BField_xy_z3->GetXaxis()->SetTitle("x[m]");
    BField_xy_z3->GetYaxis()->SetTitle("y[m]");
    BField_xy_z3->GetZaxis()->SetTitle("BField [T]");
    BField_xy_z3->Draw("COLZ");
    c1->Update();
    std::string name_xy_z3 = "Bfield_xy_z3.png";
    c1->SaveAs(name_xy_z3.c_str());
    
}

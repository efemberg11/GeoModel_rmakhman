/* drawGeantinoMaps.C ROOT macro
 *
 * author: Marilena Bandieramonte
 * email:  marilena.bandieramonte@cern.ch
 *
 * How to run: root .x 'drawGeantinoMaps("geantinoMaps.root").C'
 *
 * This ROOT macro plot GeantinoMaps heatmaps starting from
 * the ROOT file generated when running gmgeantino
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


double get_z(double eta, double r=1200){
    
    TVector3 v;
    v.SetPtEtaPhi(r, eta, 0);
    double z = v.Z();
    return z;
    
}

void draw_eta_lines(double maxr, double maxz)
{
    
    double etas[5]={1, 1.5, 2, 2.5, 3};
    double eta, r, z;
    for (int i=0; i<5; i++)
    {
        eta=etas[i];
        if (get_z(eta) > maxz){
            r = maxr * maxz / get_z(eta, r=maxr);
            z = maxz;
        }
        
        else{
            r = maxr;
            z = get_z(eta, r=maxr);
        }
        
        TLine line;
        line.SetLineStyle(3);
        line.SetLineWidth(2);
        line.DrawLine(0, 0, z, r);
        
        TLatex p;
        p.SetNDC();
        p.SetTextSize(0.04);
        p.SetTextFont(42);
//        if (get_z(eta) > maxz)
//        {
//            p.DrawLatex(0.86*z/maxz-0.05,0.87*r/maxr+0.08*340./r,"#eta=%.1f"%(eta));
//
//        }
//        else
//            p.DrawLatex(0.86*z/maxz+0.07*1300/z,0.87*r/maxr+0.005,"#eta=%.1f"%(eta));
    }
}


void ATLASLabel(double x,double y,TString text,bool color=1){
    
    TLatex l;
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    
    l.SetTextSize(0.04);
    
    double delx = 0.115*550*gPad->GetWh()/(472*gPad->GetWw());
    
    l.DrawLatex(x,y,"ATLAS");
    
    if (text){
        TLatex p;
        p.SetNDC();
        
        p.SetTextSize(0.04);
        p.SetTextFont(42);
        p.SetTextColor(color);
        p.DrawLatex(x+delx,y,text);
    }
}

void drawGeantinoMaps( TString filename){
    
    TFile * f =  new TFile(filename, "READ");
    f->ls();
    gStyle->SetOptStat(0000);
    gStyle->SetOptTitle(0);
    gStyle->SetLabelFont(22,"times");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetNumberContours(255);
    //gROOT->SetStyle("ATLAS");
    TCanvas * c1 = new TCanvas("c", "c", 1000, 600);
    c1->SetRightMargin(0.2);
    
    TH2F* RZRadLen = (TH2F*) f->Get("RZRadLen");
    RZRadLen->GetXaxis()->SetTitle("z[mm]");
    RZRadLen->GetYaxis()->SetTitle("r[mm]");
    RZRadLen->GetZaxis()->SetTitle("%X0");
    RZRadLen->Draw("COLZ");
    //draw_eta_lines(1150., 3562.5);
    ATLASLabel(0.1, 0.95, " Simulation Internal");
    c1->Update();
    TString title=RZRadLen->GetTitle();
    TString name =  filename+"_"+title +".png";
    c1->SaveAs(name);
    name =  filename+"_"+title +".pdf";
    c1->SaveAs(name);
    
    TH2F* RZIntLen = (TH2F*) f->Get("RZIntLen");
    RZIntLen->GetXaxis()->SetTitle("z[mm]");
    RZIntLen->GetYaxis()->SetTitle("r[mm]");
    RZIntLen->GetZaxis()->SetTitle("%X0");
    RZIntLen->Draw("COLZ");
    ATLASLabel(0.1, 0.95, " Simulation Internal");
    c1->Update();
    title=RZIntLen->GetTitle();
    name = filename+"_"+title +".png";
    c1->SaveAs(name);
    name = filename+"_"+title +".pdf";
    c1->SaveAs(name);
    
}

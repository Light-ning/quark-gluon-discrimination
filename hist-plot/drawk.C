#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "ATLASStyle/AtlasStyle.C"
#include "ATLASStyle/AtlasUtils.C"
#include "ATLASStyle/AtlasLabels.C"

using namespace std;

TCanvas *c = new TCanvas("c","c",1600,1100);
TPad *pad1 = new TPad("pad1","pad1",0,0.31,1,1);
TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.30);

TFile *f1;
TFile *f2;
float lum = 0;

void draw(TString name, float xmin, float xmax, float ymin, float ymax);

int main(int argc,char **argv) {
    SetAtlasStyle ();
    pad1 -> SetBottomMargin(0);
    pad1 -> SetLeftMargin(0.12);
    pad1 -> SetLogy(1);
    pad1 -> Draw();
    pad2 -> SetBottomMargin(.4);
    pad2 -> SetLeftMargin(0.12);
    pad2 -> SetGridx(1);
    pad2 -> SetGridy(1);
    pad2 -> Draw();
    
    TString dataset = argv[1];
    if(dataset=="a" || dataset=="15+16"){
        f1 = new TFile("outMC/mjja.root");
        f2 = new TFile("outdata/mjj15+16.root");
        lum = 36.17;
    }
    if(dataset=="d" || dataset=="17"){
        f1 = new TFile("outMC/mjjd.root");
        f2 = new TFile("outdata/mjj17.root");
        lum = 44.31;
    }
    if(dataset=="e" || dataset=="18"){
        f1 = new TFile("outMC/mjje.root");
        f2 = new TFile("outdata/mjj18.root");
        lum = 58.45;
    }
    
    draw("HistMjj", 1000, 8500, 0.05, 500000000);
    c->SaveAs("mjj.png");
    
    draw("HistLeadingJetPt", 0, 3000, 0.5, 1000000000);
    c->SaveAs("lpt.png");
    
    draw("HistSubJetPt", 0, 3000, 0.5, 1000000000);
    c->SaveAs("spt.png");
    
    draw("HistLjeteta", -3.1, 3.1, 0.5, 200000000);
    c->SaveAs("leta.png");
    
    draw("HistSjeteta", -3.1, 3.1, 0.5, 200000000);
    c->SaveAs("seta.png");
    
    draw("HistLjetphi", -3.1, 3.1, 0.5, 50000000);
    c->SaveAs("lphi.png");
    
    draw("HistSjetphi", -3.1, 3.1, 0.5, 50000000);
    c->SaveAs("sphi.png");
    
    draw("HistyStar", -1.2, 1.2, 0.5, 200000000);
    c->SaveAs("ystar.png");
    
    draw("Histdeltaeta", -3.1, 3.1, 0.5, 200000000);
    c->SaveAs("deltaeta.png");
    
    draw("Histdeltaphi", 0, 3.1, 0.5, 1000000000);
    c->SaveAs("deltaphi.png");
}

void draw(TString name, float xmin, float xmax, float ymin, float ymax){
    TH1D *mc = (TH1D*) f1->Get(name);
    TH1D *data = (TH1D*) f2->Get(name);
    
    pad1->cd();
    mc->Scale(lum);
    mc->SetAxisRange(xmin, xmax, "X");
    mc->SetAxisRange(ymin, ymax, "Y");
    mc->SetLineColor(1);
    mc->SetLineWidth(2);
    mc->SetMarkerColor(kYellow);
    mc->SetFillColor(kYellow);
    mc->Draw("][ hist");
    data->SetLineColor(1);
    data->SetMarkerColor(1);
    data->Draw("same");
    myText(   0.17,  0.75, 1, "#sqrt{s}= 13 TeV");
    myBoxText( 0.6, 0.8, 0.05, kYellow, "MC");
    myMarkerText( 0.6, 0.75, 1, 20, "data",1);
    ATLASLabel(0.17,0.8,"Preliminary");
    
    TH1D *mcf = (TH1D*)mc->Clone();
    TH1D *dataf = (TH1D*)data->Clone();
    pad2->cd();
    dataf->Divide(mcf);
    dataf->SetLineColor(1);
    dataf->SetMarkerColor(1);
    dataf->SetAxisRange(xmin, xmax, "X");
    dataf->SetAxisRange(0.5, 1.5, "Y");
    dataf->GetYaxis()->SetTitle("data/mc");
    //mcfdeta->SetTitleSize(42,"Y");
    dataf->Draw();
}

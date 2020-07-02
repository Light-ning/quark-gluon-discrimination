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

TCanvas *c = new TCanvas("c","c",1100,1100);
TPad *pad1 = new TPad("pad1","pad1",0,0.31,1,1);
TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.31);

TFile *f1 = new TFile("outMC/mjje.root");
TFile *f2 = new TFile("outdata/mjj18.root");

void draw(TString name);

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
    
    draw("HistMjj");
    c->SaveAs("mjj.png");
    
    draw("HistLeadingJetPt");
    c->SaveAs("lpt.png");
    
    draw("HistSubJetPt");
    c->SaveAs("spt.png");
    
    draw("HistLjeteta");
    c->SaveAs("leta.png");
    
    draw("HistSjeteta");
    c->SaveAs("seta.png");
    
    draw("HistLjetphi");
    c->SaveAs("lphi.png");
    
    draw("HistSjetphi");
    c->SaveAs("sphi.png");
    
    draw("HistyStar");
    c->SaveAs("ystar.png");
    
    draw("Histdeltaeta");
    c->SaveAs("deltaeta.png");
    
    draw("Histdeltaphi");
    c->SaveAs("deltaphi.png");
}

void draw(TString name){
    TH1D *mc = (TH1D*) f1->Get(name);
    TH1D *data = (TH1D*) f2->Get(name);
    
    pad1->cd();
    data->SetLineColor(7);
    data->SetMarkerColor(7);
    data->Draw();
    mc->Scale(58.45);
    mc->SetLineColor(2);
    mc->SetMarkerColor(2);
    mc->Draw("same");
    myText(       0.6,  0.85, 1, "#sqrt{s}= 13 TeV");
    myMarkerText( 0.8, 0.7, 2, 20, "MC",1);
    myMarkerText( 0.8, 0.6, 7, 20, "data",1);
    ATLASLabel(0.2,0.15,"Preliminary");
    
    TH1D *mcf = (TH1D*)mc->Clone();
    TH1D *dataf = (TH1D*)data->Clone();
    pad2->cd();
    dataf->Divide(mcf);
    dataf->SetLineColor(7);
    dataf->SetMarkerColor(7);
    mcf->Divide(mcf);
    mcf->SetAxisRange(0, 2, "Y");
    //mcfdeta->SetAxisRange(0, 2*edatadeta/emcdeta-1, "Y");
    mcf->SetLineColor(2);
    mcf->SetMarkerColor(2);
    mcf->GetYaxis()->SetTitle("data/mc");
    //mcfdeta->SetTitleSize(42,"Y");
    mcf->Draw();
    dataf->Draw("same");
}
